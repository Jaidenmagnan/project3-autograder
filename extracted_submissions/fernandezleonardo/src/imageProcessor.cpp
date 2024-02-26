//Sources:https://www.geeksforgeeks.org/, https://www.tutorialspoint.com/, https://stackoverflow.com/, https://www.educative.io/
//https://cplusplus.com/, https://www.tutorialspoint.com/, https://www.scaler.com/, https://www.w3schools.com/, https://www.geeksforgeeks.org/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

struct Header{
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

struct Image{
    Header header;
    std::vector<unsigned char> imageData;
};

std::string testFile(std::string filename);
Image processFile(std::string filename);
void writeFile(std::string filename, Header header, std::vector<unsigned char> imageData);

int main() {
//    std::cout << "Part 1 match: " << testFile("part1.tga") << "\n";
//    std::cout << "Part 2 match: " << testFile("part2.tga")<< "\n";
//    std::cout << "Part 3 match: " << testFile("part3.tga")<< "\n";
//    std::cout << "Part 4 match: " << testFile("part4.tga")<< "\n";
//    std::cout << "Part 5 match: " << testFile("part5.tga")<< "\n";
//    std::cout << "Part 6 match: " << testFile("part6.tga")<< "\n";
//    std::cout << "Part 7 match: " << testFile("part7.tga")<< "\n";
//    std::cout << "Part 8b match: " << testFile("part8_b.tga")<< "\n";
//    std::cout << "Part 8g match: " << testFile("part8_g.tga")<< "\n";
//    std::cout << "Part 8r match: " << testFile("part8_r.tga")<< "\n";
//    std::cout << "Part 9 match: " << testFile("part9.tga")<< "\n";
//    std::cout << "Part 10 match: " << testFile("part10.tga")<< "\n";

    Image layer1 = processFile("layer1.tga");
    Image pattern1 = processFile("pattern1.tga");

    std::vector<unsigned char> part1Data;
    for (int i = 0; i < layer1.imageData.size(); i++) {
        float val1 = (float) (layer1.imageData[i]) / 255;
        float val2 = (float) (pattern1.imageData[i]) / 255;
        float resultVal = (val1 * val2 * 255) + 0.5f;
        part1Data.push_back((unsigned char) resultVal);
    }
    writeFile("part1.tga", layer1.header, part1Data);

    Image layer2 = processFile("layer2.tga");
    Image car = processFile("car.tga");

    std::vector<unsigned char> part2Data;
    for (int i = 0; i < car.imageData.size(); i++) {
        int value = (int) car.imageData[i] - (int) layer2.imageData[i];
        if (value > 255)
            value = 255;
        else if (value < 0)
            value = 0;
        part2Data.push_back((unsigned char) value);
    }
    writeFile("part2.tga", car.header, part2Data);

    Image pattern2 = processFile("pattern2.tga");
    Image text = processFile("text.tga");

    std::vector<unsigned char> part3MultData;
    for (int i = 0; i < layer1.imageData.size(); i++) {
        float val1 = (float) (layer1.imageData[i]) / 255;
        float val2 = (float) (pattern2.imageData[i]) / 255;
        float resultVal = (val1 * val2 * 255) + 0.5f;
        part3MultData.push_back((unsigned char) resultVal);
    }
    std::vector<unsigned char> part3Data;
    for (int i = 0; i < part3MultData.size(); i++) {
        float val1 = (float) (part3MultData[i]) / 255;
        float val2 = (float) (text.imageData[i]) / 255;
        float resultVal = (1 - (1 - val1) * (1 - val2)) * 255 + 0.5f;
        part3Data.push_back((unsigned char) resultVal);
    }
    writeFile("part3.tga", layer1.header, part3Data);

    Image circles = processFile("circles.tga");

    std::vector<unsigned char> part4MultData;
    for (int i = 0; i < layer2.imageData.size(); i++) {
        float val1 = (float) (layer2.imageData[i]) / 255;
        float val2 = (float) (circles.imageData[i]) / 255;
        float resultVal = (val1 * val2 * 255) + 0.5f;
        part4MultData.push_back((unsigned char) resultVal);
    }
    std::vector<unsigned char> part4Data;
    for (int i = 0; i < part4MultData.size(); i++) {
        int value = (int) part4MultData[i] - (int) pattern2.imageData[i];
        if (value > 255)
            value = 255;
        else if (value < 0)
            value = 0;
        part4Data.push_back((unsigned char) value);
    }
    writeFile("part4.tga", pattern2.header, part4Data);

    std::vector<unsigned char> part5Data;
    for (int i = 0; i < pattern1.imageData.size() / 3; i++) {
        float tonalPixVal =
                (float) (pattern1.imageData[3 * i + 2] + pattern1.imageData[3 * i + 1] + pattern1.imageData[3 * i]) /
                3 + 0.5f;
        if ((unsigned char) tonalPixVal <= 127) {
            for (int j = 0; j < 3; j++) {
                float val1 = (float) (layer1.imageData[3 * i + j]) / 255;
                float val2 = (float) (pattern1.imageData[3 * i + j]) / 255;
                float resultVal = (255 * (2 * val1 * val2)) + 0.5f;
                if (resultVal > 255)
                    resultVal = 255;
                part5Data.push_back((unsigned char) resultVal);
            }
        } else if ((unsigned char) tonalPixVal > 127) {
            for (int j = 0; j < 3; j++) {
                float val1 = (float) (layer1.imageData[3 * i + j]) / 255;
                float val2 = (float) (pattern1.imageData[3 * i + j]) / 255;
                float resultVal = (255 * (1 - 2 * (1 - val1) * (1 - val2))) + 0.5f;
                if (resultVal > 255)
                    resultVal = 255;
                else if (resultVal < 0)
                    resultVal = 0;
                part5Data.push_back((unsigned char) resultVal);
            }
        }
    }
    writeFile("part5.tga", layer1.header, part5Data);

    std::vector<unsigned char> part6Data;
    for (int i = 0; i < car.imageData.size() / 3; i++) {
        part6Data.push_back(car.imageData[3 * i]);
        int val = (int) car.imageData[3 * i + 1];
        val += 200;
        if (val > 255)
            val = 255;
        part6Data.push_back((unsigned char) val);
        part6Data.push_back(car.imageData[3 * i + 2]);
    }
    writeFile("part6.tga", car.header, part6Data);

    std::vector<unsigned char> part7Data;
    for (int i = 0; i < car.imageData.size() / 3; i++) {
        part7Data.push_back((unsigned char) car.imageData[3 * i] * 0);
        part7Data.push_back((unsigned char) car.imageData[3 * i + 1]);
        int val = (int) car.imageData[3 * i + 2] * 4;
        if (val > 255)
            val = 255;
        part7Data.push_back((unsigned char) val);
    }
    writeFile("part7.tga", car.header, part7Data);

    std::vector<unsigned char> part8_rData;
    std::vector<unsigned char> part8_bData;
    std::vector<unsigned char> part8_gData;
    for (int i = 0; i < car.imageData.size() / 3; i++) {
        for (int j = 0; j < 3; j++) {
            part8_bData.push_back(car.imageData[3 * i]);
            part8_gData.push_back(car.imageData[3 * i + 1]);
            part8_rData.push_back(car.imageData[3 * i + 2]);
        }
    }
    writeFile("part8_r.tga", car.header, part8_rData);
    writeFile("part8_g.tga", car.header, part8_gData);
    writeFile("part8_b.tga", car.header, part8_bData);

    Image layer_blue = processFile("layer_blue.tga");
    Image layer_green = processFile("layer_green.tga");
    Image layer_red = processFile("layer_red.tga");

    std::vector<unsigned char> part9Data;
    for (int i = 0; i < layer_blue.imageData.size() / 3; i++) {
        part9Data.push_back(layer_blue.imageData[3 * i]);
        part9Data.push_back(layer_green.imageData[3 * i + 1]);
        part9Data.push_back(layer_red.imageData[3 * i + 2]);
    }
    writeFile("part9.tga", layer_blue.header, part9Data);

    Image text2 = processFile("text2.tga");

    std::vector<unsigned char> part10Data;
    for (int i = 0; i < text2.imageData.size() / 3; i++) {
        part10Data.insert(part10Data.begin(), text2.imageData[3 * i + 2]);
        part10Data.insert(part10Data.begin(), text2.imageData[3 * i + 1]);
        part10Data.insert(part10Data.begin(), text2.imageData[3 * i]);
    }
    writeFile("part10.tga", text2.header, part10Data);

    return 0;
}

std::string testFile(std::string filename){
    std::string filename2 = "EXAMPLE_"+filename;
    Header header1;
    Header header2;
    std::ifstream file1(std::filesystem::path("output")/filename, std::ios::binary);
    std::ifstream file2(std::filesystem::path("examples")/filename2, std::ios::binary);

    file1.read(&header1.idLength, 1);
    file1.read(&header1.colorMapType, 1);
    file1.read(&header1.dataTypeCode, 1);
    file1.read((char *) &header1.colorMapOrigin, 2);
    file1.read((char *) &header1.colorMapLength, 2);
    file1.read(&header1.colorMapDepth, 1);
    file1.read((char *) &header1.xOrigin, 2);
    file1.read((char *) &header1.yOrigin, 2);
    file1.read((char *) &header1.width, 2);
    file1.read((char *) &header1.height, 2);
    file1.read(&header1.bitsPerPixel, 1);
    file1.read(&header1.imageDescriptor, 1);

    unsigned char imageData1[header1.width * header1.height * 3];
    for (int i=0; i < header1.width * header1.height * 3; i++)
        file1.read((char *) &imageData1[i], 1);

    std::vector<unsigned char> vectorImageData1;
    for (int i=0; i < header1.width * header1.height * 3; i++)
        vectorImageData1.push_back((unsigned char) imageData1[i]);

    file2.read(&header2.idLength, 1);
    file2.read(&header2.colorMapType, 1);
    file2.read(&header2.dataTypeCode, 1);
    file2.read((char *) &header2.colorMapOrigin, 2);
    file2.read((char *) &header2.colorMapLength, 2);
    file2.read(&header2.colorMapDepth, 1);
    file2.read((char *) &header2.xOrigin, 2);
    file2.read((char *) &header2.yOrigin, 2);
    file2.read((char *) &header2.width, 2);
    file2.read((char *) &header2.height, 2);
    file2.read(&header2.bitsPerPixel, 1);
    file2.read(&header2.imageDescriptor, 1);

    unsigned char imageData2[header2.width * header2.height * 3];
    for (int i=0; i < header2.width * header2.height * 3; i++)
        file2.read((char *) &imageData2[i], 1);

    std::vector<unsigned char> vectorImageData2;
    for (int i=0; i < header2.width * header2.height * 3; i++)
        vectorImageData2.push_back((unsigned char) imageData2[i]);

    bool match = true;
    for (int i=0; i < vectorImageData1.size(); i++){
        if (vectorImageData1[i] != vectorImageData2[i])
            match = false;
    }
    if (match) return "PASS!";
    else return "FAIL!";
}

Image processFile(std::string filename){
    Header header;
    std::ifstream file(std::filesystem::path("input")/filename, std::ios::binary);

    file.read(&header.idLength, 1);
    file.read(&header.colorMapType, 1);
    file.read(&header.dataTypeCode, 1);
    file.read((char *) &header.colorMapOrigin, 2);
    file.read((char *) &header.colorMapLength, 2);
    file.read(&header.colorMapDepth, 1);
    file.read((char *) &header.xOrigin, 2);
    file.read((char *) &header.yOrigin, 2);
    file.read((char *) &header.width, 2);
    file.read((char *) &header.height, 2);
    file.read(&header.bitsPerPixel, 1);
    file.read(&header.imageDescriptor, 1);

    unsigned char imageData[header.width * header.height * 3];
    for (int i=0; i < header.width * header.height * 3; i++)
        file.read((char *) &imageData[i], 1);

    std::vector<unsigned char> vectorImageData;
    for (int i=0; i < header.width * header.height * 3; i++)
        vectorImageData.push_back((unsigned char) imageData[i]);

    Image image = {header, vectorImageData};

    return image;
}

void writeFile(std::string filename, Header header, std::vector<unsigned char> imageData){
    std::ofstream outfile(std::filesystem::path("output")/filename, std::ios::binary);

    outfile.write(&header.idLength, 1);
    outfile.write(&header.colorMapType, 1);
    outfile.write(&header.dataTypeCode, 1);
    outfile.write((char *) &header.colorMapOrigin, 2);
    outfile.write((char *) &header.colorMapLength, 2);
    outfile.write(&header.colorMapDepth, 1);
    outfile.write((char *) &header.xOrigin, 2);
    outfile.write((char *) &header.yOrigin, 2);
    outfile.write((char *) &header.width, 2);
    outfile.write((char *) &header.height, 2);
    outfile.write(&header.bitsPerPixel, 1);
    outfile.write(&header.imageDescriptor, 1);

    for (int i=0; i < header.width * header.height * 3; i++)
        outfile.write((char *)&imageData[i],1);
}
