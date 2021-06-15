//
//  main.cpp
//  a4q4
//
//  Created by Ethan on 2019-07-16.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include <iostream>
#include <string>
// You may include other allowed headers, as needed
#include "grid.h"
#include "state.h"
using namespace std;

// Do not remove any code; do not add code other than where indicated.

int main(int argc, char *argv[]) {
    cin.exceptions(ios::eofbit|ios::failbit);
    string cmd;
    Grid g;
    Colour init = Colour::White;
    
    // Add code here
    
    try {
        while (true) {
            cin >> cmd;
            if (cmd == "new") {
                int n;
                cin >> n;
                // Add code here
                if (n < 4) continue;
                if (n % 2 != 0) continue;
                g.init(n);
                cout << g;
            }
            else if (cmd == "play") {
                int r = 0, c = 0;
                cin >> r >> c;
                try {
                    // check if the move is valid
                    g.setPiece(r, c, init);
                }
                catch (out_of_range) {
                    continue;
                }
                catch (string s) {
                    continue;
                }
                // print out the grid after each step
                cout << g;
                //check if the game is over
                if (g.isFull()) {
                    if (g.whoWon() == Colour::Black) {
                        cout << "Black Wins!" << endl;
                    } else if (g.whoWon() == Colour::White) {
                        cout << "White Wins!" << endl;
                    } else {
                        cout << "Tie!" << endl;
                    }
                    break;
                }
            }
            // switch the player
            if (init == Colour::Black) {
                init = Colour::White;
            } else {
                init = Colour::Black;
            }
        }
    }
    catch (ios::failure &) {
        return 0;
    }  // Any I/O failure quits
}
