//
//  main.cpp
//  traverse
//
//  Created by An Zhang on 2020-01-28.
//  Copyright © 2020 An Zhang. All rights reserved.
//

#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>

class Invalid {
	std::string message;
	
public:
	Invalid(std::string s) {
		message = s;
	}
	
	std::string getMessage() {
		return message;
	}
};

class Node {
    std::string key;
    int numChild;
    std::string lexme;
    std::string rule;
	std::string type;
    std::vector<Node*> child;
    std::vector<std::string> nonterminals;
    
public:
    Node(std::string k, int n, std::vector<std::string> v) {
        key = k;
        numChild = n;
        nonterminals = v;
    }
    
    ~Node() {
        for (int i = 0; i < numChild; i++) {
            delete child[i];
        }
    }
    
    void setChild ()  {
    }
    
    void setNum (int n)  {
        numChild = n;
    }
    
    void setKey (std::string n)  {
        key = n;
    }
    
    void setRule (std::string r) {
        rule = r;
    }
	
	void setType (std::string t) {
		type = t;
	}
    
    void push_lexme(std::string s) {
        lexme = s;
    }
    
    void push(Node* n) {
        child.push_back(n);
    }
    
    std::vector<Node*> getChild () {
        return child;
    }
    
    int getNum () {
        return numChild;
    }
    
    std::string getKey () {
        return key;
    }
	
	std::string getLexme () {
		return lexme;
	}
    
    std::string getRule () {
        return rule;
    }
	
	std::string getType () {
		return type;
	}
    
    bool isTerminal() {
        return (find(nonterminals.begin(), nonterminals.end(), key) == nonterminals.end());
    }
    
    std::vector<std::string> getcheck() {
        return nonterminals;
    }
};

std::string arg(Node &n) {
	std::string args;
	// arglist → expr, arglist → expr COMMA arglist
	
	if (n.getRule() == "arglist expr")
		args = n.getChild().at(0)->getType();
	else if (n.getRule() == "arglist expr COMMA arglist") {
		args = n.getChild().at(0)->getType() + arg(*(n.getChild().at(2)));
	}
	
	return args;
}

void computeType(Node &n, std::unordered_map<std::string, std::string> signature) {
	std::string key = n.getKey();
	std::string rule = n.getRule();
	
	// check if procedure returns int
	if (key == "procedure") {
		if (n.getChild().at(9)->getType() == "")
			computeType(*(n.getChild().at(9)), signature);
		
		std::string type = n.getChild().at(9)->getType();
		
		if (key == "main")
			if (n.getChild().at(5)->getChild().at(1)->getType() != "int")
				throw Invalid("Second parameter of main must be integer: " + n.getRule());
		
		if (type != "int")
			throw Invalid("Invalid return value: " + n.getRule());
	}
	
	// check if main returns int
	if (key == "main") {
		if (n.getChild().at(11)->getType() == "")
			computeType(*(n.getChild().at(11)), signature);
		
		std::string type = n.getChild().at(11)->getType();
		
		if (n.getChild().at(5)->getChild().at(1)->getType() != "int")
			throw Invalid("Second parameter of main must be integer: " + n.getRule());
		
		if (type != "int")
			throw Invalid("Invalid return value: " + n.getRule());
	}
	
	// expr → term, expr → expr PLUS term, expr → expr MINUS term
	if (key == "expr") {
		if (rule == "expr term") {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
				
			n.setType(n.getChild().at(0)->getType());
		}
		else if (rule == "expr expr PLUS term") {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
			
			if (n.getChild().at(2)->getType() == "")
				computeType(*(n.getChild().at(2)), signature);
			
			std::string type1 = n.getChild().at(0)->getType();
			std::string type2 = n.getChild().at(2)->getType();
			
			if (type1 == "int*" && type2 == "int*")
				throw Invalid("Invalid type: " + n.getRule());
			else if (type1 == "int" && type2 == "int*")
				n.setType("int*");
			else if (type1 == "int" && type2 == "int")
				n.setType("int");
			else if (type1 == "int*" && type2 == "int")
				n.setType("int*");
		}
		else if (rule == "expr expr MINUS term") {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
			
			if (n.getChild().at(2)->getType() == "")
				computeType(*(n.getChild().at(2)), signature);
			
			
			std::string type1 = n.getChild().at(0)->getType();
			std::string type2 = n.getChild().at(2)->getType();
			
			if (type1 == "int" && type2 == "int*")
				throw Invalid("Invalid type: " + n.getRule());
			else if (type1 == "int*" && type2 == "int")
				n.setType("int*");
			else if (type1 == "int*" && type2 == "int*")
				n.setType("int");
			else if (type1 == "int" && type2 == "int")
				n.setType("int");
		}
	}
	
	// term → factor, term → term STAR factor, term → term SLASH factor, term → term PCT factor
	if (key == "term") {
		if (rule == "term factor") {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
				
			n.setType(n.getChild().at(0)->getType());
		}
		else if ((rule == "term term STAR factor")
				 || (n.getRule() == "term term SLASH factor")
				 || (n.getRule() == "term term PCT factor")) {
			
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
			
			if (n.getChild().at(2)->getType() == "")
				computeType(*(n.getChild().at(2)), signature);
			
			std::string type1 = n.getChild().at(0)->getType();
			std::string type2 = n.getChild().at(2)->getType();
			
			if ((type1 == "int") && (type2 == "int"))
				n.setType("int");
			else
				throw Invalid("Invalid type: " + n.getRule());
		}
	}
	
	// factor → ID, factor → NUM, factor → NULL, factor → LPAREN expr RPAREN, factor → AMP lvalue
	// factor → STAR factor, factor → NEW INT LBRACK expr RBRACK, factor → ID LPAREN RPAREN
	// factor → ID LPAREN arglist RPAREN
	
	if (key == "factor") {
		if ((rule == "factor ID") || (rule == "factor NULL") || (rule == "factor NUM")) {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
			
			n.setType(n.getChild().at(0)->getType());
		}
		else if (rule == "factor LPAREN expr RPAREN") {
			if (n.getChild().at(1)->getType() == "")
				computeType(*(n.getChild().at(1)), signature);
				
			n.setType(n.getChild().at(1)->getType());
		}
		else if (rule == "factor AMP lvalue") {
			
			if (n.getChild().at(1)->getType() == "")
				computeType(*(n.getChild().at(1)), signature);
			
			std::string type = n.getChild().at(1)->getType();
			
			if (type == "int")
				n.setType("int*");
			else
				throw Invalid("Invalid type with &: " + n.getRule());
		}
		else if (rule == "factor STAR factor") {
			if (n.getChild().at(1)->getType() == "")
				computeType(*(n.getChild().at(1)), signature);
			
			std::string type = n.getChild().at(1)->getType();
			
			if (type == "int*")
				n.setType("int");
			else
				throw Invalid("Invalid type with *: " + n.getRule());
		}
		else if (rule == "factor NEW INT LBRACK expr RBRACK") {
			if (n.getChild().at(3)->getType() == "")
				computeType(*(n.getChild().at(3)), signature);
			
			std::string type = n.getChild().at(3)->getType();
			
			if (type == "int")
				n.setType("int*");
			else
				throw Invalid("Invalid type with new: " + n.getRule());
		}
		else if ((rule == "factor ID LPAREN RPAREN") || (rule == "factor ID LPAREN arglist RPAREN")) {
			std::string args;
			std::string proName = n.getChild().at(0)->getLexme();
			// arglist → expr, arglist → expr COMMA arglist
			if (rule == "factor ID LPAREN arglist RPAREN") {
				computeType(*(n.getChild().at(2)), signature);
				args = arg(*(n.getChild().at(2)));
			}
			
			if (args != signature[proName])
				throw Invalid("Invalid procedure signature: " + proName + " " + args + " " + signature[proName]);
		
			n.setType("int");
		}
	}
	
	
	//lvalue → ID, lvalue → STAR factor, lvalue → LPAREN lvalue RPAREN
	
	if (key == "lvalue") {
		if (rule == "lvalue ID") {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
			
			n.setType(n.getChild().at(0)->getType());
		}
		else if (rule == "lvalue STAR factor") {
			if (n.getChild().at(1)->getType() == "")
				computeType(*(n.getChild().at(1)), signature);
			
			std::string type = n.getChild().at(1)->getType();
			if (type == "int*")
				n.setType("int");
			else
				throw Invalid("Invalid type with * (lvalue): " + n.getRule());
		}
		else if (rule == "lvalue LPAREN lvalue RPAREN") {
			if (n.getChild().at(1)->getType() == "")
				computeType(*(n.getChild().at(1)), signature);
			
			n.setType(n.getChild().at(1)->getType());
		}
	}
	
	// statement → lvalue BECOMES expr SEMI
	// statement → PRINTLN LPAREN expr RPAREN SEMI
	// statement → DELETE LBRACK RBRACK expr SEMI
	
	if (key == "statement") {
		if (rule == "statement lvalue BECOMES expr SEMI") {
			if (n.getChild().at(0)->getType() == "")
				computeType(*(n.getChild().at(0)), signature);
			
			if (n.getChild().at(2)->getType() == "")
				computeType(*(n.getChild().at(2)), signature);
			
			std::string type1 = n.getChild().at(0)->getType();
			std::string type2 = n.getChild().at(2)->getType();
			
			if (type1 != type2)
				throw Invalid("Type dismatch: " + n.getRule() + " " + type1 + " " + type2);
		}
		else if (rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
			if (n.getChild().at(2)->getType() == "")
				computeType(*(n.getChild().at(2)), signature);
			
			if (n.getChild().at(2)->getType() == "int*")
				throw Invalid("Cannot print a pointer: " + n.getRule());
		}
		else if (rule == "statement DELETE LBRACK RBRACK expr SEMI") {
			if (n.getChild().at(3)->getType() == "")
				computeType(*(n.getChild().at(3)), signature);
			
			if (n.getChild().at(3)->getType() == "int")
				throw Invalid("Cannot delete an integer: " + n.getRule());
		}
	}
	
	// test → expr EQ expr, test → expr NE expr, test → expr LT expr, test → expr LE expr
	// test → expr GE expr, test → expr GT expr
	
	if (key == "test") {
		if (n.getChild().at(0)->getType() == "")
			computeType(*(n.getChild().at(0)), signature);
		
		if (n.getChild().at(2)->getType() == "")
			computeType(*(n.getChild().at(2)), signature);
		
		std::string type1 = n.getChild().at(0)->getType();
		std::string type2 = n.getChild().at(2)->getType();
		
		if (type1 != type2)
			throw Invalid("Type dismatch: " + n.getRule() + " " + type1 + " " + type2);
	}
	
	// dcls → dcls dcl BECOMES NUM SEMI, dcls → dcls dcl BECOMES NULL SEMI
	if (key == "dcls") {
		
		if (rule == "dcls dcls dcl BECOMES NUM SEMI")
			if (n.getChild().at(1)->getChild().at(1)->getType() != "int")
				throw Invalid("Type dismatch: " + n.getRule());
		
		if (rule == "dcls dcls dcl BECOMES NULL SEMI")
			if (n.getChild().at(1)->getChild().at(1)->getType() != "int*")
				throw Invalid("Type dismatch: " + n.getRule());
	}
	
	for (int i = 0; i < n.getNum(); i ++) {
		computeType(*(n.getChild().at(i)), signature);
	}
}

std::ostream &operator<<(std::ostream &out, Node& n) {
    for (int i = 0; i < n.getNum(); i ++) {
		out<<*(n.getChild().at(i));
    }
	out<< n.getKey() <<" " << n.getType() << std::endl;
	return out;
}
        
std::istream &operator>>(std::istream &in, Node& n) {
    std::string key = "";
    int childnum = 0;
    std::string line_input;
        
    getline(std::cin, line_input);
    bool first = true;
    bool terminal = false;
    for (auto c : line_input) {
        if (c == ' ') {
            // std::cout<<key<<std::endl;
            if (first) {
                n.setKey(key);
                terminal = n.isTerminal();
                key = "";
                first = false;
            }
            else {
                if (!terminal)
                    childnum ++;
                key = "";
            }
        }
        else
			key = key + c;
    }
        
	// std::cout<<key<<std::endl;
    if (first) {
        n.setKey(key);
        terminal = n.isTerminal();
        n.push_lexme("empty");
    }
		
    if (!terminal && n.getLexme() != "empty") {
        childnum ++;
    }
	else
		n.push_lexme(key);
	
	n.setRule(line_input);
    n.setNum(childnum);
    // std::cout<<childnum;
	
    for (int i=0; i < childnum; i++) {
        Node* temp = new Node("",0,n.getcheck());
        std::cin>>*temp;
        n.push(temp);
    }
    return in;
}
	
void generateSymTable (Node &n,
					   std::vector<std::unordered_map<std::string, std::pair<std::string, bool>>> &table,
					   std::vector<std::string> &pro,
					   std::vector<std::string> &ids, bool &para, int &current) {
	std::string type = "";
	std::string id = "";
	
	// compute the type, dcl → type ID, factor → ID, factor → NUM, factor → NULL, lvalue → ID
	if ((n.getRule() == "lvalue ID") || (n.getRule() == "factor ID")) {
		id = n.getChild().at(0)->getLexme();
		if (table.at(current).find(id) == table.at(current).end())
			throw Invalid("Variable not declared: " + id);
		
		n.getChild().at(0)->setType(table.at(current)[id].first);
	}
	
	if (n.getRule() == "factor NUM") {
		n.getChild().at(0)->setType("int");
	}
	
	if (n.getRule() == "factor NULL") {
		n.getChild().at(0)->setType("int*");
	}
	
	// If other procedure, create new symbol table
	if (n.getKey() == "procedure") {
		id = n.getChild().at(1)->getLexme();
		std::unordered_map<std::string, std::pair<std::string, bool>> temp;
		table.push_back(temp);
		current ++;
		if (find(pro.begin(), pro.end(), id) == pro.end()) {
			pro.push_back(id);
		}
		else {
			throw Invalid("variable already exist: " + id + n.getRule());
		}
	}
	
	// set para to true, if it is function parameter
	if (n.getRule() == "LPAREN (") {
		para = true;
	}
	
	// set para to false, if it is not function parameter
	if (n.getRule() == "RPAREN )") {
		para = false;
	}
	
	// Find declarations and push onto symbol table
	if (n.getRule() == "dcl type ID") {
		if (n.getChild().at(0)->getRule() == "type INT") {
			type = "int";
		}
		else if (n.getChild().at(0)->getRule() == "type INT STAR") {
			type = "int*";
		}
		id = n.getChild().at(1)->getLexme();
		if (table.at(current).find(id) == table.at(current).end()) {
			table.at(current)[id].first = type;
			table.at(current)[id].second = para;
			ids.push_back(id);
		}
		else {
			throw Invalid("variable already exist: " + id + n.getRule());
		}
	
		n.getChild().at(1)->setType(type);
	}
	
	// push wain onto symbol table
	if (n.getRule() == "procedures main") {
		id = "wain";
		std::unordered_map<std::string, std::pair<std::string, bool>> temp;
		table.push_back(temp);
		current ++;
		if (find(pro.begin(), pro.end(), id) == pro.end()) {
			pro.push_back(id);
		}
		else {
			throw Invalid("variable already exist: " + id + n.getRule());
		}
	}
	
	// check if ID is declared
	if (n.getKey() == "ID") {
		id = n.getLexme();
		// check if id is a function
		bool exist = false;
		for (int i = 0; i < table.size(); i ++) {
			if (find(pro.begin(), pro.end(), id) != pro.end()) {
				exist = true;
			}
		}
	
		if (table.at(current).find(id) != table.at(current).end()) {
			exist = false;
		}
	
		if ((table.at(current).find(id) == table.at(current).end()) && !exist) {
			throw Invalid("variable not declared: " + n.getLexme() +" "+ n.getRule());
		}
	}
	
	if ((n.getRule() == "factor ID LPAREN RPAREN")
		|| (n.getRule() == "factor ID LPAREN arglist RPAREN")) {
			id = n.getChild().at(0)->getLexme();
			if (table.at(current).find(id) != table.at(current).end())
				throw Invalid("Function name collide with variable " + n.getLexme() +" "+ n.getRule());
	}
	
	for (int i = 0; i < n.getNum(); i ++) {
		generateSymTable(*(n.getChild().at(i)), table, pro,
						 ids, para, current);
	}
}
	
int main(int argc, const char * argv[]) {
    std::string arr[17] = {"procedures", "procedure", "main", "params", "paramlist", "type", "dcl",                         "dcls", "statements", "lvalue", "expr", "statement", "test", "term",                            "factor", "arglist", "start"};
    std::vector<std::string> non_terminals;
    for (int i = 0; i < 17; i ++) {
        non_terminals.push_back(arr[i]);
    }
    Node parseTree("",0,non_terminals);
    std::cin>>parseTree;
	
	try {
		std::vector<std::unordered_map<std::string, std::pair<std::string, bool>>> table;
		std::vector<std::string> ids;
		std::vector<std::string> procedures;
		std::unordered_map<std::string, std::string> signature;
		bool para = false;
		int current = -1;
		generateSymTable(parseTree, table, procedures, ids, para, current);
		std::string temp;
		std::string sign;
		
	
		int index = 0;
		for (int i = 0; i < table.size(); i ++) {
			std::cerr<<procedures.at(i)<<" ";
			sign = "";
	
			if (table.at(i).size() == 0) {
		// Leave a line if there is no variable defined in the procedure
				std::cerr<<std::endl;
				signature[procedures.at(i)] = sign;
			}
	
			for (int j = 0; j < table.at(i).size(); j++) {
				std::pair<std::string, bool> temp = table.at(i)[ids.at(index)];
	
				if (j == 0) { // output procedure signature
					for (int k = 0; k < table.at(i).size(); k++) {
						std::pair<std::string, bool> var = table.at(i)[ids.at(index+k)];
						if (var.second) {
							std::cerr<<var.first<<" ";
							sign = sign + var.first;
						}
					}
					signature[procedures.at(i)] = sign;
					std::cerr<<std::endl;
					std::cerr<<ids.at(index)<<" "<<temp.first<<std::endl;
				}
				else {
					std::cerr<<ids.at(index)<<" "<<temp.first<<std::endl;
				}
		
				index ++;
			}
			std::cerr<<std::endl;
		}
		
		computeType(parseTree, signature);
	
		
	/*
		for (int i = 0; i < ids.size(); i ++) {
			temp = ids.at(i);
			std::cerr<<temp<<" "<<table.at(0)[temp].first<<std::endl;
		}
	 */
	
	// check unidentified
	
	}
	catch (Invalid e) {
		std::cerr<<"ERROR  "<<e.getMessage()<<std::endl;
	}

	
	// function name and variable name
	
	// std::cout<<parseTree;
}
