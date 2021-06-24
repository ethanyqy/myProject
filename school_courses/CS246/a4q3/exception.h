//
//  exception.hpp
//  a4q3
//
//  Created by Ethan on 2019-07-15.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef PPMEXCEPTION_H
#define PPMEXCEPTION_H
#include <string>

class PPMInputFormatException {
    std::string s;
public:
    PPMInputFormatException(std::string);
    std::string getErrorMessage();
};

#endif
