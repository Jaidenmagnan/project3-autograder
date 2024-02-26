//
// Created by tinam on 11/9/2023.
//

#include "Header.h"

using namespace std;
using namespace std::filesystem;

// read a header from an existing file
void assignHeader(Header& headerObj, const string& fileName) {
    fstream tgaFile;
    // open file in binary mode
    tgaFile.open(fileName, ios::in | ios::binary);
    if (tgaFile) {

        // id length
        tgaFile.read(&headerObj.idLength, sizeof(headerObj.idLength));
        // color map type
        tgaFile.read(&headerObj.colorMapType, sizeof(headerObj.colorMapType));
        // image type
        tgaFile.read(&headerObj.datTypeCode, sizeof(headerObj.datTypeCode));
        // color map specification
        tgaFile.read(reinterpret_cast<char *>(&headerObj.colorMapOrigin), sizeof(headerObj.colorMapOrigin));
        tgaFile.read(reinterpret_cast<char *>(&headerObj.colorMapLength), sizeof(headerObj.colorMapLength));
        tgaFile.read(&headerObj.colorMapDepth, sizeof(headerObj.colorMapDepth));
        // image specification
        tgaFile.read(reinterpret_cast<char *>(&headerObj.xOrigin), sizeof(headerObj.xOrigin));
        tgaFile.read(reinterpret_cast<char *>(&headerObj.yOrigin), sizeof(headerObj.yOrigin));
        tgaFile.read(reinterpret_cast<char *>(&headerObj.width), sizeof(headerObj.width));
        tgaFile.read(reinterpret_cast<char *>(&headerObj.height), sizeof(headerObj.height));
        tgaFile.read(&headerObj.bitsPerPixel, sizeof(headerObj.bitsPerPixel));//pixel depth
        tgaFile.read(&headerObj.imageDescriptor, sizeof(headerObj.imageDescriptor));
        tgaFile.close();
    }
    else{
        cout << "Error: File not open" << endl;
    }
}
// function to write a header into a file
void outHeader(Header& headerObj, const string& fileName) {
    ofstream outputFile(fileName, ios::binary | ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error opening the file for writing!" << endl;
    }
    //write header into file
    outputFile.write(reinterpret_cast<const char *>(&headerObj), sizeof(headerObj));
    outputFile.close();
}
void printHeader(const Header& headerObj)
{
    std::cout << "idLength: " << static_cast<int>(headerObj.idLength) << std::endl;
    std::cout << "colorMapType: " << static_cast<int>(headerObj.colorMapType) << std::endl;
    std::cout << "datTypeCode: " << static_cast<int>(headerObj.datTypeCode) << std::endl;
    std::cout << "colorMapOrigin: " << headerObj.colorMapOrigin << std::endl;
    std::cout << "colorMapLength: " << headerObj.colorMapLength << std::endl;
    std::cout << "colorMapDepth: " << static_cast<int>(headerObj.colorMapDepth) << std::endl;
    std::cout << "xOrigin: " << headerObj.xOrigin << std::endl;
    std::cout << "yOrigin: " << headerObj.yOrigin << std::endl;
    std::cout << "width: " << headerObj.width << std::endl;
    std::cout << "height: " << headerObj.height << std::endl;
    std::cout << "bitsPerPixel: " << static_cast<int>(headerObj.bitsPerPixel) << std::endl;
    std::cout << "imageDescriptor: " << static_cast<int>(headerObj.imageDescriptor) << std::endl;
}