//
//  main.cpp
//  a3q3
//
//  Created by Ethan on 2019-06-19.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "canvas.hpp"
using namespace std;

// The test harness starts by defining 5 canvases with ids in the range 0 to 4.
// It performs no error-checking upon the commands, other than ensuring that
// the canvas id specified is in the range of 0 to 4, and that the rectangle
// index is in the range 0 to (number of rectangles in the canvas).
//
// Colours are implemented as an enumerated class, given in rectangle.h, and
// consisting of red, green, blue, yellow, orange, black and white. Colours are
// specified to the test harness by a single character, the first letter of the
// colour name, except for black where it uses the letter 'a' since blue
// already uses 'b'.
//
// See the provided files sample.in and sample.out.

// The test harness commands consist of:
//
// Command            Description
// a i c x y h w      Adds a Rectangle with the Colour corresponding to the
//                    character c, Point(x,y) and dimensions of height h and
//                    width w to the canvas i by calling the rectangle input
//                    operator and the Canvas::add method. Remember that
//                    colours are denoted by a single character, and not a
//                    string.
// r i j              Removes the rectangle at index j from canvas i. Remember
//                    that rectangles are indexed by integers from 0 to the
//                    (number of rectangles - 1).
// e i                Removes all rectangles from canvas i, using Canvas::empty.
// p c i              Prints the contents of canvas i using the Canvas output
//                    operator.
// p r i j            Prints the rectangle at index j of canvas i using the
//                    Rectangle output operator.
// s c i j colour     Changes the colour of the rectangle at index j of canvas
//                    i to the new colour, colour. (Remember that colour is
//                    denoted by a single character.)
// s t i j x y        Moves the upper-left Point of the rectangle at index j of
//                    canvas i by translating it by the specified amounts in
//                    the x and y directions. You may assume that the resulting
//                    coordinate values will not go below 0 i.e. not negative.
// s s i j s_1 s_2    Scales the height and width of the rectangle at index j
//                    of canvas i by multiplying the dimensions by the
//                    specified amounts. The resulting values are integers, and
//                    you may assume that they will stay greater than 0.
// c c i              Creates a local copy of the specified canvas, i, using
//                    the copy constructor and outputs it. Note that the copy
//                    is modified by the test harness by adding two rectangles
//                    to it, and then removing the rectangle with index 0. If
//                    you've done a proper deep copy, the original canvas
//                    should remain untouched.
// c m                Uses the move constructor to create a temporary canvas
//                    and output its contents.
// = c i j            Uses the copy assignment operator to copy the contents of
//                    canvas j to canvas i. If this is a proper deep copy,
//                    changing one will not change the other.
// = m i              Uses the move assignment operator to move the contents of
//                    a temporary canvas to canvas i.

const int NUM_CANVASES = 5;

bool validateCanvasId( int cid ) {
    if ( cid < 0 || cid >= NUM_CANVASES ) {
        cerr << "Invalid canvas id " << cid << endl;
        return false;
    } // if
    return true;
} // validateCanvasId

bool validateRectangleId( int rid, int numRectangles ) {
    if ( rid < 0 || rid >= numRectangles ) {
        cerr << "Invalid rectangle id " << rid << endl;
        return false;
    } // if
    return true;
} // validateRectangleId

void errOption( char option ) {
    cerr << "Unrecognized option: '" << option << "'" << endl;
}

Colour translateColour( char c ) {
    switch( c ) {
        case 'r' : return Colour::Red;
        case 'g' : return Colour::Green;
        case 'b' : return Colour::Blue;
        case 'y' : return Colour::Yellow;
        case 'o' : return Colour::Orange;
        case 'a' : return Colour::Black;
        case 'w' : return Colour::White;
    }
    return Colour::Red;
} // translate

// Generate a canvas. Used in testing move constructor.
Canvas makeCanvas() {
    Canvas tmpCanvas;
    tmpCanvas.add( Rectangle{ Point{20,20}, 5, 15, Colour::Yellow } );
    tmpCanvas.add( Rectangle{ Point{0,20}, 10,30, Colour::Blue } );
    return tmpCanvas;
} // makeCopy

int main() {
    
    Canvas canvases[NUM_CANVASES];
    
    while (true) {
        char command, option, c;
        int cid1, cid2, rid; // canvas id #1, canvas id #2, rectangle id #
        int x, y; // translate values
        float s1, s2; // scale factors
        Rectangle rectangle{ Point{}, 0, 0, Colour::Red };
        
        //cerr << "Enter a command: "; // sent to cerr just so can be redirected to /dev/null when testing
        cin >> command;
        if ( cin.eof() ) break;
        
        switch( command ) {
            case 'e':
                // Remove all rectangles from the specified canvas
                cin >> cid1;
                if ( validateCanvasId( cid1 ) ) {
                    canvases[ cid1 ].empty();
                }
                break;
            case 'a':
                // add a rectangle to the specified canvas
                cin >> cid1 >> rectangle;
                if ( validateCanvasId( cid1 ) ) {
                    canvases[ cid1 ].add( rectangle );
                }
                break;
            case 'r':
                // remove the specified rectangle from the specified canvas
                cin >> cid1 >> rid;
                if ( validateCanvasId( cid1 ) &&
                    validateRectangleId( rid, canvases[ cid1 ].numRectangles() ) )
                {
                    canvases[ cid1 ].remove( rid );
                }
                break;
            case 'p':
                cin >> option >> cid1;
                if ( validateCanvasId( cid1 ) ) {
                    switch ( option ) {
                        case 'c':
                            // print the specified canvas' contents
                            cout << canvases[ cid1 ];
                            break;
                        case 'r':
                            // prints the specified rectangle from the specified canvas
                            cin >> rid;
                            if ( validateRectangleId( rid, canvases[ cid1 ].numRectangles() ) ) {
                                cout << canvases[ cid1 ].get( rid ) << endl;
                            }
                            break;
                        default:
                            errOption( option );
                    } // switch
                } // if
                break;
            case '=': // assign
                cin >> option >> cid1;
                if ( validateCanvasId( cid1 ) ) {
                    switch ( option ) {
                        case 'c':
                            cin >> cid2;
                            if ( validateCanvasId( cid2 ) ) {
                                // assignment of one canvas to another (uses copy operator=)
                                canvases[ cid1 ] = canvases[ cid2 ];
                            } // if
                            break;
                        case 'm':
                        {
                            Canvas tmpCanvas;
                            tmpCanvas.add( Rectangle{ Point{20,20}, 5, 15, Colour::Yellow } );
                            tmpCanvas.add( Rectangle{ Point{0,20}, 10,30, Colour::Blue } );
                            // assignment of one canvas to another (uses move operator=)
                            canvases[ cid1 ] = std::move( tmpCanvas );
                        }
                            break;
                        default:
                            errOption( option );
                    } // switch
                } // if
                break;
            case 'c': // create a copy of the specified canvas in a temporary canvas
                cin >> option;
                switch ( option ) {
                    case 'c':
                    {
                        cin >> cid1;
                        if ( validateCanvasId( cid1 ) ) {
                            // uses copy ctor
                            Canvas tmpCanvas{ canvases[cid1] };
                            tmpCanvas.add( Rectangle{ Point{20,20}, 5, 15, Colour::Yellow } );
                            tmpCanvas.add( Rectangle{ Point{0,20}, 10,30, Colour::Blue } );
                            tmpCanvas.remove( 0 );
                            cout << "temporary canvas\n" << tmpCanvas;
                        } // if
                        break;
                    }
                    case 'm':
                    {
                        // uses move ctor
                        Canvas tmpCanvas{ std::move( makeCanvas() ) };
                        cout << "temporary canvas\n" << tmpCanvas;
                        break;
                    }
                    default:
                        errOption( option );
                } // switch
                break;
            case 's': // set rectangle from the specified canvas
                cin >> option >> cid1 >> rid;
                if ( validateCanvasId( cid1 ) &&
                    validateRectangleId( rid, canvases[ cid1 ].numRectangles() ) )
                {
                    switch( option ) {
                        case 'c': // colour
                            cin >> c;
                            canvases[ cid1 ].change( rid, translateColour(c) );
                            break;
                        case 't': // translate
                            cin >> x >> y;
                            canvases[ cid1 ].translate( rid, x, y );
                            break;
                        case 's': // scale
                            cin >> s1 >> s2;
                            canvases[ cid1 ].scale( rid, s1, s2 );
                            break;
                        default:
                            errOption( option );
                    } // switch
                } // if
                break;
            default:
                cerr << "Unrecognized command: '" << command << "'" << std::endl;
        } // switch
    } // while
    
    return 0;
}
