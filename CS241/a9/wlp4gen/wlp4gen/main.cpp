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
        cout<<rule<<endl;
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

string getType(Tree &t, unordered_map<string, vector<pair<string,string>>> &st, string cur, vector<string> &p, vector<int> &pp, int sig, string func) {
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
          return "ERROR1";
        } else {
          return "int*";
        }
    } else if(t.rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
       if(getType(t.children[9], st, cur, p, pp, sig, func) != "int"){
         return "ERROR2";
       } else {
         return getType(t.children[9], st, cur, p, pp, sig, func);
       }
    } else if (t.rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
        if(getType(t.children[11],st , cur, p, pp, sig, func) != "int"){
            return "ERROR3";
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
    } else if(t.tokens[0] == "dcl"){
        if(t.children[0].tokens.size() == 2) {
            return "int";
        } else {
            return "int*";
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
    } else if(t.rule == "factor ID LPAREN RPAREN"){
        
        return "int";
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
                return "int";
            }
        } else {
            return "ERROR: missing signatures";
        }
    } else {
        return "no";
    }
    return "";
}

bool checkType(Tree &t, unordered_map<string, vector<pair<string,string>>> &st, vector<string> &p, vector<int> &pp, string cur){
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
        if (i.tokens[0]=="procedure"||i.tokens[0]=="main"|i.tokens[0]=="factor"|i.tokens[0]=="lvalue"|i.tokens[0]=="dcls"||i.tokens[0]=="test"||i.tokens[0]=="statement") {
            checkType(i, st, p, pp, cur);
        }
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


void printSymbolTable(vector<string> &procedures, unordered_map<string, vector<pair<string,string>>> &symbolTable, vector<int> &procedureParameters) {
    for (int i = 0; i < procedures.size(); i++) {
        cerr<<procedures[i];
        for (int j = 0; j < procedureParameters[i]; j++) {
            cerr<<" "<<symbolTable[procedures[i]][j].second;
        }
        cerr<<endl;
        for (int j = 0; j < symbolTable[procedures[i]].size(); j++) {
            cerr<<symbolTable[procedures[i]][j].first<<" "<<symbolTable[procedures[i]][j].second<<endl;
        }
        if (i != procedures.size()-1) {
            cerr<<endl;
        }
    }
}

void buildOffsetTable(unordered_map<string, unordered_map<string, int>> &offsetTable,unordered_map<string, vector<pair<string,string>>> &symbolTable, vector<string> &procedures, vector<int> &procedureParameters){
    for (int i = 0; i < procedures.size(); i++){
        string p = procedures[i];
        int nop = procedureParameters[i];
        for (int j = 0; j<symbolTable[p].size(); j++) {
            offsetTable[p][symbolTable[p][j].first] = j * -4;
            if (p != "wain") {
                offsetTable[p][symbolTable[p][j].first] += (nop * 4);
            }
        }
    }
}

// push to stack
void pushr(int resgister) {
    cout<<"sub $30,$30,$4"<<endl;
    cout<<"sw $"<<resgister<<", 0($30)"<<endl;
}

// pop from stack
void popr(int resgister) {
    cout<<"lw $"<<resgister<<", 0($30)"<<endl;
    cout<<"add $30,$30,$4"<<endl;
}


void codeGenerate(Tree &t, unordered_map<string, unordered_map<string, int>> &offsetTable, string curp, unordered_map<string, vector<pair<string,string>>> &st, vector<string> &p, vector<int> &pp, int sig, string func, int &ifc, int &whilec){
    if (t.tokens[0] == "main") {
        cout<<"sub $29,$30,$4"<<endl;
        pushr(1);
        pushr(2);
        
        cout<<"lis $5"<<endl;
        cout<<".word init"<<endl;
        
        if(getType(t.children[3], st, curp, p, pp, sig, func) == "int"){
            pushr(2);
            cout<<"add $2,$0,$0"<<endl;
            pushr(31);
            cout<<"jalr $5"<<endl;
            popr(31);
            popr(2);
        } else {
            pushr(31);
            cout<<"jalr $5"<<endl;
            popr(31);
        }
        cout<<"main:"<<endl;
        codeGenerate(t.children[8], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        codeGenerate(t.children[9], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        codeGenerate(t.children[11], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        
        cout<<"add $30,$30,$4"<<endl;
        cout<<"add $30,$30,$4"<<endl;
        cout<<"add $30,$30,$4"<<endl;
        cout<<"jr $31"<<endl;
    } else if (t.tokens[0] == "procedures") {
        if (t.children.size() == 1) {
            codeGenerate(t.children[0], offsetTable, "wain", st, p, pp, sig, func, ifc, whilec);
        } else {
            codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            codeGenerate(t.children[0], offsetTable, t.children[0].children[1].tokens[1], st, p, pp, sig, func, ifc, whilec);
        }
    } else if(t.tokens[0] == "procedure") {
        cout<<"F"<<curp<<":"<<endl;
        cout<<"sub $29,$30,$4"<<endl;
        codeGenerate(t.children[6], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        for(int i = 1; i < 8; ++i) {
            if (i != 3){
                pushr(i);
            }
        }
        codeGenerate(t.children[7], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        codeGenerate(t.children[9], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        for(int i = 7; i > 0; --i) {
            if (i != 3) {
                popr(i);
            }
        }
        cout<<"add $30,$29,$4"<<endl;
        cout<<"jr $31"<<endl;
    } else if (t.tokens[0] == "dcls") {
        if (t.rule == "dcls dcls dcl BECOMES NUM SEMI") {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"lis $5"<<endl;
            cout<<".word "<<t.children[3].tokens[1]<<endl;
            pushr(5);
        } else if (t.rule == "dcls dcls dcl BECOMES NULL SEMI") {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"lis $5"<<endl;
            cout<<".word 0x1"<<endl;
            pushr(5);
        }
    } else if (t.tokens[0] == "arglist") {
        if (t.tokens.size() == 2) {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            pushr(3);
        } else {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            pushr(3);
            codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        }
    } else if (t.tokens[0] == "statements") {
        if(t.children.size()>0) {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        }
        
    } else if (t.tokens[0] == "statement") {
        if (t.rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
            pushr(1);
            codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"add $1,$3,$0"<<endl;
            pushr(31);
            cout<<"jalr $10"<<endl;
            popr(31);
            popr(1);
        } else if (t.rule == "statement lvalue BECOMES expr SEMI") {
            if (t.children[0].tokens[1] == "STAR") {
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                pushr(3);
                codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                popr(5);
                cout<<"sw $5,0($3)"<<endl;
            } else {
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            }
        } else if (t.rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
            int curwhilec = whilec;
            whilec++;
            cout<<"loop"<<curwhilec<<": "<<endl;
            codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"beq $3, $0, endloop"<<curwhilec<<endl;
            codeGenerate(t.children[5], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"beq $0, $0, loop"<<curwhilec<<endl;
            cout<<"endloop"<<curwhilec<<": "<<endl;
        } else if (t.rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
            int curif = ifc;
            ifc ++;
            codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"beq $3, $0, else"<<curif<<endl;
            codeGenerate(t.children[5], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"beq $0, $0, endif"<<curif<<endl;
            cout<<"else"<<curif<<": "<<endl;
            codeGenerate(t.children[9], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"endif"<<curif<<": "<<endl;
        } else if (t.rule == "statement DELETE LBRACK RBRACK expr SEMI"){
            codeGenerate(t.children[3], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            pushr(1);
            cout<<"beq $3,$11,skipDelete"<<endl;
            cout<<"add $1,$3,$0"<<endl;
            cout<<"lis $5"<<endl;
            cout<<".word delete"<<endl;
            pushr(31);
            cout<<"jalr $5"<<endl;
            popr(31);
            popr(1);
            cout<<"skipDelete:"<<endl;
        }
        
    } else if (t.tokens[0] == "expr"){
        if (t.children.size() == 1) {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        } else {
            string type_1, type_2, op;
            type_1 = getType(t.children[0], st, curp, p, pp, sig, func);
            type_2 = getType(t.children[2], st, curp, p, pp, sig, func);
            op = t.tokens[2];
            if (type_1 == "int" && type_2 == "int") {
                codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                pushr(3);
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                popr(5);
                if (op == "PLUS") {
                    cout<<"add $3,$5,$3"<<endl;
                } else {
                    cout<<"sub $3,$5,$3"<<endl;
                }
            } else if (type_1 == "int*" && type_2 == "int") {
                codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                pushr(3);
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                cout<<"mult $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
                popr(5);
                if (op == "PLUS") {
                    cout<<"add $3,$5,$3"<<endl;
                } else {
                    cout<<"sub $3,$5,$3"<<endl;
                }
            } else if (type_1 == "int" && type_2 == "int*") {// only for PLUS: int + int*
                codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                cout<<"mult $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
                pushr(3);
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                popr(5);
                cout<<"add $3,$5,$3"<<endl;
            } else {// only for MINUS: int* - int*
                codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                pushr(3);
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                popr(5);
                cout<<"sub $3,$5,$3"<<endl;
                cout<<"div $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
            }
        }
    } else if (t.tokens[0] == "term") {
        if (t.children.size() == 1) {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        } else {
            codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            pushr(3);
            codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            popr(5);
            if (t.tokens[2] == "STAR") {
                cout<<"mult $5,$3"<<endl;
                cout<<"mflo $3"<<endl;
            } else if (t.tokens[2] == "SLASH"){
                cout<<"div $5,$3"<<endl;
                cout<<"mflo $3"<<endl;
            } else {
                cout<<"div $5,$3"<<endl;
                cout<<"mfhi $3"<<endl;
            }
        }
    } else if (t.tokens[0] == "factor") {
        if (t.tokens[1] == "ID") {
            if (t.tokens.size() == 2) {
                cout<<"lw $3, "<<offsetTable[curp][t.children[0].tokens[1]]<<"($29)"<<endl;
            } else if (t.tokens.size() == 4){
                pushr(29);
                pushr(31);
                cout<<"lis $5"<<endl;
                cout<<".word F"<<t.children[0].tokens[1]<<endl;
                cout<<"jalr $5"<<endl;
                popr(31);
                popr(29);
            } else {
                pushr(29);
                pushr(31);
                codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
                cout<<"lis $5"<<endl;
                cout<<".word F"<<t.children[0].tokens[1]<<endl;
                cout<<"jalr $5"<<endl;
                for(int i = 0; i < countArglist(t.children[2], 0); ++i){
                    cout<<"add $30,$30,$4"<<endl;
                }
                popr(31);
                popr(29);
            }
        } else if (t.rule == "factor LPAREN expr RPAREN") {
            codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        } else if(t.tokens[1] == "NUM") {
            cout<<"lis $3"<<endl;
            cout<<".word "<<t.children[0].tokens[1]<<endl;
        } else if(t.tokens[1] == "NULL") {
            cout<<"add $3, $0, $11"<<endl;
        } else if(t.tokens[1] == "AMP") {
            if (t.children[1].rule == "lvalue ID") {
                cout<<"lis $3"<<endl;
                cout<<".word "<<offsetTable[curp][t.children[1].children[0].tokens[1]]<<endl;
                cout<<"add $3, $3, $29"<<endl;
            } else {
                codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            }
        } else if(t.tokens[1] == "STAR") {
            codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"lw $3, 0($3)"<<endl;
        } else if (t.tokens[1] == "NEW") {
            pushr(1);
            codeGenerate(t.children[3], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
            cout<<"add $1,$3,$0"<<endl;
            cout<<"lis $5"<<endl;
            cout<<".word new"<<endl;
            pushr(31);
            cout<<"jalr $5"<<endl;
            popr(31);
            popr(1);
            cout<<"bne $3,$0,1"<<endl;
            cout<<"add $3,$11,$0"<<endl;
        }
    } else if (t.tokens[0] == "lvalue") {
        if (t.rule == "lvalue ID") {
            cout<<"sw $3,"<<offsetTable[curp][t.children[0].tokens[1]]<<"($29)"<<endl;
        } else if (t.rule == "lvalue LPAREN lvalue RPAREN") {
            codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        } else if (t.rule == "lvalue STAR factor") {
            codeGenerate(t.children[1], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        }
    } else if (t.tokens[0] == "test") {
        string u = "";
        if (getType(t.children[0], st, curp, p, pp, sig, func) == "int*") {
            u = "u";
        }
        codeGenerate(t.children[0], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        pushr(3);
        codeGenerate(t.children[2], offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        popr(5);
        if (t.rule == "test expr LT expr") {
            cout<<"slt"<<u<<" $3,$5,$3"<<endl;
        } else if (t.rule == "test expr EQ expr") {
            cout<<"slt"<<u<<" $6, $3, $5"<<endl;
            cout<<"slt"<<u<<" $7, $5, $3"<<endl;
            cout<<"add $3, $6, $7"<<endl;
            cout<<"sub $3, $11, $3"<<endl;
        }  else if (t.rule == "test expr NE expr") {
            cout<<"slt"<<u<<" $6, $3, $5"<<endl;
            cout<<"slt $7, $5, $3"<<endl;
            cout<<"add $3, $6, $7"<<endl;
        } else if (t.rule == "test expr LE expr") {
            cout<<"slt"<<u<<" $3, $3, $5"<<endl;
            cout<<"sub $3, $11, $3"<<endl;
        } else if (t.rule == "test expr GE expr") {
            cout<<"slt"<<u<<" $3, $5, $3"<<endl;
            cout<<"sub $3, $11, $3"<<endl;
        } else if (t.rule == "test expr GT expr") {
            cout<<"slt"<<u<<" $3, $3, $5"<<endl;
        }
    } else {
        for (auto node: t.children) {
            codeGenerate(node, offsetTable, curp, st, p, pp, sig, func, ifc, whilec);
        }
    }
}

void init(){
    // $4 = 4
    cout<<"lis $4"<<endl;
    cout<<".word 4"<<endl;
    
    // print, $10 = print
    cout<<".import print"<<endl;
    cout<<"lis $10"<<endl;
    cout<<".word print"<<endl;
    
    // $11 = 1
    cout<<"lis $11"<<endl;
    cout<<".word 1"<<endl;
    
    cout<<".import init"<<endl;
    cout<<".import new"<<endl;
    cout<<".import delete"<<endl;
}

int main() {
    Tree parseTree;
    unordered_map<string, vector<pair<string,string>>> symbolTable;
    vector<string> procedures;
    vector<int> procedureParameters;
    unordered_map<string, unordered_map<string, int>> offsetTable;
    
    
    // build the parse tree
    string line;
    getline(cin, line);
    parseTree = buildTree(line);
    
    int ifcounter = 0;
    int whilecounter = 0;
    
    try {
        
        buildSymbolTable(symbolTable, procedures, parseTree, "", procedureParameters);
        
        printSymbolTable(procedures, symbolTable, procedureParameters);
        
        checkType(parseTree, symbolTable, procedures, procedureParameters, "");
        
        buildOffsetTable(offsetTable, symbolTable, procedures, procedureParameters);
        
        init();
        
        codeGenerate(parseTree, offsetTable, "",symbolTable, procedures, procedureParameters, 0, "",ifcounter,whilecounter);
        
    } catch (Failure &f) {
        cerr<<f.what()<<endl;
        return 1;
    }
    
    
}

