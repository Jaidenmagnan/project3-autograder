#include <iostream>
#include <utility>
#include "Image.h"
#include "pixelData.h"
#include "blends.h"
#include "outFile.h"
#include "checker.h"
using namespace std;

//This class will run parts 1-10 by calling the blends and functions needed.
class doParts{
public:
    static void doPartOne(Image topLayer, Image bottomLayer) {
        Image multiply = blends::multiply(std::move(topLayer), std::move(bottomLayer));
        outFile::createOutFileFinal("output/part1.tga", multiply);
    }
    static void doPartTwo(Image topLayer, Image bottomLayer){
        Image subtract = blends::subtract(std::move(topLayer), std::move(bottomLayer));
        outFile::createOutFileFinal("output/part2.tga", subtract);

    }
    static void doPartThree(Image image1, Image image2, Image image3){
        Image multiply = blends::multiply(std::move(image1), std::move(image2));
        Image screen = blends::screen(std::move(image3), multiply);
        outFile::createOutFileFinal("output/part3.tga", screen);
    }
    static void doPartFour(Image image1, Image image2, Image image3){
        Image multiply = blends::multiply(std::move(image1), std::move(image2));
        Image subtract = blends::subtract(std::move(image3), multiply);
        outFile::createOutFileFinal("output/part4.tga", subtract);
    }
    static void doPartFive(Image image1, Image image2){
        Image overlay = blends::overlay(std::move(image1), std::move(image2));
        outFile::createOutFileFinal("output/part5.tga", overlay);
    }
    static void doPartSix(Image image1){
        Image addedChannel = blends::addChannel(std::move(image1));
        outFile::createOutFileFinal("output/part6.tga", addedChannel);
    }
    static void doPartSeven(Image image1){
        Image scaledChannel = blends::scaleChannel(std::move(image1));
        outFile::createOutFileFinal("output/part7.tga", scaledChannel);
    }
    static void doPartEight(Image image1){
        blends::splitChannelFiles(std::move(image1));
    }
    static void doPartNine(Image image1, Image image2, Image image3){
        blends::combineChannelFiles(std::move(image1), std::move(image2), std::move(image3));
    }
    static void doPartTen(Image image1){
        blends::flipImage(std::move(image1));
    }
};

int main() {
    // Lists each input and creates a Image object for them.
    Image car;
    car.readImageData("input/car.tga");

    Image circles;
    circles.readImageData("input/circles.tga");

    Image layerBlue;
    layerBlue.readImageData("input/layer_blue.tga");

    Image layerGreen;
    layerGreen.readImageData("input/layer_green.tga");

    Image layerRed;
    layerRed.readImageData("input/layer_red.tga");

    Image layer1;
    layer1.readImageData("input/layer1.tga");

    Image layer2;
    layer2.readImageData("input/layer2.tga");

    Image pattern1;
    pattern1.readImageData("input/pattern1.tga");

    Image pattern2;
    pattern2.readImageData("input/pattern2.tga");

    Image text;
    text.readImageData("input/text.tga");

    Image text2;
    text2.readImageData("input/text2.tga");

    //Actively runs parts 1-10, generates the output files here.
    doParts::doPartOne(layer1, pattern1);
    doParts::doPartTwo(layer2, car);
    doParts::doPartThree(layer1, pattern2, text);
    doParts::doPartFour(layer2, circles, pattern2);
    doParts::doPartFive(layer1, pattern1);
    doParts::doPartSix(car);
    doParts::doPartSeven(car);
    doParts::doPartEight(car);
    doParts::doPartNine(layerRed, layerGreen, layerBlue);
    doParts::doPartTen(text2);

    //Actively checks if they are the same
    //if returned 1, the files are the same.
    cout << "Test 1: " << Checker::checkEqual("output/part1.tga", "examples/EXAMPLE_part1.tga") << endl;
    cout << "Test 2: " << Checker::checkEqual("output/part2.tga", "examples/EXAMPLE_part2.tga") << endl;
    cout << "Test 3: " << Checker::checkEqual("output/part3.tga", "examples/EXAMPLE_part3.tga") << endl;
    cout << "Test 4: " << Checker::checkEqual("output/part4.tga", "examples/EXAMPLE_part4.tga") << endl;
    cout << "Test 5: " << Checker::checkEqual("output/part5.tga", "examples/EXAMPLE_part5.tga") << endl;
    cout << "Test 6: " << Checker::checkEqual("output/part6.tga", "examples/EXAMPLE_part6.tga") << endl;
    cout << "Test 7: " << Checker::checkEqual("output/part7.tga", "examples/EXAMPLE_part7.tga") << endl;
    cout << "Test 8b: " << Checker::checkEqual("output/part8_b.tga", "examples/EXAMPLE_part8_b.tga") << endl;
    cout << "Test 8g: " << Checker::checkEqual("output/part8_g.tga", "examples/EXAMPLE_part8_g.tga") << endl;
    cout << "Test 8r: " << Checker::checkEqual("output/part8_r.tga", "examples/EXAMPLE_part8_r.tga") << endl;
    cout << "Test 9: " << Checker::checkEqual("output/part9.tga", "examples/EXAMPLE_part9.tga") << endl;
    cout << "Test 10: " << Checker::checkEqual("output/part10.tga", "examples/EXAMPLE_part10.tga") << endl;
    return 0;
}
