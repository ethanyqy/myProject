#include <iostream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

bool isTerminal(string a){
    return (a=="BOF")||(a=="EOF")||(a=="BECOMES")||(a=="COMMA")||(a=="ELSE")||(a=="EQ")||
    (a=="GE")||(a=="GT")||(a=="ID")||(a=="IF")||(a=="INT")||(a=="LBRACE")||(a=="LE")||(a=="LPAREN")||
    (a=="LT")||(a=="MINUS")||(a=="NE")||(a=="NUM")||(a=="PCT")||(a=="PLUS")||(a=="PRINTLN")||(a=="RBRACE")||
    (a=="RETURN")||(a=="RPAREN")||(a=="SEMI")||(a=="SLASH")||(a=="STAR")||(a=="WAIN")||(a=="WHILE")||
    (a=="AMP")||(a=="LBRACK")||(a=="RBRACK")||(a=="NEW")||(a=="DELETE")||(a=="NULL");
}

class ParseTree{
public:
    string me;
    int numChild;
    vector<ParseTree*> children;
    bool isFull(){
        return numChild == children.size();
    }
    ~ParseTree(){
        for(auto &x:children){
            delete x;
        }
    }
};

void print(ParseTree* temp){
    cout<<temp->me;
    for(auto &x:temp->children){
        cout<<" "<<x->me;
    }
    cout<<endl;
    for(auto &x:temp->children){
        print(x);
    }
}

string getType(ParseTree* type){
    string rv = "";
    for(auto x:type->children){
        if(isTerminal(x->me)){
            rv+=x->children.at(0)->me;
        } else throw 1;
    }
    return rv;
}

void establish(map<string, string> &symtable,vector<string> &symbols,ParseTree *&root){
    if(root->me != "procedure"){
        if(root == nullptr || root->children.size() == 0){
            return;
        }
        if(root->me == "WAIN"){
            symtable["wain"] = "";
            symbols.push_back("wain");
        }
        if(root->me == "dcl"){
            if(root->children.at(0)->me != "type"||root->children.at(1)->me !="ID") throw 1;
            string type;
            try {
                type = getType(root->children.at(0));
            } catch (...) {
                throw 1;
            }
            string id = root->children.at(1)->children.at(0)->me;
            if(symtable.find(id) == symtable.end()){
                symtable[id] = type;
                if (type =="") {
                    symbols.push_back(id);
                } else {
                    symbols.push_back(id+" "+type);
                }
            } else {
                throw 1;
            }
        } else if(root->me == "ID"){
            if(symtable.find(root->children.at(0)->me)==symtable.end()) throw 1;
        } else {
            for(auto &x:root->children){
                establish(symtable,symbols,x);
            }
        }
    } else {
        if(root->children.at(0)->me != "INT" || root->children.at(1)->me!= "ID"){
            throw 1;
        } else {
            symtable[root->children.at(1)->children.at(0)->me] = "int";
        }
    }
}

int main(int argc, const char * argv[]) {
    map<string,string> symtable;
    vector<string> symbols;
    vector<ParseTree*> queue;
    string line;
    ParseTree* root = nullptr;
    while (getline(cin,line)) {
        if(line == "break") break;
        ParseTree* temp = new ParseTree();
        if(root == nullptr) root = temp;
        istringstream iss{line};
        string token;
        iss>>token;
        temp->me = token;
        int numchild = 0;
        if(isTerminal(token)){
            iss>>token;
            ParseTree *child = new ParseTree();
            child->me = token;
            child->numChild=0;
            temp->children.push_back(child);
            temp->numChild=1;
        } else {
            while(iss>>token){
                numchild++;
            }
            temp->numChild = numchild;
        }
        //cout<<temp->me<<" "<<temp->numChild<<endl;
        if(!queue.empty()){
            queue.back()->children.push_back(temp);
            while (!queue.empty()) {
                if(queue.back()->isFull()) {
                    queue.pop_back();
                } else {
                    break;
                }
            }
        }
        if(!temp->isFull()){
            queue.push_back(temp);
        }
    }
    //print(root);
    try {
        establish(symtable,symbols,root);
    } catch (...) {
        cerr<<"ERROR"<<endl;
        delete root;
        return 1;
    }
    for(auto x:symbols){
        cerr<<x<<endl;
    }
    delete root;
    return 0;
}

