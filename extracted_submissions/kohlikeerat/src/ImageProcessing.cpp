//
// Created by kkeer on 11/9/2023.
//

#include "ImageProcessing.h"
#include <iostream>

// Constructor that creates an instance of ImageProcessing given the filename
ImageProcessing::ImageProcessing(string filename) {

    ifstream inStream(filename, ios::binary); // Instantiates ifstream to read in file in binary mode
    readHeader(headerObject, inStream); // Reads Header using function in Header.h
    this->totalPixels = headerObject.height * headerObject.width; // Defines total pixels as width x height and stores it in private variable for future use
    this->image.resize(totalPixels); // Before loading in image data, we need to resize the image vector to hold the image

    // Loops through each pixel and reads in the data to vector
    for(int i = 0; i < totalPixels; i++) {
        inStream.read((char*)&image[i].blue, sizeof(image[i].blue));
        inStream.read((char*)&image[i].green, sizeof(image[i].green));
        inStream.read((char*)&image[i].red, sizeof(image[i].red));
    }

    // Closes the ifstream
    inStream.close();
}

// Constructor that creates an instance of ImageProcessing given the image data and header data explicitly.
ImageProcessing::ImageProcessing(vector<Pixel>& image, Header& headerObject) {
    // Instantiates private clsas variables
    this->headerObject = headerObject;
    this->totalPixels = headerObject.width * headerObject.height;
    this->image.resize(totalPixels);

    // Instantiates image data in the image vector
    for(int i = 0; i < totalPixels; i++) {
        this->image[i].blue = image[i].blue;
        this->image[i].green = image[i].green;
        this->image[i].red = image[i].red;
    }
}

// Getter for Header
Header ImageProcessing::getHeader() {
    return headerObject;
}

// Getter for Total Pixels
int ImageProcessing::getTotalPixels() {
    return totalPixels;
}

// Returns a reference to the new image after multiplying the two images
vector<Pixel>& ImageProcessing::multiply(ImageProcessing& otherImage) {

    // Resizes the newImage vector to size of what image will end up being
    newImage.resize(totalPixels);

    // Loop through each pixel,
    // Normalize each pixel component (divide by 255) and then multiply to prevent overflow
    // Convert back by multiplying by 255 and adding 0.5 to ensure rounding is precise
    for(int i = 0; i < totalPixels; i++) {
        double blue = ((float)image[i].blue / 255.0 ) * ((float)otherImage.image[i].blue /255.0 ) ;
        double green = ((float)image[i].green / 255.0) * ((float)otherImage.image[i].green / 255.0) ;
        double red = ((float)image[i].red / 255.0) * ((float)otherImage.image[i].red / 255.0) ;

        newImage[i].blue = (blue * 255.0 + 0.5);
        newImage[i].green = (green * 255.0 + 0.5);
        newImage[i].red = (red * 255.0 + 0.5);
    }

    // Returns the new image
    return newImage;
}

// Returns a reference to the new Image after implementing subtract feature
// This function subtracts THIS (top layer) image FROM the OTHER IMAGE (bottom layer)
vector<Pixel>& ImageProcessing::subtract(ImageProcessing &otherImage) {

    // Make sure the size of the newImage vector is the same as other two images
    newImage.resize(totalPixels);

    // Loop through each pixel in images
    for(int i = 0; i < totalPixels; i++) {
        // To prevent overflow, check first to see if THIS (top layer) image RGB value is greater than OTHER IMAGE RGB value
        // If it is greater, then we know subtracting would give a negative number so just clamp new image value at 0
        // If it is less, then we know we can subtract without getting a negative number and therefore do so

        if(otherImage.image[i].blue <= this->image[i].blue) newImage[i].blue = 0;
        else newImage[i].blue = otherImage.image[i].blue - this->image[i].blue;

        if(otherImage.image[i].green <= this->image[i].green) newImage[i].green = 0;
        else newImage[i].green = otherImage.image[i].green - this->image[i].green;

        if(otherImage.image[i].red <= this->image[i].red) newImage[i].red = 0;
        else newImage[i].red = otherImage.image[i].red - this->image[i].red;
    }

    // Returns reference to new image vector
    return newImage;
}

// Returns reference to new image that results from screening two images together
vector<Pixel>& ImageProcessing::screen(ImageProcessing &otherImage) {

    // Make sure the size of the newImage vector is the same as other two images
    newImage.resize(totalPixels);

    // Loop through each pixel,
    // And implement screen formula : 1 - (1-a)*(1-b) where a = THIS image (top layer) and b = OTHER IMAGE (bottom layer)
    for(int i = 0; i < totalPixels; i++) {
        // Note that the formula uses 1 which implies that the images RGB values should be normalized
        // Hence we divide by 255 here
        double blue = 1 - (1 - (float)this->image[i].blue / 255.0) * (1 - (float)otherImage.image[i].blue / 255.0);
        double green = 1 - (1 - (float)this->image[i].green / 255.0) * (1 - (float)otherImage.image[i].green / 255.0);
        double red = 1 - (1 - (float)this->image[i].red / 255.0) * (1 - (float)otherImage.image[i].red / 255.0);

        // At the end we need to un normalize the values and therefore multiply by 255
        // Add 0.5 to ensure the reounding is precise and consistent
        newImage[i].blue = (blue * 255.0 + 0.5);
        newImage[i].green = (green * 255.0 + 0.5);
        newImage[i].red = (red * 255.0 + 0.5);

    }

    return newImage;
}

// Return a reference to the image that results from overlaying THIS image (top layer) with the BACKGROUND image (bottom layer)
vector<Pixel>& ImageProcessing::overlay(ImageProcessing &background) {

    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    // Placeholder values to temporarily hold some calculations
    double blue, green, red;

    // Loop through each pixel
    for(int i = 0; i < totalPixels; i++) {

        // Simply executes the formula:
        // If bacgkround image RGB component is less than or equal to 0.5, we calculate 2 * THIS image * BACKGROUND image
        // Otherwise, we calculate 1 - 2 * (1 - THIS image) * (1 - BACKGROUND image)
        // Also normalize each value to ensure that no overflow problems occur

        if(background.image[i].blue / 255.0 <= 0.5) {
            blue = 2 * (this->image[i].blue / 255.0) * (background.image[i].blue / 255.0);
        }
        else {
            blue = 1 - 2 * (1 - this->image[i].blue / 255.0) * (1 - background.image[i].blue / 255.0);
        }

        if(background.image[i].green / 255.0 <= 0.5) {
            green = 2 * (this->image[i].green / 255.0) * (background.image[i].green / 255.0);
        }
        else {
            green = 1 - 2 * (1 - this->image[i].green / 255.0) * (1 - background.image[i].green / 255.0);
        }

        if(background.image[i].red / 255.0 <= 0.5) {
            red = 2 * (this->image[i].red / 255.0) * (background.image[i].red / 255.0);
        }
        else {
            red = 1 - 2 * (1 - this->image[i].red / 255.0) * (1 - background.image[i].red / 255.0);
        }

        // At the end we need to un normalize the values and therefore multiply by 255
        // Add 0.5 to ensure the reounding is precise and consistent
        newImage[i].blue = (blue * 255.0 + 0.5);
        newImage[i].green = (green * 255.0 + 0.5);
        newImage[i].red = (red * 255.0 + 0.5);
    }

    return newImage;
}

// Returns reference to image that results from only adding some value to all green values of each pixel
vector<Pixel>& ImageProcessing::addGreenChannel(unsigned char green) {

    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    unsigned char limit = 255;

    // Loop through each pixel,

    for(int i = 0; i < totalPixels; i++){
        // Instead of simply adding the green value to each green component and worrying about overflow
        // Check to see IF adding the new green value would go over the limit
        // IF TRUE -> clamp it at the limit
        // IF FALSE -> just add because we know no overflow will occur
        if(image[i].green > (limit - green)) newImage[i].green = limit;
        else newImage[i].green = image[i].green += green;

        // Other channels stay exact same
        newImage[i].blue = image[i].blue;
        newImage[i].red = image[i].red;
    }

    return newImage;
}

// Returns a reference to a new image that results from individually scaling each RGB channel of the old image
vector<Pixel>& ImageProcessing::scaleChannels(int redFactor, int greenFactor, int blueFactor) {

    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    // Placeholder values
    double blue, green, red;

    // Loop through each pixel,
    for(int i = 0; i < totalPixels; i++) {

        // Checks to see if multiplying by the scale factor will cause overflow
        // If yes, clamp the value at the limit
        // Otherwise, just multiply without any worry

        if(redFactor != 0 && image[i].red > 255.0 / redFactor) red = 255;
        else red = image[i].red * redFactor;

        if(greenFactor != 0 && image[i].green > 255.0 / greenFactor) green = 255;
        else green = image[i].green * greenFactor;

        if(blue != 0 && image[i].blue > 255.0 / blueFactor) blue = 255;
        else blue = image[i].blue * blueFactor;

        // Add 0.5 to keep rounding precise and consistent
        newImage[i].blue = (blue + 0.5);
        newImage[i].green = (green + 0.5);
        newImage[i].red = (red + 0.5);
    }

    return newImage;
}

// Returns reference to new image that results from separating one of the channels from the others
vector<Pixel>& ImageProcessing::separateChannel(bool red, bool green, bool blue) {

    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    // Loop through each pixel
    for(int i = 0; i < totalPixels; i++) {
        // Checks to see which channel we are trying to separate
        // Then simply assigns only that components values to the new Image RGB pixel values
        if(red) {
            newImage[i].red = image[i].red;
            newImage[i].green = image[i].red;
            newImage[i].blue = image[i].red;
        }
        else if(green) {
            newImage[i].green = image[i].green;
            newImage[i].red = image[i].green;
            newImage[i].blue = image[i].green;
        }
        else if(blue) {
            newImage[i].blue = image[i].blue;
            newImage[i].green = image[i].blue;
            newImage[i].red = image[i].blue;
        }
    }

    return newImage;
}

// Returns reference to new image that results from combining the three RGB channels together
// Instead of taking in vectors, it takes three references to Image Processing objects
vector<Pixel>& ImageProcessing::combineChannel(ImageProcessing& redChannel, ImageProcessing &greenChannel,
                                              ImageProcessing &blueChannel) {
    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    // Loop through each pixel value,
    for(int i = 0; i < totalPixels; i++) {
        // Assigns each RGB value of new image to corresponding pixel channel value
        newImage[i].red = redChannel.image[i].red;
        newImage[i].green = greenChannel.image[i].green;
        newImage[i].blue = blueChannel.image[i].blue;
    }

    return newImage;
}

// Returns reference to image that results from flipping the old one 180 degrees
vector<Pixel>& ImageProcessing::flipImage() {

    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    // Loop through each pixel value,
    for(int i = 0; i < totalPixels; i++) {
        // Note that flipping an image 180 means that the top right pixel value is now the starting pixel value
        // Looking at this relationship further reveals that the indx i in the new image corresponds with indx TotalPixel - i -1 in old image
        newImage[i].red = image[totalPixels - i - 1].red;
        newImage[i].green = image[totalPixels - i -1].green;
        newImage[i].blue = image[totalPixels - i - 1].blue;
    }

    return newImage;
}

// Extra Credit
// Returns reference to new image that results from combining 4 images into one
vector<Pixel>& ImageProcessing::combineImages(ImageProcessing &bottomLeft, ImageProcessing &bottomRight, ImageProcessing &topLeft, ImageProcessing &topRight) {

    // Makes sure that the size of the image is the same as the other two images
    newImage.resize(totalPixels);

    // Indices for each image respectively
    int bottomLeftIndx = 0; int bottomRightIndx = 0; int topLeftIndx = 0; int topRightIndx = 0;

    // Loop through each pixel in NEW IMAGE
    for(int i = 0; i < totalPixels; i++) {
        // This checks to see if current pixel value is in the bottom left quadrant
        // If so assign it to the value of the bottom left image
        if( (i % headerObject.width) < (headerObject.width / 2) && i < (totalPixels / 2) ) {
            newImage[i].red = bottomLeft.image[bottomLeftIndx].red;
            newImage[i].green = bottomLeft.image[bottomLeftIndx].green;
            newImage[i].blue = bottomLeft.image[bottomLeftIndx].blue;
            bottomLeftIndx++;
        }

        // This checks to see if current pixel value is in the bottom right quadrant
        // If so assign it to the value of the bottom right image
        else if( (i % headerObject.width) >= (headerObject.width / 2) && i < (totalPixels / 2) ) {
            newImage[i].red = bottomRight.image[bottomRightIndx].red;
            newImage[i].green = bottomRight.image[bottomRightIndx].green;
            newImage[i].blue = bottomRight.image[bottomRightIndx].blue;
            bottomRightIndx++;
        }

        // This checks to see if current pixel value is in the top left quadrant
        // If so assign it to the value of the top left image
        else if( (i % headerObject.width) < (headerObject.width / 2) && i >= (totalPixels / 2) ) {
            newImage[i].red = topLeft.image[topLeftIndx].red;
            newImage[i].green = topLeft.image[topLeftIndx].green;
            newImage[i].blue = topLeft.image[topLeftIndx].blue;
            topLeftIndx++;
        }

        // This checks to see if current pixel value is in the top right quadrant
        // If so assign it to the value of the top right image
        else {
            newImage[i].red = topRight.image[topRightIndx].red;
            newImage[i].green = topRight.image[topRightIndx].green;
            newImage[i].blue = topRight.image[topRightIndx].blue;
            topRightIndx++;
        }
    }

    return newImage;
}

// Overloads the equality operator for checking if two .tga images are the same
bool ImageProcessing::operator==(const ImageProcessing &imageB) {

    // Instantiates the Headers for each image
    Header headerA, headerB;
    headerA = this->headerObject;
    headerB = imageB.headerObject;

    // Uses overloaded equality operator for headers to check if the two headers are the same
    if (!(headerA == headerB)) {
        return false;
    }

    // Now loops through each pixel to make sure that the two images are the same
    for(int i = 0; i < totalPixels; i++) {
        // If even one component is off, we return false because they are different images
        if(image[i].blue != imageB.image[i].blue || image[i].green != imageB.image[i].green || image[i].red != imageB.image[i].red) {
            return false;
        }
    }
    return true;
}