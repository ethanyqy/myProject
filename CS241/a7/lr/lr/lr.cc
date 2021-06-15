#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

// error message class
class Failure {
    std::string message;
    
public:
    Failure(string message): message(move(message)) {}
    
    // Returns the message associated with the exception.
    const std::string &what() const{return message;}
};

string trim(const string &str) {
    size_t begin = str.find_first_not_of(" \t\n");
    if (begin == string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n");
    
    return str.substr(begin, end - begin + 1);
}

//Skip the grammar part of the input.
void skipLine(istream &in) {
    string s;
    getline(in, s);
}

void skipGrammar(istream &in) {
    int i, numTerm, numNonTerm;
    
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
    
}


int main(){
    bool b = 1;
    int i,numProductRule,numStates,numTrans,state=0,correct=0;
    string line,nextChar;
    vector<string> sequence,processing;
    vector<int> printing,preStates;
    unordered_map<int, vector<string>> rules;
    unordered_map<int, vector<string>> trans1;
    unordered_map<int, vector<int>> trans2;
    preStates.push_back(state);
    try {
        skipGrammar(cin);
        cin >> numProductRule;
        cin.ignore();
        for (i = 0; i < numProductRule; i++) {
            getline(cin, line);
            stringstream ss(line);
            string s;
            vector<string> r;
            while(getline(ss, s, ' ')) {
                r.push_back(s);
            }
            rules[i] = r;
        }
        cin >> numStates;
        cin >> numTrans;
        cin.ignore();
        for(i = 0; i < numTrans; i++) {
            getline(cin, line);
            stringstream ss(line);
            string s,t;
            int n1,n2;
            ss >> n1;
            ss >> s;
            ss >> t;
            ss >> n2;
            if (t == "reduce") {
                n2 *= -1;
            }
            trans1[n1].push_back(s);
            trans2[n1].push_back(n2);
        }
        
        string s;
        vector<string> temp;
        while(getline(cin,line)) {
            if (cin.eof()){
                break;
            }
            line = trim(line);
            stringstream ss(line);
            while(getline(ss, s, ' ')) {
                temp.push_back(s);
            }
        }
        while (temp.size()>0) {
            s = temp[temp.size()-1];
            sequence.push_back(s);
            temp.pop_back();
        }
        while(sequence.size() > 0) {
            if (b) {
                nextChar = sequence[sequence.size()-1];
                sequence.pop_back();
                correct++;
            }
            vector<string>::iterator iter = find(trans1[state].begin(), trans1[state].end(), nextChar);
            if (iter == trans1[state].end()) {
                throw Failure("ERROR at "+to_string(correct));
            }
            int index = distance(trans1[state].begin(), iter);
            state = trans2[state][index];
            
            if (state < 0) {
                int rule = state * -1;
                int n = rules[rule].size() - 1;
                sequence.push_back(nextChar);
                correct--;
                for (i = 0; i < n; i++) {
                    
                    processing.pop_back();
                    preStates.pop_back();
                }
                nextChar = rules[rule][0];
                printing.push_back(rule);
                state = preStates[preStates.size()-1];
                b = 0;
            } else {
                preStates.push_back(state);
                processing.push_back(nextChar);
                b = 1;
            }
        }
        // accepted
        printing.push_back(0);
        for (i = 0; i < printing.size(); i++) {
            for (int j = 0; j < rules[printing[i]].size(); j++) {
                cout << rules[printing[i]][j];
                if (j != rules[printing[i]].size() - 1) {
                    cout << " ";
                } else {
                    cout << endl;
                }
            }
        }
        rules.clear();
        trans1.clear();
        trans2.clear();
        return 0;
        
    } catch (Failure &f) {
        cerr << f.what() << endl;
        rules.clear();
        trans1.clear();
        trans2.clear();
        return 1;
    }
    
}





