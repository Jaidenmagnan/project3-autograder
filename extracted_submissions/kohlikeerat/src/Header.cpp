//
// Created by kkeer on 11/9/2023.
//

#include "Header.h"

// Given an instance of a Header and an ofstream, we simply write the Header to the ofstream buffer
void writeHeader(Header &headerObject, ofstream &outStream) {

    outStream.write(&headerObject.idLength, sizeof(headerObject.idLength));
    outStream.write(&headerObject.colorMapType, sizeof(headerObject.colorMapType));
    outStream.write(&headerObject.dataTypeCode, sizeof(headerObject.dataTypeCode));
    outStream.write((char*)&headerObject.colorMapOrigin, sizeof(headerObject.colorMapOrigin));
    outStream.write((char*)&headerObject.colorMapLength, sizeof(headerObject.colorMapLength));
    outStream.write(&headerObject.colorMapDepth, sizeof(headerObject.colorMapDepth));
    outStream.write((char*)&headerObject.xOrigin, sizeof(headerObject.xOrigin));
    outStream.write((char*)&headerObject.yOrigin, sizeof(headerObject.yOrigin));
    outStream.write((char*)&headerObject.width, sizeof(headerObject.width));
    outStream.write((char*)&headerObject.height, sizeof(headerObject.height));
    outStream.write(&headerObject.bitsPerPixel, sizeof(headerObject.bitsPerPixel));
    outStream.write(&headerObject.imageDescriptor, sizeof(headerObject.imageDescriptor));
}

// Given an instance of a Header and an ifstream, we simply read in the Header to the ifstream buffer
void readHeader(Header &headerObject, ifstream &inStream) {
    inStream.read(&headerObject.idLength, sizeof(headerObject.idLength));
    inStream.read(&headerObject.colorMapType, sizeof(headerObject.colorMapType));
    inStream.read(&headerObject.dataTypeCode, sizeof(headerObject.dataTypeCode));
    inStream.read((char*)&headerObject.colorMapOrigin, sizeof(headerObject.colorMapOrigin));
    inStream.read((char*)&headerObject.colorMapLength, sizeof(headerObject.colorMapLength));
    inStream.read(&headerObject.colorMapDepth, sizeof(headerObject.colorMapDepth));
    inStream.read((char*)&headerObject.xOrigin, sizeof(headerObject.xOrigin));
    inStream.read((char*)&headerObject.yOrigin, sizeof(headerObject.yOrigin));
    inStream.read((char*)&headerObject.width, sizeof(headerObject.width));
    inStream.read((char*)&headerObject.height, sizeof(headerObject.height));
    inStream.read(&headerObject.bitsPerPixel, sizeof(headerObject.bitsPerPixel));
    inStream.read(&headerObject.imageDescriptor, sizeof(headerObject.imageDescriptor));
}

// Overload the equality operator to make it easier to check if two headers are equal.
bool operator==(const Header& headerA, const Header& headerB) {
    return (headerA.idLength == headerB.idLength && headerA.colorMapType == headerB.colorMapType && headerA.dataTypeCode == headerB.dataTypeCode
            && headerA.colorMapOrigin == headerB.colorMapOrigin && headerA.colorMapLength == headerB.colorMapLength && headerA.colorMapDepth ==
            headerB.colorMapDepth && headerA.xOrigin == headerB.xOrigin && headerA.yOrigin == headerB.yOrigin && headerA.width == headerB.width &&
            headerA.height == headerB.height && headerA.bitsPerPixel == headerB.bitsPerPixel && headerA.imageDescriptor == headerB.imageDescriptor);
}