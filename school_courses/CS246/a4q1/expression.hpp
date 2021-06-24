//
//  expression.hpp
//  a4q1
//
//  Created by Ethan on 2019-07-13.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include <string>
using namespace std;


class Expression {
    
public:
    // dtor:
    virtual ~Expression();
    
    // methods:
    virtual void set(int value, string name) = 0;
    virtual void unset(string name) = 0;
    virtual string prettyprint() = 0;
    virtual int evaluate() = 0;
    
    virtual Expression* copy() = 0;
};


class LoneInteger: public Expression {
    int num;
    
public:
    // ctor & dtor:
    LoneInteger(int num);
    ~LoneInteger();
    
    // methods:
    void set(int value, string name) override;
    void unset(string name) override;
    string prettyprint() override;
    int evaluate() override;
    
    Expression * copy() override;
};


class Variable: public Expression {
    string name;
    bool initialization;
    int value;
    
public:
    // ctor & dtor:
    Variable(string name);
    Variable(string name, bool initialization, int value);
    ~Variable();
    
    // methods:
    void set(int value, string name) override;
    void unset(string name) override;
    string prettyprint() override;
    int evaluate() override;
    string getName();
    bool getInit();
    int getValue();
    
    Expression * copy() override;
};


class UnaryExpression: public Expression {
    string unary;
    Expression *expres;
    
public:
    // ctors & dtor:
    UnaryExpression(string unary, Expression *expres);
    ~UnaryExpression();
    
    // methods:
    void set(int value, string name) override;
    void unset(string name) override;
    string prettyprint() override;
    int evaluate() override;
    string getUnary();
    Expression *getExpres();
    
    Expression * copy() override;
};


class BinaryExpression: public Expression {
    string binary;
    Expression *expres1;
    Expression *expres2;
    
public:
    // ctors & dtor:
    BinaryExpression(string binary, Expression *expres1, Expression *expres2);
    ~BinaryExpression();
    
    // methods:
    void set(int value, string name) override;
    void unset(string name) override;
    string prettyprint() override;
    int evaluate() override;
    string getBinary();
    Expression *getExpres1();
    Expression *getExpres2();
    
    Expression * copy() override;
};

#endif
