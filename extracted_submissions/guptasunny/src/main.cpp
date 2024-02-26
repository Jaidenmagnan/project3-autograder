//
// Created by sgupt on 11/15/2023.
//
#include "iostream"
#include "fstream"
#include "TgaFile.h"
using namespace std;

bool test1();
bool test2();
bool test3();
bool test4();
bool test5();
bool test6();
bool test7();
bool test8();
bool test9();
bool test10();
//basically, each test function carries out the operations mentioned in the project document. a file is then written. this file is then opened and turned into a tgaFile object, along with the example output version. they are then checked for equality
void writeToTga(Header& imgHeader, vector<Pixel>& imgData, string& filename);

int main() {
    int testsPassed = 0; //runs each test and increments testsPasses counter
    if (test1()){
        testsPassed+= 1;
        cout << "Test 1....Passed!" << endl;
    }
    if (test2()){
        testsPassed+= 1;
        cout << "Test 2....Passed!" << endl;
    }
    if (test3()){
        testsPassed+= 1;
        cout << "Test 3....Passed!" << endl;
    }
    if (test4()){
        testsPassed+= 1;
        cout << "Test 4....Passed!" << endl;
    }
    if (test5()){
        testsPassed+= 1;
        cout << "Test 5....Passed!" << endl;
    }
    if (test6()){
        testsPassed+= 1;
        cout << "Test 6....Passed!" << endl;
    }
    if (test7()){
        testsPassed+= 1;
        cout << "Test 7....Passed!" << endl;
    }
    if (test8()){
        testsPassed+= 1;
        cout << "Test 8....Passed!" << endl;
    }
    if (test9()){
        testsPassed+= 1;
        cout << "Test 9....Passed!" << endl;
    }
    if (test10()){
        testsPassed+= 1;
        cout << "Test 10....Passed!" << endl;
    }
    cout << "Tests Passed: " << testsPassed << endl;
    return 0;
}

void writeToTga(Header& fileHeader, vector<Pixel>& imgData, string& filename){ //basically the same as the instantiation of the file, but writing instead of reading
    ofstream inStream(filename, ios_base::binary);
inStream.write(&fileHeader.idLength, sizeof(fileHeader.idLength)); //following block reads out first 18 bytes of data from the header into the file
inStream.write(&fileHeader.colorMapType, sizeof(fileHeader.colorMapType));
inStream.write(&fileHeader.dataTypeCode, sizeof(fileHeader.dataTypeCode));
inStream.write((char*) &fileHeader.colorMapOrigin, sizeof(fileHeader.colorMapOrigin));
inStream.write((char*) &fileHeader.colorMapLength, sizeof(fileHeader.colorMapLength));
inStream.write(&fileHeader.colorMapDepth, sizeof(fileHeader.colorMapDepth));
inStream.write((char*) &fileHeader.xOrigin, sizeof(fileHeader.xOrigin));
inStream.write((char*) &fileHeader.yOrigin, sizeof(fileHeader.yOrigin));
inStream.write((char*) &fileHeader.width, sizeof(fileHeader.width));
inStream.write((char*) &fileHeader.height, sizeof(fileHeader.height));
inStream.write(&fileHeader.bitsPerPixel, sizeof(fileHeader.bitsPerPixel));
inStream.write(&fileHeader.imageDescriptor, sizeof(fileHeader.imageDescriptor));

for (int i = 0; i < imgData.size(); i++){ //iterates through image data
    Pixel newPixel = imgData[i]; //creates a pixel equal to the pixel at that point into the loop
    inStream.write((char*)&newPixel.blue, sizeof(unsigned char)); //writes the bgr values of the pixel into the file, 1 byte each
    inStream.write((char*)&newPixel.green, sizeof(unsigned char));
    inStream.write((char*)&newPixel.red, sizeof(unsigned char));
}
inStream.close();

}

bool checkEqual(TgaFile file1, TgaFile file2){
    vector<Pixel> imgData = file1.getImgData(); //grabs the image data from the two files that have been passed in, the first created by the method, the second based on the example output
    vector<Pixel> exampleData = file2.getImgData();
    for (int i = 0; i < imgData.size(); i++){ //iterates through each pixel in both images and checks if the rgb values match
        if(imgData[i].blue != exampleData[i].blue || imgData[i].green != exampleData[i].green || imgData[i].red != exampleData[i].red){
            return false; //if at any point the values don't match, return false
        }
    }
    return true;
}

bool test1(){
    //Test 1: Use Multiply blending mode to combine “layer1.tga” (top layer) with “pattern1.tga” (bottom).
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/layer1.tga";
    TgaFile tester1(fileName); //instantiates a new tgaFile based on the specified files
    fileName = "input/pattern1.tga";
    TgaFile tester2(fileName);
    //TgaFile objects created, now to use multiply, with layer1 being the top and pattern1 being the bottom
    testVec = tester1.multiply(tester2); //multiply called and resulting image data stored into a vector
    testHeader = tester2.getHeader();
    fileName = "output/part1.tga";
    writeToTga(testHeader, testVec,fileName); //creates file using the returned imageData vector, and the header of tester2 (which is identical to tester1)
    TgaFile testResult (fileName); //creates tgaFile object from the newly-created file for comparisons
    fileName = "examples/EXAMPLE_part1.tga";
    TgaFile example(fileName);  //creates tga file from the example output for comparisons
    return checkEqual(testResult, example); //checks for similarity and then returns the value
}

bool test2(){
    //Test 2: Use the Subtract blending mode to combine “layer2.tga” (top layer) with “car.tga” (bottom layer).
    //This mode subtracts the top layer from the bottom layer.
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/layer2.tga";
    TgaFile tester1(fileName); //instantiates a new tgaFile based on the specified files
    fileName = "input/car.tga";
    TgaFile tester2(fileName);
    //TgaFile objects created, now to use subtract, with layer2 being the top and car being the bottom
    testVec = tester1.subtract(tester2); //subtract called and resulting image data stored into a vector
    testHeader = tester2.getHeader();
    fileName = "output/part2.tga";
    writeToTga(testHeader, testVec,fileName); //creates file using the returned imageData vector, and the header of tester2 (which is identical to tester1)
    TgaFile testResult (fileName); //creates tgaFile object from the newly-created file for comparisons
    fileName = "examples/EXAMPLE_part2.tga";
    TgaFile example(fileName); //creates tga file from the example output for comparisons
    return checkEqual(testResult, example); //checks for similarity and then returns the value
}

bool test3(){
    //Test 3: Use the Multiply blending mode to combine “layer1.tga” with “pattern2.tga”, and store the
    //results temporarily. Load the image “text.tga” and, using that as the top layer, combine it with
    //the previous results of layer1/pattern2 using the Screen blending mode.
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/layer1.tga";
    TgaFile tester1(fileName);
    fileName = "input/pattern2.tga";
    TgaFile tester2(fileName);
    //TgaFile objects created, now to use multiply, with layer1 being the top and pattern2 being the bottom
    testVec = tester1.multiply(tester2); //multiply called and resulting image data stored into a vector
    testHeader = tester2.getHeader();
    TgaFile subtractResult (testVec, testHeader); //new tga file to be screened with the next tga obj

    fileName = "input/text.tga";
    TgaFile layer3(fileName); //new tgaFile obj to use with screen

    testVec = layer3.screen(subtractResult); //screen called and stored in testVec
    testHeader = layer3.getHeader();

    fileName = "output/part3.tga";
    writeToTga(testHeader, testVec,fileName); //creates file using testVec vector

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part3.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

bool test4(){
    //Test 4: Multiply “layer2.tga” with “circles.tga”, and store it. Load “pattern2.tga” and, using that as the
    //top layer, combine it with the previous result using the Subtract blending mode.
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/layer2.tga";
    TgaFile tester1(fileName);
    fileName = "input/circles.tga";
    TgaFile tester2(fileName);
    //TgaFile objects created, now to use multiply, with layer2 being the top and circles being the bottom
    testVec = tester1.multiply(tester2); //multiply called and resulting image data stored into a vector
    testHeader = tester2.getHeader();
    TgaFile multiplyResult (testVec, testHeader); //new tga file to be subtracting with the next tga obj
    fileName = "input/pattern2.tga";
    TgaFile layer3(fileName);
    testVec = layer3.subtract(multiplyResult); //layer3 calls subtract, being the top layer, with multiply result being the bottom
    testHeader = layer3.getHeader();
    fileName = "output/part4.tga";
    writeToTga(testHeader, testVec,fileName); //writes to file

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part4.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

bool test5(){
    //Test 5: Combine “layer1.tga” (as the top layer) with “pattern1.tga” using the Overlay blending mode.
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/layer1.tga";
    TgaFile tester1(fileName);
    fileName = "input/pattern1.tga";
    TgaFile tester2(fileName);
    //TgaFile objects created, now to use overlay, with layer1 being the top and pattern1 being the bottom
    testVec = tester1.overlay(tester2); //overlay called and resulting image data stored into a vector
    testHeader = tester2.getHeader();
    fileName = "output/part5.tga";
    writeToTga(testHeader, testVec,fileName); //writes to file

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part5.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

bool test6(){
    //Test 6: Load “car.tga” and add 200 to the green channel.
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/car.tga";
    TgaFile tester1(fileName);
    //TgaFile object created, now to use addchannel
    string channel = "green";
    int amount = 200;
    testVec = tester1.addChannel(channel, amount); //addchannel called and resulting image data stored into a vector
    testHeader = tester1.getHeader();
    fileName = "output/part6.tga";
    writeToTga(testHeader, testVec,fileName); //file written

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part6.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

bool test7(){
    //Test 7: Load “car.tga” and scale (multiply) the red channel by 4, and the blue channel by 0. This will
    //increase the intensity of any red in the image, while negating any blue it may have.
    vector<Pixel> testVec;
    Header testHeader;
    string fileName = "input/car.tga";
    TgaFile tester1(fileName);
    //TgaFile object created, now to use scale
    string channel = "red"; //specified channel and multiplier
    float amount = 4;
    testVec = tester1.scale(amount, channel); //scale called and resulting image data stored into a vector
    testHeader = tester1.getHeader();
    TgaFile scaleResult (testVec, testHeader); //new tga file with red scaled up
    channel = "blue";
    amount = 0;

    testVec = scaleResult.scale(amount, channel); //calls scale on the red-scaled up version of the file, using blue and 0 as the values now
    fileName = "output/part7.tga";
    writeToTga(testHeader, testVec,fileName); //writes to file

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part7.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

bool test8(){
    //Test 8: Load “car.tga” and write each channel to a separate file: the red channel should be “part8_r.tga”,
    //the green channel should be “part8_g.tga”, and the blue channel should be “part8_b.tga”
    vector<Pixel> testVecRed;
    vector<Pixel> testVecGreen;
    vector<Pixel> testVecBlue; //creates three empty vector
    string fileName = "input/car.tga";
    TgaFile tester1(fileName); //one tga file that will be read and split up
    vector<Pixel> imgData = tester1.getImgData();
    Header testHeader = tester1.getHeader();
    //TgaFile object created, now to split it up
    for (int i = 0; i < imgData.size(); i++){
        Pixel red;
        Pixel blue;
        Pixel green; //three pixel objects, one for each vector
        red.red = imgData[i].red; //all three values of the pixel are set to the red channel
        red.blue = imgData[i].red;
        red.green = imgData[i].red;
        blue.blue = imgData[i].blue; //same for blue and gree
        blue.red = imgData[i].blue;
        blue.green = imgData[i].blue;
        green.green = imgData[i].green;
        green.blue = imgData[i].green;
        green.red = imgData[i].green;
        testVecRed.push_back(red); //pixels are pushed to their respective vectors
        testVecGreen.push_back(green);
        testVecBlue.push_back(blue);
    }
    fileName = "output/part8_r.tga";
    writeToTga(testHeader, testVecRed,fileName);
    TgaFile redFile(fileName);
    fileName = "examples/EXAMPLE_part8_r.tga";
    TgaFile redEx(fileName);

    fileName = "output/part8_g.tga";
    writeToTga(testHeader, testVecGreen,fileName);
    TgaFile greenFile(fileName);
    fileName = "examples/EXAMPLE_part8_g.tga";
    TgaFile greenEx(fileName);

    fileName = "output/part8_b.tga";
    writeToTga(testHeader, testVecBlue,fileName); //writes file

    TgaFile blueFile(fileName); //creates objects for comparisons, calls comparator, then returns result based on 3 comparisons as all three files must be identical
    fileName = "examples/EXAMPLE_part8_b.tga";
    TgaFile blueEx(fileName);
    return (checkEqual(redFile, redEx) && checkEqual(blueFile, blueEx) && checkEqual(greenFile, greenEx));
}

bool test9(){
    //Test 9: Load “layer_red.tga”, “layer_green.tga” and “layer_blue.tga”, and combine the three files into
    //one file. The data from “layer_red.tga” is the red channel of the new image, layer_green is
    //green, and layer_blue is blue.
    string fileName = "input/layer_red.tga";
    TgaFile redLayer(fileName);

    fileName = "input/layer_blue.tga";
    TgaFile blueLayer(fileName);

    fileName = "input/layer_green.tga";
    TgaFile greenLayer(fileName);

    vector<Pixel> redData = redLayer.getImgData();
    vector<Pixel> blueData = blueLayer.getImgData();
    vector<Pixel> greenData = greenLayer.getImgData(); //three vectors again
    vector<Pixel> finalData; //vector for all the data to be pushed into
    Header testHeader = redLayer.getHeader();
    //TgaFile objects created, now to combine them up
    for (int i = 0; i < redData.size(); i++){
        Pixel newPixel; //empty pixel
        newPixel.red = redData[i].red; //each of its chars are set to the appropriate value of the component vectors
        newPixel.blue = blueData[i].blue;
        newPixel.green = greenData[i].green;
        finalData.push_back(newPixel); //pixel pushed back to vector
    }
    fileName = "output/part9.tga";
    writeToTga(testHeader, finalData,fileName); //writes file using newly populated vector

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part9.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

bool test10(){
    //Test 10: Load “text2.tga”, and rotate it 180 degrees, flipping it upside down.
    string fileName = "input/text2.tga";
    TgaFile tester1(fileName); //tgaFile obj based on text2
    vector<Pixel> rawData = tester1.getImgData();
    Header testHeader = tester1.getHeader();
    vector<Pixel> flippedData; //empty vector to be populated
    for (int i = rawData.size()-1; i >0; i--){ //iterates through the imageData in reverse order, assigning each pixel to the flippedData vector, reversing and thus flipping the file
        Pixel newPixel;
        newPixel.red = rawData[i].red;
        newPixel.blue = rawData[i].blue;
        newPixel.green = rawData[i].green;
        flippedData.push_back(newPixel);
    }
    fileName = "output/part10.tga";
    writeToTga(testHeader, flippedData,fileName); //writes file

    TgaFile testResult (fileName); //creates objects for comparisons, calls comparator, then returns result
    fileName = "examples/EXAMPLE_part10.tga";
    TgaFile example(fileName);
    return checkEqual(testResult, example);
}

