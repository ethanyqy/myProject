//
//  main.cpp
//  a4q1
//
//  Created by Ethan on 2019-07-13.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "expression.hpp"
using namespace std;

// You will need to write your own test harness

// Read and construct expression object. Returns a pointer to a heap-allocated
// object (caller must delete it).
istream &operator>>(istream &in, Expression *&e) {
    vector<Expression *> E;
    string s;
    while (in >> s) {
        istringstream ss(s);
        int n;
        if (ss >> n) {
            Expression *loneInt = new LoneInteger{n};
            E.push_back(loneInt);
        }
        else if (s == "NEG" || s == "ABS") {
            Expression *unary = new UnaryExpression{s, E.back()};
            E.pop_back();
            E.push_back(unary);
        }
        else if (s == "+" || s == "-" || s == "*" || s == "/") {
            Expression *temp = E.back();
            E.pop_back();
            Expression *binary = new BinaryExpression{s, E.back(), temp};
            E.pop_back();
            E.push_back(binary);
        }
        else if (s == "done") {
            cout << E.back()->prettyprint() << endl;
            break;
        }
        else {
            Expression *variable = new Variable{s};
            E.push_back(variable);
        }
    }
    e = E.back();
    E.pop_back();
    return in;
}

int main () {
    string s;
    
    Expression *e;
    cin >> e;
    
    // Command interpreter
    while (cin >> s) {
        if (s == "eval") {
            try {
                e->evaluate();
                cout << to_string(e->evaluate()) << endl;
            } catch (string name) {
                cout << name << " has no value" << endl;
            }
        } else if (s == "set") {
            string name;
            int value;
            cin >> name;
            cin >> value;
            e->set(value, name);
        } else if (s == "unset") {
            string name;
            cin >> name;
            e->unset(name);
        } else if (s == "print") {
            cout << e->prettyprint() << endl;
        } else {
            Expression *theCopy = e->copy();
            cout << theCopy->prettyprint() << endl;
            theCopy->set(1, "x");
            cout << theCopy->prettyprint() << endl;
            try { cout << theCopy->evaluate() << endl; }
            catch(string name){
            cout << name << " has no value" << endl;
            }
            delete theCopy;
        }
    }
    delete e;
}
