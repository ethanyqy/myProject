#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    vector<string> alphabet;
    alphabet.push_back("a");
    alphabet.push_back("b");
    alphabet.push_back("c");
    alphabet.push_back("d");
    alphabet.push_back("e");
    alphabet.push_back("f");
    alphabet.push_back("g");
    alphabet.push_back("h");
    alphabet.push_back("i");
    alphabet.push_back("j");
    alphabet.push_back("k");
    alphabet.push_back("l");
    alphabet.push_back("m");
    alphabet.push_back("n");
    alphabet.push_back("o");
    alphabet.push_back("p");
    alphabet.push_back("q");
    alphabet.push_back("r");
    alphabet.push_back("s");
    alphabet.push_back("t");
    alphabet.push_back("u");
    alphabet.push_back("v");
    alphabet.push_back("w");
    alphabet.push_back("x");
    alphabet.push_back("y");
    alphabet.push_back("z");
    alphabet.push_back("A");
    alphabet.push_back("B");
    alphabet.push_back("C");
    alphabet.push_back("D");
    alphabet.push_back("E");
    alphabet.push_back("F");
    alphabet.push_back("G");
    alphabet.push_back("H");
    alphabet.push_back("I");
    alphabet.push_back("J");
    alphabet.push_back("K");
    alphabet.push_back("L");
    alphabet.push_back("M");
    alphabet.push_back("N");
    alphabet.push_back("O");
    alphabet.push_back("P");
    alphabet.push_back("Q");
    alphabet.push_back("R");
    alphabet.push_back("S");
    alphabet.push_back("T");
    alphabet.push_back("U");
    alphabet.push_back("V");
    alphabet.push_back("W");
    alphabet.push_back("X");
    alphabet.push_back("Y");
    alphabet.push_back("Z");
    alphabet.push_back("0");
    alphabet.push_back("1");
    alphabet.push_back("2");
    alphabet.push_back("3");
    alphabet.push_back("4");
    alphabet.push_back("5");
    alphabet.push_back("6");
    alphabet.push_back("7");
    alphabet.push_back("8");
    alphabet.push_back("9");
    alphabet.push_back("(");
    alphabet.push_back(")");
    alphabet.push_back("{");
    alphabet.push_back("}");
    alphabet.push_back("[");
    alphabet.push_back("]");
    alphabet.push_back("%");
    alphabet.push_back("&");
    alphabet.push_back("*");
    alphabet.push_back("-");
    alphabet.push_back("=");
    alphabet.push_back("+");
    alphabet.push_back("!");
    alphabet.push_back(",");
    alphabet.push_back(";");
    alphabet.push_back("/");
    alphabet.push_back("<");
    alphabet.push_back(">");
    
    vector<string> states;
    states.push_back("start");
    states.push_back("w");
    states.push_back("wa");
    states.push_back("wai");
    states.push_back("i");
    states.push_back("in");
    states.push_back("e");
    states.push_back("el");
    states.push_back("els");
    states.push_back("wh");
    states.push_back("whi");
    states.push_back("whil");
    states.push_back("p");
    states.push_back("pr");
    states.push_back("pri");
    states.push_back("prin");
    states.push_back("print");
    states.push_back("printl");
    states.push_back("r");
    states.push_back("re");
    states.push_back("ret");
    states.push_back("retu");
    states.push_back("retur");
    states.push_back("N");
    states.push_back("NU");
    states.push_back("NUL");
    states.push_back("n");
    states.push_back("ne");
    states.push_back("d");
    states.push_back("de");
    states.push_back("del");
    states.push_back("dele");
    states.push_back("delet");
    states.push_back("=");
    states.push_back("!");
    states.push_back("<");
    states.push_back(">");
    states.push_back("keyword1");
    states.push_back("keyword2");
    states.push_back("num");
    states.push_back("id");
    
    vector<string> digits;
    digits.push_back("0");
    digits.push_back("1");
    digits.push_back("2");
    digits.push_back("3");
    digits.push_back("4");
    digits.push_back("5");
    digits.push_back("6");
    digits.push_back("7");
    digits.push_back("8");
    digits.push_back("9");
    
    vector<string> az;
    az.push_back("a");
    az.push_back("b");
    az.push_back("c");
    az.push_back("d");
    az.push_back("e");
    az.push_back("f");
    az.push_back("g");
    az.push_back("h");
    az.push_back("i");
    az.push_back("j");
    az.push_back("k");
    az.push_back("l");
    az.push_back("m");
    az.push_back("n");
    az.push_back("o");
    az.push_back("p");
    az.push_back("q");
    az.push_back("r");
    az.push_back("s");
    az.push_back("t");
    az.push_back("u");
    az.push_back("v");
    az.push_back("w");
    az.push_back("x");
    az.push_back("y");
    az.push_back("z");
    az.push_back("A");
    az.push_back("B");
    az.push_back("C");
    az.push_back("D");
    az.push_back("E");
    az.push_back("F");
    az.push_back("G");
    az.push_back("H");
    az.push_back("I");
    az.push_back("J");
    az.push_back("K");
    az.push_back("L");
    az.push_back("M");
    az.push_back("N");
    az.push_back("O");
    az.push_back("P");
    az.push_back("Q");
    az.push_back("R");
    az.push_back("S");
    az.push_back("T");
    az.push_back("U");
    az.push_back("V");
    az.push_back("W");
    az.push_back("X");
    az.push_back("Y");
    az.push_back("Z");
    
    for(auto s:states){
        for(auto a:alphabet) {
            if (s == "start") {
                if (find(states.begin(), states.end(), a) != states.end()) {
                    cout << s << " " << a << " " << a << endl;
                } else {
                    if (not(find(digits.begin(), digits.end(), a) != digits.end())
                        and (find(az.begin(), az.end(), a) != az.end())) {
                        cout << s << " " << a << " " << "id" << endl;
                    } else if (find(digits.begin(), digits.end(), a) != digits.end()){
                        cout << s << " " << a << " " << "num" << endl;
                    } else {
                        cout << s << " " << a << " " << "keyword2" << endl;
                    }
                }
            } else if(s == "w") {
                if (a == "a") {
                    cout << s << " " << a << " " << "wa" << endl;
                } else if(a == "h") {
                    cout << s << " " << a << " " << "wh" << endl;
                }else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                    }
            }else if(s == "wa") {
                if (a == "i") {
                    cout << s << " " << a << " " << "wai" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "wai") {
                if (a == "n") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "i") {
                if (a == "n") {
                    cout << s << " " << a << " " << "in" << endl;
                } else if (a == "f"){
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "in") {
                if (a == "t") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "e") {
                if (a == "l") {
                    cout << s << " " << a << " " << "el" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "el") {
                if (a == "s") {
                    cout << s << " " << a << " " << "els" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "els") {
                if (a == "e") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "wh") {
                if (a == "i") {
                    cout << s << " " << a << " " << "whi" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "whi") {
                if (a == "l") {
                    cout << s << " " << a << " " << "whil" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "whil") {
                if (a == "e") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "p") {
                if (a == "r") {
                    cout << s << " " << a << " " << "pr" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "pr") {
                if (a == "i") {
                    cout << s << " " << a << " " << "pri" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "pri") {
                if (a == "n") {
                    cout << s << " " << a << " " << "prin" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "prin") {
                if (a == "t") {
                    cout << s << " " << a << " " << "print" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "print") {
                if (a == "l") {
                    cout << s << " " << a << " " << "printl" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "printl") {
                if (a == "n") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "r") {
                if (a == "e") {
                    cout << s << " " << a << " " << "re" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "re") {
                if (a == "t") {
                    cout << s << " " << a << " " << "ret" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "ret") {
                if (a == "u") {
                    cout << s << " " << a << " " << "retu" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "retu") {
                if (a == "r") {
                    cout << s << " " << a << " " << "retur" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "retur") {
                if (a == "n") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "N") {
                if (a == "U") {
                    cout << s << " " << a << " " << "NU" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "NU") {
                if (a == "L") {
                    cout << s << " " << a << " " << "NUL" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "NUL") {
                if (a == "L") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "n") {
                if (a == "e") {
                    cout << s << " " << a << " " << "ne" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "ne") {
                if (a == "w") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "d") {
                if (a == "e") {
                    cout << s << " " << a << " " << "de" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "de") {
                if (a == "l") {
                    cout << s << " " << a << " " << "del" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "del") {
                if (a == "e") {
                    cout << s << " " << a << " " << "dele" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "dele") {
                if (a == "t") {
                    cout << s << " " << a << " " << "delet" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "delet") {
                if (a == "e") {
                    cout << s << " " << a << " " << "keyword1" << endl;
                } else if(find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }else if(s == "=") {
                if (a == "=") {
                    cout << s << " " << a << " " << "keyword2" << endl;
                }
            }else if(s == "!") {
                if (a == "=") {
                    cout << s << " " << a << " " << "keyword2" << endl;
                }
            }else if(s == ">") {
                if (a == "=") {
                    cout << s << " " << a << " " << "keyword2" << endl;
                }
            }else if(s == "<") {
                if (a == "=") {
                    cout << s << " " << a << " " << "keyword2" << endl;
                }
            }else if(s == "num") {
                if (find(digits.begin(), digits.end(), a) != digits.end()) {
                    cout << s << " " << a << " " << "num" << endl;
                }
            }else if (s == "keyword1") {
                if (find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            } else if (s == "id") {
                if (find(digits.begin(), digits.end(), a) != digits.end()
                    || find(az.begin(), az.end(), a) != az.end()) {
                    cout << s << " " << a << " " << "id" << endl;
                }
            }
        }
    }
}
