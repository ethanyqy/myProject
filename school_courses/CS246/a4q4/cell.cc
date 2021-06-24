//
//  cell.cpp
//  a4q4
//
//  Created by Ethan on 2019-07-16.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "cell.h"

Cell::Cell(size_t r, size_t c): r{r}, c{c} {}

// set NewPiece
void Cell::setPiece(Colour colour) {
    // set info.colour
    this->colour = colour;
    // set state
    State newState;
    newState.type = StateType::NewPiece;
    newState.colour = colour;
    newState.direction = Direction::W;
    this->setState(newState);
    // notify other observers
    this->notifyObservers();
}

// change a cell
void Cell::toggle() {
    if (this->colour == Colour::Black) {
        this->colour = Colour::White;
    } else if (this->colour == Colour::White) {
        this->colour = Colour::Black;
    }
}

// the neighbors will call this method when I notify them and they will
// change their state.
void Cell::notify(Subject<Info, State> &whoFrom) {
    if (whoFrom.getInfo().colour == Colour::NoColour) {
        // if the cell has not been filled, skip it
        return;
    }
    State from = whoFrom.getState();
    State newState = this->getState();
    size_t column = whoFrom.getInfo().col - this->getInfo().col;
    size_t row = whoFrom.getInfo().row - this->getInfo().row;
    
    // calculate the direction of whoFrom
    if (column == -1 && row == -1) {
        newState.direction = Direction::NW;
    } else if( column == 0 && row == -1) {
        newState.direction = Direction::N;
    } else if (column == 1 && row == -1) {
        newState.direction = Direction::NE;
    } else if (column == -1 && row == 0) {
        newState.direction = Direction::W;
    } else if (column == 1 && row == 0) {
        newState.direction = Direction::E;
    } else if (column == -1 && row == 1) {
        newState.direction = Direction::SW;
    } else if (column == 0 && row == 1) {
        newState.direction = Direction::S;
    } else {
        newState.direction = Direction::SE;
    }
    
    // if notified by NewPiece
    if (from.type == StateType::NewPiece) {
        if (from.colour == this->getInfo().colour) {
            // colours are the same, do nothing
            return;
        } else {
            newState.type = StateType::Relay;
            newState.colour = from.colour;
            this->setState(newState);
        }
    }
    
    // if notified by Relay
    else if (from.type == StateType::Relay) {
        // if the direction of whoFrom and NewPiece are the same
        if (from.direction == newState.direction) {
            if (from.colour != this->getInfo().colour) {
                newState.type = StateType::Relay; // keep passing Relay in the same direction
            } else {
                newState.type = StateType::Reply; // gonna pass back Reply
            }
            newState.colour = from.colour;
            this->setState(newState);
        } else {
            //not in the correct direction, do nothing
            return;
        }
    }
    
    // if notified by Reply, gonna pass back the Reply to NewPiece
    else if (from.type == StateType::Reply) {
        // if the direction of whoFrom and Newpiece are the opposite(back direction)
        if (from.direction == Direction::W & newState.direction == Direction::E ||
            from.direction == Direction::S & newState.direction == Direction::N ||
            from.direction == Direction::N & newState.direction == Direction::S ||
            from.direction == Direction::E & newState.direction == Direction::W ||
            from.direction == Direction::NE & newState.direction == Direction::SW ||
            from.direction == Direction::SE & newState.direction == Direction::NW ||
            from.direction == Direction::NW & newState.direction == Direction::SE ||
            from.direction == Direction::SW & newState.direction == Direction::NE) {
            if (from.colour != this->getInfo().colour) {
                // the colours are different, we should change the cell
                toggle();
                newState.colour = from.colour;
                newState.direction = from.direction;
                newState.type = StateType::Reply;
                this->setState(newState);
            } else {
                // the colours are the same, stop
                return;
            }
        } else {
            // not in the correct direction, do nothing
            return;
        }
    }
    
    // notify other observers that this has changed
    this->notifyObservers();
}

Info Cell::getInfo() const {
    return Info{r, c, colour};
}
