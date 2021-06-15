//
//  decorator.hpp
//  a4q3
//
//  Created by Ethan on 2019-07-15.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef DECORATOR_H
#define DECORATOR_H

#include <fstream>
#include "ppm.h"
#include "image.h"
#include "basic.h"

class Decorator: public Image {
protected:
    Image * origin;
public:
    virtual void render(PPM &ppm) = 0;
};

class Flip: public Decorator {
public:
    Flip(Image *img);
    void render(PPM &ppm) override;
    ~Flip();
};

class Rotate: public Decorator {
public:
    Rotate(Image *img);
    void render(PPM &ppm) override;
    ~Rotate();
};

class Sepia: public Decorator {
public:
    Sepia(Image *img);
    void render(PPM &ppm) override;
    ~Sepia();
};

#endif
