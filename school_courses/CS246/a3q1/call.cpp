//
//  call.cpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "call.hpp"
#include <iomanip>

Call::Call( Date date, int startTime, int duration ):
date(date), startTime(startTime), duration(duration) {};

bool Call::operator<( Call other) const {
    if (date < other.date) {
        return true;
    } else if ( date == other.date ) {
        if ( startTime < other.startTime ) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}



std::istream &operator>>( std::istream &in, Call &call ) {
    in >> call.date;
    in >> call.startTime;
    in >> call.duration;
    return in;
}

std::ostream & operator<<( std::ostream & out, Call call ){
    out << call.date;
    out << " ";
    out << std::setfill('0') << std::setw(4) << call.startTime;
    out << " ";
    out << call.duration;
    return out;
}
