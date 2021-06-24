//
//  graphicsdisplay.cpp
//  a4q5
//
//  Created by Ethan on 2019-07-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "graphicsdisplay.h"

GraphicsDisplay::GraphicsDisplay(size_t size): size(size) {
    window.fillRectangle(0, 0, 500, 500, Xwindow::Blue);
    cellSize = 500 / size;
}

void GraphicsDisplay::notify(Subject<Info, State> &whoFrom) {
    size_t r = whoFrom.getInfo().row;
    size_t c = whoFrom.getInfo().col;
    Colour colour = whoFrom.getInfo().colour;
    int fillColour;
    if (colour == Colour::Black) {
        fillColour = Xwindow::Black;
    } else {
        fillColour = Xwindow::White;
    }
    window.fillRectangle(r * cellSize, c * cellSize, cellSize, cellSize, fillColour);
}
