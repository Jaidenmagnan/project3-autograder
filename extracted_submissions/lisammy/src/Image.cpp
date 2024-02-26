#include <fstream>
#include "Image.h"
#include <iostream>
#include "pixelData.h"
using namespace std;

//Reads the image data and stores in the Image object.
//Can call on each aspect of the image.
void Image::readImageData(const string& filename) {
    fstream inFile(filename, ios::in | ios::binary);
    if (inFile.is_open()){
        inFile.read(&idLength, sizeof(idLength));
        inFile.read(&colorMapType, sizeof(colorMapType));
        inFile.read(&dataTypeCode, sizeof(dataTypeCode));
        inFile.read((char *)(&colorMapOrigin), sizeof(colorMapOrigin));
        inFile.read((char *)(&colorMapLength), sizeof(colorMapLength));
        inFile.read(&colormapDepth, sizeof(colormapDepth));
        inFile.read((char *)(&xOrigin), sizeof(xOrigin));
        inFile.read((char *)(&yOrigin), sizeof(yOrigin));
        inFile.read((char *)(&width), sizeof(width));
        inFile.read((char *)(&height), sizeof(height));
        inFile.read(&bitsPerPixel, sizeof(bitsPerPixel));
        inFile.read(&ImageDescriptor, sizeof(ImageDescriptor));
        inFile.seekg(18);
        int size = width * height;

        pixelData pixeldata{};
        for (int i=0; i<size; i++){
            inFile.read((char *)(&pixeldata.blueValue), sizeof(pixeldata.blueValue));
            inFile.read((char *)(&pixeldata.greenValue), sizeof(pixeldata.greenValue));
            inFile.read((char *)(&pixeldata.redValue), sizeof(pixeldata.redValue));
            pixelVectors.emplace_back(pixeldata);
        }
        inFile.close();
    }
    else{
        cout << "failed" << endl;
    }
}

