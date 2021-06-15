//
//  exception.cpp
//  a4q3
//
//  Created by Ethan on 2019-07-15.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "exception.h"
#include <string>
using std::string;

string PPMInputFormatException::getErrorMessage(){
    return s;
}

PPMInputFormatException::PPMInputFormatException(std::string s)
: s{s} {}
