// Copyright 2023 Dogan Torosdagli

#pragma once

#include "Headers.h"
#include <iostream>
#include <fstream>
#include <string>

void Write(Image img, const char *dest) {
    Headers *headers = img.headers;
    unsigned char *subpixels = img.subpixels;

    std::ofstream file(dest, std::ios::binary);
    if (!file) {
        std::string s1 = std::string("Cannot open file ");
        std::string s2 = std::string(dest);
        std::string s3 = s1 + s2;
        const char * err = s3.c_str();
        throw std::invalid_argument(err);
    }

    file.write(reinterpret_cast<char *>(&headers->idLength), sizeof(uint8_t));
    file.write(reinterpret_cast<char *>(&headers->colorMapType), sizeof(uint8_t));
    file.write(reinterpret_cast<char *>(&headers->dataTypeCode), sizeof(uint8_t));
    file.write(reinterpret_cast<char *>(&headers->colorMapOrigin), sizeof(uint16_t));
    file.write(reinterpret_cast<char *>(&headers->colorMapLength), sizeof(uint16_t));
    file.write(reinterpret_cast<char *>(&headers->colorMapDepth), sizeof(uint8_t));
    file.write(reinterpret_cast<char *>(&headers->xOrigin), sizeof(uint16_t));
    file.write(reinterpret_cast<char *>(&headers->yOrigin), sizeof(uint16_t));
    file.write(reinterpret_cast<char *>(&headers->width), sizeof(uint16_t));
    file.write(reinterpret_cast<char *>(&headers->height), sizeof(uint16_t));
    file.write(reinterpret_cast<char *>(&headers->bitsPerPixel), sizeof(uint8_t));
    file.write(reinterpret_cast<char *>(&headers->imageDescriptor), sizeof(uint8_t));

    file.write((const char *)subpixels, sizeof(uint8_t) * 3 * headers->width * headers->height);

    file.close();
}
