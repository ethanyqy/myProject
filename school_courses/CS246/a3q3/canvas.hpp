//
//  canvas.hpp
//  a3q3
//
//  Created by Ethan on 2019-06-19.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _CANVAS_H
#define _CANVAS_H
#include "rectangle.hpp"

struct rnode {
    Rectangle record;
    rnode *next;
    
    rnode( Rectangle record ) : record(record), next(nullptr) {}
    
    rnode( const rnode & other ) : record( other.record.pt, other.record.h, other.record.w, other.record.c ) {
        if ( other.next ) {
            next = new rnode(*other.next);
        } else {
            next = nullptr;
        }
    }
    
    ~rnode() {
        delete next;
    }
};

// *The upper left-hand corner of a Canvas is at (0,0). X-coordinates increase
// towards the right (there is no negative value). Y-coordinates increase
// downwards (there is no negative value). Canvases hold Rectangles.
struct Canvas {
    
    /* Add your variables and functions HERE */
    int numR, w, h;
    rnode *first;
    
    // Creates an initially empty Canvas, which has its height == width == 0.
    Canvas();
    Canvas( const Canvas & other );
    Canvas( Canvas && other );
    ~Canvas();
    Canvas & operator=( const Canvas & other );
    Canvas & operator=( Canvas && other );
    
    // Adds the given Rectangle after already existing rectangles. The
    // dimensions of the Canvas "stretch" to fit the Rectangle, if necessary,
    // depending upon where the Rectangle's upper-left-hand corner is defined
    // to be and its dimensions.
    void add( const Rectangle & rectangle );
    
    // Returns the number of rectangles in the Canvas.
    int numRectangles() const;
    
    // Returns the width of the Canvas.
    int width() const;
    
    // Returns the height of the Canvas.
    int height() const;
    
    // Returns the rectangle indexed, where 0 <= rectangleId < numRectangles().
    Rectangle get( int rectangleId ) const;
    
    // Moves the Rectangle's Point to the specified coordinate. Requires
    // 0 <= rectangleId < numRectangles(). The resulting Point's coordinates
    // must stay >= 0.
    void translate( int rectangleId, int x = 0, int y = 0 );
    
    // Scales the Rectangle's dimensions by the specified amounts. The resulting
    // dimensions must be integers > 0. Requires 0 <= rectangleId <
    // numRectangles().
    void scale( int rectangleId, float heightFactor = 1.0, float widthFactor = 1.0 );
    
    // Changes the Rectangle's colour to the new colour.
    void change( int rectangleId, Colour newColour );
    
    // Removes Rectangle with specified index. Requires 0 <= rectangleId <
    // numRectangles(). For all Rectangles with index j > rectangleId, their
    // new index becomes j -= 1.
    void remove( int rectangleId );
    
    // calculates the new dimensions of the canvas.
    void resize();
    
    // remove all the rectangles in a canvas.
    void empty();
};

// You may add an input operator if you wish.

// Outputs the Canvas. Format is:
// Dimensions: heightxwidth
// Number of rectangles: N
// \t[Rectangle_0]
// ...
// \t[Rectangle_N-1]
// where '\t' is a tab character.
std::ostream & operator<<( std::ostream & out, const Canvas & canvas );

#endif
