//
//  expression.cpp
//  a4q1
//
//  Created by Ethan on 2019-07-13.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "expression.hpp"

Expression::~Expression() {}


// LoneInteger methods
LoneInteger::LoneInteger(int num): num{num} {}

LoneInteger::~LoneInteger() {}

void LoneInteger::set(int value, string name) {}

void LoneInteger::unset(string name) {}

string LoneInteger::prettyprint() {
    return std::to_string(num);
}

int LoneInteger::evaluate() {
    return num;
}

Expression * LoneInteger::copy() {
    Expression * cpy = new LoneInteger{num};
    return cpy;
}


// Variable methods
Variable::Variable(string name): name{name}, initialization{false} {}

Variable::Variable(string name, bool initialization, int value): name(name), initialization(initialization), value(value) {}

Variable::~Variable() {}

void Variable::set(int value, string name) {
    if (this->name == name){
        this->value = value;
        initialization = true;
    }
}

void Variable::unset(string name) {
    if (this->name == name){
        initialization = false;
    }
}

string Variable::prettyprint() {
    if (!getInit()) {
        return getName();
    } else {
        return std::to_string(getValue());
    }
}

int Variable::evaluate() {
    if (!getInit()) {
        throw name;
    } else {
        return getValue();
    }
}

string Variable::getName() {
    return name;
}

bool Variable::getInit() {
    return initialization;
}

int Variable::getValue() {
    return value;
}

Expression * Variable::copy() {
    Expression * cpy = new Variable{name, initialization, value};
    return cpy;
}


// UnaryExpression methods
UnaryExpression::UnaryExpression(string unary, Expression *expres): unary{unary}, expres{expres} {}

UnaryExpression::~UnaryExpression() {
    delete expres;
}

void UnaryExpression::set(int value, string name) {
    expres->set(value, name);
}

void UnaryExpression::unset(string name) {
    expres->unset(name);
}

string UnaryExpression::prettyprint() {
    if (getUnary() == "NEG") {
        return ("-" + getExpres()->prettyprint());
    } else {
        return ("|" + getExpres()->prettyprint() + "|");
    }
}

int UnaryExpression::evaluate() {
    if (getUnary() == "NEG") {
        return (-1 * getExpres()->evaluate());
    } else {
        if (getExpres()->evaluate() < 0) {
            return (-1 * getExpres()->evaluate());
        } else {
            return getExpres()->evaluate();
        }
    }
}

string UnaryExpression::getUnary() {
    return unary;
}

Expression *UnaryExpression::getExpres() {
    return expres;
}

Expression *UnaryExpression::copy() {
    Expression *cpy = new UnaryExpression{unary, expres->copy()};
    return cpy;
}


// BinaryExpression methods
BinaryExpression::BinaryExpression(string binary, Expression *expres1, Expression *expres2): binary{binary}, expres1{expres1}, expres2{expres2} {}

BinaryExpression::~BinaryExpression() {
    delete expres1;
    delete expres2;
}


void BinaryExpression::set(int value, string name) {
    expres1->set(value, name);
    expres2->set(value, name);
}

void BinaryExpression::unset(string name) {
    expres1->unset(name);
    expres2->unset(name);
}

string BinaryExpression::prettyprint() {
    if (getBinary() == "+") {
        return ("(" + getExpres1()->prettyprint() + " + " + getExpres2()->prettyprint() + ")");
    } else if (getBinary() == "-") {
        return ("(" + getExpres1()->prettyprint() + " - " + getExpres2()->prettyprint() + ")");
    } else if (getBinary() == "*") {
        return ("(" + getExpres1()->prettyprint() + " * " + getExpres2()->prettyprint() + ")");
    } else {
        return ("(" + getExpres1()->prettyprint() + " / " + getExpres2()->prettyprint() + ")");
    }
}

int BinaryExpression::evaluate() {
    if (getBinary() == "+") {
        return (getExpres1()->evaluate() + getExpres2()->evaluate());
    } else if (getBinary() == "-") {
        return (getExpres1()->evaluate() - getExpres2()->evaluate());
    } else if (getBinary() == "*") {
        return (getExpres1()->evaluate() * getExpres2()->evaluate());
    } else {
        return (getExpres1()->evaluate() / getExpres2()->evaluate());
    }
}

string BinaryExpression::getBinary() {
    return binary;
}

Expression *BinaryExpression::getExpres1() {
    return expres1;
}

Expression *BinaryExpression::getExpres2() {
    return expres2;
}

Expression *BinaryExpression::copy() {
    Expression *cpy = new BinaryExpression{binary, expres1->copy(), expres2->copy()};
    return cpy;
}
