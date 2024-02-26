#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include "Image.h"



//Declare tasks
void task1();
void task2();
void task3();
void task4();
void task5();
void task6();
void task7();
void task8();
void task9();
void task10();
void extraCredit();
void testOutput(ifstream* mine, ifstream* example);

int main() {
    task1();
    //testing similarity
    ifstream mine1("output/part1.tga", ios_base::binary);
    ifstream example1("examples/EXAMPLE_part1.tga", ios_base::binary);
    cout<<"Task 1: ";
    testOutput(&mine1, &example1);

    task2();
    //testing similarity
    ifstream mine2("output/part2.tga", ios_base::binary);
    ifstream example2("examples/EXAMPLE_part2.tga", ios_base::binary);
    cout<<"Task 2: ";
    testOutput(&mine2, &example2);

    task3();
    //testing similarity
    ifstream mine3("output/part3.tga", ios_base::binary);
    ifstream example3("examples/EXAMPLE_part3.tga", ios_base::binary);
    cout<<"Task 3: ";
    testOutput(&mine3, &example3);

    task4();
    //testing similarity
    ifstream mine4("output/part4.tga", ios_base::binary);
    ifstream example4("examples/EXAMPLE_part4.tga", ios_base::binary);
    cout<<"Task 4: ";
    testOutput(&mine4, &example4);

    task5();
    //testing similarity
    ifstream mine5("output/part5.tga", ios_base::binary);
    ifstream example5("examples/EXAMPLE_part5.tga", ios_base::binary);
    cout<<"Task 5: ";
    testOutput(&mine5, &example5);

    task6();
    //testing similarity
    ifstream mine6("output/part6.tga", ios_base::binary);
    ifstream example6("examples/EXAMPLE_part6.tga", ios_base::binary);
    cout<<"Task 6: ";
    testOutput(&mine6, &example6);

    task7();
    //testing similarity
    ifstream mine7("output/part7.tga", ios_base::binary);
    ifstream example7("examples/EXAMPLE_part7.tga", ios_base::binary);
    cout<<"Task 7: ";
    testOutput(&mine7, &example7);

    task8();
    //testing similarity
    ifstream mine8r("output/part8_r.tga", ios_base::binary);
    ifstream example8r("examples/EXAMPLE_part8_r.tga", ios_base::binary);
    cout<<"Task 8r: ";
    testOutput(&mine8r, &example8r);
    ifstream mine8g("output/part8_g.tga", ios_base::binary);
    ifstream example8g("examples/EXAMPLE_part8_g.tga", ios_base::binary);
    cout<<"Task 8g: ";
    testOutput(&mine8g, &example8g);
    ifstream mine8b("output/part8_b.tga", ios_base::binary);
    ifstream example8b("examples/EXAMPLE_part8_b.tga", ios_base::binary);
    cout<<"Task 8b: ";
    testOutput(&mine8b, &example8b);

    task9();
    //testing similarity
    ifstream mine9("output/part9.tga", ios_base::binary);
    ifstream example9("examples/EXAMPLE_part9.tga", ios_base::binary);
    cout<<"Task 9: ";
    testOutput(&mine9, &example9);

    task10();
    //testing similarity
    ifstream mine10("output/part10.tga", ios_base::binary);
    ifstream example10("examples/EXAMPLE_part10.tga", ios_base::binary);
    cout<<"Task 10: ";
    testOutput(&mine10, &example10);

    extraCredit();
    //testing similarity
    ifstream mineEc("output/extracredit.tga", ios_base::binary);
    ifstream exampleEc("examples/EXAMPLE_extracredit.tga", ios_base::binary);
    cout<<"Extra Credit: ";
    testOutput(&mineEc, &exampleEc);

    return 0;
}

//Define tasks
void task1(){
    //open tga files
    ifstream layer1Tga;
    layer1Tga.open("input/layer1.tga", ios_base::binary);
    Image layer1(&layer1Tga);
    layer1Tga.close();
    ifstream pattern1Tga;
    pattern1Tga.open("input/pattern1.tga", ios_base::binary);
    Image pattern1(&pattern1Tga);
    pattern1Tga.close();
    //create new image
    Image newImage = layer1.multiply(pattern1);
    ofstream newFile;
    newFile.open("output/part1.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task2(){
    //open tga files
    ifstream layer2Tga;
    layer2Tga.open("input/layer2.tga", ios_base::binary);
    Image layer2(&layer2Tga);
    layer2Tga.close();
    ifstream carTga;
    carTga.open("input/car.tga", ios_base::binary);
    Image car(&carTga);
    carTga.close();
    //create new image
    Image newImage = layer2.subtract(car);
    ofstream newFile;
    newFile.open("output/part2.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task3(){
    //open layer 1 and pattern 2
    ifstream layer1Tga;
    layer1Tga.open("input/layer1.tga", ios_base::binary);
    Image layer1(&layer1Tga);
    layer1Tga.close();
    ifstream pattern2Tga;
    pattern2Tga.open("input/pattern2.tga", ios_base::binary);
    Image pattern2(&pattern2Tga);
    pattern2Tga.close();
    //store multiplied image
    Image multiplied = layer1.multiply(pattern2);
    ifstream textTga;
    textTga.open("input/text.tga");
    Image text(&textTga);
    textTga.close();
    //create new image
    Image newImage = text.screen(multiplied);
    ofstream newFile;
    newFile.open("output/part3.tga", ios_base::binary);
    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task4(){
    ifstream layer2Tga;
    layer2Tga.open("input/layer2.tga");
    Image layer2(&layer2Tga);
    layer2Tga.close();
    ifstream circlesTga;
    circlesTga.open("input/circles.tga", ios_base::binary);
    Image circles(&circlesTga);
    circlesTga.close();
    //store multiplied image
    Image multiplied = layer2.multiply(circles);
    ifstream pattern2Tga;
    pattern2Tga.open("input/pattern2.tga", ios_base::binary);
    Image pattern2(&pattern2Tga);
    pattern2Tga.close();
    Image newImage = pattern2.subtract(multiplied);
    ofstream newFile;
    newFile.open("output/part4.tga", ios_base::binary);
    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task5(){
    ifstream layer1Tga;
    layer1Tga.open("input/layer1.tga", ios_base::binary);
    Image layer1(&layer1Tga);
    layer1Tga.close();
    ifstream pattern1Tga;
    pattern1Tga.open("input/pattern1.tga", ios_base::binary);
    Image pattern1(&pattern1Tga);
    pattern1Tga.close();
    //create new image
    Image newImage = layer1.overlay(pattern1);
    ofstream newFile;
    newFile.open("output/part5.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task6(){
    ifstream carTga;
    carTga.open("input/car.tga", ios_base::binary);
    Image car(&carTga);
    carTga.close();
    Image newImage = car.addChannel("green", 200);
    ofstream newFile;
    newFile.open("output/part6.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task7(){
    ifstream carTga;
    carTga.open("input/car.tga", ios_base::binary);
    Image car(&carTga);
    carTga.close();
    //scale channels
    Image tempImage = car.scaleChannel("red",4);
    Image newImage = tempImage.scaleChannel("blue", 0);
    ofstream newFile;
    newFile.open("output/part7.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}
void task8(){
    ifstream carTga;
    carTga.open("input/car.tga", ios_base::binary);
    Image car(&carTga);
    carTga.close();
    //initialize new image objects for each channel
    Image redChannel=car;
    Image greenChannel=car;
    Image blueChannel=car;
    unsigned char* newPixelData= new unsigned char[car.getSize()];

    for(int i=2; i<car.getSize();i+=3){
        newPixelData[i]=car.getPixelData()[i];
        //set other channels = to red data
        newPixelData[i-1]=car.getPixelData()[i];
        newPixelData[i-2]=car.getPixelData()[i];
    }
    //set red channel data
    redChannel.setPixelData(newPixelData);

    for(int i=1; i<car.getSize();i+=3){
        newPixelData[i]=car.getPixelData()[i];
        //set other channels = to green data
        newPixelData[i-1]=car.getPixelData()[i];
        newPixelData[i+1]=car.getPixelData()[i];
    }
    //set green channel data
    greenChannel.setPixelData(newPixelData);

    for(int i=0; i<car.getSize();i+=3){
        newPixelData[i]=car.getPixelData()[i];
        //set other channels = to blue data
        newPixelData[i+1]=car.getPixelData()[i];
        newPixelData[i+2]=car.getPixelData()[i];
    }
    //set blue channel data
    blueChannel.setPixelData(newPixelData);
    delete[] newPixelData;
    //write red file
    ofstream redFile;
    redFile.open("output/part8_r.tga", ios_base::binary);
    for(int i=0; i<18; i++) {
        redFile<<(redChannel.getHeaderData())[i];
    }
    for(int i=0; i<redChannel.getSize(); i++){
        redFile<<(redChannel.getPixelData()[i]);
    }
    redFile.close();
    //write green file
    ofstream greenFile;
    greenFile.open("output/part8_g.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        greenFile<<(greenChannel.getHeaderData())[i];
    }
    for(int i=0; i<greenChannel.getSize(); i++){
        greenFile<<(greenChannel.getPixelData()[i]);
    }
    greenFile.close();
    //write blue file
    ofstream blueFile;
    blueFile.open("output/part8_b.tga", ios_base::binary);
    for(int i=0; i<18; i++) {
        blueFile<<(blueChannel.getHeaderData())[i];
    }
    for(int i=0; i<blueChannel.getSize(); i++){
        blueFile<<(blueChannel.getPixelData()[i]);
    }
    blueFile.close();
}
void task9(){
    //read red layer
    ifstream redTga("input/layer_red.tga", ios_base::binary);
    Image red(&redTga);
    //read green layer
    ifstream greenTga("input/layer_green.tga", ios_base::binary);
    Image green(&greenTga);
    //read blue layer
    ifstream blueTga("input/layer_blue.tga", ios_base::binary);
    Image blue(&blueTga);
    Image newImage=red;
    unsigned char* newPixelData= new unsigned char[red.getSize()];
    for(int i=0; i<newImage.getSize(); i+=3){
        newPixelData[i+2]=red.getPixelData()[i+2];
        newPixelData[i+1]=green.getPixelData()[i+1];
        newPixelData[i]=blue.getPixelData()[i];
    }
    newImage.setPixelData(newPixelData);
    ofstream newFile;
    newFile.open("output/part9.tga", ios_base::binary);
    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    delete[] newPixelData;
}
void task10(){
    ifstream text2Tga;
    text2Tga.open("input/text2.tga", ios_base::binary);
    Image text1(&text2Tga);
    text2Tga.close();
    Image newImage= text1.rotate();
    ofstream newFile;
    newFile.open("output/part10.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
}

void extraCredit(){
    //open files
    ifstream carTga("input/car.tga", ios_base::binary);
    ifstream circlesTga("input/circles.tga", ios_base::binary);
    ifstream textTga("input/text.tga", ios_base::binary);
    ifstream pattern1Tga("input/pattern1.tga", ios_base::binary);
    //create images
    Image car(&carTga);
    Image circles(&circlesTga);
    Image text(&textTga);
    Image pattern1(&pattern1Tga);

    Image newImage = car;
    newImage.setWidth(car.getWidth() + circles.getWidth());
    newImage.setHeight(car.getHeight() + text.getHeight());
    unsigned char *newPixelData = new unsigned char[newImage.getSize()];
    unsigned int count = 0;
    //populate top half of image
    for (unsigned int i = 0; i < (newImage.getHeight() / 2); i++) {
        //third quarter of image
        for (unsigned int j = 0; j < (newImage.getWidth() / 2*3); j++) {
            newPixelData[count] = text.getPixelData()[(i * text.getWidth())*3 + j]; //get pixel at correct position
            count++;
        }
        //fourth quarter of image
        for (unsigned int j = 0; j < (newImage.getWidth() / 2*3); j++) {
            newPixelData[count] = pattern1.getPixelData()[(i * pattern1.getWidth())*3 + j]; //get pixel at correct position
            count++;
        }
    }
    //bottom half
    for (unsigned int i = 0; i < (newImage.getHeight() / 2); i++) {
        //first quarter of image
        for (unsigned int j = 0; j < (newImage.getWidth() / 2*3); j++) {
            newPixelData[count] = car.getPixelData()[(i * car.getWidth())*3 + j]; //get pixel at correct position
            count++;
        }
        //second quarter of image
        for (unsigned int j = 0; j < (newImage.getWidth() / 2*3); j++) {
            newPixelData[count] = circles.getPixelData()[(i * circles.getWidth())*3 + j]; //get pixel at correct position
            count++;
        }
    }

    newImage.resizePixelData(newPixelData, newImage.getSize());
    newImage.resetHeaderData();
    ofstream newFile;
    newFile.open("output/extracredit.tga", ios_base::binary);

    for(int i=0; i<18; i++) {
        newFile<<(newImage.getHeaderData())[i];
    }
    for(unsigned int i=0; i<newImage.getSize(); i++){
        newFile<<(newImage.getPixelData()[i]);
    }
    newFile.close();
    delete[] newPixelData;
}

void testOutput(ifstream* mine, ifstream* example){
    int count=0;
    Image myImage(mine);
    Image exampleImage(example);
    //check pixel data for my output vs example
    for(int i=0;i< myImage.getSize()/3; i++){
        if(myImage.getPixelData()[i] != exampleImage.getPixelData()[i])
        {
            count+=1;
        }
    }
    if(count==0){
        cout<<"Test passed successfully! :)"<<endl;
    }
    else
    {
        //print number of differences between pixels if failed
        cout<<"Test failed :( There were "<<count<<" differences."<<endl;
    }
}

