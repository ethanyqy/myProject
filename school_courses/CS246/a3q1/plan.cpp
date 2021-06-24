//
//  plan.cpp
//  a3q1
//
//  Created by Ethan on 2019-06-17.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "plan.hpp"
#include<iomanip>

Plan::Plan() {
    totalDuration = 0;
    basic = 250;
    bill = basic;
    rate = 5;
    freeDuration = 150;
    first = nullptr;
}

Plan::~Plan() {
    Records * temp = nullptr;
    while ( first ) {
        temp = first->next;
        delete first;
        first = temp;
    }
}

void Plan::add( Call call ) {
    Records *newEntry = new Records(call);
    Records *curr = first;
    newEntry->next = first;
    if (! first ) {
        first = newEntry;
    } else if ( newEntry->entry < first->entry ) {
        first = newEntry;
    } else {
        Records *prev = curr;
        curr = curr->next;
        while ( curr ) {
            if ( newEntry->entry < curr->entry ) break;
            prev = curr;
            curr = curr->next;
        }
        prev->next = newEntry;
        newEntry->next = curr;
    }
    if ( call.date.getDay() != DayOfWeek::Saturday && call.date.getDay() != DayOfWeek::Sunday ) {
        int timeBegin = call.startTime / 100 * 60 + call.startTime % 100;
        int maxTime = 18 * 60;
        int minTime = 7 * 60;
        int timeEnd = timeBegin + call.duration;
        if ( timeBegin >= minTime && timeEnd <= maxTime ) {
            totalDuration += (call.duration);
        } else if ( timeBegin <= minTime && minTime <= timeEnd && timeEnd <= maxTime ) {
            totalDuration += (timeEnd - minTime);
        } else if ( timeBegin >= minTime && timeBegin <= maxTime && timeEnd >= maxTime ) {
            totalDuration += (maxTime - timeBegin);
        } else if ( timeBegin <= minTime && timeEnd >= maxTime ) {
            totalDuration += (maxTime - minTime);
        }
    }
}

void Plan::calculateBill() {
    if ( totalDuration > freeDuration ) {
    bill = (totalDuration - freeDuration) * rate + basic;
    } else {
        bill = basic;
    }
    Records *temp = first;
    while ( temp ) {
        std::cout << temp->entry << std::endl;
        temp = temp->next;
    }
    std::cout << std::endl;
    std::cout << "Day time minutes: ";
    if (totalDuration > freeDuration) {
        std::cout << (totalDuration - freeDuration) << std::endl;
    } else {
        std::cout << totalDuration << std::endl;
    }
    std::cout << "Total cost $" << (bill/10) << "." << std::setfill('0') << std::setw(2) << (bill%10*10) << std::endl;
    temp = nullptr;
    while ( first ) {
        temp = first->next;
        delete first;
        first = temp;
    }
    totalDuration = 0;
    bill = basic;
}

std::ostream & operator<<( std::ostream & out, const Plan & plan ) {
    Records *temp = plan.first;
    while ( temp ) {
        std::cout << temp->entry << std::endl;
        temp = temp->next;
    }
    return out;
}
