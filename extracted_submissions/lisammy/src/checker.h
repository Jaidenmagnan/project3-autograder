#include <iostream>
#include <fstream>
using namespace std;

#ifndef IMAGE_PROCESSOR_CHECKER_H
#define IMAGE_PROCESSOR_CHECKER_H
//Checks if the two files are equal
class Checker{
public:
    static bool checkEqual(const string& filename, const string& fileExample){
        fstream fileIn(filename, ios::in | ios::binary); //takes in the file we create
        fstream fileEx(fileExample, ios::in | ios::binary); // takes in example file
        if (fileIn.fail() || fileEx.fail()){
            cout << "file cannot be found" << endl;
            return false; //file cannot be found?
        }
        if (fileIn.tellg() != fileIn.tellg()) {
            cout << "size mismatch" << endl;
            return false; //size mismatch
        }
        fileIn.seekg(0, ifstream::beg); //starts at beginning
        fileEx.seekg(0, ifstream::beg); //starts at beginning

        //If returned 1, then it is true.
        return equal(istreambuf_iterator<char>(fileIn.rdbuf()),
                     istreambuf_iterator<char>(),
                     istreambuf_iterator<char>(fileEx.rdbuf()));
    }
};
#endif //IMAGE_PROCESSOR_CHECKER_H
