//
// Created by kkeer on 11/9/2023.
//

#ifndef IMAGE_PROCESSOR_UNITTESTING_H
#define IMAGE_PROCESSOR_UNITTESTING_H

#include "ImageProcessing.h"
#include <iostream>
using namespace std;

class UnitTesting {
public:
    UnitTesting(); // Defauolt Constructor
    bool task(const string& outputFilename, const string& exampleFilename, const string& testNumber); // Checks if the task output matches example output
};


#endif //IMAGE_PROCESSOR_UNITTESTING_H
