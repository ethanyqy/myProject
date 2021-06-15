//
//  textdisplay.cpp
//  a4q4
//
//  Created by Ethan on 2019-07-16.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "textdisplay.h"

TextDisplay::TextDisplay(int n): gridSize(n) {
    for (int i = 0; i < n; ++i) {
        std::vector<char> temp;
        for (int j = 0; j < n; ++j) {
            //initialize cells with "-"
            temp.push_back('-');
        }
        theDisplay.push_back(temp);
    }
}

void TextDisplay::notify(Subject<Info, State> &whoNotified) {
    size_t r = whoNotified.getInfo().row;
    size_t c = whoNotified.getInfo().col;
    Colour colour = whoNotified.getInfo().colour;
    if (colour == Colour::Black) {
        theDisplay.at(r).at(c) = 'B';
    } else if (colour == Colour::White) {
        theDisplay.at(r).at(c) = 'W';
    }
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    for (int i = 0; i < td.gridSize; ++i) {
        for (int j = 0; j < td.gridSize; ++j) {
            out << td.theDisplay.at(i).at(j);
        }
        out << std::endl;
    }
    return out;
}
