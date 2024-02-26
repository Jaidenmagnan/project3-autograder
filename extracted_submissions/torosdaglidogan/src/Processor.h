// Copyright 2023 Dogan Torosdagli

#pragma once

#include "Headers.h"
#include <exception>
#include <algorithm>

// Depreceated, I only used these for testing
// (I'm scared that if I get rid of it something will break)
const double RECIPROCAL  = 1/255.0;
const double RECIPROCAL2 = RECIPROCAL * RECIPROCAL;

class Process {
    Image img;

 public:
    explicit Process(Image img) {
        this->img = img;
    }

    // Depreceated, I only used this for testing
    Image getImage() {
        return img;
    }

    Process multiply(Image img2) {
        for (int i = 0; i < numpix(img); i++) {
            // Normalized Multiplication
            img.pixels[i] = img.pixels[i] * img2.pixels[i];
        }
        return * this;
    }

    Process subtract(Image img2) {
        for (int i = 0; i < numpix(img); i++) {
            img.pixels[i] = img.pixels[i] - img2.pixels[i];
        }
        return * this;
    }

    Process screen(Image img2) {
        for (int i = 0; i < numpix(img); i++) {
            img.pixels[i] = ~((~img.pixels[i]) * (~img2.pixels[i]));
        }
        return * this;
    }

    // Regular overlay
    Process overlay(Image img2) {
        for (int i = 0; i < numpix(img) * 3; i++) {
            if (img2.subpixels[i] > 127)
                img.subpixels[i] = 255 - (2 * (uint16_t)(255 - img.subpixels[i]) *
                 (uint16_t)(255 - img2.subpixels[i]))/255.0f + 0.5f;
            else
                img.subpixels[i] = 2 * (uint16_t)img2.subpixels[i] * (uint16_t)img.subpixels[i] / 255.0f + 0.5f;
        }
        return * this;
    }

    // Inverse overlay
    Process overlayUnder(Image img2) {
        for (int i = 0; i < numpix(img) * 3; i++) {
            if (img.subpixels[i] > 127)
                img.subpixels[i] = 255 - (2 * (uint16_t)(255 - img.subpixels[i]) *
                 (uint16_t)(255 - img2.subpixels[i]))/255.0f + 0.5f;
            else
                img.subpixels[i] = 2 * (uint16_t)img2.subpixels[i] * (uint16_t)img.subpixels[i] / 255.0f + 0.5f;
        }
        return * this;
    }

    // Add image to image (clamped)
    Process add(Image img2) {
        for (int i = 0; i < numpix(img); i++) {
            img.pixels[i] = img.pixels[i] + img2.pixels[i];
        }
        return * this;
    }
    
    // Add value to each pixel (clamped)
    Process add(Pixel pixel) {
        for (int i = 0; i < numpix(img); i++) {
            img.pixels[i] = img.pixels[i] + pixel;
        }
        return * this;
    }

    // Scale each pixel by a constant float
    Process scale(float R, float G, float B) {
        if (R < 0 || G < 0 || B < 0) {
            throw std::invalid_argument("Process::scale(float, float, float) MUST have non-negative arguments");
        }
        for (int i = 0; i < numpix(img); i++) {
            int tmpR = img.pixels[i].Red   * R;
            int tmpG = img.pixels[i].Green * G;
            int tmpB = img.pixels[i].Blue  * B;
            img.pixels[i].Red   = tmpR > 255 ? 255 : tmpR;
            img.pixels[i].Green = tmpG > 255 ? 255 : tmpG;
            img.pixels[i].Blue  = tmpB > 255 ? 255 : tmpB;
        }
        return * this;
    }

    // Scale each pixel by a constant
    Process scale(unsigned int R, unsigned int G, unsigned int B) {
        if (R > 1000 || G > 1000 || B > 1000) {
            throw std::invalid_argument
            ("Process::scale(unsigned int, unsigned int, unsigned int) may not have arguments > 1000");
        }
        for (int i = 0; i < numpix(img); i++) {
            int tmpR = img.pixels[i].Red   * R;
            int tmpG = img.pixels[i].Green * G;
            int tmpB = img.pixels[i].Blue  * B;
            img.pixels[i].Red   = tmpR > 255 ? 255 : tmpR;
            img.pixels[i].Green = tmpG > 255 ? 255 : tmpG;
            img.pixels[i].Blue  = tmpB > 255 ? 255 : tmpB;
        }
        return * this;
    }

    // Treats RGB as a vector and applies matrix to it.
    Process matrix(unsigned int RR, unsigned int GR, unsigned int BR,
                    unsigned int RG, unsigned int GG, unsigned int BG,
                    unsigned int RB, unsigned int GB, unsigned int BB) {
        for (int i = 0; i < numpix(img); i++) {
            int tmpR = img.pixels[i].Red * RR + img.pixels[i].Green * GR + img.pixels[i].Blue * BR;
            int tmpG = img.pixels[i].Red * RG + img.pixels[i].Green * GG + img.pixels[i].Blue * BG;
            int tmpB = img.pixels[i].Red * RB + img.pixels[i].Green * GB + img.pixels[i].Blue * BB;
            img.pixels[i].Red   = tmpR > 255 ? 255 : tmpR;
            img.pixels[i].Green = tmpG > 255 ? 255 : tmpG;
            img.pixels[i].Blue  = tmpB > 255 ? 255 : tmpB;
        }
        return * this;
    }

    // Invert order of pixels, thus rotating it 180 degrees
    Process rot180() {
        int pix = numpix(img);
        Pixel * copy = new Pixel[pix];
        std::copy(img.pixels, img.pixels + pix, copy);
        for (int i = 0; i < numpix(img); i++) {
            img.pixels[i] = copy[pix - i - 1];
        }
        delete [] copy;
        return * this;
    }
};
