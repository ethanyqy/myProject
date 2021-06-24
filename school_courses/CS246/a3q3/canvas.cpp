//
//  canvas.cpp
//  a3q3
//
//  Created by Ethan on 2019-06-19.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "canvas.hpp"

Canvas::Canvas() {
    numR = 0;
    h = 0;
    w = 0;
    first = nullptr;
}

Canvas::Canvas( const Canvas & other ) {
    numR = other.numR;
    h = other.height();
    w = other.width();
    if (other.first) {
        first = new rnode(*other.first);
    } else {
        first = nullptr;
    }
}

Canvas::Canvas( Canvas && other ) {
    if (this != &other) {
        delete first;
    }
        numR = other.numR;
        h = other.height();
        w = other.width();
        first = other.first;
        other.first = nullptr;
}

Canvas::~Canvas() {
    delete  first;
}

Canvas & Canvas::operator=( const Canvas &other ) {
    numR = other.numR;
    h = other.height();
    w = other.width();
    if (other.first) {
        first = new rnode(*other.first);
    } else {
        first = nullptr;
    }
    return *this;
}

Canvas & Canvas::operator=( Canvas &&other ) {
    if (this != &other) {
        delete first;
    }
    numR = other.numR;
    h = other.height();
    w = other.width();
    first = other.first;
    other.first = nullptr;
    return *this;
}

void Canvas::add( const Rectangle & rectangle ) {
    numR += 1;
    rnode *curr = first;
    if ( !first ) {
        first = new rnode(rectangle);
    } else {
        while ( curr->next ) {
            curr = curr->next;
        }
        curr->next = new rnode(rectangle);
    }
    int rw = rectangle.point().x() + rectangle.width();
    int rh = rectangle.point().y() + rectangle.height();
    if ( rw > width() ) {
        w = rw;
    }
    if ( rh > height() ) {
        h = rh;
    }
}
    

int Canvas::numRectangles() const {
    return numR;
}


int Canvas::width() const {
    return w;
}


int Canvas::height() const {
    return h;
}


Rectangle Canvas::get( int rectangleId ) const {
    rnode *curr = first;
    for ( int i = 0; i < rectangleId; ++i, curr = curr->next );
    return curr->record;
}



void Canvas::translate( int rectangleId, int x, int y ) {
    rnode *curr = first;
    for ( int i = 0; i < rectangleId; ++i, curr = curr->next );
    curr->record.translate( x, y );
    resize();
}


void Canvas::scale( int rectangleId, float heightFactor, float widthFactor ) {
    rnode *curr = first;
    for ( int i = 0; i < rectangleId; ++i, curr = curr->next );
    curr->record.scale( heightFactor, widthFactor );
    resize();
}


void Canvas::change( int rectangleId, Colour newColour ) {
    rnode *curr = first;
    for ( int i = 0; i < rectangleId; ++i, curr = curr->next );
    curr->record.change( newColour );
}


void Canvas::remove( int rectangleId ) {
    --numR;
    rnode *curr = first;
    for ( int i = 0; i < rectangleId - 1 ; ++i, curr = curr->next );
    if ( rectangleId == 0 ) {
        first = curr->next;
        curr->next = nullptr;
        delete curr;
    } else {
        rnode *temp = curr->next;
        curr->next = temp->next;
        temp->next = nullptr;
        delete temp;
    }
    resize();
}

void Canvas::resize() {
    rnode *curr = first;
    w = 0;
    h = 0;
    for ( int i = 0; i < numR; ++i ) {
        int rw = curr->record.point().x() + curr->record.width();
        int rh = curr->record.point().y() + curr->record.height();
        if ( rw > width() ) {
            w = rw;
        }
        if ( rh > height() ) {
            h = rh;
        }
        curr = curr->next;
    }
}

void Canvas::empty() {
    numR = 0;
    w = 0;
    h = 0;
    delete first;
    first = nullptr;
}


std::ostream & operator<<( std::ostream & out, const Canvas & canvas ) {
    out << "Dimensions: " << canvas.height() << 'x' << canvas.width() << std::endl;
    out << "Number of rectangles: " << canvas.numRectangles() << std::endl;
    rnode *temp = canvas.first;
    for ( int i = 0; i < canvas.numRectangles(); ++i ) {
        out << "\t" << temp->record << std::endl;
        temp = temp->next;
    }
    return out;
}

