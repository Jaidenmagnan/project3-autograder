#include <string>
#include <vector>
#include "pixelData.h"
using namespace std;

#ifndef IMAGE_PROCESSOR_HEADER_H
#define IMAGE_PROCESSOR_HEADER_H

//Structure image data
struct Image{
public:
    void readImageData(const string& filename);
    vector <pixelData> pixelVectors;
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colormapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char ImageDescriptor;
};
#endif //IMAGE_PROCESSOR_HEADER_H
