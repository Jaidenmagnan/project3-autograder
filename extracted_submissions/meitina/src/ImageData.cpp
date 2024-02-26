//
// Created by tinam on 11/10/2023.
// This class handles the image data part of a tga file

#include "ImageData.h"

using namespace std;

// function to round and clamp when converting unsigned char to float
unsigned char roundAndClamp(float value) {
    // add 0.5 for rounding and clamp the result to the valid range [0, 255]
    return static_cast<unsigned char>(min(255.0f, max(0.0f, value + 0.5f)));
}
//functions to assign pixels values from a function to pixelArray
void assignPixels(ImageData& imgObj, const string& fileName, int rows, int cols)
{
    imgObj.pixelArray = new unsigned char[rows*cols*3];
    fstream tgaFile;
    // open file in binary mode
    tgaFile.open(fileName, ios::in | ios::binary);

    if (tgaFile.is_open()) {
        tgaFile.seekg(18, ios::beg);
        for(int i = 0; i < rows*cols*3; i++)
        {
            unsigned char buffer;
            tgaFile.read(reinterpret_cast<char*>(&buffer), sizeof(unsigned char));
            imgObj.pixelArray[i] = buffer;
        }
        tgaFile.close();
    }
    else{
        cout << "Error: File not open" << endl;
    }
}
//function to multiply layers
void multiplyPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData)
{
    newData.pixelArray = new unsigned char[rows*cols*3];
    const int arraySize = rows * cols * 3;
    float* floatTop = new float[arraySize];
    float* floatBot = new float[arraySize];

    for(int i = 0 ; i < arraySize; i++) {
        // divide RGB values by 255 and put into float array
        floatTop[i] = static_cast<float>(top.pixelArray[i])/255.0f;
        floatBot[i] = static_cast<float>(bot.pixelArray[i])/255.0f;
        //multiply top and bot together, then by 255 and convert back to unsigned char with rounding
        newData.pixelArray[i] = roundAndClamp((floatTop[i]*floatBot[i])*255.0f);
    }
    // deallocate memory
    delete[] floatTop;
    delete[] floatBot;
}
// function to subtract layers
void subtractPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData)
{
    newData.pixelArray = new unsigned char[rows*cols*3];
    const int arraySize = rows * cols * 3;
    for(int i = 0 ; i < arraySize; i++) {
        // subtract each pixel in top from bot
        newData.pixelArray[i] = roundAndClamp(static_cast<float>(bot.pixelArray[i])-static_cast<float>(top.pixelArray[i]));
    }
}
//function to screen layers
void screenPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData)
{
    newData.pixelArray = new unsigned char[rows*cols*3];
    const int arraySize = rows * cols * 3;
    float* floatTop = new float[arraySize];
    float* floatBot = new float[arraySize];

    for(int i = 0 ; i < arraySize; i++) {
        // divide RGB values by 255 and subtract from 1
        floatTop[i] = 1.0f - (static_cast<float>(top.pixelArray[i])/255.0f);
        floatBot[i] = 1.0f - (static_cast<float>(bot.pixelArray[i])/255.0f);
        //multiply negatives together and subtract from 1
        newData.pixelArray[i] = roundAndClamp((1.0f-(floatTop[i]*floatBot[i]))*255.0f);
    }
    // deallocate memory
    delete[] floatTop;
    delete[] floatBot;
}
// function to overlay layers
void overlayPixels(ImageData& top, ImageData& bot, int rows, int cols, ImageData& newData)
{
    const int arraySize = rows * cols * 3;
    newData.pixelArray = new unsigned char[arraySize];
    float* floatTop = new float[arraySize];
    float* floatBot = new float[arraySize];
    // copy unsigned chars to float array to do calculations
    for (int i = 0; i < arraySize; i++) {
        floatTop[i] = static_cast<float>(top.pixelArray[i]);
        floatBot[i] = static_cast<float>(bot.pixelArray[i]);
    }
    // iterate through each pixel instead of each byte
    for(int i = 0 ; i < arraySize; i += 3) {
        // if tonal value is less than or equal to 50%
        if (((floatBot[i]/255.0f) + (floatBot[i+1]/255.0f) + (floatBot[i+2])/255.0f)/3.0f <= 0.5f) {
            for (int j = 0; j < 3; j++) { // j to iterate through single pixel
                // multiply then x2
                newData.pixelArray[i+j] = roundAndClamp(((2.0f*((floatTop[i+j]/255.0f)*(floatBot[i+j]/255.0f)))*255.0f));
            }
        }
            //if tonal value is greater than 50% then screen but x 2
        else {
            for (int j = 0; j < 3; j++) { // j to iterate through single pixel

                floatTop[i+j] = 1.0f - (static_cast<float>(top.pixelArray[i+j])/255.0f);
                floatBot[i+j] = 1.0f - (static_cast<float>(bot.pixelArray[i+j])/255.0f);
                // c = 1-2(1-A)(1-B)
                newData.pixelArray[i+j] = roundAndClamp((1.0f-(2.0f*floatTop[i+j]*floatBot[i+j]))*255.0f);
            }
        }
    }
    // deallocate memory
    delete[] floatTop;
    delete[] floatBot;
}
// edit channel
void editChannel(ImageData& imgObj, int rows, int cols, const string& channel, float add, float scale, ImageData& newData)
{
    // choose channel based on argument passed
    int chan;
    if(channel == "red"){
        chan = 2;
    }
    else if (channel == "blue"){
        chan = 0;
    }
    else if(channel == "green"){
        chan = 1;
    }
    else{
        cout << "invalid channel" << endl;
        return;
    }
    const int arraySize = rows*cols*3;
    newData.pixelArray = new unsigned char[arraySize];
    // copy values to new array
    memcpy(newData.pixelArray, imgObj.pixelArray, arraySize);
    // add value or multiply value or both to one channel of every pixel
    for(int i = chan; i < arraySize; i = i + 3){
        newData.pixelArray[i] = roundAndClamp(static_cast<float>(imgObj.pixelArray[i]) * scale + add);
    }
}
//separate channels into different files
void separateChannel(ImageData& imgObj, int rows, int cols, const string& channel, ImageData& newData) {
    // choose channel based on argument passed
    int chan;
    if (channel == "red") {
        chan = 2;
    } else if (channel == "blue") {
        chan = 0;
    } else if (channel == "green") {
        chan = 1;
    } else {
        cout << "invalid channel" << endl;
        return;
    }
    const int arraySize = rows*cols*3;
    newData.pixelArray = new unsigned char[arraySize];
    // iterate through pixels
    for (int pix = 0; pix < arraySize; pix = pix + 3) {
        // assign each channel in one pixel to the same value
        newData.pixelArray[pix] = imgObj.pixelArray[pix+chan];
        newData.pixelArray[pix + 1] = imgObj.pixelArray[pix+chan];
        newData.pixelArray[pix + 2] = imgObj.pixelArray[pix+chan];
    }
}
// function to rotate an image 180 degrees
void rotate180(ImageData& imgData, int rows, int cols, ImageData& newData)
{
    const int arraySize = rows*cols*3;
    newData.pixelArray = new unsigned char[arraySize];
    // iterate through each pixel
    for(int i = 0; i < arraySize; i = i + 3) {
        for (int j = 0; j < 3; j++) { // j to iterate through single pixel
            newData.pixelArray[i + j] = imgData.pixelArray[arraySize - (i + (2 - j)) -1];
        }
    }
}
// function to create ImageData obj from 3 files
void assignChannels(ImageData& redImg, ImageData& greenImg, ImageData& blueImg, int rows, int cols, ImageData& newData)
{
    const int arraySize = rows*cols*3;
    newData.pixelArray = new unsigned char[arraySize];
    // iterate through each pixel
    for(int i = 0; i < arraySize; i = i + 3){
        newData.pixelArray[i] = blueImg.pixelArray[i]; // blue channel
        newData.pixelArray[i + 1] = greenImg.pixelArray[i]; // green channel
        newData.pixelArray[i + 2] = redImg.pixelArray[i]; // red channel
    }
}

// function to write data into a file
void outData(ImageData& writeImg, const string& fileName, int rows, int cols)
{
    const int arraySize = rows*cols*3;
    ofstream outputFile(fileName, ios::binary| std::ios::app);
    if (!outputFile.is_open()) {
        cerr << "Error opening the file for writing!" << std::endl;
        return;
    }
    outputFile.write(reinterpret_cast<const char *>(writeImg.pixelArray), arraySize);
    outputFile.close();
}
void deallImgData(ImageData& imgObj) {
    // make sure pixelArray is not nullptr
    delete[] imgObj.pixelArray;
    imgObj.pixelArray = nullptr;  // set to nullptr after deletion to avoid dangling ptrs
}
