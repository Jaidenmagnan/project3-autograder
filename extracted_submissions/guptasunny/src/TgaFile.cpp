//
// Created by sgupt on 11/15/2023.
//
#pragma once
#include "TgaFile.h"
#include "vector"
#include <fstream>
#include <iostream>

TgaFile::TgaFile(vector<Pixel>& imageData, Header& imgHeader) {
    this->fileHeader = imgHeader; //secondary constructor; this one is mostly for testing purposes
    this->imgData = imageData;
}

TgaFile::TgaFile(string& fileName) {
    ifstream inStream( fileName, ios::binary); //start attempting to read file
    if (!inStream.is_open()){
        throw runtime_error("File does not exist."); //checks that file is open and readable
    }
    inStream.read(&fileHeader.idLength, sizeof(fileHeader.idLength)); //following block reads out first 18 bytes of data from the file into a header struct instance
    inStream.read(&fileHeader.colorMapType, sizeof(fileHeader.colorMapType));
    inStream.read(&fileHeader.dataTypeCode, sizeof(fileHeader.dataTypeCode));
    inStream.read((char*) &fileHeader.colorMapOrigin, sizeof(fileHeader.colorMapOrigin));
    inStream.read((char*) &fileHeader.colorMapLength, sizeof(fileHeader.colorMapLength));
    inStream.read(&fileHeader.colorMapDepth, sizeof(fileHeader.colorMapDepth));
    inStream.read((char*) &fileHeader.xOrigin, sizeof(fileHeader.xOrigin));
    inStream.read((char*) &fileHeader.yOrigin, sizeof(fileHeader.yOrigin));
    inStream.read((char*) &fileHeader.width, sizeof(fileHeader.width));
    inStream.read((char*) &fileHeader.height, sizeof(fileHeader.height));
    inStream.read(&fileHeader.bitsPerPixel, sizeof(fileHeader.bitsPerPixel));
    inStream.read(&fileHeader.imageDescriptor, sizeof(fileHeader.imageDescriptor));

    int imageSize = int(fileHeader.height * fileHeader.width); //imageData size is number of pixels
    for (int i = 0; i < imageSize; i++){ //iterates through the rest of the file byte by byte; each byte is stored in an instance of the pixel class, going blue-green-red, as that is the order of each color in the file
        Pixel newPixel; //creates new empty pixel that has 3 unsigned char member variables
        inStream.read((char*)&newPixel.blue, sizeof(unsigned char));
        inStream.read((char*)&newPixel.green, sizeof(unsigned char));
        inStream.read((char*)&newPixel.red, sizeof(unsigned char));
        imgData.push_back(newPixel); //after values are stored in the pixel, it is pushed to the imgData vector of this object
    }
    inStream.close(); //file closed!
}


vector<Pixel> TgaFile::getImgData() {
    return this->imgData; //accessor for imgData vector
}


Header TgaFile::getHeader() {
    return this->fileHeader; //accessor for header
}


unsigned char de_normalize(float deNormal){
    unsigned char deNormaled = (unsigned char)(deNormal*255.0f + 0.5f); //denormalizes float values by multiplying by 255, adding 0.5, and then casting to unsigned char
    return deNormaled;
}

vector<Pixel> TgaFile::multiply(TgaFile& bottomLayer){ //multiply function; called by top layer, bottom layer passed in
    vector<Pixel> imgCData; //empty vector to be added to
    vector<Pixel> imgAData = this->getImgData(); //top layer data
    vector<Pixel> imgBData = bottomLayer.getImgData(); //bottom layer data
    for (int i = 0; i < imgAData.size(); i++){
        Pixel newPixel; //creates empty pixel
        float cBlue = (imgAData[i].blue/255.0f) * (imgBData[i].blue/255.0f);  //normalizes top and bottom layers pixel values, multiplies them, and stores them in a float
        float cGreen = (imgAData[i].green/255.0f) * (imgBData[i].green/255.0f);
        float cRed = (imgAData[i].red/255.0f) * (imgBData[i].red/255.0f);
        newPixel.blue = de_normalize(cBlue); //values are denormalized and made member variables of newPixel
        newPixel.green = de_normalize(cGreen);
        newPixel.red = de_normalize(cRed);
        imgCData.push_back(newPixel); //newPixel pushed to new vector that is then returned
    }
    return imgCData;
}

vector<Pixel> TgaFile::screen(TgaFile& bottomLayer){
    vector<Pixel> imgCData; //empty vector to be added to
    vector<Pixel> imgAData = this->getImgData(); //top layer data
    vector<Pixel> imgBData = bottomLayer.getImgData(); //bottom layer data
    for (int i = 0; i < (imgAData).size(); i++){
        Pixel newPixel; //creates empty pixel
        float cBlue = 1-(1-imgAData[i].blue/255.0f) * (1-imgBData[i].blue/255.0f); //normalizes top and bottom layers pixel values, works them as specified by the documentation, and stores them in a float
        float cGreen = 1-(1-imgAData[i].green/255.0f) * (1-imgBData[i].green/255.0f);
        float cRed = 1-(1-imgAData[i].red/255.0f) * (1-imgBData[i].red/255.0f);
        newPixel.blue = de_normalize(cBlue); //values are denormalized and made member variables of newPixel
        newPixel.green = de_normalize(cGreen);
        newPixel.red = de_normalize(cRed);
        imgCData.push_back(newPixel); //newPixel pushed to new vector that is then returned
    }
    return imgCData;
}

vector<Pixel> TgaFile::overlay(TgaFile& bottomLayer){
    vector<Pixel> imgCData;
    vector<Pixel> imgAData = this->getImgData();
    vector<Pixel> imgBData = bottomLayer.getImgData();
    for (int i = 0; i < imgAData.size(); i++){
        Pixel newPixel;
        float cBlue = imgBData[i].blue/255.0f; //values are initialized to bottom layer values
        float cRed = imgBData[i].red/255.0f;
        float cGreen = imgBData[i].green/255.0f;
        if(cBlue <= 0.5){ //checks the bottom layer value's strength
            cBlue = 2*(imgAData[i].blue/255.0f) * (imgBData[i].blue/255.0f); //performs necessary calculations
        }
        else{ //if blue stronger than 0.5, performs alternative calc
            cBlue = 1-2*(1-imgAData[i].blue/255.0f) * (1-imgBData[i].blue/255.0f);
        }
        if(cRed <= 0.5){ //same if-else block for other colors
            cRed = 2*(imgAData[i].red/255.0f) * (imgBData[i].red/255.0f);
        }
        else{
            cRed = 1-2*(1-imgAData[i].red/255.0f) * (1-imgBData[i].red/255.0f);
        }
        if(cGreen <= 0.5){
            cGreen = 2*(imgAData[i].green/255.0f) * (imgBData[i].green/255.0f);
        }
        else{
            cGreen = 1-2*(1-imgAData[i].green/255.0f) * (1-imgBData[i].green/255.0f);
        }
        newPixel.blue = de_normalize(cBlue); //values are denormalized and made member variables of newPixel
        newPixel.green = de_normalize(cGreen);
        newPixel.red = de_normalize(cRed);
        imgCData.push_back(newPixel); //newPixel pushed to new vector that is then returned
    }
    return imgCData;
}

vector<Pixel> TgaFile::addChannel(string& channel, int& amount){
    vector<Pixel> imgCData;
    vector<Pixel> imgAData = this->getImgData(); //base imageData
    for (int i = 0; i < imgAData.size(); i++){
        Pixel p; //creates empty pixel
        if (channel == "red"){ //if they want to manipulate red, blue and green are added to the pixel as they are, and then red is added after manipulatio0n
            p.blue = imgAData[i].blue;
            p.green = imgAData[i].green;
            if ((int)imgAData[i].red + amount > 255){ //clamping for upper limit
                p.red = 255;
            }
            else if ((int)imgAData[i].red + amount < 0){ //clamping for lower limit
                p.red = 0;
            }
            else{ //if no need to clamp
                p.red = imgAData[i].red + amount;
            }
        }
        else if (channel == "green"){ //same function but for other colors
            p.blue = imgAData[i].blue;
            p.red = imgAData[i].red;
            if ((int)imgAData[i].green + amount > 255){
                p.green = 255;
            }
            else if ((int)imgAData[i].green + amount < 0){
                p.green = 0;
            }
            else{
                p.green = imgAData[i].green + amount;
            }
        }
        else if (channel == "blue"){
            p.red = imgAData[i].red;
            p.green = imgAData[i].green;
            if ((int)imgAData[i].blue + amount > 255){
                p.blue = 255;
            }
            else if ((int)imgAData[i].blue + amount < 0){
                p.blue = 0;
            }
            else{
                p.blue = imgAData[i].blue + amount;
            }
        }
        imgCData.push_back(p); //pixel is pushed to vector which is then returned
    }
    return imgCData;
}

vector<Pixel>  TgaFile::subtract(TgaFile& bottomLayer){
    vector<Pixel> imgCData;
    vector<Pixel> imgAData = this->getImgData(); //method is called by the top layer, and bottom layer is passed in
    vector<Pixel> imgBData = bottomLayer.getImgData();
    for (int i = 0; i < imgAData.size(); i++){
        Pixel p; //creates empty pixel
        if ((int)imgBData[i].blue - (int)imgAData[i].blue < 0 ){
            p.blue = 0; //blue clamping
        }
        else{
            p.blue = (unsigned char)((int)imgBData[i].blue - (int)imgAData[i].blue); //if no need to be clamped, perform the subtraction, subtracting the top layer from the bottom
        }
        if ((int)imgBData[i].green - (int)imgAData[i].green < 0 ){ //same for green and red
            p.green = 0;
        }
        else{
            p.green = (unsigned char)((int)imgBData[i].green - (int)imgAData[i].green);
        }
        if ((int)imgBData[i].red - (int)imgAData[i].red < 0 ){
            p.red = 0;
        }
        else{
            p.red = (unsigned char)((int)imgBData[i].red - (int)imgAData[i].red);
        }
        imgCData.push_back(p); //pixel is pushed to vector which is then returned
    }
    return imgCData;
}


vector<Pixel> TgaFile::scale(float& multiplier, string& channel){
    vector<Pixel> imgCData;
    vector<Pixel> imgAData = this->getImgData();
    for (int i = 0; i < imgAData.size(); i++){
        Pixel newPixel; //empty pixel
        float cBlue = imgAData[i].blue/255.0f; //float values are initialized for simplicity
        float cRed = imgAData[i].red/255.0f;
        float cGreen = imgAData[i].green/255.0f;
        if (channel == "blue") { //checks which channel to change
            if (cBlue * multiplier > 1.0f) { //clamping upper limit
                cBlue = 1.0f;
            } else if (cBlue * multiplier == 0.0f) { //clamping lower limit
                cBlue = 0.0f;
            } else { //if no need to clamp
                cBlue *= multiplier;
            }
        }
        else if (channel == "green") { //same for green and red
            if (cGreen * multiplier > 1.0f) {
                cGreen = 1.0f;
            } else if (cGreen * multiplier == 0.0f) {
                cGreen = 0.0f;
            } else {
                cGreen *= multiplier;
            }
        }
        else if (channel == "red") {
            if (cRed * multiplier > 1.0f) {
                cRed = 1.0f;
            } else if (cRed * multiplier == 0.0f) {
                cRed = 0.0f;
            } else {
                cRed *= multiplier;
            }
        }
        newPixel.blue = de_normalize(cBlue); //values denormalized and then added to new pixel
        newPixel.green = de_normalize(cGreen);
        newPixel.red = de_normalize(cRed);
        imgCData.push_back(newPixel); //newPixel pushed to vector, which is then returned
    }
    return imgCData;
}


void TgaFile::setImgData(vector<Pixel>& newData){
    this->imgData = newData;
}




