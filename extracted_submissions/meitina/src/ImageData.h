//
// Created by tinam on 11/10/2023.
// This is a header file for the ImageData struct

#ifndef P3_SKELETON_IMAGEDATA_H
#define P3_SKELETON_IMAGEDATA_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>


using namespace std;
using namespace std::filesystem;

struct ImageData {
    unsigned char* pixelArray;
};
unsigned char roundAndClamp(float value);
void assignPixels(ImageData& imgObj, const string& fileName, int rows, int cols);
void multiplyPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData);
void subtractPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData);
void screenPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData);
void overlayPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData);
void editChannel(ImageData& imgObj, int rows, int cols, const string& channel, float add, float scale, ImageData& newData);
void separateChannel(ImageData& imgObj, int rows, int cols, const string& channel, ImageData& newData);
void assignChannels(ImageData& redImg, ImageData& greenImg, ImageData& blueImg, int rows, int cols, ImageData& newData);
void rotate180(ImageData& imgData, int rows, int cols, ImageData& newData);
void outData(ImageData& writeImg, const string& fileName, int rows, int cols);
void deallImgData(ImageData& imgObj);


#endif //P3_SKELETON_IMAGEDATA_H
