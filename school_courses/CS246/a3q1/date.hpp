//
//  date.hpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _DATE_H
#define _DATE_H
#include <iostream>

enum struct DayOfWeek {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday,
    Saturday};

struct Date {
    int year, month, day;
    
    // Date must be initialized with year, month, and day.
    Date( int year, int month, int day );
    DayOfWeek getDay() const; // Requires date to be in Gregorian calendar.
    
    // Comparison operators (you may add others if you wish)
    bool operator==( Date other ) const;
    bool operator<( Date other ) const;
};

// Overwrites the contents of the given Date object with data read from the
// input stream "in". The format of the input is "year/month/day", so May 6,
// 2019, would appear as 2019/05/06 when being read from input.
std::istream & operator>>( std::istream & in, Date & date );

// Outputs the contents of the given Date object to the output stream "out".
// Date must be output in the form: year/month/day, where the year is 4 digits,
// the month is 2 digits, and the day is 2 digits. For example, "May 6, 2019"
// would appear as 2019/05/06 when printed.
std::ostream & operator<<( std::ostream & out, Date date );

#endif

