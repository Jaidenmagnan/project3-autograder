//
// Created by lmvas on 11/14/2023.
//

#ifndef PROJECT03_IMAGE_H
#define PROJECT03_IMAGE_H
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct Image {
    struct Header {
        char idLen;
        char cMapType;
        char dataTypeCode;
        short cMapOrigin;
        short cMapLen;
        char cMapDepth;
        short xO;
        short yO;
        short width;
        short height;
        char bitsPerPixel;
        char imageDesc;
    };
    struct Pixel {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
    Header h;
    vector<Pixel> pixels;
    string filepath;
    Image();
    Image(string);
    void GetFileInfo(string);
    void PrintFileInfo(Header);
    void CopyPixelInfo(string);
    void WriteFile(string);
    void AddToColor(string, int);
    void Scale(string, int);
    void ToColor(string);
    void ReloadImage();
    bool Equals(Image);
    void SetSize(int, int);
};


#endif //PROJECT03_IMAGE_H
