//
//  main.cpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "plan.hpp"

// Command Description
// q       Quits the program. Program also quits if EOF detected.
// p       Outputs the current contents of the plan. Used in testing correctness
//         of your implementation.
// b       Calls Plan::calculateBill().
// +       Invokes the input operator on Call and then adds the resulting call to
//         the plan.

int main() {
    // Since date and call objects must be initialized, start them with garbage
    // values that will be overwritten. The plan is initially empty.
    Date date{2018, 9, 26};
    Call call{date, 500, 10};
    Plan plan;
    bool done = false;
    
    while ( ! done ) {
        char command;
        std::cin >> command;
        if ( std::cin.eof() ) break;
        
        switch( command ) {
            case '+':
                std::cin >> call;
                plan.add( call );
                break;
            case 'p':
                std::cout << plan;
                break;
            case 'b':
                plan.calculateBill();
                break;
            case 'q':
                done = true;
                break;
            default:
                std::cerr << "Unrecognized command: '" << command << "'" << std::endl;
        } // switch
    } // while
    return 0;
} // main

