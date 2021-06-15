//
//  image.hpp
//  a4q3
//
//  Created by Ethan on 2019-07-15.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include "ppm.h"

class Image {
public:
    virtual void render(PPM &ppm) = 0;
    virtual ~Image();
};

#endif
