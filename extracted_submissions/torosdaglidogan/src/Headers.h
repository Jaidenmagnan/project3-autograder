// Copyright 2023 Dogan Torosdagli

#pragma once

#include <stdint.h>

struct Headers {
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t dataTypeCode;
    // 1 byte

    uint16_t colorMapOrigin;
    uint16_t colorMapLength;

    uint8_t colorMapDepth;
    uint16_t xOrigin;
    // 1 byte

    uint16_t yOrigin;
    uint16_t width;

    uint16_t height;
    uint8_t bitsPerPixel;
    // 1 byte

    uint8_t imageDescriptor;
};

struct Pixel {
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
};

constexpr Pixel operator* (const double scalar, const Pixel &pixel) {
    Pixel out = {};
    out.Blue  = scalar * pixel.Blue;
    out.Green = scalar * pixel.Green;
    out.Red   = scalar * pixel.Red;
    return out;
}

constexpr Pixel operator/ (const Pixel &pixel, const int scalar) {
    Pixel out = {};
    out.Blue  = pixel.Blue / scalar;
    out.Green = pixel.Green / scalar;
    out.Red   = pixel.Red / scalar;
    return out;
}

constexpr Pixel operator* (const Pixel &pixel1, const Pixel &pixel2) {
    Pixel out = {};
    out.Blue  = (static_cast<double>(pixel1.Blue) * pixel2.Blue / 255.0) + 0.5;
    out.Green = (static_cast<double>(pixel1.Green) * pixel2.Green / 255.0) + 0.5;
    out.Red   = (static_cast<double>(pixel1.Red) * pixel2.Red / 255.0) + 0.5;
    return out;
}

constexpr Pixel operator- (const Pixel &pixel1, const Pixel &pixel2) {
    Pixel out = {};
    int16_t tmpB = ((int16_t) pixel1.Blue - pixel2.Blue);
    int16_t tmpG = ((int16_t) pixel1.Green - pixel2.Green);
    int16_t tmpR = ((int16_t) pixel1.Red - pixel2.Red);
    out.Blue  = tmpB >= 0 ? tmpB : 0;
    out.Green = tmpG >= 0 ? tmpG : 0;
    out.Red   = tmpR >= 0 ? tmpR : 0;
    return out;
}

constexpr Pixel operator+ (const Pixel &pixel1, const Pixel &pixel2) {
    Pixel out = {};
    uint16_t tmpB = ((uint16_t) pixel1.Blue + pixel2.Blue);
    uint16_t tmpG = ((uint16_t) pixel1.Green + pixel2.Green);
    uint16_t tmpR = ((uint16_t) pixel1.Red + pixel2.Red);
    out.Blue  = tmpB <= 255 ? tmpB : 255;
    out.Green = tmpG <= 255 ? tmpG : 255;
    out.Red   = tmpR <= 255 ? tmpR : 255;
    return out;
}

constexpr Pixel operator~ (const Pixel &pixel) {
    Pixel out = {};
    out.Blue = 255 - pixel.Blue;
    out.Green = 255 - pixel.Green;
    out.Red = 255 - pixel.Red;
    return out;
}

struct Image {
    Headers * headers;
    union {
        uint8_t * subpixels;
        Pixel * pixels;
    };
    // For efficiency, the image is imported/written using subpixels
    // some processes also use subpixels
    // pixels use the same the data but easier to use for certain functions
};

unsigned int numpix(Image img) {
    return img.headers->height * img.headers->width;
}

