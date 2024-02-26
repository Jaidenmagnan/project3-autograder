// Name: Keerat Kohli
// Date: 10/31/2023

#include "Header.h"
#include "ImageProcessing.h"
#include "Pixel.h"
#include "UnitTesting.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdint>

using namespace std;

// Function for writing Image Data to an ofstream
void writeImageData(ofstream& outStream, vector<Pixel>& image) {
    // Loops through each pixel and just writes it to the file
    for(int i = 0; i < image.size(); i++) {
        outStream.write((char*)&image[i].blue, sizeof(image[i].blue));
        outStream.write((char*)&image[i].green, sizeof(image[i].green));
        outStream.write((char*)&image[i].red, sizeof(image[i].red));
    }
}

// Multiplies Layer1 and Patern1
void task1(string inputPath) {
    // Reads in the two files and instantiates an ImageProcessing object from them (one for top layer A and for bottom layer B)
    ImageProcessing imageA(inputPath + "input/layer1.tga");
    ImageProcessing imageB(inputPath + "input/pattern1.tga");

    // Gets the Pixel data of the new image after multiplying both of the files
    vector<Pixel> part1 = imageA.multiply(imageB);

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part1.tga", ios::binary);

    // TGA files need header so we need to write this first and then the pixel data
    Header headerPart1 = imageA.getHeader();

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart1, outStream);
    writeImageData(outStream, part1);

    // Closes outstream
    outStream.close();
}

// Subtracts layer2 from car
void task2(string inputPath) {
    // Reads in the two files and instantiates an ImageProcessing object from them (one for top layer A and for bottom layer B)
    ImageProcessing imageA(inputPath + "input/layer2.tga");
    ImageProcessing imageB(inputPath + "input/car.tga");

    // Gets the Pixel data of the new image after subtracting both of the files
    vector<Pixel> part2 = imageA.subtract(imageB);

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part2.tga", ios::binary);

    // TGA files need header so we need to write this first and then the pixel data
    Header headerPart2 = imageA.getHeader();

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart2, outStream);
    writeImageData(outStream, part2);

    // Closes outstream
    outStream.close();
}

// First multiplies layer1 with pattern2 and uses that result to screen it with text.tga
void task3(string inputPath) {
    // Reads in the two files and instantiates an ImageProcessing object from them (one for top layer A and for bottom layer B)
    ImageProcessing imageA(inputPath + "input/layer1.tga");
    ImageProcessing imageB(inputPath + "input/pattern2.tga");

    // Gets the Pixel data of the temporary image after multiplying both of the files
    vector<Pixel> tempPixels = imageA.multiply(imageB);

    // We need temporary images Header to create a full Image
    Header tempHeader = imageA.getHeader();

    // Using both we instantiate an temporary object that will be used to screen with text.tga
    ImageProcessing tempImage(tempPixels, tempHeader);

    // Read in the text.tga file that will be used in the screen method
    ImageProcessing imageScreenWith(inputPath + "input/text.tga");

    // Gets the Pixel data of the new image after screening both of the files
    vector<Pixel> part3 = imageScreenWith.screen(tempImage);

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part3.tga", ios::binary);

    // TGA files need header so we need to write this first and then the pixel data
    Header headerPart3 = imageScreenWith.getHeader();

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart3, outStream);
    writeImageData(outStream, part3);

    outStream.close();
}

// First multiplies layer2 with circles and then uses that result as the top layer for subtraction with pattern2
void task4(string inputPath) {
    // Reads in the two files and instantiates an ImageProcessing object from them (one for top layer A and for bottom layer B)
    ImageProcessing imageA(inputPath + "input/layer2.tga");
    ImageProcessing imageB(inputPath + "input/circles.tga");

    // Gets the Pixel data of the temporary image after multiplying both of the files
    vector<Pixel> tempImageData = imageA.multiply(imageB);

    // We need temporary images Header to create a full Image
    Header tempHeader = imageA.getHeader();

    // Using both we instantiate an temporary object that will be used to subtract with pattern2
    ImageProcessing tempImage(tempImageData, tempHeader);

    // Read in the pattern2.tga file that will be used in the subtract method
    ImageProcessing imageSubtractWith(inputPath + "input/pattern2.tga");

    // Gets pixel data after subtracting using the temporaryImage as the bottom layer
    vector<Pixel> part4 = imageSubtractWith.subtract(tempImage);

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part4.tga", ios::binary);

    // TGA files need header so we need to write this first and then the pixel data
    Header headerPart4 = imageSubtractWith.getHeader();

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart4, outStream);
    writeImageData(outStream, part4);

    outStream.close();
}

// Overlays the images layer1 and pattern1
void task5(string inputPath) {
    // Reads in the two files and instantiates an ImageProcessing object from them (one for top layer A and for bottom layer B)
    ImageProcessing imageA(inputPath + "input/layer1.tga");
    ImageProcessing imageB(inputPath + "input/pattern1.tga");

    // Gets Pixel data and Header of the image that results from overlaying layer1 (top layer) and pattern1 (background)
    vector<Pixel> part5 = imageA.overlay(imageB);
    Header headerPart5 = imageA.getHeader();

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part5.tga", ios::binary);

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart5, outStream);
    writeImageData(outStream, part5);

    outStream.close();
}

// Adds a constant of 200 to all pixel's green component in car.tga
void task6(string inputPath) {
    // Reads in the file and instantiates an Image Porcessing object
    ImageProcessing imageA(inputPath + "input/car.tga");

    // Gets the data from new image and Header that results from adding to green channel only
    vector<Pixel> part6 = imageA.addGreenChannel(200);
    Header headerPart6 = imageA.getHeader();

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part6.tga", ios::binary);

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart6, outStream);
    writeImageData(outStream, part6);

    outStream.close();
}

// Takes the car.tga file and scales each of its channels by a specific constant
void task7(string inputPath) {
    // Reads in the file and instantiates an Image Porcessing object
    ImageProcessing imageA(inputPath + "input/car.tga");

    // Gets the data from new image and Header that results from scaling each channel
    vector<Pixel> part7 = imageA.scaleChannels(4, 1, 0);
    Header headerPart7 = imageA.getHeader();

    // Creates ofstream in binary mode to write the new image data to its respective file
    ofstream outStream(inputPath + "output/part7.tga", ios::binary);

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart7, outStream);
    writeImageData(outStream, part7);

    outStream.close();
}

// Reads in car.tga and writes 3 different files, all from separating car into its 3 channels
void task8(string inputPath) {
    // Reads in the file and instantiates an Image Porcessing object
    ImageProcessing imageA(inputPath + "input/car.tga");

    // Gets the Pixel data from separating car into red, green, and blue channels respectively and stores it
    vector<Pixel> part8R = imageA.separateChannel(true, false, false);
    vector<Pixel> part8G = imageA.separateChannel(false, true, false);
    vector<Pixel> part8B = imageA.separateChannel(false, false, true);

    // Gets header needed for the beginning of each file
    Header headerPart8 = imageA.getHeader();

    // Creates 3 ofstreams  in binary mode to write the new image data to their respective file
    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    ofstream outStream(inputPath + "output/part8_r.tga", ios::binary);
    writeHeader(headerPart8, outStream);
    writeImageData(outStream, part8R);

    outStream.close();

    outStream.open(inputPath + "output/part8_g.tga", ios::binary);
    writeHeader(headerPart8, outStream);
    writeImageData(outStream, part8G);

    outStream.close();

    outStream.open(inputPath + "output/part8_b.tga", ios::binary);
    writeHeader(headerPart8, outStream);
    writeImageData(outStream, part8B);

    outStream.close();
}

// Given three different files with different channels, combines them into one image
void task9(string inputPath) {
    // Reads in the three files that will be the red, green, and blue channels in the new iamge respectively
    ImageProcessing imageA(inputPath + "input/layer_red.tga");
    ImageProcessing imageB(inputPath + "input/layer_green.tga");
    ImageProcessing imageC(inputPath + "input/layer_blue.tga");

    // Gets the image data from combining all three files into one image and stores it
    vector<Pixel> part9 = imageA.combineChannel(imageA, imageB, imageC);

    // Gets the header needed for the beginnign of each file
    Header headerPart9 = imageA.getHeader();

    // Creates the ofstream in binary mode to write the new image data to their respective file
    ofstream outStream(inputPath + "output/part9.tga", ios::binary);

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart9, outStream);
    writeImageData(outStream, part9);

    outStream.close();
}

// Takes in text2.tga and flips it by 180 degrees
void task10(string inputPath) {
    // Reads in the file that will be flipped and instantiates an ImageProcessing obejct
    ImageProcessing imageA(inputPath + "input/text2.tga");

    // Gets the image data and Header from flipping the image 180 degrees
    vector<Pixel> part10 = imageA.flipImage();
    Header headerPart10 = imageA.getHeader();

    // Creates the ofstream in binary mode to write the new image data to their respective file
    ofstream outStream(inputPath + "output/part10.tga", ios::binary);

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerPart10, outStream);
    writeImageData(outStream, part10);

    outStream.close();
}

// Takes in 4 different images and carefully arranges them into one bigger picture
void extraCredit(string inputPath) {
    // Reads in the files that will be combined and instantiates an ImageProcessing object for each one of them
    ImageProcessing imageA(inputPath + "input/text.tga");
    ImageProcessing imageB(inputPath + "input/pattern1.tga");
    ImageProcessing imageC(inputPath + "input/car.tga");
    ImageProcessing imageD(inputPath + "input/circles.tga");

    // Gets the header needed for the beginning of the output file
    Header headerEC = imageA.getHeader();

    // This time we need to modify the header because its width and height is changing
    headerEC.width = 1024;
    headerEC.height = 1024;

    // This is a temporary vector that is just used as a placeholder to make an ImageProcessing object that we can then use to call the combineImages() method
    vector<Pixel> tempData(headerEC.width * headerEC.height, {0, 0, 0});
    ImageProcessing tempImage(tempData, headerEC);

    // Calls the combineImages() function using the temporary image object and stores the output
    vector<Pixel> partEC = tempImage.combineImages(imageA, imageB, imageC, imageD);

    // Creates the ofstream in binary mode to write the new image data to their respective file
    ofstream outStream(inputPath + "output/extracredit.tga", ios::binary);

    // Calls two functions from Header.cpp to write the header first and then pixel image data stored in the vector
    writeHeader(headerEC, outStream);
    writeImageData(outStream, partEC);

    outStream.close();
}

int main() {
//    auto start = std::chrono::high_resolution_clock::now();

    string inputPath = ""; // Allows me to easily change relative path in case it is being run on a different computer with different directories
//    UnitTesting test; // Declaring instance of UnitTest class to test if my images are the same as examples

    task1(inputPath);
    task2(inputPath);
    task3(inputPath);
    task4(inputPath);
    task5(inputPath);
    task6(inputPath);
    task7(inputPath);
    task8(inputPath);
    task9(inputPath);
    task10(inputPath);
    extraCredit(inputPath);

    //Testing
//    test.task(inputPath + "output/part1.tga", inputPath + "examples/EXAMPLE_part1.tga", "1");
//    test.task(inputPath + "output/part2.tga", inputPath + "examples/EXAMPLE_part2.tga", "2");
//    test.task(inputPath + "output/part3.tga", inputPath + "examples/EXAMPLE_part3.tga", "3");
//    test.task(inputPath + "output/part4.tga", inputPath + "examples/EXAMPLE_part4.tga", "4");
//    test.task(inputPath + "output/part5.tga", inputPath + "examples/EXAMPLE_part5.tga", "5");
//    test.task(inputPath + "output/part6.tga", inputPath + "examples/EXAMPLE_part6.tga", "6");
//    test.task(inputPath + "output/part7.tga", inputPath + "examples/EXAMPLE_part7.tga", "7");
//    test.task(inputPath + "output/part8_r.tga", inputPath + "examples/EXAMPLE_part8_r.tga", "8a");
//    test.task(inputPath + "output/part8_g.tga", inputPath + "examples/EXAMPLE_part8_g.tga", "8b");
//    test.task(inputPath + "output/part8_b.tga", inputPath + "examples/EXAMPLE_part8_b.tga", "8c");
//    test.task(inputPath + "output/part9.tga", inputPath + "examples/EXAMPLE_part9.tga", "9");
//    test.task(inputPath + "output/part10.tga", inputPath + "examples/EXAMPLE_part10.tga", "10");
//    test.task(inputPath + "output/extracredit.tga", inputPath + "examples/EXAMPLE_extracredit.tga", "EC");

//    auto end = std::chrono::high_resolution_clock::now();
//    double timeTaken = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
//    cout << timeTaken * 1e-9 << endl;

    return 0;
}
