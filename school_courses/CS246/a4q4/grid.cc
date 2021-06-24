//
//  grid.cpp
//  a4q4
//
//  Created by Ethan on 2019-07-16.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "grid.h"


Grid::~Grid() {
    // delete heap-allocated fields
    delete td;
    delete ob;
}

void Grid::setObserver(Observer<Info, State> *ob) {
    ob = ob;
}

bool Grid::isFull() const {
    // check if all cells have been filled
    return (moves == length * length);
}

Colour Grid::whoWon() const {
    int black = 0;
    int white = 0;
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
            if (theGrid.at(i).at(j).getInfo().colour == Colour::Black) {
                ++black;
            } else {
                ++white;
            }
        }
    }
    if (black > white) {
        // Black wins!
        return Colour::Black;
    } else if (black < white) {
        // White wins!
        return Colour::White;
    } else {
        // There is a tie
        return Colour::NoColour;
    }
}

void Grid::init(size_t len) {
    if (td) {
        // clear the old grid
        theGrid.clear();
        delete td;
        delete ob;
    }
    // initialize the moves and length
    moves = 0;
    length = len;
    
    // initialize the textDisplay
    td = new TextDisplay(int(len));
    ob = new GraphicsDisplay(len);
    
    // initialize a new grid
    for (int i = 0; i < len; ++i) {
        std::vector<Cell> temp;
        for (int j = 0; j < len; ++j) {
            temp.push_back(Cell(i,j));
        }
        theGrid.push_back(temp);
    }
    
    // attach observes to every cell
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            theGrid.at(i).at(j).attach(td);
            theGrid.at(i).at(j).attach(ob);
            if (i == 0) {
                if (j == 0) {
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j+1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j+1));
                } else if (j == len - 1) {
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j-1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j-1));
                } else {
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j-1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j+1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j-1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j));
                    theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j+1));
                }
            } else if (i == len - 1) {
                if (j == 0) {
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j+1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j+1));
                } else if (j == len - 1) {
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j-1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j-1));
                } else {
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j-1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i).at(j+1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j-1));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j));
                    theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j+1));
                }
            } else if (j == 0) {
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j));
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j+1));
                theGrid.at(i).at(j).attach(&theGrid.at(i).at(j+1));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j+1));
            } else if (j == len - 1) {
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j));
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j-1));
                theGrid.at(i).at(j).attach(&theGrid.at(i).at(j-1));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j-1));
            } else {
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j-1));
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j));
                theGrid.at(i).at(j).attach(&theGrid.at(i-1).at(j+1));
                theGrid.at(i).at(j).attach(&theGrid.at(i).at(j-1));
                theGrid.at(i).at(j).attach(&theGrid.at(i).at(j+1));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j-1));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j));
                theGrid.at(i).at(j).attach(&theGrid.at(i+1).at(j+1));
            }
        }
    }
    
    // initialize the central 4 cells
    int center = int(len) / 2 - 1;
    setPiece(center, center, Colour::Black);
    setPiece(center, center + 1, Colour::White);
    setPiece(center + 1, center, Colour::White);
    setPiece(center + 1, center + 1, Colour::Black);
}

void Grid::setPiece(size_t r, size_t c, Colour colour) {
    if (theGrid.at(r).at(c).getInfo().colour == Colour::NoColour) { // the input is valid,i.e. the cell has not been filled (out_of_range will be caught later)
        theGrid.at(r).at(c).setPiece(colour);
        ++moves;
    } else {
        throw "invalid";
    }
}

void Grid::toggle(size_t r, size_t c) {
    theGrid.at(r).at(c).toggle();
}

std::ostream &operator<<(std::ostream &out, const Grid &g) {
    out << *(g.td);
    return out;
}
