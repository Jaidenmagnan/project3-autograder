//
// Created by kkeer on 11/9/2023.
//

#include "UnitTesting.h"

UnitTesting::UnitTesting() {}

// Checks if the task output matches example output
bool UnitTesting::task(const string& outputFilename, const string& exampleFilename, const string& testNumber) {
    // Loads both files into a ImageProcessing object
    ImageProcessing taskImage(outputFilename);
    ImageProcessing exampleImage(exampleFilename);

    // Using the overloaded equality operator for Image Processing, checks to see if the two images are equal
    if(taskImage == exampleImage) {
        // Because both are equal we passed
        cout << "Test #" + testNumber << "..... Passed!" << endl;
        return true;
    }
    else {
        // Because both are not equal we failed
        cout << "Test #" + testNumber << "..... Failed!" << endl;
        return false;
    }
}