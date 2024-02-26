#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

//Reads header from TGA file
Header readHeader(std::ifstream &file) {
    Header header;
    file.read(reinterpret_cast<char*>(&header.idLength), sizeof(header.idLength));
    file.read(reinterpret_cast<char*>(&header.colorMapType), sizeof(header.colorMapType));
    file.read(reinterpret_cast<char*>(&header.dataTypeCode), sizeof(header.dataTypeCode));
    file.read(reinterpret_cast<char*>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    file.read(reinterpret_cast<char*>(&header.colorMapLength), sizeof(header.colorMapLength));
    file.read(reinterpret_cast<char*>(&header.colorMapDepth), sizeof(header.colorMapDepth));
    file.read(reinterpret_cast<char*>(&header.xOrigin), sizeof(header.xOrigin));
    file.read(reinterpret_cast<char*>(&header.yOrigin), sizeof(header.yOrigin));
    file.read(reinterpret_cast<char*>(&header.width), sizeof(header.width));
    file.read(reinterpret_cast<char*>(&header.height), sizeof(header.height));
    file.read(reinterpret_cast<char*>(&header.bitsPerPixel), sizeof(header.bitsPerPixel));
    file.read(reinterpret_cast<char*>(&header.imageDescriptor), sizeof(header.imageDescriptor));
    return header;
}
//Reads the pixel data from tga file
std::vector<Pixel> readPixels(std::ifstream &file, int pixelCount) {
    std::vector<Pixel> pixels;
    pixels.reserve(pixelCount);
    for (int i = 0; i < pixelCount; ++i) {
        Pixel pixel;
        file.read(reinterpret_cast<char*>(&pixel.blue), sizeof(pixel.blue));
        file.read(reinterpret_cast<char*>(&pixel.green), sizeof(pixel.green));
        file.read(reinterpret_cast<char*>(&pixel.red), sizeof(pixel.red));
        pixels.push_back(pixel);
    }
    return pixels;
}

// writes TGA file
void writeTGA(const std::string &outputFilePath, const Header &header, const std::vector<Pixel> &pixels) {
    std::ofstream file(outputFilePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open the file for writing.");
    }

    file.write(reinterpret_cast<const char*>(&header.idLength), sizeof(header.idLength));
    file.write(reinterpret_cast<const char*>(&header.colorMapType), sizeof(header.colorMapType));
    file.write(reinterpret_cast<const char*>(&header.dataTypeCode), sizeof(header.dataTypeCode));
    file.write(reinterpret_cast<const char*>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    file.write(reinterpret_cast<const char*>(&header.colorMapLength), sizeof(header.colorMapLength));
    file.write(reinterpret_cast<const char*>(&header.colorMapDepth), sizeof(header.colorMapDepth));
    file.write(reinterpret_cast<const char*>(&header.xOrigin), sizeof(header.xOrigin));
    file.write(reinterpret_cast<const char*>(&header.yOrigin), sizeof(header.yOrigin));
    file.write(reinterpret_cast<const char*>(&header.width), sizeof(header.width));
    file.write(reinterpret_cast<const char*>(&header.height), sizeof(header.height));
    file.write(reinterpret_cast<const char*>(&header.bitsPerPixel), sizeof(header.bitsPerPixel));
    file.write(reinterpret_cast<const char*>(&header.imageDescriptor), sizeof(header.imageDescriptor));

    for (const auto &pixel : pixels) {
        file.write(reinterpret_cast<const char*>(&pixel.blue), sizeof(pixel.blue));
        file.write(reinterpret_cast<const char*>(&pixel.green), sizeof(pixel.green));
        file.write(reinterpret_cast<const char*>(&pixel.red), sizeof(pixel.red));
    }
    if (!file.good()) {
        throw std::runtime_error("Error occurred while writing the file.");
    }
}

// Helper function to perform division by 255 with rounding
inline unsigned char divideBy255(int value) {
    return (value + 128) / 255;
}


void multiplyBlend(std::vector<Pixel>& topLayerPixels, const std::vector<Pixel>& bottomLayerPixels) {
    for (size_t i = 0; i < topLayerPixels.size(); ++i) {
        topLayerPixels[i].blue = static_cast<unsigned char>((topLayerPixels[i].blue * bottomLayerPixels[i].blue / 255.0) + 0.5f);
        topLayerPixels[i].green = static_cast<unsigned char>((topLayerPixels[i].green * bottomLayerPixels[i].green / 255.0) + 0.5f);
        topLayerPixels[i].red = static_cast<unsigned char>((topLayerPixels[i].red * bottomLayerPixels[i].red / 255.0) + 0.5f);
    }
}


    // Helper function to clamp the values to the 0-255 range
    inline unsigned char clampColorValue(int value) {
        if (value < 0) return 0;
        if (value > 255) return 255;
        return static_cast<unsigned char>(value);
    }

// Subtract blending mode
    void subtractBlend(std::vector<Pixel> &topLayerPixels, const std::vector<Pixel> &bottomLayerPixels) {
        for (size_t i = 0; i < topLayerPixels.size(); ++i) {
            topLayerPixels[i].blue = clampColorValue(
                    static_cast<int>(bottomLayerPixels[i].blue) - topLayerPixels[i].blue);
            topLayerPixels[i].green = clampColorValue(
                    static_cast<int>(bottomLayerPixels[i].green) - topLayerPixels[i].green);
            topLayerPixels[i].red = clampColorValue(static_cast<int>(bottomLayerPixels[i].red) - topLayerPixels[i].red);
        }
    }

// Screen blending mode
    void screenBlend(std::vector<Pixel> &topLayerPixels, const std::vector<Pixel> &bottomLayerPixels) {
        for (size_t i = 0; i < topLayerPixels.size(); ++i) {
            topLayerPixels[i].blue = clampColorValue(
                    round(255 - ((255 - topLayerPixels[i].blue) * (255 - bottomLayerPixels[i].blue) / 255.0)));
            topLayerPixels[i].green = clampColorValue(
                    round(255 - ((255 - topLayerPixels[i].green) * (255 - bottomLayerPixels[i].green) / 255.0)));
            topLayerPixels[i].red = clampColorValue(
                    round(255 - ((255 - topLayerPixels[i].red) * (255 - bottomLayerPixels[i].red) / 255.0)));
        }
    }
// Task 6: Add 200 to the green channel of "car.tga"
void adjustGreenChannel(std::vector<Pixel>& pixels, int value) {
    for (auto& pixel : pixels) {
        int newValue = static_cast<int>(pixel.green) + value;
        pixel.green = clampColorValue(newValue);
    }
}

// Task 7: Scale (multiply) the red channel by 4 and the blue channel by 0 of "car.tga"
void adjustRedBlueChannels(std::vector<Pixel>& pixels, int redScale, int blueScale) {
    for (auto& pixel : pixels) {
        pixel.red = clampColorValue(static_cast<int>(pixel.red) * redScale);
        pixel.blue = clampColorValue(static_cast<int>(pixel.blue) * blueScale);
    }
}

// Overlay blending mode
void overlayBlend(std::vector<Pixel>& topLayerPixels, const std::vector<Pixel>& bottomLayerPixels) {
    for (size_t i = 0; i < topLayerPixels.size(); ++i) {
        // Process blue channel
        unsigned char topBlue = topLayerPixels[i].blue;
        unsigned char bottomBlue = bottomLayerPixels[i].blue;
        float bottomNormalizedBlue = bottomBlue / 255.0f;
        if (bottomNormalizedBlue < 0.5f) {
            topLayerPixels[i].blue = clampColorValue(static_cast<int>((2 * bottomNormalizedBlue * topBlue) + 0.5f));
        } else {
            topLayerPixels[i].blue = clampColorValue(255 - static_cast<int>((1 - (2 * (bottomNormalizedBlue - 0.5f))) * (255 - topBlue) + 0.5f));
        }

        // Process green channel
        unsigned char topGreen = topLayerPixels[i].green;
        unsigned char bottomGreen = bottomLayerPixels[i].green;
        float bottomNormalizedGreen = bottomGreen / 255.0f;
        if (bottomNormalizedGreen < 0.5f) {
            topLayerPixels[i].green = clampColorValue(static_cast<int>((2 * bottomNormalizedGreen * topGreen) + 0.5f));
        } else {
            topLayerPixels[i].green = clampColorValue(255 - static_cast<int>((1 - (2 * (bottomNormalizedGreen - 0.5f))) * (255 - topGreen) + 0.5f));
        }

        // Process red channel
        unsigned char topRed = topLayerPixels[i].red;
        unsigned char bottomRed = bottomLayerPixels[i].red;
        float bottomNormalizedRed = bottomRed / 255.0f;
        if (bottomNormalizedRed < 0.5f) {
            topLayerPixels[i].red = clampColorValue(static_cast<int>((2 * bottomNormalizedRed * topRed) + 0.5f));
        } else {
            topLayerPixels[i].red = clampColorValue(255 - static_cast<int>((1 - (2 * (bottomNormalizedRed - 0.5f))) * (255 - topRed) + 0.5f));
        }
    }
}
// Additional function to separate channels into individual files
void writeChannelToTGA(const std::string &outputFilePath, const Header &header, const std::vector<Pixel> &pixels, char channel) {
    std::ofstream file(outputFilePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open the file for writing.");
    }

    file.write(reinterpret_cast<const char*>(&header.idLength), sizeof(header.idLength));
    file.write(reinterpret_cast<const char*>(&header.colorMapType), sizeof(header.colorMapType));
    file.write(reinterpret_cast<const char*>(&header.dataTypeCode), sizeof(header.dataTypeCode));
    file.write(reinterpret_cast<const char*>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    file.write(reinterpret_cast<const char*>(&header.colorMapLength), sizeof(header.colorMapLength));
    file.write(reinterpret_cast<const char*>(&header.colorMapDepth), sizeof(header.colorMapDepth));
    file.write(reinterpret_cast<const char*>(&header.xOrigin), sizeof(header.xOrigin));
    file.write(reinterpret_cast<const char*>(&header.yOrigin), sizeof(header.yOrigin));
    file.write(reinterpret_cast<const char*>(&header.width), sizeof(header.width));
    file.write(reinterpret_cast<const char*>(&header.height), sizeof(header.height));
    file.write(reinterpret_cast<const char*>(&header.bitsPerPixel), sizeof(header.bitsPerPixel));
    file.write(reinterpret_cast<const char*>(&header.imageDescriptor), sizeof(header.imageDescriptor));

    for (const auto &pixel : pixels) {
        unsigned char color = (channel == 'r') ? pixel.red : (channel == 'g') ? pixel.green : pixel.blue;
        file.write(reinterpret_cast<const char*>(&color), sizeof(color));
        file.write(reinterpret_cast<const char*>(&color), sizeof(color));
        file.write(reinterpret_cast<const char*>(&color), sizeof(color));
    }
    if (!file.good()) {
        throw std::runtime_error("Error occurred while writing the file.");
    }
}

// Function to rotate an image by 180 degrees (flipping it upside down)
void rotate180(std::vector<Pixel>& pixels, int width, int height) {
    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate the index for the current pixel and the pixel to swap with
            int index1 = (y * width) + x;
            int index2 = ((height - y - 1) * width) + (width - x - 1);

            // Swap the pixels
            std::swap(pixels[index1], pixels[index2]);
        }
    }
}

//Combines images for extra credit
void combineImages(const std::vector<Pixel>& carPixels, const Header& carHeader,
                   const std::vector<Pixel>& circlesPixels, const Header& circlesHeader,
                   const std::vector<Pixel>& pattern1Pixels, const Header& pattern1Header,
                   const std::vector<Pixel>& textPixels, const Header& textHeader) {

    int newWidth = carHeader.width* 2;
    int newHeight = carHeader.height * 2;
    std::vector<Pixel> combinedPixels(newWidth * newHeight);

    // Place car image at the top left
    for (int y = 0; y < carHeader.height; ++y) {
        for (int x = 0; x < carHeader.width; ++x) {
            combinedPixels[y * newWidth + x] = carPixels[y * carHeader.width + x];
        }
    }

    // Place circles image at the top right
    for (int y = 0; y < circlesHeader.height; ++y) {
        for (int x = 0; x < circlesHeader.width; ++x) {
            combinedPixels[y * newWidth + (carHeader.width + x)] = circlesPixels[y * circlesHeader.width + x];
        }
    }

    // Place text image at the bottom left
    for (int y = 0; y < textHeader.height; ++y) {
        for (int x = 0; x < textHeader.width; ++x) {
            combinedPixels[(newHeight - textHeader.height + y) * newWidth + x] = textPixels[y * textHeader.width + x];
        }
    }

    // Place pattern1 image at the bottom right
    for (int y = 0; y < pattern1Header.height; ++y) {
        for (int x = 0; x < pattern1Header.width; ++x) {
            combinedPixels[(newHeight - pattern1Header.height + y) * newWidth + (carHeader.width + x)] = pattern1Pixels[y * pattern1Header.width + x];
        }
    }

    // Create a new header for the combined image
    Header combinedHeader = carHeader; // Use one of the headers as a starting point
    combinedHeader.width = newWidth;
    combinedHeader.height = newHeight;

    // Write the new image
    writeTGA("output/extracredit.tga", combinedHeader, combinedPixels);
}


int main() {
    // Define the base paths for the input and output directories
    const std::string inputBasePath = R"(input/)";
    const std::string outputBasePath = "";

    try {
        std::ifstream layer1File("input/layer1.tga", std::ios::binary);
        std::ifstream pattern1File("input/pattern1.tga", std::ios::binary);
        std::ifstream layer2File("input/layer2.tga", std::ios::binary);
        std::ifstream carFile("input/car.tga", std::ios::binary);
        std::ifstream pattern2File("input/pattern2.tga", std::ios::binary);
        std::ifstream textFile("input/text.tga", std::ios::binary);
        std::ifstream circlesFile("input/circles.tga", std::ios::binary);

        // Make sure all files are open
        if (!layer1File || !pattern1File || !layer2File || !carFile || !pattern2File || !textFile || !circlesFile) {
            throw std::runtime_error("One or more input files could not be opened.");
        }

        // Read headers once
        Header layer1Header = readHeader(layer1File);
        Header pattern1Header = readHeader(pattern1File);
        Header layer2Header = readHeader(layer2File);
        Header carHeader = readHeader(carFile);
        Header pattern2Header = readHeader(pattern2File);
        Header textHeader = readHeader(textFile);
        Header circlesHeader = readHeader(circlesFile);

        // Task 1: Multiply layer1.tga with pattern1.tga
        std::vector<Pixel> layer1Pixels = readPixels(layer1File, layer1Header.width * layer1Header.height);
        std::vector<Pixel> pattern1Pixels = readPixels(pattern1File, pattern1Header.width * pattern1Header.height);
        multiplyBlend(layer1Pixels, pattern1Pixels);
        writeTGA(outputBasePath + "output/part1.tga", layer1Header, layer1Pixels);

        // Task 2: Subtract layer2.tga from car.tga
        std::vector<Pixel> layer2Pixels = readPixels(layer2File, layer2Header.width * layer2Header.height);
        std::vector<Pixel> carPixels = readPixels(carFile, carHeader.width * carHeader.height);
        subtractBlend(layer2Pixels, carPixels);
        writeTGA(outputBasePath + "output/part2.tga", layer2Header, layer2Pixels);

        // Task 3: Multiply layer1.tga with pattern2.tga, then Screen with text.tga
        std::vector<Pixel> pattern2Pixels = readPixels(pattern2File, pattern2Header.width * pattern2Header.height);
        // Reset layer1File to read it again
        layer1File.clear(); // Clear any end-of-file flags
        layer1File.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(layer1File); // Skip the header, as we've already read it
        std::vector<Pixel> layer1Pixels2 = readPixels(layer1File, layer1Header.width * layer1Header.height);
        multiplyBlend(layer1Pixels2, pattern2Pixels); // Reusing layer1Pixels from Task 1
        std::vector<Pixel> textPixels = readPixels(textFile, textHeader.width * textHeader.height);
        screenBlend(textPixels, layer1Pixels2); // textPixels are now the top layer
        writeTGA(outputBasePath + "output/part3.tga", textHeader, textPixels);

        // Task 4
// Task 4: Multiply "layer2.tga" with "circles.tga" and subtract the result from "pattern2.tga"
        // Reset and re-read layer2File
        layer2File.clear(); // Clear any EOF flags
        layer2File.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(layer2File); // Skip the header

        std::vector<Pixel> layer2Pixels2 = readPixels(layer2File, layer2Header.width * layer2Header.height);
        std::vector<Pixel> circlesPixels = readPixels(circlesFile, circlesHeader.width * circlesHeader.height);
        multiplyBlend(layer2Pixels2, circlesPixels);

        // Reset and re-read pattern2File
        pattern2File.clear(); // Clear any EOF flags
        pattern2File.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(pattern2File); // Skip the header, we already have it in pattern2Header

        std::vector<Pixel> pattern2Pixels2 = readPixels(pattern2File, pattern2Header.width * pattern2Header.height);
        subtractBlend(pattern2Pixels2, layer2Pixels2);
        writeTGA(outputBasePath + "output/part4.tga", pattern2Header, pattern2Pixels2);

        // Task 5: Overlay layer1.tga with pattern1.tga
        // Reusing layer1Pixels and pattern1Pixels from Task 1
        layer1File.clear(); // Clear any EOF flags
        layer1File.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(layer1File); // Skip the header, we already have it in layer1Header
        pattern1File.clear(); // Clear any EOF flags
        pattern1File.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(pattern1File); // Skip the header
        std::vector<Pixel> layer1Pixels3 = readPixels(layer1File, layer1Header.width * layer1Header.height);
        std::vector<Pixel> pattern1Pixels2 = readPixels(pattern1File, pattern1Header.width * pattern1Header.height);
        overlayBlend(layer1Pixels3, pattern1Pixels2);
        writeTGA(outputBasePath + "output/part5.tga", layer1Header, layer1Pixels3);


        // Task 6
        carFile.clear();
        carFile.seekg(0, std::ios::beg);
        readHeader(carFile); // Skip the header again
        carPixels = readPixels(carFile, carHeader.width * carHeader.height);
        adjustGreenChannel(carPixels, 200);  // Add 200 to the green channel
        writeTGA(outputBasePath + "output/part6.tga", carHeader, carPixels);

        // Reset carPixels for Task 7
        carFile.clear();
        carFile.seekg(0, std::ios::beg);
        readHeader(carFile); // Skip the header again
        carPixels = readPixels(carFile, carHeader.width * carHeader.height);

        // Task 7
        adjustRedBlueChannels(carPixels, 4, 0);  // Scale red by 4 and blue by 0
        writeTGA(outputBasePath + "output/part7.tga", carHeader, carPixels);

        // Task 8: Separate the channels of "car.tga"
        carFile.clear();
        carFile.seekg(0, std::ios::beg);
        readHeader(carFile); // Skip the header again
        carPixels = readPixels(carFile, carHeader.width * carHeader.height);
        writeChannelToTGA(outputBasePath + "output/part8_r.tga", carHeader, carPixels, 'r');
        writeChannelToTGA(outputBasePath + "output/part8_g.tga", carHeader, carPixels, 'g');
        writeChannelToTGA(outputBasePath + "output/part8_b.tga", carHeader, carPixels, 'b');

// Task 9: Combine "layer_red.tga", "layer_green.tga", and "layer_blue.tga" into one image
        std::ifstream layerRedFile("input/layer_red.tga", std::ios::binary);
        std::ifstream layerGreenFile("input/layer_green.tga", std::ios::binary);
        std::ifstream layerBlueFile("input/layer_blue.tga", std::ios::binary);

        Header layerRedHeader = readHeader(layerRedFile);
        Header layerGreenHeader = readHeader(layerGreenFile);
        Header layerBlueHeader = readHeader(layerBlueFile);

        std::vector<Pixel> layerRedPixels = readPixels(layerRedFile, layerRedHeader.width * layerRedHeader.height);
        std::vector<Pixel> layerGreenPixels = readPixels(layerGreenFile, layerGreenHeader.width * layerGreenHeader.height);
        std::vector<Pixel> layerBluePixels = readPixels(layerBlueFile, layerBlueHeader.width * layerBlueHeader.height);

        std::vector<Pixel> combinedPixels(layerRedHeader.width * layerRedHeader.height);

        for (size_t i = 0; i < combinedPixels.size(); ++i) {
            combinedPixels[i].red = layerRedPixels[i].red;
            combinedPixels[i].green = layerGreenPixels[i].green;
            combinedPixels[i].blue = layerBluePixels[i].blue;
        }
        writeTGA(outputBasePath + "output/part9.tga", layerRedHeader, combinedPixels);

        // Task 10: Rotate "text2.tga" 180 degrees and flip it upside down
        std::ifstream text2File("input/text2.tga", std::ios::binary);
        if (!text2File) {
            throw std::runtime_error("Failed to open text2.tga file.");
        }

        Header text2Header = readHeader(text2File);
        std::vector<Pixel> text2Pixels = readPixels(text2File, text2Header.width * text2Header.height);

        rotate180(text2Pixels, text2Header.width, text2Header.height);
        writeTGA(outputBasePath + "output/part10.tga", text2Header, text2Pixels);

        //EXTRA CREDIT
        carFile.clear();
        carFile.seekg(0, std::ios::beg);
        readHeader(carFile); // Skip the header again
        carPixels = readPixels(carFile, carHeader.width * carHeader.height);
        circlesFile.clear();
        circlesFile.seekg(0, std::ios::beg);
        readHeader(circlesFile); // Skip the header again
        circlesPixels = readPixels(circlesFile, circlesHeader.width * circlesHeader.height);
        pattern1File.clear(); // Clear any EOF flags
        pattern1File.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(pattern1File); // Skip the header
        pattern1Pixels = readPixels(pattern1File, pattern1Header.width * pattern1Header.height);
        textFile.clear(); // Clear any EOF flags
        textFile.seekg(0, std::ios::beg); // Seek back to the beginning of the file
        readHeader(textFile); // Skip the header
        textPixels = readPixels(textFile, textHeader.width * textHeader.height);
        combineImages(textPixels, textHeader,pattern1Pixels,circlesHeader,circlesPixels, pattern1Header, carPixels, carHeader);

        std::cout << "All blending tasks completed successfully." << std::endl;
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}