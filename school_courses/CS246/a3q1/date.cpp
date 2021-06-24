//
//  date.cpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "date.hpp"
#include <string>
#include <iomanip>

// Put your constructor implementation HERE.
Date::Date(int year, int month, int day):
year(year), month(month), day(day) {};

// https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Corresponding_days
DayOfWeek Date::getDay() const {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int year_ = year - (month < 3);
    return static_cast<DayOfWeek>(
                                  (year_ + year_/4 - year_/100 + year_/400 + t[month-1] + day) % 7 );
}

// Put your member and non-member functions HERE.
bool Date::operator==( Date other ) const {
    if ( this->year == other.year ) {
        if ( this->month == other.month ) {
            if ( this->day == other.day ) {
                return true;
            }
        }
    }
    return false;
}

bool Date::operator<( Date other ) const {
    if ( this->year >= other.year ) {
        if ( this->month >= other.month ) {
            if ( this->day >= other.day ) {
                return false;
            }
        }
    }
    return true;
}

std::istream &operator>>( std::istream &in, Date & date ) {
    in >> date.year;
    in.clear();
    in.ignore();
    in >> date.month;
    in.clear();
    in.ignore();
    in >> date.day;
    return in;
}

std::ostream &operator<<( std::ostream &out, Date date ) {
    out << std::setfill('0') << std::setw(4) << date.year << '/';
    out << std::setfill('0') << std::setw(2) << date.month << '/';
    out << std::setfill('0') << std::setw(2) << date.day ;
    return out;
}

