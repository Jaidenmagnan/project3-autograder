/* 
Project 3 
-------------
Lucas Tembras
Prof. Yichi Zhang
COP3504C
11/14/2023
*/

#include "Header.h"
#include "Pixel.h"
#include <iostream>
#include <fstream>
#include <exception>

//function declarations
Header returnHeader(const std::string&);
std::vector<Pixel*> getPixelData(const std::string&);
std::vector<Pixel*> multiply(const std::string&, const std::string&);
std::vector<Pixel*> subtract(const std::string&, const std::string&);
std::vector<Pixel*> subtract(const std::vector<Pixel*>& , const std::vector<Pixel*>&); //overloaded version of the first that accepts two vectors containing pixel data
std::vector<Pixel*> screen(const std::vector<Pixel*>& , const std::vector<Pixel*>&); 
std::vector<Pixel*> overlay(const std::string&, const std::string&);
void outputFile(const std::string&, const std::vector<Pixel*>&, const std::string&);
unsigned char grayScale(const Pixel*);
unsigned char clamp (const int &);
void unitTests(const std::vector<Pixel*>&, const std::vector<Pixel*>&, const int&);


//functions that take file paths as parameters and output new images to /output folder (part1 - extra credit)
void part1(const std::string&, const std::string&);
void part2(const std::string&, const std::string&);
void part3(const std::string&, const std::string&, const std::string&);
void part4(const std::string&, const std::string&, const std::string&);
void part5(const std::string&, const std::string&);
void part6(const std::string&);
void part7(const std::string&);
void part8r(const std::string&);
void part8g(const std::string&);
void part8b(const std::string&);
void part9(const std::string&, const std::string&,const std::string&);
void part10(const std::string&);
void extra_credit(const std::string&, const std::string&, const std::string&, const std::string&);


int main() {

    //all the functions within main will perform the required operations and output the new .tga fields to the /output folder 
    part1("input/layer1.tga", "input/pattern1.tga");
    part2("input/layer2.tga","input/car.tga");
    part3("input/layer1.tga","input/pattern2.tga", "input/text.tga");
    part4("input/layer2.tga", "input/circles.tga", "input/pattern2.tga");
    part5("input/layer1.tga","input/pattern1.tga");
    part6("input/car.tga");
    part7("input/car.tga");
    part8r("input/car.tga");
    part8g("input/car.tga");
    part8b("input/car.tga");
    part9("input/layer_red.tga","input/layer_green.tga","input/layer_blue.tga");
    part10("input/text2.tga");
    extra_credit("input/text.tga","input/pattern1.tga","input/car.tga","input/circles.tga");
    return 0;
}

Header returnHeader(const std::string& file){ //pass in fileName
    Header headerObject; //creates headerObject
    std::ifstream inStream; //creates ifstream object
    inStream.open(file, std::ios::binary); //opens ifstream file
    inStream.read(&headerObject.idLength, sizeof(headerObject.idLength)); //read idLength
    inStream.read(&headerObject.colorMapType, sizeof(headerObject.colorMapType)); //read colorMapType
    inStream.read(&headerObject.dataTypeCode, sizeof(headerObject.dataTypeCode)); //read dataTypeCode
    inStream.read((char *) &headerObject.colorMapOrigin, sizeof(headerObject.colorMapOrigin)); //read colorMapOrigin
    inStream.read((char *) &headerObject.colorMapLength, sizeof(headerObject.colorMapLength)); //read colorMapDepth
    inStream.read(&headerObject.colorMapDepth, sizeof(headerObject.colorMapDepth)); //read colorMapDepth
    inStream.read((char *) &headerObject.xOrigin, sizeof(headerObject.xOrigin)); //read xOrigin
    inStream.read((char *) &headerObject.yOrigin, sizeof(headerObject.yOrigin)); //read yOrigin
    inStream.read((char *) &headerObject.width, sizeof(headerObject.width)); //read width
    inStream.read((char *) &headerObject.height, sizeof(headerObject.height)); //read height
    inStream.read(&headerObject.bitsPerPixel, sizeof(headerObject.bitsPerPixel)); //read bitsPerPixel
    inStream.read(&headerObject.imageDescriptor, sizeof(headerObject.imageDescriptor)); //read imageDescriptor
    inStream.close(); //close the ifstream object
    return headerObject; 

};

std::vector<Pixel*> getPixelData(const std::string& fileName){
    Header headerObject = returnHeader(fileName); //sets headerObject equal to the header corresponding to the specified filepath
    std::ifstream inStream;
    std::string file = fileName; 
    inStream.open(file, std::ios::binary);
    std::vector<Pixel*> pixels; //will store vectors
    inStream.seekg(18, std::ios::beg); //begins reading file after header
    unsigned char color_data[headerObject.height * headerObject.width * 3]; //store all the pixel data (need to multiply by 3 since every pixel contains 3 values for r,g, and b)
    inStream.read((char *)color_data, sizeof(color_data)); //reads in pixel data into color_data

    for(int i = 0; i < headerObject.height * headerObject.width * 3; i+=3){ //iterate through populated color_data array by pixel containing r,g,b values
        pixels.push_back(new Pixel(color_data[i], color_data[i+1], color_data[i+2])); //create new pixel object and add it to pixels vector
    }
    inStream.close(); //close file
    return pixels;

};

//multiply feature
std::vector<Pixel*> multiply(const std::string& top, const std::string& bottom){
    //store pixel data of the top and bottom layer
    std::vector<Pixel*> top_pixels = getPixelData(top); 
    std::vector<Pixel*> bottom_pixels = getPixelData(bottom);
    std::vector<Pixel*> result; //will store the final product
    unsigned int size = std::min(top_pixels.size(), bottom_pixels.size()); //done in event the two vectors are not the same size
    for (int i = 0; i < size; ++i) { //iterate through the pixels of the image
       //variables storing the product of the top pixel and the bottom pixel (properly clamps and rounds values)
       unsigned char red = (float(top_pixels.at(i)->red) * (bottom_pixels.at(i)->red)/255)+0.5f;
       unsigned char green = (float(top_pixels.at(i)->green) * (bottom_pixels.at(i)->green)/255)+0.5f;
       unsigned char blue = (float(top_pixels.at(i)->blue) * (bottom_pixels.at(i)->blue)/255)+0.5f;

       result.push_back(new Pixel(blue, green, red)); //create new pixel object and add it to final vector
    };

    //measure to delete memory created
    for (Pixel* t: top_pixels){
        delete t;
    }
    top_pixels.clear();

    for (Pixel* b: bottom_pixels){
        delete b;
    }
    bottom_pixels.clear();
    return result;
    
};

//subtract
std::vector<Pixel*> subtract(const std::string& top, const std::string& bottom){
    std::vector<Pixel*> top_pixels = getPixelData(top);
    std::vector<Pixel*> bottom_pixels = getPixelData(bottom);
    std::vector<Pixel*> result;
    unsigned int size = std::min(top_pixels.size(), bottom_pixels.size());
    for (int i = 0; i < size; ++i) {
       //subtracts top pixel from bottom pixel with proper clamping techniques to ensure values do not exceed 255 or go below 0
       int red_clamp = bottom_pixels.at(i)->red-top_pixels.at(i)->red;
       int green_clamp = bottom_pixels.at(i)->green-top_pixels.at(i)->green;
       int blue_clamp = bottom_pixels.at(i)->blue-top_pixels.at(i)->blue;
       unsigned char red = clamp(red_clamp);
       unsigned char blue = clamp(blue_clamp);
       unsigned char green = clamp(green_clamp);

       result.push_back(new Pixel(blue, green, red));
    };

    //deletes, what will be, unused memory
     for (Pixel* t: top_pixels){
        delete t;
    }
    top_pixels.clear();

    for (Pixel* b: bottom_pixels){
        delete b;
    }
    bottom_pixels.clear();
    return result;
};


std::vector<Pixel*> subtract(const std::vector<Pixel*>& top_pixels, const std::vector<Pixel*>& bottom_pixels){
    //this function is the overloaded function of the one above that takes the pixel data of the top layer and bottom layer
    std::vector<Pixel*> result;
    unsigned int size = std::min(top_pixels.size(), bottom_pixels.size());
    for (int i = 0; i < size; ++i) {
       int red_clamp = bottom_pixels.at(i)->red-top_pixels.at(i)->red;
       int green_clamp = bottom_pixels.at(i)->green-top_pixels.at(i)->green;
       int blue_clamp = bottom_pixels.at(i)->blue-top_pixels.at(i)->blue;
       unsigned char red = clamp(red_clamp);
       unsigned char blue = clamp(blue_clamp);
       unsigned char green = clamp(green_clamp);

       result.push_back(new Pixel(blue, green, red));
    };
    return result;
};

//screen function
std::vector<Pixel*> screen(const std::vector<Pixel*>& top_pixels, const std::vector<Pixel*>& bottom_pixels){
    std::vector<Pixel*> result;
    unsigned int size = std::min(top_pixels.size(), bottom_pixels.size());
    for (int i = 0; i < size; ++i) {
        //performs appropriate calculation (1-(1-A)*(1-B)) with clamping and rounding
        unsigned char red = (1 - (1 - (float)top_pixels.at(i)->red / 255) * (1 - (float)bottom_pixels.at(i)->red / 255)) * 255 + 0.5f;
        unsigned char green = (1 - (1 - (float)top_pixels.at(i)->green / 255) * (1 - (float)bottom_pixels.at(i)->green / 255)) * 255 + 0.5f;
        unsigned char blue = (1 - (1 - (float)top_pixels.at(i)->blue / 255) * (1 - (float)bottom_pixels.at(i)->blue / 255)) * 255 + 0.5f;
       result.push_back(new Pixel(blue, green, red));
    };
    return result;
}

std::vector<Pixel*> overlay(const std::string& top, const std::string& bottom){
    std::vector<Pixel*> top_pixels = getPixelData(top);
    std::vector<Pixel*> bottom_pixels = getPixelData(bottom);
    std::vector<Pixel*> result;
    unsigned int size = std::min(top_pixels.size(), bottom_pixels.size());
    for (int i = 0; i < size; ++i) {
        //performs appropriate calculation with clamping and rounding
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        if (grayScale(bottom_pixels.at(i)) >= 128){ //grayscale greater than or equal to 0.5 (1-2*(1-A)*(1-B))
            red = (1 - 2* (1 - (float)top_pixels.at(i)->red / 255) * (1 - (float)bottom_pixels.at(i)->red / 255)) * 255 + 0.5f;
            green = (1 - 2* (1 - (float)top_pixels.at(i)->green / 255) * (1 - (float)bottom_pixels.at(i)->green / 255)) * 255 + 0.5f;
            blue = (1 - 2*(1 - (float)top_pixels.at(i)->blue / 255) * (1 - (float)bottom_pixels.at(i)->blue / 255)) * 255 + 0.5f;
        }
        else{ //grayscale less than 0.5 (2*A*B)
            red = (2*(((float)top_pixels.at(i)->red) * ((float)bottom_pixels.at(i)->red)))/255 +0.5f;
            green = (2*(((float)top_pixels.at(i)->green) * ((float)bottom_pixels.at(i)->green)))/255  + 0.5f;
            blue = (2*(((float)top_pixels.at(i)->blue) * ((float)bottom_pixels.at(i)->blue)))/255+0.5f;
        }

       result.push_back(new Pixel(blue, green, red));
    };
    
    //delete unused memroy to prevent memory leaks
    for (Pixel* t: top_pixels){
        delete t;
    }
    top_pixels.clear();

    for (Pixel* b: bottom_pixels){
        delete b;
    }
    bottom_pixels.clear();
    return result;
};


unsigned char clamp(const int& value){

    //clamps values to 255 if they exceed 255 and to 0 if value is less than 0
    if (value > 255){
        return 255;
    }
    else if(value < 0){
        return 0;
    }
    return (unsigned char) value;   

};

void unitTests(const std::vector<Pixel*>& pixels, const std::vector<Pixel*>& compare, const int& test_num){
    //function used to perform unit tests goes pixel by pixel comparing the resulting vector to the one in the examples folder
    std::cout << "Test #" << test_num << "...... ";
    for (int i = 0; i < pixels.size(); ++i) {
        if (pixels.at(i)->red != compare.at(i)->red || pixels.at(i)->blue != compare.at(i)->blue ||
            pixels.at(i)->green != compare.at(i)->green) {
            std::cout << "Failed!";
            break;
        }
    }

    std::cout << "Success!";
}

unsigned char grayScale(const Pixel* pixel){
    //calculates grayscale value by taking the average of rgb values
    int rgb =(pixel->red + pixel->blue + pixel->green);
    return (rgb/3.0f)+0.5f;
}

void outputFile(const std::string& fileName, const std::vector<Pixel*>& pixels, const std::string& outputPath){
    std::ofstream outFile(outputPath, std::ios::binary); //creates ofstream object where data will be written to
    Header headerObject = returnHeader(fileName); //header data used to create new file
    //writes the header data to the new file
    outFile.write(&headerObject.idLength, sizeof(headerObject.idLength)); 
    outFile.write(&headerObject.colorMapType, sizeof(headerObject.colorMapType)); 
    outFile.write(&headerObject.dataTypeCode, sizeof(headerObject.dataTypeCode)); 
    outFile.write((char *) &headerObject.colorMapOrigin, sizeof(headerObject.colorMapOrigin));
    outFile.write((char *) &headerObject.colorMapLength, sizeof(headerObject.colorMapLength));
    outFile.write(&headerObject.colorMapDepth, sizeof(headerObject.colorMapDepth));
    outFile.write((char *) &headerObject.xOrigin, sizeof(headerObject.xOrigin));
    outFile.write((char *) &headerObject.yOrigin, sizeof(headerObject.yOrigin));
    outFile.write((char *) &headerObject.width, sizeof(headerObject.width));
    outFile.write((char *) &headerObject.height, sizeof(headerObject.height));
    outFile.write(&headerObject.bitsPerPixel, sizeof(headerObject.bitsPerPixel));
    outFile.write(&headerObject.imageDescriptor, sizeof(headerObject.imageDescriptor));
    //iterate through resulting vector and write data to ofstream object
    for (int i = 0; i < pixels.size(); ++i) {
       unsigned char red = pixels[i]->red;
       unsigned char green = pixels[i]->green;
       unsigned char blue =  pixels[i]->blue;
       outFile.write((char *) &blue, sizeof(blue));
       outFile.write((char *) &green, sizeof(green));
       outFile.write((char *) &red, sizeof(red));
    };
    outFile.close();
    
}

//all functions below take file paths as parameters and output new images to /output folder (part1 - extra credit)
void part1(const std::string& top, const std::string& bottom){
    std::vector<Pixel*> result = multiply(top, bottom);
    outputFile("input/layer1.tga", result, "output/part1.tga"); //creates part1.tga in output folder
    //delete unused memory
    for (Pixel* r : result){
        delete r;
    }
    result.clear();
};

void part2(const std::string& top, const std::string& bottom){
    std::vector<Pixel*> result = subtract(top,bottom);
    outputFile("input/layer2.tga", result, "output/part2.tga"); //creates part2.tga in output folder
    //delete unused memory
    for (Pixel* r : result){
        delete r;
    }

};

void part3(const std::string& one, const std::string& two, const std::string& three){
    std::vector<Pixel*> top = getPixelData(three);
    std::vector<Pixel*> bottom = multiply(one, two);
    std::vector<Pixel*> result = screen(top, bottom);
    outputFile("input/layer1.tga", result, "output/part3.tga"); //creates part3.tga in output folder
    //delete unused memory
    for (Pixel* t: top){
        delete t;
    }
    top.clear();

    for (Pixel* b: bottom){
        delete b;
    }
    bottom.clear();

    for(Pixel* r: result){
        delete r;
    }
    result.clear();
};

void part4(const std::string& one, const std::string& two, const std::string& three){
    std::vector<Pixel*> top = getPixelData(three);
    std::vector<Pixel*> bottom = multiply(one, two);
    std::vector<Pixel*> result = subtract(top, bottom);
    outputFile("input/layer2.tga", result, "output/part4.tga"); //creates part4.tga in output folder
    //delete unused memory
    for (Pixel* t: top){
        delete t;
    }
    top.clear();

    for (Pixel* b: bottom){
        delete b;
    }
    bottom.clear();

    for(Pixel* r: result){
        delete r;
    }
    result.clear();
};

void part5(const std::string& one, const std::string& two){
    std::vector<Pixel*> result = overlay(one, two);
    outputFile("input/layer1.tga", result, "output/part5.tga"); //creates part5.tga in output folder
    //delete unused memory
    for(Pixel* r: result){
        delete r;
    }
    result.clear();
};


//increases green channel in image by 200 pixels
void part6(const std::string& file){
    Header headerObject = returnHeader(file);
    std::vector<Pixel*> base = getPixelData(file);
    std::vector<Pixel*> result;
    unsigned int size = base.size();
    for (int i = 0; i < size; ++i) {
        //variables pre-clamp
        int red_clamp = base.at(i)->red;
        int green_clamp = base.at(i)->green+200; 
        int blue_clamp = base.at(i)->blue;

        //clamped variables
        unsigned char red = clamp(red_clamp);
        unsigned char blue = clamp(blue_clamp);
        unsigned char green = clamp(green_clamp);

        result.push_back(new Pixel(blue, green, red));
    };

    outputFile("input/car.tga", result, "output/part6.tga"); //creates part6.tga in output folder

    //delete unused memory
    for(Pixel* b: base){
        delete b;
    }
    base.clear();

    for(Pixel* r: result){
        delete r;
    }
    result.clear();


};

//scales red channel by 4 and blue by 0
void part7(const std::string& file){
    Header headerObject = returnHeader(file);
    std::vector<Pixel*> base = getPixelData(file);
    std::vector<Pixel*> result;
    unsigned int size = base.size();
    for (int i = 0; i < size; ++i) {
        unsigned char red = clamp((4 * (float)base.at(i)->red)); //clamps the product of 4 and the current red value for the pixel
        unsigned char green = (base.at(i)->green);
        unsigned char blue = 0; //anything times zero is 0
 
        result.push_back(new Pixel(blue, green, red));
    };

    outputFile("input/car.tga", result, "output/part7.tga"); //creates part7.tga in output folder


    //delete unused memory
    for(Pixel* p: base){
        delete p;
    }
    base.clear();

    for(Pixel* pixel: result){
        delete pixel;
    }
    result.clear();
};

//functions part8r through part8g creates 3 files which correspond to the three channels of the original image
void part8r(const std::string& file){
    Header headerObject = returnHeader(file);
    std::vector<Pixel*> base = getPixelData(file);
    std::vector<Pixel*> result;
    unsigned int size = base.size();
    for (int i = 0; i < size; ++i) {
        unsigned char red = base.at(i)->red;
        unsigned char green = base.at(i)->red;
        unsigned char blue = base.at(i)->red;

        result.push_back(new Pixel(blue, green, red));
    };
 
    outputFile("input/car.tga", result, "output/part8_r.tga"); //creates part8_r.tga in output folder

    //delete unused memory
    for(Pixel* p: base){
        delete p;
    }
    base.clear();

    for(Pixel* pixel: result){
        delete pixel;
    }
    result.clear();
};

void part8g(const std::string& file){
    Header headerObject = returnHeader(file);
    std::vector<Pixel*> base = getPixelData(file);
    std::vector<Pixel*> result;
    unsigned int size = base.size();
    for (int i = 0; i < size; ++i) {
        unsigned char red = base.at(i)->green;
        unsigned char green = base.at(i)->green;
        unsigned char blue = base.at(i)->green;

        result.push_back(new Pixel(blue, green, red));
    };

    outputFile("input/car.tga", result, "output/part8_g.tga"); //creates part8_g.tga in output folder
    //delete unused memory
    for(Pixel* p: base){
        delete p;
    }
    base.clear();

    for(Pixel* pixel: result){
        delete pixel;
    }
    result.clear();
};

void part8b(const std::string& file){
    Header headerObject = returnHeader(file);
    std::vector<Pixel*> base = getPixelData(file);
    std::vector<Pixel*> result;
    unsigned int size = base.size();
    for (int i = 0; i < size; ++i) {
        unsigned char red = base.at(i)->blue;
        unsigned char green = base.at(i)->blue;
        unsigned char blue = base.at(i)->blue;

        result.push_back(new Pixel(blue, green, red));
    };

    outputFile("input/car.tga", result, "output/part8_b.tga"); //creates part8_b.tga in output folder
    //delete unused memory
    for(Pixel* b: base){
        delete b;
    }
    base.clear();

    for(Pixel* pixel: result){
        delete pixel;
    }
    result.clear();
};

void part9(const std::string& one, const std::string& two, const std::string& three){
    //pixel data of the three files
    std::vector<Pixel*> red = getPixelData(one);
    std::vector<Pixel*> green = getPixelData(two);
    std::vector<Pixel*> blue = getPixelData(three);
    std::vector<Pixel*> result;
    unsigned int size = std::min(red.size(), green.size());
    for (int i = 0; i < size; ++i) {
        //takes colors from each respective color file
        unsigned char redVal = red.at(i)->red; 
        unsigned char greenVal = green.at(i)->green;
        unsigned char blueVal = blue.at(i)->blue;

        result.push_back(new Pixel(blueVal, greenVal, redVal));
    };

    outputFile("input/layer_red.tga", result, "output/part9.tga"); //creates part9.tga in output folder
    //delete unused memory
    for(Pixel* r: red){
        delete r;
    }
    red.clear();

    for(Pixel* b: blue){
        delete b;
    }
    blue.clear();

    for(Pixel* g: green){
        delete g;
    }
    green.clear();

    for(Pixel* r: result){
        delete r;
    }

    result.clear();
};

void part10(const std::string & file){
    std::vector<Pixel*> pre_flip = getPixelData(file);
    std::vector<Pixel*> result;
    //iterates through vector backwards to flip image
    for (int i = pre_flip.size()-1; i >=0; --i){
        result.push_back(pre_flip[i]);
    };
    outputFile("input/text2.tga", result, "output/part10.tga"); //creates part10.tga in output folder

    //delete unused memory
    for(Pixel* r: result){
        delete r;
    }
    result.clear();
}

void extra_credit(const std::string& one, const std::string& two, const std::string& three, const std::string& four){

    //the header data and pixel data of the images
    Header bottomLeftHeader = returnHeader(one);
    Header bottomRightHeader = returnHeader(two);
    Header topLeftHeader = returnHeader(three);
    Header topRightHeader = returnHeader(four); 
    std::vector<Pixel*> bottom_left = getPixelData(one);
    std::vector<Pixel*> bottom_right = getPixelData(two);
    std::vector<Pixel*> top_left = getPixelData(three);
    std::vector<Pixel*> top_right = getPixelData(four);

    std::vector<Pixel*> result; 
    
   
    /* The loops below functions as follows:
    1) Segment image into halves (top and bottom) and iterate through each row of the combined image.
    2) Starting from the bottom, loop through the vector corresponding to the pixel data of the bottom left section of the desired new image. Stop once it is equal to one half of the new image width.
    3) Loop through vector corresponding to pixel data of the bottom right section of the desired new image. Stop once again when it is equal to one half of the new image width.
    4) Repeat until the process has been completed for the bottom half of the image.
    5) Do the same for the top segment of the new image .
    */

    //lower half of the image
    for(int b = 0; b < bottomLeftHeader.height; ++b){ //the variable b reprents the current height
      
        unsigned char red;
        unsigned char blue;
        unsigned char green;
        
        //l and r are equal to b*[bottomLeftheader/bottomRightHeader].width so the loop will start one pixel after the index ended on during the previous iteration of b
        for(int l = (b*bottomLeftHeader.width); l < bottomLeftHeader.width+(b*bottomLeftHeader.width); ++l){
            red = bottom_left[l]->red;
            green = bottom_left[l]->green;
            blue = bottom_left[l]->blue;
            result.push_back(new Pixel(blue, green, red));
        }
        for(int r = (b*bottomRightHeader.width); r < bottomRightHeader.width+(b*bottomRightHeader.width); ++r){
            red = bottom_right[r]->red;
            green = bottom_right[r]->green;
            blue = bottom_right[r]->blue;
            result.push_back(new Pixel(blue, green, red));
        }
        
    }
    //upper half of the image
    for(int t = 0; t < topLeftHeader.height; ++t){ //same logic from above applies here
      
        unsigned char red;
        unsigned char blue;
        unsigned char green;
        for(int l = (t*topLeftHeader.width); l < topLeftHeader.width+(t*topLeftHeader.width); ++l){
            red = top_left[l]->red;
            green = top_left[l]->green;
            blue = top_left[l]->blue;
            result.push_back(new Pixel(blue, green, red));
        }
        for(int r = (t*topRightHeader.width); r < topRightHeader.width+(t*topRightHeader.width); ++r){
            red = top_right[r]->red;
            green = top_right[r]->green;
            blue = top_right[r]->blue;
            result.push_back(new Pixel(blue, green, red));
        }
        
    }
    outputFile("examples/EXAMPLE_extracredit.tga", result, "output/extracredit.tga"); //creates extracredit.tga in output folder

    //delete unused memory
    for(Pixel* bl : bottom_left){
        delete bl;
    }
    bottom_left.clear();

    for(Pixel* br : bottom_right){
        delete br;
    }
    bottom_right.clear();

    for(Pixel* tl : top_left){
        delete tl;
    }
    top_left.clear();
    
    for(Pixel* tr : top_right){
        delete tr;
    }
    top_right.clear();

    for(Pixel* r : result){
        delete r;
    }
    result.clear();


};