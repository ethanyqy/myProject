//
//  bacis.hpp
//  a4q3
//
//  Created by Ethan on 2019-07-15.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef BASIC_H
#define BASIC_H

#include "image.h"

class BasicImage: public Image {
    std::string source;
public:
    BasicImage(std::string source) : source{source}{}
    void render(PPM &ppm) override;
};
#endif
