//
// Created by krist on 11/2/2023.
//

#ifndef PROJECT3_IMAGEPROCESSING_H
#define PROJECT3_IMAGEPROCESSING_H

#endif //PROJECT3_IMAGEPROCESSING_H
#include <vector>
#include <string>

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct ImageProcessing {
public:
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct Image
{
    ImageProcessing header;
    std::vector<Pixel> pix;
    void readFile(const std::string& fileName);
    void writeFile(const std::string& fileName);
    void newReadFile(const std::string& fileName);
    void newWriteFile(const std::string& fileName);
};

