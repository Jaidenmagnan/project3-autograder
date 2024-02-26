//
// Created by kkeer on 11/9/2023.
//

#ifndef IMAGE_PROCESSOR_IMAGEPROCESSING_H
#define IMAGE_PROCESSOR_IMAGEPROCESSING_H

#include <vector>
#include "Pixel.h"
#include "Header.h"

#include <fstream>

using namespace std;

class ImageProcessing {
private:
    vector<Pixel> image;
    vector<Pixel> newImage;
    Header headerObject;
    int totalPixels;
public:
    ImageProcessing(string filename);
    ImageProcessing(vector<Pixel>& image, Header& headerObject);
    Header getHeader();
    int getTotalPixels();
    vector<Pixel>& multiply(ImageProcessing &otherImage);
    vector<Pixel>& subtract(ImageProcessing &otherImage);
    vector<Pixel>& screen(ImageProcessing &otherImage);
    vector<Pixel>& overlay(ImageProcessing &background);
    vector<Pixel>& addGreenChannel(unsigned char green);
    vector<Pixel>& scaleChannels(int redFactor, int greenFactor, int blueFactor);
    vector<Pixel>& separateChannel(bool red, bool green, bool blue);
    vector<Pixel>& combineChannel(ImageProcessing& redChannel, ImageProcessing& greenChannel, ImageProcessing& blueChannel);
    vector<Pixel>& flipImage();
    vector<Pixel>& combineImages(ImageProcessing& bottomLeft, ImageProcessing& bottomRight, ImageProcessing& topLeft, ImageProcessing& topRight);

    bool operator==(const ImageProcessing& imageB);
};


#endif //IMAGE_PROCESSOR_IMAGEPROCESSING_H
