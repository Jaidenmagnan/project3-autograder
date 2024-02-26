
#pragma once
using namespace std;
#include <string>
#include<iostream>
#include <fstream>


class Image{
    char* idLength;
    char* colorMapType;
    char* dataTypeCode;
    short* colorMapOrigin;
    short* colorMapLength;
    char* colorMapDepth;
    short* xOrigin;
    short* yOrigin;
    short* width;
    short* height;
    char* bitsPerPixel;
    char* imageDescriptor;
    int size; // size of pixel data
    unsigned char headerData[18]={0};
    unsigned char* pixelData;
public:
    Image(ifstream* tga);
    Image();
    unsigned char* getPixelData();
    void setPixelData(unsigned char* newPixelData);
    void resizePixelData(unsigned char* newPixelData, int size);
    unsigned char* getHeaderData();
    void resetHeaderData();

    int getSize();
    int getWidth();
    int getHeight();
    void setWidth(short newWidth);
    void setHeight(short newHeight);
    //topLayer=Image object multiply is called on
    Image multiply(Image& bottomLayer);
    Image subtract(Image& bottomLayer);
    Image screen(Image& bottomLayer);
    Image overlay(Image& bottomLayer);
    Image addChannel(string channel, float value);
    Image scaleChannel(string channel, int value);
    Image rotate();
    void deleteVars();
    ~Image();
    Image(Image& other);
    Image& operator=(Image& other);
};
