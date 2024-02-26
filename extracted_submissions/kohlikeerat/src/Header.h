//
// Created by kkeer on 11/3/2023.
//

#ifndef IMAGE_PROCESSOR_HEADER_H
#define IMAGE_PROCESSOR_HEADER_H

using namespace std;

#include <fstream>

// Struct for the first 18 bytes in the .tga file
// Width and Height are two important ones
struct Header {
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

// Because reading/writing the Header is tedious, these functions make it easier to do  so
void writeHeader(Header &headerObject, ofstream &outStream);
void readHeader(Header &headerObject, ifstream &inStream);
bool operator==(const Header& headerA, const Header& headerB);

#endif //IMAGE_PROCESSOR_HEADER_H
