//Kristen Moy

#include "ImageProcessing.h"
#include <iostream>
#include <fstream>
#include <vector>

//Reading files
void Image::readFile(const std::string& fileName) {
    std::ifstream myFile(fileName, std::ios_base::binary);

    myFile.read(&header.idLength, sizeof(header.idLength));
    myFile.read(&header.colorMapType, sizeof(header.colorMapType));
    myFile.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
    myFile.read((char *) &header.colorMapOrigin, sizeof(header.colorMapOrigin));
    myFile.read((char *) &header.colorMapLength, sizeof(header.colorMapLength));
    myFile.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
    myFile.read((char *) &header.xOrigin, sizeof(header.xOrigin));
    myFile.read((char *) &header.yOrigin, sizeof(header.yOrigin));
    myFile.read((char *) &header.width, sizeof(header.width));
    myFile.read((char *) &header.height, sizeof(header.height));
    myFile.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    myFile.read(&header.imageDescriptor, sizeof(header.imageDescriptor));

    int imageSize = int(header.width) * int(header.height);

    for (int i = 0; i < imageSize; i++) {
        Pixel pixel;
        myFile.read((char*)(&pixel.blue), 1);
        myFile.read((char*)(&pixel.green), 1);
        myFile.read((char*)(&pixel.red), 1);

        pix.push_back(pixel);
    }
    myFile.close();
}

//Writing files
void Image::writeFile(const std::string& fileName){
    std::ofstream myFile(fileName, std::ios::binary);

    myFile.write(&header.idLength, sizeof(header.idLength));
    myFile.write(&header.colorMapType, sizeof(header.colorMapType));
    myFile.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
    myFile.write((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
    myFile.write((char*)&header.colorMapLength, sizeof(header.colorMapLength));
    myFile.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
    myFile.write((char*)&header.xOrigin, sizeof(header.xOrigin));
    myFile.write((char*)&header.yOrigin, sizeof(header.yOrigin));
    myFile.write((char*)&header.width, sizeof(header.width));
    myFile.write((char*)&header.height, sizeof(header.height));
    myFile.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    myFile.write(&header.imageDescriptor, sizeof(header.imageDescriptor));

    int imageSize = int(header.height) * int(header.width);

    for (int i = 0; i < imageSize; i++){
        Pixel pixel = pix[i];

        myFile.write((char*)(&pixel.blue), 1);
        myFile.write((char*)(&pixel.green), 1);
        myFile.write((char*)(&pixel.red), 1);

    }

    myFile.close();
}

//Multiplies pixels
std::vector<Pixel> multiply(std::vector<Pixel> pixelData, std::vector<Pixel> pixelData2) {
    std::vector<Pixel> multVector;

    for (int i = 0; i < pixelData.size(); i++){

        float p1_b = pixelData[i].blue / 255.0f;
        float p1_g = pixelData[i].green / 255.0f;
        float p1_r = pixelData[i].red / 255.0f;

        float p2_b = pixelData2[i].blue / 255.0f;
        float p2_g = pixelData2[i].green / 255.0f;
        float p2_r = pixelData2[i].red / 255.0f;

        float multPixelB = (p1_b * p2_b) * 255.0f + 0.5f;
        float multPixelG = (p1_g * p2_g) * 255.0f + 0.5f;
        float multPixelR = (p1_r * p2_r) * 255.0f + 0.5f;

        Pixel multPix;

        multPix.blue = multPixelB;
        multPix.green = multPixelG;
        multPix.red = multPixelR;

        multVector.push_back(multPix);
    }
    return multVector;
}

//Subtracts pixels
std::vector<Pixel> subtract(std::vector<Pixel> pixelData, std::vector<Pixel> pixelData2)
{
    std::vector<Pixel> subVector;

    for(int i = 0; i < pixelData.size(); i++)
    {
        float p1_b = pixelData[i].blue / 255.0f;
        float p1_g = pixelData[i].green / 255.0f;
        float p1_r = pixelData[i].red / 255.0f;

        float p2_b = pixelData2[i].blue / 255.0f;
        float p2_g = pixelData2[i].green / 255.0f;
        float p2_r = pixelData2[i].red / 255.0f;

        float subPixelB = (p2_b - p1_b) * 255.0f;
        float subPixelG = (p2_g - p1_g) * 255.0f;
        float subPixelR = (p2_r - p1_r) * 255.0f;

        if(subPixelB < 0)
        {
            subPixelB = 0;
        }
        else if(subPixelB > 255)
        {
            subPixelB = 255;
        }
        if(subPixelG < 0)
        {
            subPixelG = 0;
        }
        else if(subPixelG > 255)
        {
            subPixelG = 255;
        }
        if(subPixelR < 0)
        {
            subPixelR = 0;
        }
        else if(subPixelR > 255)
        {
            subPixelR = 255;
        }
        Pixel subPix;
        subPix.blue = subPixelB;
        subPix.green = subPixelG;
        subPix.red = subPixelR;

        subVector.push_back(subPix);
    }
    return subVector;
}

//Screens pixels
std::vector<Pixel> screen(std::vector<Pixel> pixelData, std::vector<Pixel> pixelData2)
{
    std::vector<Pixel> screenVector;
    for (int i = 0; i < pixelData.size(); i++) {
        float p1_b = pixelData[i].blue / 255.0f;
        float p1_g = pixelData[i].green / 255.0f;
        float p1_r = pixelData[i].red / 255.0f;

        float p2_b = pixelData2[i].blue / 255.0f;
        float p2_g = pixelData2[i].green / 255.0f;
        float p2_r = pixelData2[i].red / 255.0f;

        float screenPB = (1 - (1 - p1_b) * (1 - p2_b)) * 255.0f + 0.5f;
        float screenPG = (1 - (1 - p1_g) * (1 - p2_g)) * 255.0f + 0.5f;
        float screenPR = (1 - (1 - p1_r) * (1 - p2_r)) * 255.0f + 0.5f;

        //Checks if pixels are over 255 or under 0
        if(screenPB < 0)
        {
            screenPB = 0;
        }
        else if(screenPB > 255)
        {
            screenPB = 255;
        }
        if(screenPG < 0)
        {
            screenPG = 0;
        }
        else if(screenPG > 255)
        {
            screenPG = 255;
        }
        if(screenPR < 0)
        {
            screenPR = 0;
        }
        else if(screenPR > 255)
        {
            screenPR = 255;
        }
        Pixel screenPix;
        screenPix.blue = screenPB;
        screenPix.green = screenPG;
        screenPix.red = screenPR;

        screenVector.push_back(screenPix);
    }
    return screenVector;
}

//Overlays function
std::vector<Pixel> overlay(std::vector<Pixel> pixelData, std::vector<Pixel> pixelData2)
{
    std::vector<Pixel> overlayVector;
    for(int i = 0; i < pixelData.size(); i++)
    {
        float p1_b = pixelData[i].blue / 255.0f;
        float p1_g = pixelData[i].green / 255.0f;
        float p1_r = pixelData[i].red / 255.0f;

        float p2_b = pixelData2[i].blue / 255.0f;
        float p2_g = pixelData2[i].green / 255.0f;
        float p2_r = pixelData2[i].red / 255.0f;

        float overlayPB;
        float overlayPG;
        float overlayPR;

        if(p2_b <= 0.5)
        {
            overlayPB = 2 * p1_b * p2_b;
        }
        else
        {
            overlayPB = 1 - 2 * (1 - p1_b) * (1 - p2_b);
        }
        if(p2_g <= 0.5)
        {
            overlayPG = 2 * p1_g * p2_g;
        }
        else
        {
            overlayPG = 1 - 2 * (1 - p1_g) * (1 - p2_g);
        }
        if(p2_r <= 0.5)
        {
            overlayPR = 2 * p1_r * p2_r;
        }
        else
        {
            overlayPR = 1 - 2 * (1 - p1_r) * (1 - p2_r);
        }

        overlayPB = overlayPB * 255.0f + 0.5f;
        overlayPG = overlayPG * 255.0f + 0.5f;
        overlayPR = overlayPR * 255.0f + 0.5f;

        if(overlayPB < 0)
        {
            overlayPB = 0;
        }
        else if(overlayPB > 255)
        {
            overlayPB = 255;
        }
        if(overlayPG < 0)
        {
            overlayPG = 0;
        }
        else if(overlayPG > 255)
        {
            overlayPG = 255;
        }
        if(overlayPR < 0)
        {
            overlayPR = 0;
        }
        else if(overlayPR > 255)
        {
            overlayPR = 255;
        }

        Pixel overlayPix;
        overlayPix.blue = overlayPB;
        overlayPix.green = overlayPG;
        overlayPix.red = overlayPR;

        overlayVector.push_back(overlayPix);
    }
    return overlayVector;
}

//Adds 200 to green
std::vector<Pixel> addGreen(std::vector<Pixel> pixelData)
{
    std::vector<Pixel> addedVector;
    for(int i = 0; i < pixelData.size(); i++)
    {
        float p1_g = pixelData[i].green + 200.0f;

        if(p1_g > 255.0f)
        {
            p1_g = 255.0f;
        }
        Pixel addPix;
        addPix.blue = pixelData[i].blue;
        addPix.green = p1_g;
        addPix.red = pixelData[i].red;

        addedVector.push_back(addPix);
    }
    return addedVector;
}

std::vector<Pixel> t7(std::vector<Pixel> pixelData)
{
    std::vector<Pixel> task7Vector;
    for(int i = 0; i < pixelData.size(); i++)
    {
        float p_b = pixelData[i].blue * 0.0f;
        float p_r = pixelData[i].red * 4;
        if(p_r > 255.0f)
        {
            p_r = 255.0f;
        }
        Pixel newPix;
        newPix.blue = p_b;
        newPix.green = pixelData[i].green;
        newPix.red = p_r;

        task7Vector.push_back(newPix);
    }
    return task7Vector;
}

// Task 8 Blue channel
std::vector<Pixel> task8Blue(std::vector<Pixel> pixelData)
{
    std::vector<Pixel> blue;
    for(int i = 0; i < pixelData.size(); i++)
    {
        float pB = pixelData[i].blue;

        Pixel bluePix;
        bluePix.blue = pB;
        bluePix.green = pB;
        bluePix.red = pB;


        blue.push_back(bluePix);
    }
    return blue;
}

//Task 8 Green channel
std::vector<Pixel> task8Green(std::vector<Pixel> pixelData)
{
    std::vector<Pixel> green;
    for(int i = 0; i < pixelData.size(); i++)
    {
        float pG = pixelData[i].green;

        Pixel greenPix;
        greenPix.green = pG;
        greenPix.blue = pG;
        greenPix.red = pG;

        green.push_back(greenPix);
    }
    return green;
}

// Task 8 red channel
std::vector<Pixel> task8Red(std::vector<Pixel> pixelData)
{
    std::vector<Pixel> red;
    for(int i = 0; i < pixelData.size(); i++)
    {
        float pR = pixelData[i].red;

        Pixel redPix;
        redPix.blue = pR;
        redPix.green = pR;
        redPix.red = pR;


        red.push_back(redPix);
    }
    return red;
}

//Task 9 new vector using red, green, and blue channel
std::vector<Pixel> t9(std::vector<Pixel> redPixel, std::vector<Pixel> greenPixel, std::vector<Pixel> bluePixel)
{
    std::vector<Pixel> task9Vector;
    for(int i = 0; i < redPixel.size(); i++)
    {
        float pB = bluePixel[i].blue;
        float pG = greenPixel[i].green;
        float pR = redPixel[i].red;

        Pixel newPix;
        newPix.blue = pB;
        newPix.green = pG;
        newPix.red = pR;

        task9Vector.push_back(newPix);
    }
    return task9Vector;
}

//Reverses image
std::vector<Pixel> reverse(std::vector<Pixel> pixelData)
{
    std::vector<Pixel> newPixels;

    for(int i = pixelData.size() - 1; i >= 0; i--)
    {
        newPixels.push_back(pixelData[i]);
    }
    return newPixels;
}

//Runs task 1
void task1()
{
    Image layer1;
    layer1.readFile("input/layer1.tga");

    Image pattern1;
    pattern1.readFile("input/pattern1.tga");

    std::vector<Pixel> task1 = multiply(layer1.pix, pattern1.pix);
    layer1.pix = task1;
    layer1.writeFile("output/part1.tga");
}

//Runs task 2
void task2()
{
    Image layer2;
    layer2.readFile("input/layer2.tga");

    Image car;
    car.readFile("input/car.tga");

    std::vector<Pixel> task2 = subtract(layer2.pix, car.pix);
    layer2.pix = task2;
    layer2.writeFile("output/part2.tga");
}

//Runs task 3
void task3()
{
    Image layer1;
    layer1.readFile("input/layer1.tga");

    Image pattern2;
    pattern2.readFile("input/pattern2.tga");

    std::vector<Pixel> multTask3 = multiply(layer1.pix, pattern2.pix);

    Image text;
    text.readFile("input/text.tga");

    std::vector<Pixel> task3 = screen(text.pix, multTask3);
    pattern2.pix = task3;

    pattern2.writeFile("output/part3.tga");
}

//Runs task 4
void task4()
{
    Image layer2;
    layer2.readFile("input/layer2.tga");

    Image circles;
    circles.readFile("input/circles.tga");

    std::vector<Pixel> multTask4 = multiply(layer2.pix, circles.pix);

    Image pattern2;
    pattern2.readFile("input/pattern2.tga");

    std::vector<Pixel> task4 = subtract(pattern2.pix, multTask4);
    circles.pix = task4;
    circles.writeFile("output/part4.tga");

}

//Runs task 5
void task5()
{
    Image layer1;
    layer1.readFile("input/layer1.tga");

    Image pattern1;
    pattern1.readFile("input/pattern1.tga");

    std::vector<Pixel> task5 = overlay(layer1.pix, pattern1.pix);
    pattern1.pix = task5;

    pattern1.writeFile("output/part5.tga");

}

//Runs task 6
void task6()
{
    Image car;
    car.readFile("input/car.tga");

    std::vector<Pixel> task6 = addGreen(car.pix);
    car.pix = task6;

    car.writeFile("output/part6.tga");

}

//Runs task 7
void task7()
{
    Image car;
    car.readFile("input/car.tga");
    std::vector<Pixel> task7 = t7(car.pix);

    car.pix = task7;
    car.writeFile("output/part7.tga");

}

//Runs task 8
void task8()
{
    Image car;
    car.readFile("input/car.tga");
    std::vector<Pixel> bluePixel = task8Blue(car.pix);
    car.pix = bluePixel;
    car.writeFile("output/part8_b.tga");

    std::vector<Pixel> greenPixel = task8Green(car.pix);
    car.pix = greenPixel;
    car.writeFile("output/part8_g.tga");

    std::vector<Pixel> redPixel = task8Red(car.pix);
    car.pix = redPixel;
    car.writeFile("output/part8_r.tga");

}

//Runs task 9
void task9()
{
    Image layerRed;
    layerRed.readFile("input/layer_red.tga");
    std::vector<Pixel> redPixel = task8Red(layerRed.pix);

    Image layerGreen;
    layerGreen.readFile("input/layer_green.tga");
    std::vector<Pixel> greenPixel = task8Green(layerGreen.pix);

    Image layerBlue;
    layerBlue.readFile("input/layer_blue.tga");
    std::vector<Pixel> bluePixel = task8Blue(layerBlue.pix);

    std::vector<Pixel> task9 = t9(redPixel, greenPixel, bluePixel);
    layerRed.pix = task9;

    layerRed.writeFile("output/part9.tga");
}

//Runs task 10
void task10()
{
    Image text2;
    text2.readFile("input/text2.tga");
    std::vector<Pixel> reversePixels = reverse(text2.pix);
    text2.pix = reversePixels;

    text2.writeFile("output/part10.tga");

}

int main(){
   task1();
   task2();
   task3();
   task4();
   task5();
   task6();
   task7();
   task8();
   task9();
   task10();
};
