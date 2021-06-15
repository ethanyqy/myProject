#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "scanner.h"
#include <cmath>
/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */

int main() {
    
    std::string line;
    int address = 0;
    std::unordered_map<std::string, int> labels;
    std::vector<std::string> inputs;
    try {
        while (getline(std::cin, line)) {
            
            inputs.push_back(line);
            std::vector<Token> tokenLine = scan(line);
            
            for (auto token = tokenLine.begin(); token != tokenLine.end(); ++token) {
                while (token->getKind() == Token::LABEL) {
                    std::string name;
                    name = token->getLexeme();
                    name = name.substr(0,name.length()-1);
                    if (labels.count(name) > 0) {
                        std::cerr << "ERROR: Redefinition of label: " << name << std::endl;
                        break;
                    }
                    std::cerr << name << " " << address << std::endl;
                    labels[name] = address;
                    ++token;
                    if (token == tokenLine.end()) {
                        break;
                    }
                }
                if (token == tokenLine.end()) {
                break;
                }
                if (token->getKind() == Token::ID || token->getKind() == Token::WORD) {
                    address += 4;
                    break;
                }
            }
        }
        for (auto a:inputs) {
            line = a;
            address = 0;
            std::vector<Token> tokenLine = scan(line);
            
            for (auto token = tokenLine.begin(); token != tokenLine.end(); ++token) {
                
                int numOfLabel = 0;
                while (token->getKind() == Token::LABEL) {
                    ++numOfLabel;
                    ++token;
                    if (token == tokenLine.end()) {
                        break;
                    }
                }
                if (token == tokenLine.end()) {
                    break;
                }
                if (token->getKind() == Token::WORD) {
                    if (tokenLine.size() - numOfLabel > 2) {
                        std::cerr << "ERROR: Wrong number of arguments for .word" << std::endl;
                        break;
                    }
                    ++token;
                    if (token == tokenLine.end()) {
                        std::cerr << "ERROR: Missing input after .word" << std::endl;
                        break;
                    }
                    if (token->getKind() == Token::INT || token->getKind() == Token::HEXINT) {
                        long i = token->toLong();
                        if (i < -1*pow(2, 32) || i > pow(2, 32)-1) {
                            std::cerr << "ERROR: Numerical value out of range" << std::endl;
                            break;
                        }
                        int instr = i;
                        unsigned char c = instr >> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                    } else if (token->getKind() == Token::ID) {
                        std::string name = token->getLexeme();
                        if (labels.count(name) == 0) {
                            std::cerr << "ERROR: Cannot find the label" << std::endl;
                        } else {
                            long i = labels[name];
                            if (i < -1*pow(2, 32) || i > pow(2, 32)-1) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                break;
                            }
                            int instr = i;
                            unsigned char c = instr >> 24;
                            std::cout << c;
                            c = instr >> 16;
                            std::cout << c;
                            c = instr >> 8;
                            std::cout << c;
                            c = instr;
                            std::cout << c;
                        }
                        address += 4;
                    } else {
                        std::cerr << "ERROR: Invalid input after .word" << std::endl;
                        break;
                    }
                } else if (token->getKind() == Token::ID){
                    if (token->getLexeme() == "jr" || token->getLexeme() == "jalr") {
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel > 2) {
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            break;
                        }
                        ++token;
                        if (token == tokenLine.end()) {
                            std::cerr << "ERROR: Missing input after "<< cmd << std::endl;
                            break;
                        }
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << std::endl;
                            break;
                        }
                        int s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            break;
                        }
                        int f;
                        if (cmd == "jr") {
                            f = 8;
                        } else {
                            f = 9;
                        }
                        int instr = (s << 21)|f;
                        unsigned char c = instr >> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    } else if(token->getLexeme() == "add" || token->getLexeme() == "sub" || token->getLexeme()  =="slt" || token->getLexeme() == "sltu"){
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel != 6) {
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$d invalid" << std::endl;
                            break;
                        }
                        int d = token->toLong();
                        if (d > 31 || d < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$s invalid" << std::endl;
                            break;
                        }
                        int s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            break;
                        }
                        int t = token->toLong();
                        if (t > 31 || t < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            break;
                        }
                        int f;
                        if (cmd == "add") {
                            f = 32;
                        } else if (cmd == "sub") {
                            f = 34;
                        } else if (cmd == "slt") {
                            f = 42;
                        } else {
                            f = 43;
                        }
                        int instr = (s << 21)|(t << 16)|(d << 11)|f;
                        unsigned char c = instr>> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    } else if (token->getLexeme() == "beq" || token->getLexeme() == "bne") {
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel != 6) {
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$s invalid" << std::endl;
                            break;
                        }
                        int s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            break;
                        }
                        int t = token->toLong();
                        if (t > 31 || t < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            break;
                        }
                        ++token;
                        if (token->getKind() != Token::INT && token->getKind() != Token::HEXINT && token->getKind() != Token::ID) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":i invalid" << std::endl;
                            break;
                        }
                        int i;
                        if (token->getKind() == Token::INT || token->getKind() == Token::HEXINT) {
                            i = token->toLong();
                            if (i < -1*pow(2, 32) || i > pow(2, 32)-1) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                break;
                            }
                        } else {
                            if (labels.count(token->getLexeme()) == 0) {
                                std::cerr << "ERROR: Cannot find the label" << std::endl;
                                break;
                            }
                            int val = labels[token->getLexeme()];
                            i = (val - address - 4) / 4;
                            if (i < -1*pow(2, 32) || i > pow(2, 32)-1) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                break;
                            }
                        }
                        int f;
                        if (cmd == "beq") {
                            f = 4;
                        } else {
                            f = 5;
                        }
                        int instr = (f << 26)|(s << 21)|(t << 16)|(i & 0xffff);
                        unsigned char c = instr>> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    }
                } else {
                    std::cerr << "ERROR: Invalid instruction" << std::endl;
                }
            }
        }
    }
        catch (ScanningFailure &f) {
            std::cerr << f.what() << std::endl;
            return 1;
        }
    return 0;
}

