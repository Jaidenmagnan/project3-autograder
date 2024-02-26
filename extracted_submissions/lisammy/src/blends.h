#include <iostream>
#include <fstream>
#include "Image.h"
#include "pixelData.h"
#include "outFile.h"

#ifndef IMAGE_PROCESSOR_BLENDS_H
#define IMAGE_PROCESSOR_BLENDS_H

//This class holds every blend type, as well as doing the certain functions that part 1-10 asks for.
class blends {
public:
    // Multiply blend
     static Image multiply(Image image, Image image2) {
        //creates temporary outfile that writes in the new pixel data.
        outFile x;
        auto outfile = x.createOutFileHeader("output/temp.tga", image);
        for (int i = 0; i < size(image.pixelVectors); i++) {
            float normalizedBlue1 = float(image.pixelVectors[i].blueValue) / 255;
            float normalizedBlue2 = float(image2.pixelVectors[i].blueValue) / 255;
            float normalizedGreen1 = float(image.pixelVectors[i].greenValue) / 255;
            float normalizedGreen2 = float(image2.pixelVectors[i].greenValue) / 255;
            float normalizedRed1 = float(image.pixelVectors[i].redValue) / 255;
            float normalizedRed2 = float(image2.pixelVectors[i].redValue) / 255;
            outfile << char(255 * (normalizedBlue1 * normalizedBlue2) + .5f);
            outfile << char(255 * (normalizedGreen1 * normalizedGreen2) + .5f);
            outfile << char(255 * (normalizedRed1 * normalizedRed2) + .5f);

        }
        outfile.close();
        Image multiply;
        multiply.readImageData("output/temp.tga");
        remove ("output/temp.tga");
        // returns Image object
        return multiply;
    }

    //Subtract blend
    static Image subtract(Image image, Image image2) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/temp2.tga", image);
        for (int i = 0; i < size(image.pixelVectors); i++) {
            char blue, red, green;

            // Checks if they are less than 0, will set to Char(0)
            if (int(image2.pixelVectors[i].blueValue) - int(image.pixelVectors[i].blueValue) < 0) {
                blue = char(0);
            } else {
                blue = char(int(image2.pixelVectors[i].blueValue) - int(image.pixelVectors[i].blueValue));
            }

            if (int(image2.pixelVectors[i].greenValue) - int(image.pixelVectors[i].greenValue) < 0) {
                green = char(0);
            } else {
                green = char(int(image2.pixelVectors[i].greenValue) - int(image.pixelVectors[i].greenValue));
            }

            if (int(image2.pixelVectors[i].redValue) - int(image.pixelVectors[i].redValue) < 0) {
                red = char(0);
            } else {
                red = char(int(image2.pixelVectors[i].redValue) - int(image.pixelVectors[i].redValue));
            }
            outfile << blue;
            outfile << green;
            outfile << red;
        }
        outfile.close();
        Image subtract;
        subtract.readImageData("output/temp2.tga");
        remove ("output/temp2.tga");
        return subtract;
    }

    // Screen blend
    static Image screen(Image topLayer, Image bottomLayer) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/temp3.tga", bottomLayer);

        for (int i = 0; i < size(topLayer.pixelVectors); i++) {
            float normalizedForegoundBlue = float(topLayer.pixelVectors[i].blueValue) / 255;
            float normalizedBackgroundBlue = float(bottomLayer.pixelVectors[i].blueValue) / 255;
            float normalizedForegroundGreen = float(topLayer.pixelVectors[i].greenValue) / 255;
            float normalizedBackgroundGreen = float(bottomLayer.pixelVectors[i].greenValue) / 255;
            float normalizedForegroundRed = float(topLayer.pixelVectors[i].redValue) / 255;
            float normalizedBackgroundRed = float(bottomLayer.pixelVectors[i].redValue) / 255;
            outfile << char(255 * (1 - (1 - normalizedForegoundBlue) * (1 - normalizedBackgroundBlue)) + 0.5f);
            outfile << char(255 * (1 - (1 - normalizedForegroundGreen) * (1 - normalizedBackgroundGreen)) + 0.5f);
            outfile << char(255 * (1 - (1 - normalizedForegroundRed) * (1 - normalizedBackgroundRed)) + 0.5f);
        }
        outfile.close();
        Image screen;
        screen.readImageData("output/temp3.tga");
        remove ("output/temp3.tga");
        return screen;
    }

    //overlay blend
    static Image overlay(Image foreground, Image background) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/temp5.tga", background);
        for (int i = 0; i < size(foreground.pixelVectors); i++) {
            char newBlue, newGreen, newRed;
            float foregroundBlueNormalized = float(foreground.pixelVectors[i].blueValue) / 255;
            float backgroundBlueNormalized = float(background.pixelVectors[i].blueValue) / 255;
            float foregroundGreenNormalized = float(foreground.pixelVectors[i].greenValue) / 255;
            float backgroundGreenNormalized = float(background.pixelVectors[i].greenValue) / 255;
            float foregroundRedNormalized = float(foreground.pixelVectors[i].redValue) / 255;
            float backgroundRedNormalized = float(background.pixelVectors[i].redValue) / 255;
            if (backgroundBlueNormalized <= 0.5) {
                newBlue = char(255 * (2 * foregroundBlueNormalized * backgroundBlueNormalized) + 0.5f);
            } else {
                newBlue = char(255 * (1 - 2 * (1 - foregroundBlueNormalized) * (1 - backgroundBlueNormalized)) + 0.5f);
            }

            if (backgroundGreenNormalized <= 0.5) {
                newGreen = char(255 * (2 * foregroundGreenNormalized * backgroundGreenNormalized) + 0.5f);
            } else {
                newGreen = char(
                        255 * (1 - 2 * (1 - foregroundGreenNormalized) * (1 - backgroundGreenNormalized)) + 0.5f);
            }

            if (backgroundRedNormalized <= 0.5) {
                newRed = char(255 * (2 * foregroundRedNormalized * backgroundRedNormalized) + 0.5f);
            } else {
                newRed = char(255 * (1 - 2 * (1 - foregroundRedNormalized) * (1 - backgroundRedNormalized)) + 0.5f);
            }

            outfile << newBlue;
            outfile << newGreen;
            outfile << newRed;
        }
        outfile.close();
        Image overlay;
        overlay.readImageData("output/temp5.tga");
        remove ("output/temp5.tga");
        return overlay;
    }

    //Adds 200 to green channel
    static Image addChannel(Image image) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/temp6.tga", image);

        for (int i = 0; i < size(image.pixelVectors); i++) {
            char green;
            if (int(image.pixelVectors[i].greenValue) + 200 > 255) {
                green = char(255);
            } else {
                green = char(int(image.pixelVectors[i].greenValue) + 200);
            }
            outfile << image.pixelVectors[i].blueValue;
            outfile << green;
            outfile << image.pixelVectors[i].redValue;
        }
        outfile.close();
        Image addedChannel;
        addedChannel.readImageData("output/temp6.tga");
        remove ("output/temp6.tga");
        return addedChannel;
    }

    //Scales the red and blue channels of the image
    static Image scaleChannel(Image image) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/temp7.tga", image);

        for (int i = 0; i < size(image.pixelVectors); i++) {
            char blue, green, red;
            blue = char(0);
            green = char(int(image.pixelVectors[i].greenValue));
            if (int(image.pixelVectors[i].redValue) * 4 > 255) {
                red = char(255);
            } else {
                red = char(int(image.pixelVectors[i].redValue) * 4);
            }
            outfile << blue;
            outfile << green;
            outfile << red;
        }
        outfile.close();
        Image scaledChannel;
        scaledChannel.readImageData("output/temp7.tga");
        remove ("output/temp7.tga");
        return scaledChannel;
    }

    //Splits the RGB values into their own respective files.
    static void splitChannelFiles(Image image) {
        outFile x;
        outFile y;
        outFile z;
        auto redFile = x.createOutFileHeader("output/part8_r.tga", image);
        auto greenFile = y.createOutFileHeader("output/part8_g.tga", image);
        auto blueFile = z.createOutFileHeader("output/part8_b.tga", image);
        for (int i = 0; i < size(image.pixelVectors); i++) {
            char blue, green, red;
            blue = char(int(image.pixelVectors[i].blueValue));
            green = char(int(image.pixelVectors[i].greenValue));
            red = char(int(image.pixelVectors[i].redValue));

            blueFile << blue;
            blueFile << blue;
            blueFile << blue;

            greenFile << green;
            greenFile << green;
            greenFile << green;

            redFile << red;
            redFile << red;
            redFile << red;
        }
        blueFile.close();
        greenFile.close();
        redFile.close();
    }

    //Combines three files of RGB value to create one image.
    static void combineChannelFiles(Image redFile, Image greenFile, Image blueFile) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/part9.tga", redFile);
        for (int i = 0; i < size(redFile.pixelVectors); i++) {
            outfile << char(int(blueFile.pixelVectors[i].blueValue));
            outfile << char(int(greenFile.pixelVectors[i].greenValue));
            outfile << char(int(redFile.pixelVectors[i].redValue));
        }
        outfile.close();
    }

    //Flips the image 180 degrees.
    static void flipImage(Image image) {
        outFile x;
        auto outfile = x.createOutFileHeader("output/part10.tga", image);
        for (int i = size(image.pixelVectors) - 1; i >= 0; i--) {
            outfile << char(int(image.pixelVectors[i].blueValue));
            outfile << char(int(image.pixelVectors[i].greenValue));
            outfile << char(int(image.pixelVectors[i].redValue));
        }
        outfile.close();
    }
};
#endif //IMAGE_PROCESSOR_BLENDS_H
