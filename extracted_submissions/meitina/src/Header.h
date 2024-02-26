//
// Created by tinam on 11/9/2023.
// This struct handles the header of a tga file
//

#ifndef P3_SKELETON_HEADER_H
#define P3_SKELETON_HEADER_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

#pragma pack(push, 1)
struct Header {
    char idLength;
    char colorMapType;
    char datTypeCode;
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
#pragma pack(pop)

void assignHeader(Header& headerObj, const string& fileName);
void outHeader(Header& headerObj, const string& fileName);
void printHeader(const Header& headerObj); // // just for looksies


#endif //P3_SKELETON_HEADER_H