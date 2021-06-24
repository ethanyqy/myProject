//
//  observer.hpp
//  a4q4
//
//  Created by Ethan on 2019-07-16.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _OBSERVER_H_
#define _OBSERVER_H_

// DO NOT MODIFY THIS FILE!

template <typename InfoType, typename StateType> class Subject;
class Cell;

template <typename InfoType, typename StateType> class Observer {
public:
    virtual void notify(Subject<InfoType, StateType> &whoFrom) = 0; // pass the Subject that called the notify method
    virtual ~Observer() = default;
};
#endif
