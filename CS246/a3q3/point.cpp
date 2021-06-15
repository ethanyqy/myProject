//
//  point.cpp
//  a3q3
//
//  Created by Ethan on 2019-06-19.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "point.hpp"


Point::Point( int x, int y ) : xi( x ), yi( y ) { }

int Point::x() const {
    return xi;
}

int Point::y() const {
    return yi;
}

Point Point::operator+( const Point & other ) const {
    Point newpoint( xi+other.xi, yi + other.yi);
    return newpoint;
}

std::ostream & operator<<( std::ostream & out, const Point & point ) {
    out << '(' << point.x() << ',' << point.y() << ')' ;
    return out;
}
