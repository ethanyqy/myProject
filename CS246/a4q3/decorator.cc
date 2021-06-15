//
//  decorator.cpp
//  a4q3
//
//  Created by Ethan on 2019-07-15.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "decorator.h"

Flip::Flip(Image * img) {
    origin = img;
}

void Flip::render(PPM &ppm) {
    origin->render(ppm);
    int w = ppm.getWidth();
    int h = ppm.getHeight();
    std::vector<Pixel> &ps = ppm.getPixels();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (i * w + j >= i * w + w - 1 - j) continue;
            std::swap(ps.at(i * w + j), ps.at(i * w + w - 1 - j));
        }
    }
}

Flip::~Flip() {
    delete origin;
}


Rotate::Rotate(Image *img) {
    origin = img;
}

void Rotate::render(PPM &ppm) {
    origin->render(ppm);
    int w = ppm.getWidth();
    int h = ppm.getHeight();
    std::vector<Pixel> &ps = ppm.getPixels();
    std::vector<Pixel> newPixel = ps;
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            ps.at(i * h + j) = newPixel.at(w * (h - j - 1) + i);
        }
    }
}

Rotate::~Rotate() {
    delete origin;
}


Sepia::Sepia(Image *img) {
    origin = img;
}

void Sepia::render(PPM &ppm) {
    origin->render(ppm);
    int n = ppm.getWidth() * ppm.getHeight();
    std::vector<Pixel> & ps = ppm.getPixels();
    for (int i = 0; i < n; ++i) {
        int temp_r = ps.at(i).r;
        int temp_g = ps.at(i).g;
        int temp_b = ps.at(i).b;
        ps.at(i).r = temp_r *.393 + temp_g * .769 + temp_b * .189;
        ps.at(i).g = temp_r *.349 + temp_g * .686 + temp_b * .168;
        ps.at(i).b = temp_r *.272 + temp_g * .534 + temp_b * .131;
        if (ps.at(i).r > 255) {
            ps.at(i).r = 255;
        }
        if (ps.at(i).g > 255) {
            ps.at(i).g = 255;
        }
        if (ps.at(i).b > 255) {
            ps.at(i).b = 255;
        }
    }
}

Sepia::~Sepia() {
    delete origin;
}
