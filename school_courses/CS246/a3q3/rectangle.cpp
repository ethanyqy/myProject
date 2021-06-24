//
//  rectangle.cpp
//  a3q3
//
//  Created by Ethan on 2019-06-19.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "rectangle.hpp"

Rectangle::Rectangle( Point upperLeft, int height, int width, Colour colour ): pt(upperLeft), h(height), w(width), c(colour) { }

void Rectangle::translate( int x, int y ) {
    pt.xi += x;
    pt.yi += y;
}

void Rectangle::scale( float heightFactor, float widthFactor ) {
    h = h * heightFactor;
    w = w * widthFactor;
}

void Rectangle::change( Colour newColour ) {
    c = newColour;
}

Colour Rectangle::colour() const {
    return c;
}

Point Rectangle::point() const {
    return pt;
}

int Rectangle::width() const {
    return w;
}

int Rectangle::height() const {
    return h;
}

std::istream & operator>>( std::istream & in, Rectangle & rectangle ) {
    char newc;
    in >> newc;
    if ( newc == 'r' ) {
        rectangle.change( Colour::Red );
    } else if ( newc == 'g' ) {
        rectangle.change( Colour::Green );
    } else if ( newc == 'b' ) {
        rectangle.change( Colour::Blue );
    } else if ( newc == 'o' ) {
        rectangle.change( Colour::Orange );
    } else if ( newc == 'y' ) {
        rectangle.change( Colour::Yellow );
    } else if ( newc == 'a' ) {
        rectangle.change( Colour::Black );
    } else {
        rectangle.change( Colour::White );
    }
    int x = 0;
    int y = 0;
    in >> x;
    in >> y;
    rectangle.translate( x, y );
    in >> rectangle.h;
    in >> rectangle.w;
    return in;
}

std::ostream & operator<<( std::ostream & out, const Rectangle & rectangle ) {
    out << '[';
    if ( rectangle.colour() == Colour::Red ) {
        out << "Red ";
    } else if ( rectangle.colour() == Colour::Green ) {
        out << "Green ";
    } else if ( rectangle.colour() == Colour::Blue ) {
        out << "Blue ";
    } else if ( rectangle.colour() == Colour::Orange ) {
        out << "Orange ";
    } else if ( rectangle.colour() == Colour::Yellow ) {
        out << "Yellow ";
    } else if ( rectangle.colour() == Colour::White ) {
        out << "White ";
    } else  {
        out << "Black ";
    }
    out << rectangle.point() << " ";
    out << rectangle.height() << "x" << rectangle.width() << ']';
    return out;
}
