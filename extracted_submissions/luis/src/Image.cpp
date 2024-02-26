//
// Created by lmvas on 11/14/2023.
//

#include "Image.h"

using namespace std;
Image::Image() {}

Image::Image(string str) {
    GetFileInfo(str + ".tga");
}
void Image::GetFileInfo(string f) {
    filepath = f;
    ifstream file(filepath, ios_base::binary);
    if(file.is_open()){
        file.read(&h.idLen, sizeof(h.idLen));
        file.read(&h.cMapType, sizeof(h.cMapType));
        file.read(&h.dataTypeCode, sizeof(h.dataTypeCode));
        file.read((char *)&h.cMapOrigin, sizeof(h.cMapOrigin));
        file.read((char *)&h.cMapLen, sizeof(h.cMapLen));
        file.read(&h.cMapDepth, sizeof(h.cMapDepth));
        file.read((char *)&h.xO, sizeof(h.xO));
        file.read((char *)&h.yO, sizeof(h.yO));
        file.read((char *)&h.width, sizeof(h.width));
        file.read((char *)&h.height, sizeof(h.height));
        file.read(&h.bitsPerPixel, sizeof(h.bitsPerPixel));
        file.read(&h.imageDesc, sizeof(h.imageDesc));
        unsigned int area = h.width*h.height;
        vector<Pixel> v(area);
        for(unsigned int i = 0; i < area; i++){ //each pixel
            file.read((char *)&v[i].blue, 1);
            file.read((char *)&v[i].green, 1);
            file.read((char *)&v[i].red, 1);
        }
        pixels = v;
        cout << "...";
    }
}
void Image::SetSize(int height, int width) {
    this->h.height = height;
    this->h.width = width;
}
void Image::PrintFileInfo(Header file) {
    cout<<"HEADER INFO\n------------\n\n";
    cout << "ID Length: " << (int) file.idLen;
    cout << "\nColor Map Type: " << (int) file.cMapType;
    cout << "\nData Type Code: " << (int) file.dataTypeCode;
    cout << "\nColor Map Origin: " << (int) file.cMapOrigin;
    cout << "\nColor Map Length: " << (int) file.cMapLen;
    cout << "\nColor Map Depth: " << (int) file.cMapDepth;
    cout << "\nxOrigin: " << (int) file.xO;
    cout << "\nyOrigin: " << (int) file.yO;
    cout << "\nWidth: " << (int) file.width;
    cout << "\nHeight: " << (int) file.height;
    cout << "\nBits Per Pixel: " << (int) file.bitsPerPixel;
    cout << "\nImage Descriptor: " << (int) file.imageDesc;
}
void Image::CopyPixelInfo(string str) {
    ofstream outfile(str, ios_base::binary);
    if(outfile.is_open()){
        for(unsigned int i = 0; i < pixels.size(); i++){
            outfile.write((char *)&pixels[i].blue, 1);
            outfile.write((char *)&pixels[i].green, 1);
            outfile.write((char *)&pixels[i].red, 1);

        }
    }
}
void Image::WriteFile(string str) {
    string fn = "output/";
    fn += str;
    ofstream outfile(fn, ios_base::binary);
    if (outfile.is_open()) {
        outfile.write((char *) &h.idLen, sizeof(h.idLen));
        outfile.write((char *) &h.cMapType, sizeof(h.cMapType));
        outfile.write((char *) &h.dataTypeCode, sizeof(h.dataTypeCode));
        outfile.write((char *) &h.cMapOrigin, sizeof(h.cMapOrigin));
        outfile.write((char *) &h.cMapLen, sizeof(h.cMapLen));
        outfile.write((char *) &h.cMapDepth, sizeof(h.cMapDepth));
        outfile.write((char *) &h.xO, sizeof(h.xO));
        outfile.write((char *) &h.yO, sizeof(h.yO));
        outfile.write((char *) &h.width, sizeof(h.width));
        outfile.write((char *) &h.height, sizeof(h.height));
        outfile.write((char *) &h.bitsPerPixel, sizeof(h.bitsPerPixel));
        outfile.write((char *) &h.imageDesc, sizeof(h.imageDesc));
        for (unsigned int i = 0; i < pixels.size(); i++) {
            outfile.write((char *) &pixels[i].blue, 1);
            outfile.write((char *) &pixels[i].green, 1);
            outfile.write((char *) &pixels[i].red, 1);
        }
    }
    outfile.close();

}
void Image::ReloadImage() {
    ifstream file(filepath, ios_base::binary);

    if (file.is_open()) {

        if (file.is_open()) {
            file.read(&h.idLen, sizeof(h.idLen));
            file.read(&h.cMapType, sizeof(h.cMapType));
            file.read(&h.dataTypeCode, sizeof(h.dataTypeCode));
            file.read((char *)&h.cMapOrigin, sizeof(h.cMapOrigin));
            file.read((char *)&h.cMapLen, sizeof(h.cMapLen));
            file.read(&h.cMapDepth, sizeof(h.cMapDepth));
            file.read((char *)&h.xO, sizeof(h.xO));
            file.read((char *)&h.yO, sizeof(h.yO));
            file.read((char *)&h.width, sizeof(h.width));
            file.read((char *)&h.height, sizeof(h.height));
            file.read(&h.bitsPerPixel, sizeof(h.bitsPerPixel));
            file.read(&h.imageDesc, sizeof(h.imageDesc));
        }


        //pixel data
        unsigned int area = h.width*h.height;

        for (unsigned int i = 0; i < area; i++) {

            Pixel p;

            file.read((char *)&p.blue, 1);
            file.read((char *)&p.green, 1);
            file.read((char *)&p.red, 1);


            pixels[i].blue = p.blue;
            pixels[i].green = p.green;
            pixels[i].red = p.red;

        }


    }
}
void Image::AddToColor(string color, int val) {
    if(color == "blue") {
        for (unsigned int i = 0; i < pixels.size(); i++) {

            if ((pixels[i].blue + val) > 255) {
                pixels[i].blue = 255;
            } else {
                pixels[i].blue += val;
            }
        }
    }
    if (color == "green") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            if ((pixels[i].green + val) > 255) {
                pixels[i].green = 255;
            }
            else {
                pixels[i].green += val;
            }
        }
    }
    if (color == "red") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            if ((pixels[i].red + val) > 255) {
                pixels[i].red = 255;
            }
            else {
                pixels[i].red += val;
            }
        }
    }
}
void Image::Scale(string color, int val) {
    if (color == "blue") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            if ((pixels[i].blue * val) > 255) {
                pixels[i].blue = 255;
            }
            else {
                pixels[i].blue *= val;
            }
        }
    }
    if (color == "green") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            if ((pixels[i].green * val) > 255) {
                pixels[i].green = 255;
            }
            else {
                pixels[i].green *= val;
            }
        }
    }
    if (color == "red") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            if ((pixels[i].red * val) > 255) {
                pixels[i].red = 255;
            }
            else {
                pixels[i].red *= val;
            }
        }
    }
}
void Image::ToColor(string color) {
    int val = 0;
    if (color == "blue") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            val = pixels[i].blue;
            pixels[i].green = val;
            pixels[i].red = val;
        }
    }
    else if (color == "green") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            val = pixels[i].green;
            pixels[i].blue = val;
            pixels[i].red = val;
        }
    }
    else if (color == "red") {
        for (unsigned int i = 0; i < pixels.size(); i++) {
            val = pixels[i].red;
            pixels[i].green = val;
            pixels[i].blue = val;
        }
    }
}
bool Image::Equals(Image img) { //if the image matches the example
    bool equal = true;

    if (h.idLen != img.h.idLen) {
        equal = false;
    }
    if (h.cMapType != img.h.cMapType) {
        equal = false;
    }
    if (h.dataTypeCode != img.h.dataTypeCode) {
        equal = false;
    }
    if (h.cMapOrigin != img.h.cMapOrigin) {
        equal = false;
    }
    if (h.cMapLen != img.h.cMapLen) {
        equal = false;
    }
    if (h.cMapDepth != img.h.cMapDepth) {
        equal = false;
    }
    if (h.xO != img.h.xO) {
        equal = false;
    }
    if (h.yO != img.h.yO) {
        equal = false;
    }
    if (h.width != img.h.width) {
        equal = false;
    }
    if (h.height != img.h.height) {
        equal = false;
    }
    if (h.bitsPerPixel != img.h.bitsPerPixel) {
        equal = false;
    }
    if (h.imageDesc != img.h.imageDesc) {
        equal = false;
    }

    if (pixels.size() != img.pixels.size()) {
        equal = false;
    }

    for (unsigned int i = 0; i < pixels.size(); i++) {

        if (pixels[i].blue != img.pixels[i].blue) {
            equal = false;
        }
        if (pixels[i].green != img.pixels[i].green) {
            equal = false;
        }
        if (pixels[i].red != img.pixels[i].red) {
            equal = false;
        }

    }

    return equal;

}
