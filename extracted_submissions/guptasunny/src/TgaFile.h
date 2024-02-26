//
// Created by sgupt on 11/15/2023.
//

#ifndef IMAGEPROCESSING_TGAFILE_H
#define IMAGEPROCESSING_TGAFILE_H
#include "Header.h"
#include <vector>
#include "string"
using namespace std;

class Pixel { //Pixel class defintion; just 3 unsigned chars, 1 for each color
public:
    unsigned char blue;
    unsigned char red;
    unsigned char green;

};

class TgaFile {

private:
    Header fileHeader;
    vector<Pixel> imgData;
public:
    TgaFile(string& fileName);
    TgaFile(vector<Pixel>& imgData, Header& imgHeader);
    vector<Pixel> multiply(TgaFile& bottomLayer);
    vector<Pixel> screen(TgaFile& bottomLayer);
    vector<Pixel> subtract(TgaFile& bottomLayer);
    vector<Pixel> scale(float& multiplier, string& channel);
    vector<Pixel> overlay(TgaFile& bottomLayer);
    vector<Pixel> addChannel(string& channel, int& amount);
    vector<Pixel> getImgData();
    Header getHeader();
    void setImgData(vector<Pixel>& newData);

};



#endif //IMAGEPROCESSING_TGAFILE_H
