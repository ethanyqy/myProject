#include <iostream>
#include <string>
#include <sstream>
using std::string;
using std::istream;
using std::ostream;


//Skip the grammar part of the input.
void skipLine(istream &in) {
    string s;
    getline(in, s);
}

void skipGrammar(istream &in) {
    int i, numTerm, numNonTerm, numRules;
    
    // read the number of terminals and move to the next line
    in >> numTerm;
    skipLine(in);
    
    // skip the lines containing the terminals
    for (i = 0; i < numTerm; i++) {
        skipLine(in);
    }
    
    // read the number of non-terminals and move to the next line
    in >> numNonTerm;
    skipLine(in);
    
    // skip the lines containing the non-terminals
    for (i = 0; i < numNonTerm; i++) {
        skipLine(in);
    }
    
    // skip the line containing the start symbol
    skipLine(in);
    
    // read the number of rules and move to the next line
    in >> numRules;
    skipLine(in);
    
    // skip the lines containing the production rules
    for (i = 0; i < numRules; i++) {
        skipLine(in);
    }
}

string trim(const string &str) {
    size_t begin = str.find_first_not_of(" \t\n");
    if (begin == string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n");
    
    return str.substr(begin, end - begin + 1);
}

// Prints the derivation with whitespace trimmed.
int evaluateDerivation(istream &in) {
    string line;
    int value = 0;
    
    while (getline(in, line)) {
        trim(line);
        std::istringstream iss(line);
        std::string op, s;
        iss >> op;
        if (op == "term") {
            iss >> s;
            if (s == "id") {
                return 42;
            } else {
                return evaluateDerivation(in);
            }
        }
        if (op == "S"){
            return evaluateDerivation(in);
        }
        if (op == "expr") {
            iss >> s;
            if (s == "term") {
                return evaluateDerivation(in);
            } else {
                value += evaluateDerivation(in);
                value -= evaluateDerivation(in);
                return value;
            }
        }
    }
    return value;
}

// Reads a .cfg file and prints the left-canonical
// derivation without leading or trailing spaces.
int main() {
    skipGrammar(std::cin);
    std::cout << evaluateDerivation(std::cin) << std::endl;
}
