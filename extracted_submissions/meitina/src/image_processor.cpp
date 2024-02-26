//
// Created by tinam on 11/9/2023.
//
#include "Header.h"
#include "ImageData.h"

using namespace std;
// function to assign values
void getInfo(const string& fileName, Header& header, ImageData& pix){
    assignHeader(header, fileName);
    assignPixels(pix, fileName, static_cast<int>(header.height), static_cast<int>(header.width));
}
// function to compare two TGA files
bool compareTGA(const Header& header1, const ImageData& data1, const Header& header2, const ImageData& data2, const string& testNum) {
    // compare header information
    if (header1.width != header2.width || header1.height != header2.height) {
        cout << "Test #" << testNum << "...... Failed!" << endl;
        return false;
    }

    // Compare pixel data
    for (int i = 0; i < header1.width * header1.height * 3; ++i) {
        if (data1.pixelArray[i] != data2.pixelArray[i]) {
            cout << "Test #" << testNum << "...... Failed!" << endl;
            return false;
        }
    }
    if(testNum == "8"){ // for test 8
        return true;
    }
    else{
        cout << "Test #" << testNum << "...... Passed!" << endl;
        return true;
        }
}
int main(){
    // task 1
    // get layer1 info
    string layer1 = "input/layer1.tga";
    Header Hlayer1{};
    ImageData Dlayer1{};
    getInfo(layer1, Hlayer1, Dlayer1);
    // get pattern1 info
    string pattern1 = "input/pattern1.tga";
    Header Hpattern1{};
    ImageData Dpattern1{};
    getInfo(pattern1, Hpattern1, Dpattern1);
    // multiply layers and create new ImageData obj
    string part1 = "output/part1.tga";
    ImageData Dpart1{}; //no header needed bc header stays the same
    multiplyPixels(Dlayer1, Dpattern1, static_cast<int>(Hpattern1.height), static_cast<int>(Hpattern1.width), Dpart1);
    // create a file with header and add data
    outHeader(Hpattern1, part1);
    outData(Dpart1, part1, static_cast<int>(Hpattern1.height), static_cast<int>(Hpattern1.width));

    // load example1 for test
    string ex1= "examples/EXAMPLE_part1.tga";
    Header Hex1{};
    ImageData Dex1{};
    getInfo(ex1, Hex1, Dex1);
    Header Hpart1{};
    assignHeader(Hpart1, part1);
    // compare output to example
    compareTGA(Hex1, Dex1, Hpart1, Dpart1, "1");

    // deallocate part 1 files
    deallImgData(Dpart1);
    deallImgData(Dex1);

    // task 2
    // get layer 2 info
    string layer2 = "input/layer2.tga";
    Header Hlayer2{};
    ImageData Dlayer2{};
    getInfo(layer2, Hlayer2, Dlayer2);
    // get car info
    string car = "input/car.tga";
    Header Hcar{};
    ImageData Dcar{};
    getInfo(car, Hcar, Dcar);
    // subtract layer2 from car
    string part2 = "output/part2.tga";
    ImageData Dpart2{};
    subtractPixels(Dlayer2, Dcar, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), Dpart2);
    // create a file with header and add data
    outHeader(Hlayer2, part2);
    outData(Dpart2, part2, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width));

    // load example2 for test
    string ex2= "examples/EXAMPLE_part2.tga";
    Header Hex2{};
    ImageData Dex2{};
    getInfo(ex2, Hex2, Dex2);
    Header Hpart2{};
    assignHeader(Hpart2, part2);
    // compare output to example
    compareTGA(Hex2, Dex2, Hpart2, Dpart2, "2");

    // deallocate part 2 files
    deallImgData(Dpart2);
    deallImgData(Dex2);

    // task 3
    // get pattern 2 info
    string pattern2 = "input/pattern2.tga";
    Header Hpattern2{};
    ImageData Dpattern2{};
    getInfo(pattern2, Hpattern2, Dpattern2);
    // multiply layer1 (previously loaded) by pattern2 and store result in temp3
    ImageData Dtemp3{};
    multiplyPixels(Dlayer1, Dpattern2, static_cast<int>(Hpattern2.height), static_cast<int>(Hpattern2.width), Dtemp3);
    // get text info
    string text = "input/text.tga";
    Header Htext{};
    ImageData Dtext{};
    getInfo(text, Htext, Dtext);
    // screen text and previous result (Dtemp3)
    string part3 = "output/part3.tga";
    ImageData Dpart3{};
    screenPixels(Dtext, Dtemp3, static_cast<int>(Hpattern2.height), static_cast<int>(Hpattern2.width), Dpart3);
    // create file with header and add data
    outHeader(Htext, part3);
    outData(Dpart3, part3, static_cast<int>(Htext.height), static_cast<int>(Htext.width));

    // load example3 for test
    string ex3= "examples/EXAMPLE_part3.tga";
    Header Hex3{};
    ImageData Dex3{};
    getInfo(ex3, Hex3, Dex3);
    Header Hpart3{};
    assignHeader(Hpart3, part3);
    // compare output to example
    compareTGA(Hex3, Dex3, Hpart3, Dpart3, "3");

    // deallocate part 3 files
    deallImgData(Dpart3);
    deallImgData(Dtext);
    deallImgData(Dtemp3);
    deallImgData(Dex3);

    // task 4
    // get circles info
    string circles = "input/circles.tga";
    Header Hcircles{};
    ImageData Dcircles{};
    getInfo(circles, Hcircles, Dcircles);
    // multiply layer 2 and circles and store result in temp4
    ImageData Dtemp4{};
    multiplyPixels(Dlayer2, Dcircles, static_cast<int>(Hcircles.height), static_cast<int>(Hcircles.width), Dtemp4);
    string part4 = "output/part4.tga";
    ImageData Dpart4{};
    subtractPixels(Dpattern2, Dtemp4, static_cast<int>(Hcircles.height), static_cast<int>(Hcircles.width),Dpart4);
    // create file with header and add data
    outHeader(Hcircles, part4);
    outData(Dpart4, part4, static_cast<int>(Hcircles.height), static_cast<int>(Hcircles.width));

    // load example4 for test
    string ex4= "examples/EXAMPLE_part4.tga";
    Header Hex4{};
    ImageData Dex4{};
    getInfo(ex4, Hex4, Dex4);
    Header Hpart4{};
    assignHeader(Hpart4, part4);
    // compare output to example
    compareTGA(Hex4, Dex4, Hpart4, Dpart4, "4");

    //deallocate part 4 files
    deallImgData(Dpattern2);
    deallImgData(Dtemp4);
    deallImgData(Dpart4);
    deallImgData(Dcircles);
    deallImgData(Dlayer2);
    deallImgData(Dex4);

    // task 5 overlay layer1 (top) and pattern1 (bottom)
    string part5 = "output/part5.tga";
    ImageData Dpart5{};
    overlayPixels(Dlayer1, Dpattern1, static_cast<int>(Hpattern1.height), static_cast<int>(Hpattern1.width), Dpart5);
    outHeader(Hlayer1, part5);
    outData(Dpart5, part5,static_cast<int>(Hpattern1.height), static_cast<int>(Hpattern1.width));

    // load example5 for test
    string ex5= "examples/EXAMPLE_part5.tga";
    Header Hex5{};
    ImageData Dex5{};
    getInfo(ex5, Hex5, Dex5);
    Header Hpart5{};
    assignHeader(Hpart5, part5);
    // compare output to example
    compareTGA(Hex5, Dex5, Hpart5, Dpart5, "5");

    //deallocate part 5 files
    deallImgData(Dlayer1);
    deallImgData(Dpart5);
    deallImgData(Dpattern1);
    deallImgData(Dex5);

    // task 6 add 200 to green channel of car
    string part6 = "output/part6.tga";
    ImageData Dpart6{};
    editChannel(Dcar, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), "green", 200.0f, 1.0f, Dpart6);
    outHeader(Hcar, part6);
    outData(Dpart6, part6, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width));

    // load example6 for test
    string ex6= "examples/EXAMPLE_part6.tga";
    Header Hex6{};
    ImageData Dex6{};
    getInfo(ex6, Hex6, Dex6);
    Header Hpart6{};
    assignHeader(Hpart6, part6);
    // compare output to example
    compareTGA(Hex6, Dex6, Hpart6, Dpart6, "6");

    //deallocate part 6 files
    deallImgData(Dex6);
    deallImgData(Dpart6);

    // task 7 scale red by 4 and blue by 0 in car
    ImageData DpartTemp7{};
    // scale red channel by 4
    editChannel(Dcar, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), "red", 0.0f, 4.0f, DpartTemp7);
    // scale blue channel by 0
    ImageData Dpart7{};
    string part7 = "output/part7.tga";
    editChannel(DpartTemp7, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), "blue", 0.0f, 0.0f, Dpart7);
    outHeader(Hcar, part7);
    outData(Dpart7, part7, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width));

    // load example7 for test
    string ex7= "examples/EXAMPLE_part7.tga";
    Header Hex7{};
    ImageData Dex7{};
    getInfo(ex7, Hex7, Dex7);
    Header Hpart7{};
    assignHeader(Hpart7, part7);
    // compare output to example
    compareTGA(Hex7, Dex7, Hpart7, Dpart7, "7");

    //deallocate part 7 files
    deallImgData(DpartTemp7);
    deallImgData(Dpart7);
    deallImgData(Dex7);

    // task 8 seperate each channel of car to a diff file
    ImageData Dpart8g{};
    // green file
    string part8g = "output/part8_g.tga";
    separateChannel(Dcar, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), "green", Dpart8g);
    outHeader(Hcar, part8g);
    outData(Dpart8g, part8g, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width));
    // blue file
    ImageData Dpart8b{};
    string part8b = "output/part8_b.tga";
    separateChannel(Dcar, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), "blue", Dpart8b);
    outHeader(Hcar, part8b);
    outData(Dpart8b, part8b, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width));
    // red file
    ImageData Dpart8r{};
    string part8r = "output/part8_r.tga";
    separateChannel(Dcar, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width), "red", Dpart8r);
    outHeader(Hcar, part8r);
    outData(Dpart8r, part8r, static_cast<int>(Hcar.height), static_cast<int>(Hcar.width));

    // load red example for test
    string ex8r= "examples/EXAMPLE_part8_r.tga";
    Header Hex8r{};
    ImageData Dex8r{};
    getInfo(ex8r, Hex8r, Dex8r);
    Header Hpart8r{};
    assignHeader(Hpart8r, part8r);
    // compare output to example
    bool redBool = compareTGA(Hex8r, Dex8r, Hpart8r, Dpart8r, "8");
    // load green example for test
    string ex8g= "examples/EXAMPLE_part8_g.tga";
    Header Hex8g{};
    ImageData Dex8g{};
    getInfo(ex8g, Hex8g, Dex8g);
    Header Hpart8g{};
    assignHeader(Hpart8g, part8g);
    // compare output to example
    bool greenBool = compareTGA(Hex8g, Dex8g, Hpart8g, Dpart8g, "8");
    // load blue example for test
    string ex8b= "examples/EXAMPLE_part8_b.tga";
    Header Hex8b{};
    ImageData Dex8b{};
    getInfo(ex8b, Hex8b, Dex8b);
    Header Hpart8b{};
    assignHeader(Hpart8b, part8b);
    // compare output to example
    bool blueBool = compareTGA(Hex8b, Dex8b, Hpart8b, Dpart8b, "8");
    //test is only passed if all three files match their examples
    if(redBool && greenBool && blueBool){
        cout << "Test #8 ...... Passed!" << endl;
    }

    // deallocate part 8 files
    // input files
    deallImgData(Dpart8r);
    deallImgData(Dpart8b);
    deallImgData(Dpart8g);
    deallImgData(Dcar);
    // examples
    deallImgData(Dex8r);
    deallImgData(Dex8g);
    deallImgData(Dex8b);

    // task 9
    // load red file
    string red = "input/layer_red.tga";
    Header Hred{};
    ImageData Dred{};
    getInfo(red, Hred, Dred);
    // load green file
    string green = "input/layer_green.tga";
    Header Hgreen{};
    ImageData Dgreen{};
    getInfo(green, Hgreen, Dgreen);
    // load blue file
    string blue = "input/layer_blue.tga";
    Header Hblue{};
    ImageData Dblue{};
    getInfo(blue, Hblue, Dblue);
    // assign channels to part9
    string part9 = "output/part9.tga";
    ImageData Dpart9{};
    assignChannels(Dred, Dgreen, Dblue, static_cast<int>(Hgreen.height), static_cast<int>(Hgreen.width), Dpart9);
    outHeader(Hgreen, part9);
    outData(Dpart9, part9, static_cast<int>(Hgreen.height), static_cast<int>(Hgreen.width));

    // load example for test
    string ex9= "examples/EXAMPLE_part9.tga";
    Header Hex9{};
    ImageData Dex9{};
    getInfo(ex9, Hex9, Dex9);
    Header Hpart9{};
    assignHeader(Hpart9, part9);
    // compare output to example
    compareTGA(Hex9, Dex9, Hpart9, Dpart9, "9");

    //deallocate task 9 files
    deallImgData(Dred);
    deallImgData(Dgreen);
    deallImgData(Dblue);
    deallImgData(Dpart9);
    deallImgData(Dex9);

    //task 10 rotate text2 by 180
    string text2 = "input/text2.tga";
    Header Htext2{};
    ImageData Dtext2{};
    getInfo(text2, Htext2, Dtext2);
    string part10 = "output/part10.tga";
    ImageData Dpart10{};
    rotate180(Dtext2, static_cast<int>(Htext2.height), static_cast<int>(Htext2.width), Dpart10);
    outHeader(Htext2, part10);
    outData(Dpart10, part10, static_cast<int>(Htext2.height), static_cast<int>(Htext2.width));

    // load example for test
    string ex10= "examples/EXAMPLE_part10.tga";
    Header Hex10{};
    ImageData Dex10{};
    getInfo(ex10, Hex10, Dex10);
    Header Hpart10{};
    assignHeader(Hpart10, part10);
    // compare output to example
    compareTGA(Hex10, Dex10, Hpart10, Dpart10, "10");

    // deallocate task 10
    deallImgData(Dpart10);
    deallImgData(Dtext2);
    deallImgData(Dex10);
    return 0;
}