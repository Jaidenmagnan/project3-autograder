#include <iostream>
#include <fstream>
#include "Image.h"

#ifndef IMAGE_PROCESSOR_OUTFILE_H
#define IMAGE_PROCESSOR_OUTFILE_H
//Used for creating the new outfiles
class outFile{
public:
    //Creates the outfiles header and returns the outfile with just the header.
    static fstream createOutFileHeader(const string& filename, Image readFile){
        fstream outFile(filename, ios::out | ios::binary);
        outFile.write(&readFile.idLength, sizeof(readFile.idLength));
        outFile.write(&readFile.colorMapType, sizeof(readFile.colorMapType));
        outFile.write(&readFile.dataTypeCode, sizeof(readFile.dataTypeCode));
        outFile.write((char*)(&readFile.colorMapOrigin), sizeof(readFile.colorMapOrigin));
        outFile.write((char*)(&readFile.colorMapLength), sizeof(readFile.colorMapLength));
        outFile.write(&readFile.colormapDepth, sizeof(readFile.colormapDepth));
        outFile.write((char*)(&readFile.xOrigin), sizeof(readFile.xOrigin));
        outFile.write((char*)(&readFile.yOrigin), sizeof(readFile.yOrigin));
        outFile.write((char*)(&readFile.width), sizeof(readFile.width));
        outFile.write((char*)(&readFile.height), sizeof(readFile.height));
        outFile.write(&readFile.bitsPerPixel, sizeof(readFile.bitsPerPixel));
        outFile.write(&readFile.ImageDescriptor, sizeof(readFile.ImageDescriptor));
        return outFile;
    }
    //Creates the final file and image needed to go to output file.
    static void createOutFileFinal(const string& filename, Image readFile){
        fstream outFile(filename, ios::out | ios::binary);
        outFile.write(&readFile.idLength, sizeof(readFile.idLength));
        outFile.write(&readFile.colorMapType, sizeof(readFile.colorMapType));
        outFile.write(&readFile.dataTypeCode, sizeof(readFile.dataTypeCode));
        outFile.write((char*)(&readFile.colorMapOrigin), sizeof(readFile.colorMapOrigin));
        outFile.write((char*)(&readFile.colorMapLength), sizeof(readFile.colorMapLength));
        outFile.write(&readFile.colormapDepth, sizeof(readFile.colormapDepth));
        outFile.write((char*)(&readFile.xOrigin), sizeof(readFile.xOrigin));
        outFile.write((char*)(&readFile.yOrigin), sizeof(readFile.yOrigin));
        outFile.write((char*)(&readFile.width), sizeof(readFile.width));
        outFile.write((char*)(&readFile.height), sizeof(readFile.height));
        outFile.write(&readFile.bitsPerPixel, sizeof(readFile.bitsPerPixel));
        outFile.write(&readFile.ImageDescriptor, sizeof(readFile.ImageDescriptor));
        for (int i=0; i<size(readFile.pixelVectors); i++){
            outFile << readFile.pixelVectors[i].blueValue;
            outFile << readFile.pixelVectors[i].greenValue;
            outFile << readFile.pixelVectors[i].redValue;
        }
    }
};
#endif //IMAGE_PROCESSOR_OUTFILE_H
