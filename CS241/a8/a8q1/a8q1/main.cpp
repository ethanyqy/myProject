#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
using namespace std;

// ERROR messages class
class Failure{
    string message;
public:
    Failure(string message): message(message){}
    
    const string what(){return message;}
};

// Parse tree for reading wlp4
class Tree {
public:
    string rule;
    vector<string> tokens;
    vector<Tree> children;
    
    // traverse the parse tree
    void print(){
        cout << rule << endl;
        for (auto child: children) {
            child.print();
        }
    }
    
    // check if current node is a leaf
    bool isTerminal(){
        string lhs = tokens[0];
        string terminals[] = {"BOF","BECOMES","COMMA","ELSE","EOF","EQ","GE","GT","ID","IF","INT","LBRACE","LE","LPAREN","LT","MINUS","NE","NUM","PCT","PLUS","PRINTLN","RBRACE","RETURN","RPAREN","SEMI","SLASH","STAR","WAIN","WHILE","AMP","LBRACK","RBRACK","NEW","DELETE","NULL"};
        for(auto t: terminals){
            if (t == lhs) {
                return true;
            }
        }
        return false;
    }
    
};

// construct a parse tree
Tree buildTree(string rule) {
    Tree node;
    // initialize rule
    node.rule = rule;
    istringstream ss{rule};
    string token;
    
    // initialize tokens
    while (ss>> token){
        node.tokens.push_back(token);
    }
    
    // initialize children if node is not a leaf
    if (!node.isTerminal()) {
        for (int i = 0; i < node.tokens.size() -1; ++i) {
            string line;
            getline(cin, line);
            node.children.push_back(buildTree(line));
        }
    }
    return node;
}

// count number of parameters
int countPramlist(Tree pramslist, int n){
    if (pramslist.children.size() == 1) {
        return n+1;
    } else {
        return countPramlist(pramslist.children[2], n+1);
    }
}

int countArglist(Tree arglist, int n){
    if (arglist.children.size() == 1) {
        return n+1;
    } else {
        return countArglist(arglist.children[2], n+1);
    }
}

string getType(Tree t, unordered_map<string, vector<pair<string,string>>> st, string cur, vector<string> p, vector<int> pp, int sig, string func) {
    if(t.tokens[0] == "ID") {
        for(auto i: st[cur]) {
            if (i.first == t.tokens[1]) {
                return i.second;
            }
        }
    } else if (t.tokens[0] == "NUM") {
        return "int";
    } else if (t.rule == "expr term" || t.rule == "term factor" || t.rule == "factor ID" || t.rule == "factor NUM" || t.rule == "lvalue ID") {
        return getType(t.children[0], st, cur, p, pp, sig, func);
    } else if (t.rule == "lvalue LPAREN lvalue RPAREN" || t.rule == "factor LPAREN expr RPAREN") {
        return getType(t.children[1], st, cur, p, pp, sig, func);
    } else if (t.rule == "lvalue STAR factor" || t.rule == "factor STAR factor") {
        if(getType(t.children[1], st, cur, p, pp, sig, func) == "int*") {
            return "int";
        } else {
            return "ERROR: not a pointer";
        }
    } else if (t.rule == "factor AMP lvalue") {
        if(getType(t.children[1], st, cur, p, pp, sig, func) != "int"){
          return "ERROR";
        } else {
          return "int*";
        }
    } else if(t.rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
       if(getType(t.children[9], st, cur, p, pp, sig, func) != "int"){
         return "ERROR";
       } else {
         return getType(t.children[9], st, cur, p, pp, sig, func);
       }
    } else if (t.rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
        if(getType(t.children[11],st , cur, p, pp, sig, func) != "int"){
            return "ERROR";
        } else {
            return getType(t.children[11], st, cur, p, pp, sig, func);
        }
    } else if(t.rule == "dcls dcls dcl BECOMES NULL SEMI" ){
        if(getType(t.children[1].children[1], st, cur, p, pp, sig, func) != "int*"){
            return "ERROR: cannot initiate int to NULL";
        } else {
            return getType(t.children[1], st, cur, p, pp, sig, func);
        }
    } else if(t.rule == "dcls dcls dcl BECOMES NUM SEMI" ){
        if(getType(t.children[1].children[1], st, cur, p, pp, sig, func) != "int"){
            return "ERROR: cannot initiate int* to int";
        } else {
            return getType(t.children[1], st, cur, p, pp, sig, func);
        }
    } else if(t.rule == "test expr EQ expr" || t.rule == "test expr EQ expr"){
        if(getType(t.children[0], st, cur, p, pp, sig, func) != getType(t.children[2], st, cur, p, pp, sig, func)){
            return "ERROR: test type mismatch";
        } else {
            return getType(t.children[0], st, cur, p, pp, sig, func);
        }
    } else if(t.rule == "test expr LT expr" || t.rule == "test expr GT expr" || t.rule == "test expr LE expr" || t.rule == "test expr GE expr" || t.rule == "test expr EQ expr" || t.rule == "test expr NE expr"){
        if(getType(t.children[0], st, cur, p, pp, sig, func) != getType(t.children[2], st, cur, p, pp, sig, func)){
            return "ERROR: test type mismatch";
        } else {
            if(getType(t.children[0], st, cur, p, pp, sig, func) == "*int"){
                return "ERROR: cannot do array comparison";
            }
      }
    } else if(t.rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
        if(getType(t.children[2], st, cur, p, pp, sig, func) != "int"){
            return "ERROR: println requires int";
        }
    } else if(t.rule == "statement DELETE LBRACK RBRACK expr SEMI"){
        if(getType(t.children[3], st, cur, p, pp, sig, func) != "int*"){
            return "ERROR: delete required pointer";
        }
    } else if(t.rule == "factor NEW INT LBRACK expr RBRACK"){
        if(getType(t.children[3], st, cur, p, pp, sig, func) != "int"){
            return "ERROR: argument of NEW INT [] must be an int";
        } else {
            return "int*";
        }
    } else if(t.rule == "statement lvalue BECOMES expr SEMI"){
        if(getType(t.children[0], st, cur, p, pp, sig, func) != getType(t.children[2], st, cur, p, pp, sig, func)){
            return "ERROR: type mismatch";
        } else {
            return getType(t.children[0], st, cur, p, pp, sig, func);
        }
    } else if(t.rule == "expr expr PLUS term"){
      if(getType(t.children[0], st, cur, p, pp, sig, func) == "int" && getType(t.children[2], st, cur, p, pp, sig, func) == "int"){
        return "int";
      } else if(getType(t.children[0], st, cur, p, pp, sig, func) == "int*" && getType(t.children[2], st, cur, p, pp, sig, func) == "int*"){
        return "ERROR: int* + int*";
      } else {
        return "int*";
      }
    } else if(t.rule == "expr expr MINUS term"){
      if(getType(t.children[0], st, cur, p, pp, sig, func) == "int*" && getType(t.children[2], st, cur, p, pp, sig, func) == "int"){
        return "int*";
      } else if(getType(t.children[0], st, cur, p, pp, sig, func) == "int" && getType(t.children[2], st, cur, p, pp, sig, func) == "int*"){
        return "ERROR: int - int*";
      } else {
        return "int";
      }
    } else if(t.rule == "term term STAR factor" || t.rule == "term term SLASH factor" || t.rule == "term term PCT factor"){
      if(getType(t.children[0], st, cur, p, pp, sig, func) == "int" && getType(t.children[2], st, cur, p, pp, sig, func) == "int"){
        return "int";
      } else {
        return "ERROR: multiplication error";
      }
    }
    //
    else if(t.rule == "factor ID LPAREN arglist RPAREN"){
        return getType(t.children[2], st, cur, p, pp, 0, t.children[0].tokens[1]);
    } else if (t.rule == "arglist expr COMMA arglist") {
        vector<string>::iterator iter = find(p.begin(), p.end(), func);
        int index = distance(p.begin(), iter);
        
        if (sig < pp[index]) {
            if (st[func][sig].second != getType(t.children[0], st, cur, p, pp, sig, func)) {
                return "ERROR: signature type mismatch";
            } else {
                return getType(t.children[2], st, cur, p, pp, sig+1, func);
                
            }
        } else {
            return "ERROR: missing signatures";
        }
    } else if (t.rule == "arglist expr") {
        
        vector<string>::iterator iter = find(p.begin(), p.end(), func);
        int index = distance(p.begin(), iter);
        if(sig != pp[index] -1) {
            return "ERROR: lack of parameters";
        } else if(sig < pp[index]) {
            if (st[func][sig].second != getType(t.children[0], st, cur, p, pp, sig, func)) {
                return "ERROR: signature type mismatch";
            } else {
                return getType(t.children[0], st, cur, p, pp, sig+1, func);
            }
        } else {
            return "ERROR: missing signatures";
        }
    } else {
        return "no";
    }
}

bool checkType(Tree t, unordered_map<string, vector<pair<string,string>>> st, vector<string> p, vector<int> pp, string cur){
    for(auto i: t.children) {
        if (i.tokens[0] == "procedure") {
            cur = i.children[1].tokens[1];
        } else if (i.tokens[0] == "main") {
            cur = "wain";
        }
        if (getType(i, st, cur, p, pp, 0, "").find("ERROR") != string::npos) {
            throw Failure(getType(i, st, cur, p, pp, 0, ""));
            return false;
        }
        checkType(i, st, p, pp, cur);
    }
    if (st["wain"][1].second == "int*") {
        throw Failure("ERROR: Second parameter of wain can't be int");
        return false;
    }
    return true;
}

// p: procedures, st: vectors of variables in a procedure
void buildSymbolTable(unordered_map<string, vector<pair<string,string>>> &st, vector<string> &p, const Tree t, string cur, vector<int> &pp){
    if (t.tokens[0] == "procedures") {
        if (t.tokens[1] != "main") {
            string procedure = t.children[0].children[1].tokens[1];
            if (count(p.begin(), p.end(), procedure) > 0){
                throw Failure("ERROR: procedure, "+ procedure +", has been declared");
            }
            if (t.children[0].children[3].children.size() == 0) {
                pp.push_back(0);
            } else {
                pp.push_back(countPramlist(t.children[0].children[3].children[0], 0));
            }
            p.push_back(procedure);
            vector<pair<string, string>> tmp;
            st[procedure] = tmp;
            for (auto node: t.children) {
                buildSymbolTable(st, p, node, procedure, pp);
            }
        } else {
            if (count(p.begin(), p.end(), "wain") > 0){
                throw Failure("ERROR: main function, wain, has been declared");
            }
            pp.push_back(2);
            p.push_back("wain");
            vector<pair<string, string>> tmp;
            st["wain"] = tmp;
            buildSymbolTable(st, p, t.children[0], "wain", pp);
        }
    } else if (t.tokens[0] == "dcl") {
        for(auto i: st[cur]) {
            if (i.first == t.children[1].tokens[1]) {
                throw Failure("ERROR: redefinition of "+i.first);
            }
        }
        pair<string,string> variable;
        variable.first = t.children[1].tokens[1];
        if (t.children[0].tokens.size() == 2){
            variable.second = "int";
        } else {
            variable.second = "int*";
        }
        st[cur].push_back(variable);
        
    } else if (t.tokens.size() > 1) {
        int found_1 = 0;
        int found_2 = 0;
        if (t.tokens[1] == "ID") {
            for(auto i: p){
                if (i == t.children[0].tokens[1]){
                    found_1 = 1;
                }
            }
            for(auto j: st[cur]) {
                if (j.first == t.children[0].tokens[1]){
                    found_2 = 1;
                }
            }
            if (found_2 == 0 && found_1 == 0) {
                throw Failure("ERROR: undeclared identifier " + t.children[0].tokens[1]);
            } else if (t.tokens.size()>2) {
                if (found_2 != 0){
                    throw Failure("ERROR: cannot access function since local variable is declared");
                }
                if (found_1 == 0){
                    throw Failure("ERROR: function, "+t.children[0].tokens[1]+", is not declared");
                }
            } else if (found_2 == 0 && t.tokens.size() == 2) {
                throw Failure("ERROR: variable, "+t.children[0].tokens[1]+", is not decalred");
            }
        }
        for (auto node: t.children) {
            buildSymbolTable(st, p, node, cur, pp);
        }
    } else {
        for (auto node: t.children) {
            buildSymbolTable(st, p, node, cur, pp);
        }
    }
}


void printSymbolTable(vector<string> procedures, unordered_map<string, vector<pair<string,string>>> symbolTable, vector<int> procedureParameters) {
    for (int i = 0; i < procedures.size(); i++) {
        cerr << procedures[i];
        for (int j = 0; j < procedureParameters[i]; j++) {
            cerr << " " << symbolTable[procedures[i]][j].second;
        }
        cerr << endl;
        for (int j = 0; j < symbolTable[procedures[i]].size(); j++) {
            cerr << symbolTable[procedures[i]][j].first << " " << symbolTable[procedures[i]][j].second << endl;
        }
        if (i != procedures.size()-1) {
            cerr << endl;
        }
    }
}

int main() {
    Tree parseTree;
    unordered_map<string, vector<pair<string,string>>> symbolTable;
    vector<string> procedures;
    vector<int> procedureParameters;
    
    // build the parse tree
    string line;
    getline(cin, line);
    parseTree = buildTree(line);
    
    try {
        
        buildSymbolTable(symbolTable, procedures, parseTree, "", procedureParameters);
        
        printSymbolTable(procedures, symbolTable, procedureParameters);
        
        checkType(parseTree, symbolTable, procedures, procedureParameters, "");
        
    } catch (Failure &f) {
        cerr << f.what() << endl;
        return 1;
    }
    
    
}

