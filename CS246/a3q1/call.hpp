//
//  call.hpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _CALL_H
#define _CALL_H
#include "date.hpp"

// A call consists of the date that the call started (calls don't span multiple
// days), the start time of the call (in a 24-hour format, where midnight is
// listed as 0000 and the final time is 2359) and the number of minutes the
// call lasted, rounded up. A call cannot last for 0 minutes i.e. the minimum
// number of minutes is 1.
struct Call {
    Date date;
    int startTime;
    int duration;
    
    // Note that a Call *must* be initialized with a date, a start time, and
    // a duration.
    Call( Date date, int startTime, int duration );
    
    // You may add Call comparison operator declarations here if you wish.
    bool operator<( Call other ) const;
    
};

// Overwrites the contents of the given Call object with data read from the
// input stream "in". The information for a Call appears on a single line. It
// consists of the date the call took place, followed by two positive integer
// values, the start time and the duration, separated by one or more whitespace
// characters (spaces or tabs). It should make use of the Date input operator.
// For example, a 5-minute call starting at 9:05pm on June 12, 2019 would be
// read in as:
//    2019/06/12 2105 5
std::istream & operator>>( std::istream & in, Call & call );

// Outputs the contents of the given Call object to the output stream "out".
// Output format is the date the call took place, followed by the start time
// and the duration, separated by a single space. It should make use of the Date
// output operator. For example, a 14-minute call starting at 1 minute past
// midnight on June 3, 2018, would be output as:
//   2018/06/03 0001 14
std::ostream & operator<<( std::ostream & out, Call call );

#endif
