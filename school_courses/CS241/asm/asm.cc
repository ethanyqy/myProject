#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "scanner.h"
#include <cmath>
#include "limits.h"
#include <algorithm>

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
    std::vector<std::string> used_labels;
    try {
        // loop through the input first to get all labels
        // read all lablels and store in the symbol table(unordered_map)
        while (getline(std::cin, line)) {
            inputs.push_back(line);// store all inputs in a vector for the second loop
            
            std::vector<Token> tokenLine = scan(line);
            for (auto token = tokenLine.begin(); token != tokenLine.end(); ++token) {
                while (token->getKind() == Token::LABEL) {// check multiple labels in a line
                    std::string name;
                    name = token->getLexeme();
                    name = name.substr(0,name.length()-1);// trim the label's name
                    if (labels.count(name) > 0) {// label already exists in the symbol table
                        std::cerr << "ERROR: Redefinition of label: " << name << std::endl;
                        return 1;
                    }
                    std::cerr << name << " " << address << std::endl;// print the label to std::cerr
                    labels[name] = address;// store in unordered_map
                    ++token;// next token in the line
                    if (token == tokenLine.end()) {// check if all tokens have been checked, i.e. a line of labels
                        break;
                    }
                }
                if (token == tokenLine.end()) {// if it's a line of labels, address no change
                break;
                }
                // if after labels there is some instruction, address + 4 and move to the next line
                if (token->getKind() == Token::WORD) {// check if .word uses undefined label
                    if ((token+1) != tokenLine.end()) {
                        ++token;
                        if (token->getKind() == Token::ID) {
                            used_labels.push_back(token->getLexeme());
                        }
                    }
                    address += 4;
                    break;// skip the rest of the tokenline
                } else if (token->getKind() == Token::ID) {
                    if (token->getLexeme() == "beq" || token->getLexeme() == "bne") {// check if beq or bne uses undefined label
                        int n = 4;
                        ++token;
                        while (token != tokenLine.end()) {
                            ++token;
                            --n;
                            if (n == 0) break;
                        }
                        if (token != tokenLine.end() && n == 0) {
                            if (token->getKind() == Token::ID) {
                                used_labels.push_back(token->getLexeme());
                            }
                        }
                    }
                    address += 4;
                    break;// skip the rest of the tokenline
                }
            }
        }
        
        for (auto u:used_labels) {
            if (not (labels.count(u) > 0)) {
                std::cerr << "ERROR: Cannot find the label " << u << std::endl;
                return 1;
            }
        }
        
        address = 0;// looping for the second time to load the instructions, need to reset address
        for (auto a:inputs) {
            line = a;// get the line from the vector "inputs"
            
            std::vector<Token> tokenLine = scan(line);
            for (auto token = tokenLine.begin(); token != tokenLine.end(); ++token) {
                
                int numOfLabel = 0;// count for labels in the front of a tokenline
                while (token->getKind() == Token::LABEL) {
                    ++numOfLabel;
                    ++token;
                    if (token == tokenLine.end()) {
                        break;
                    }
                }
                if (token == tokenLine.end()) {// line of labels
                    break;
                }
                if (token->getKind() == Token::WORD) {// .word readed
                    if (tokenLine.size() - numOfLabel != 2) {// check if one argument follows .word
                        std::cerr << "ERROR: Wrong number of arguments for .word" << std::endl;
                        return 1;
                    }
                    ++token;
                    if (token->getKind() == Token::INT || token->getKind() == Token::HEXINT) {// check if the argument is numerical value
                        long i = token->toLong();
                        if (i < INT_MIN || i > UINT_MAX) {// range check
                            std::cerr << "ERROR: Numerical value out of range" << std::endl;
                            return 1;
                        }
                        long instr;
                        instr = i;// output
                        unsigned char c = instr>> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    } else if (token->getKind() == Token::ID) {// check if the argument is labeled
                        std::string name = token->getLexeme();
                        if (labels.count(name) == 0) {// check if label exists in the symbol table
                            std::cerr << "ERROR: Cannot find the label" << std::endl;
                            return 1;
                        } else {
                            long i = labels[name];
                            if (i < INT_MIN || i > UINT_MAX) {// range check
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                return 1;
                            }
                            long instr;
                            instr = i;// output
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
                        std::cerr << "ERROR: Invalid input after .word" << std::endl;
                        return 1;
                    }
                    
                } else if (token->getKind() == Token::ID){// instructions readed(jr, jalr ...)
                    if (token->getLexeme() == "jr" || token->getLexeme() == "jalr") {
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel != 2) {// check if cmd followed by one argument
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {// check if the argument is register
                            std::cerr << "ERROR: Invalid input after "<< cmd << std::endl;
                            return 1;
                        }
                        int64_t s = token->toLong();// store register number
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;// range check
                            return 1;
                        }
                        int f;// function value
                        if (cmd == "jr") {
                            f = 8;
                        } else {
                            f = 9;
                        }
                        long instr = (s << 21)|f;// output
                        unsigned char c = instr>> 24;
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
                        if (tokenLine.size() - numOfLabel != 6) {// check format: cmd $d , $s , $t
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$d invalid" << std::endl;
                            return 1;
                        }
                        int64_t d = token->toLong();
                        if (d > 31 || d < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$s invalid" << std::endl;
                            return 1;
                        }
                        int64_t s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            return 1;
                        }
                        int64_t t = token->toLong();
                        if (t > 31 || t < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
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
                        long instr = (s << 21)|(t << 16)|(d << 11)|f;
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
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$s invalid" << std::endl;
                            return 1;
                        }
                        int64_t s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            return 1;
                        }
                        int64_t t = token->toLong();
                        if (t > 31 || t < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::INT && token->getKind() != Token::HEXINT && token->getKind() != Token::ID) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":i invalid" << std::endl;
                            return 1;
                        }
                        int64_t i;
                        if (token->getKind() == Token::INT) {
                            i = token->toLong();
                            if (token->toLong() < -32768 || token->toLong() > 32767) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                return 1;
                            }
                        } else if (token->getKind() == Token::HEXINT) {
                            i = token->toLong();
                            if (token->toLong() > 0xffff) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                return 1;
                            }
                        } else {
                            if (not (labels.count(token->getLexeme()) > 0)) {
                                std::cerr << "ERROR: Cannot find the label" << std::endl;
                                return 1;
                            }
                            int dst = labels[token->getLexeme()];
                            i = (dst - address - 4) / 4;
                            if (i < -32768|| i > 32767) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                return 1;
                            }
                        }
                        int f;
                        if (cmd == "beq") {
                            f = 4;
                        } else {
                            f = 5;
                        }
                        long instr = (f << 26)|(s << 21)|(t << 16)|(i & 0xffff);
                        unsigned char c = instr>> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    } else if (token->getLexeme() == "lis" || token->getLexeme() == "mflo" || token->getLexeme() == "mfhi") {
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel != 2) {
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$d invalid" << std::endl;
                            return 1;
                        }
                        int64_t d = token->toLong();
                        if (d > 31 || d < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        int f;
                        if (cmd == "lis") {
                            f = 20;
                        } else if (cmd == "mflo") {
                            f = 18;
                        } else {
                            f = 16;
                        }
                        long instr = (d << 11)|f;
                        unsigned char c = instr>> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    } else if (token->getLexeme() == "mult" || token->getLexeme() == "multu" || token->getLexeme() == "div" || token->getLexeme() == "divu") {
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel != 4) {
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$s invalid" << std::endl;
                            return 1;
                        }
                        int64_t s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            return 1;
                        }
                        int64_t t = token->toLong();
                        if (t > 31 || t < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        int64_t f;
                        if (cmd == "mult") {
                            f = 24;
                        } else if (cmd == "multu") {
                            f = 25;
                        } else if (cmd == "div") {
                            f = 26;
                        } else {
                            f = 27;
                        }
                        long instr = (0 << 26)|(s << 21)|(t << 16)|f;
                        unsigned char c = instr>> 24;
                        std::cout << c;
                        c = instr >> 16;
                        std::cout << c;
                        c = instr >> 8;
                        std::cout << c;
                        c = instr;
                        std::cout << c;
                        address += 4;
                    } else if (token->getLexeme() == "lw" || token->getLexeme() == "sw") {
                        std::string cmd = token->getLexeme();
                        if (tokenLine.size() - numOfLabel != 7) {
                            std::cerr << "ERROR: Wrong number of arguments for "<< cmd << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            return 1;
                        }
                        int64_t t = token->toLong();
                        if (t > 31 || t < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::COMMA) {
                            std::cerr << "ERROR: Expected comma, given " << token->getLexeme() << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::INT && token->getKind() != Token::HEXINT) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":i invalid" << std::endl;
                            return 1;
                        }
                        int i;
                        if (token->getKind() == Token::INT) {
                            i = token->toLong();
                            if (token->toLong() < -32768 || token->toLong() > 32767) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                return 1;
                            }
                        } else {
                            i = token->toLong();
                            if (token->toLong() > 0xffff) {
                                std::cerr << "ERROR: Numerical value out of range" << std::endl;
                                return 1;
                            }
                        }
                        ++token;
                        if (token->getKind() != Token::LPAREN) {
                            std::cerr << "ERROR: Missing left parenthesis" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::REG) {
                            std::cerr << "ERROR: Invalid input after "<< cmd << ":$t invalid" << std::endl;
                            return 1;
                        }
                        int64_t s = token->toLong();
                        if (s > 31 || s < 0) {
                            std::cerr << "ERROR: Register out of range" << std::endl;
                            return 1;
                        }
                        ++token;
                        if (token->getKind() != Token::RPAREN) {
                            std::cerr << "ERROR: Missing right parenthesis" << std::endl;
                            return 1;
                        }
                        int f;
                        if (cmd == "lw") {
                            f = 35;
                        } else {
                            f = 43;
                        }
                        long instr = (f << 26)|(s << 21)|(t << 16)|(i & 0xffff);
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
                    return 1;
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
