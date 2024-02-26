// Copyright 2023 Dogan Torosdagli

#pragma once
#include "Headers.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <algorithm>
#include <cstring>

struct Read {
    Headers * headers;
    uint8_t * subpixels;
    Pixel * pixels;
    explicit Read(const char * fileName) {
        std::ifstream file(fileName, std::ios::binary);
        if (!file) {
            std::string s1 = std::string("Cannot open file ");
            std::string s2 = std::string(fileName);
            std::string s3 = s1 + s2;
            const char * err = s3.c_str();
            throw std::invalid_argument(err);
        }

        // cppcheck-suppress noOperatorEq
        headers = new Headers();
        file.read(reinterpret_cast<char*>(&headers->idLength), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&headers->colorMapType), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&headers->dataTypeCode), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&headers->colorMapOrigin), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&headers->colorMapLength), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&headers->colorMapDepth), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&headers->xOrigin), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&headers->yOrigin), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&headers->width), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&headers->height), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&headers->bitsPerPixel), sizeof(char));
        file.read(reinterpret_cast<char*>(&headers->imageDescriptor), sizeof(char));


        size_t numsubpix = headers->width * headers->height * 3;
        subpixels = new uint8_t[numsubpix];
        file.read(reinterpret_cast<char*>(subpixels), numsubpix * sizeof(uint8_t));

        // C++ standard doesn't guarentee that there will be no padding at the end of structs
        // Though this runs fine on my env, I want to make sure it will also run fine on any other
        static_assert(sizeof(Pixel) == sizeof(uint8_t) * 3, "Array Padding Error");
        pixels = reinterpret_cast<Pixel*>(subpixels);

        file.close();
    }

    ~Read() {
        delete [] subpixels;
        delete headers;
    }

    explicit Read(const Read &other) {
        headers = new Headers();
        *headers = *other.headers;

        size_t numsubpix = headers->width * headers->height * 3;
        subpixels = new uint8_t[numsubpix];
        std::copy(other.subpixels, other.subpixels + numsubpix, subpixels);
        static_assert(sizeof(Pixel) == sizeof(uint8_t) * 3, "Array Padding Error");
        pixels = reinterpret_cast<Pixel*>(subpixels);
    }

    Read(const Read &other, unsigned int scale, unsigned int xOffset, unsigned int yOffset) {
        headers = new Headers();
        *headers = *other.headers;
        xOffset *= headers->width;
        yOffset *= headers->height;
        headers->width *= scale;
        headers->height *= scale;


        size_t numsubpix = headers->width * headers->height * 3;
        subpixels = new uint8_t[numsubpix]();
        static_assert(sizeof(Pixel) == sizeof(uint8_t) * 3, "Array Padding Error");
        pixels = reinterpret_cast<Pixel*>(subpixels);

        int prewidth = other.headers->width;
        int pix = numpix(other.getImage());
        int postpix = numsubpix / 3;
        for (int i = 0; i < pix; i++) {
            int index = headers->width * (yOffset + (i / prewidth)) + (i % prewidth) + xOffset;
            if (index < postpix && i % prewidth + xOffset < headers->width) {
                pixels[index] = other.pixels[i];
            } else {
                int a = 1;
            }
        }
    }

    Image getImage() const {
        Image img;
        img.headers = headers;
        img.subpixels = subpixels;
        return img;
    }
};

bool operator== (const Read &file1, const Read &file2) {
    bool out = true;
    out &= !memcmp(file1.headers, file2.headers, sizeof(Headers));
    if (!out)
        return out;
    out &= !memcmp(file1.subpixels, file2.subpixels, numpix(file1.getImage()) * sizeof(uint8_t));
    return out;
}
