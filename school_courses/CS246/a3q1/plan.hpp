//
//  plan.hpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _PLAN_H
#define _PLAN_H
#include "call.hpp"


struct Records {
    Call entry;
    Records *next;
    
    Records( Call call ): entry( call ), next( nullptr ){};
    
};

struct Plan {
    
    // Put your private member variables and functions here.
    int totalDuration;
    int basic;
    int bill;
    int rate;
    int freeDuration;
    Records *first;
    
public:
    Plan();
    ~Plan();
    
    // Adds the call to the month's current list of calls.
    // You must implement and use the overloaded operator>> for a Call.
    void add( Call call );
    
    // Uses the output operator to print the list of calls made,
    // outputs the amount of money owed, and then clears out the
    // list of calls made. Note that you must implement and use
    // the operator<< for the Call as part of your implemenation.
    // See the provided sample output and executable for the format
    // of the bill.
    void calculateBill();
};

// You may add an input operator if you wish.

// Outputs the current month's calls, ordered by date and then by start time
// within date. Uses the Call output operator.
std::ostream & operator<<( std::ostream & out, const Plan & plan );

#endif
