
#include <vector>
#include "Image.h"
#include <cmath>

using namespace std;


void Multiply(Image &a, Image &b) {
    for (unsigned int i = 0; i < a.pixels.size(); i++) {
        a.pixels[i].blue = (int) (round(((double)a.pixels[i].blue*(double)b.pixels[i].blue)/255));
        a.pixels[i].green = (int)( round(((double)a.pixels[i].green*(double)b.pixels[i].green) / 255));
        a.pixels[i].red = (int) (round(((double)a.pixels[i].red*(double)b.pixels[i].red) / 255));
    }
}
void Subtract(Image &a, Image &b) {
    for (unsigned int i = 0; i < a.pixels.size(); i++) {
        int blue = (int)a.pixels[i].blue - (int) b.pixels[i].blue;
        int green = (int)(a.pixels[i].green - (int) b.pixels[i].green);
        int red = (int)(a.pixels[i].red - (int) b.pixels[i].red);
        if (blue > 255) { blue = 255; }
        if (green >255) { green = 255; }
        if (red > 255) { red = 255; }
        if (blue < 0) { blue = 0; }
        if (green < 0) { green = 0; }
        if (red< 0) { red = 0; }
        a.pixels[i].blue = blue;
        a.pixels[i].green = green;
        a.pixels[i].red = red;
    }
}
void Subtract2(Image &a, Image &b) {
    int red;
    int blue;
    int green;
    for (unsigned int i = 0; i < a.pixels.size(); i++) {
        blue = (int)(b.pixels[i].blue - a.pixels[i].blue);
        green = (int)(b.pixels[i].green - a.pixels[i].green);
        red = (int)(b.pixels[i].red - a.pixels[i].red);
        if (blue < 0) { blue = 0; }
        if (green < 0) { green = 0; }
        if (red < 0) { red = 0; }
        a.pixels[i].blue = blue;
        a.pixels[i].green = green;
        a.pixels[i].red = red;
    }
}
void Screen(Image &a, Image &b) {
    for (unsigned int i = 0; i < a.pixels.size(); i++) {
        a.pixels[i].blue =(int) ( round(255 - (((double)(255 - a.pixels[i].blue))*((double)(255 - b.pixels[i].blue)) / 255)));
        a.pixels[i].green = (int) (round(255 - (((double)(255 - a.pixels[i].green))*((double)(255 - b.pixels[i].green)) / 255)));
        a.pixels[i].red = (int) (round(255 - (((double)(255 - a.pixels[i].red))*((double)(255 - b.pixels[i].red)) / 255)));
    }
}
void Overlay(Image &a, Image &b) {
    int red, green, blue;
    for (unsigned int i = 0; i < a.pixels.size(); i++) {
        Image::Pixel p;
        if (((a.pixels[i].red) / 255.0)>0.5) {
            p.red = (int) (((1 - (2 * (1 - a.pixels[i].red / 255.0)*(1 - b.pixels[i].red / 255.0)))*255.0) + 0.5f);
            red = p.red;
        }
        else {
            p.red = (int) (((2 * (a.pixels[i].red / 255.0)*(b.pixels[i].red / 255.0))*255.0) + 0.5f);
            red = p.red;
        }
        if (((a.pixels[i].green) / 255.0)>0.5) {
            p.green = (int) (((1 - (2 * (1 - a.pixels[i].green / 255.0)*(1 - b.pixels[i].green / 255.0)))*255.0) + 0.5f);
            green = p.green;
        }
        else {
            p.green = (int) (((2 * (a.pixels[i].green / 255.0)*(b.pixels[i].green / 255.0))*255.0) + 0.5f);
            green = p.green;
        }
        if (((a.pixels[i].blue) / 255.0)>0.5) {
            p.blue = (int) (((1 - (2 * (1 - a.pixels[i].blue / 255.0)*(1 - b.pixels[i].blue / 255.0)))*255.0) + 0.5f);
            blue = p.blue;
        }
        else {
            p.blue = (int) (((2 * (a.pixels[i].blue / 255.0)*(b.pixels[i].blue / 255.0))*255.0) + 0.5f);
            blue = p.blue;
        }
        a.pixels[i] = p;
    }
}
void Combine3(Image &red, Image &green, Image &blue) {
    for (unsigned int i = 0; i < red.pixels.size(); i++) {
        red.pixels[i].green = green.pixels[i].green;
        red.pixels[i].blue = blue.pixels[i].blue;
    }
}
void Rotate180(Image &a, Image &b) {
    int num = 0;
    for (unsigned int i = a.pixels.size() - 1; i > 0; i--) {
        b.pixels[i] = a.pixels[num];
        num++;
    }
}
void CollateHorizontal(Image &a, Image &b) {
    int totalWidth = a.h.width + b.h.width;
    int height = a.h.height;
    int width = a.h.width;
    bool activateleft = true;
    vector<Image::Pixel> pix;
    for (unsigned int i = 0; i < height; i++) {
        int current = 0;
        while (activateleft) {
            if (current == width) {
                activateleft = false;
                break;
            }
            pix.push_back(a.pixels[i*height+current]);
            current++;
        }
        current = 0;
        while (!activateleft) {
            if (current == width) {
                activateleft = true;
                break;
            }
            pix.push_back(b.pixels[i*height+current]);
            current++;
        }
    }
    a.SetSize(height, totalWidth);
    a.pixels = pix;
}
void CollateVertical(Image &a, Image &b) {
    int totalHeight = a.h.height + b.h.height;
    int height = a.h.height;
    int width = a.h.width;
    a.SetSize(totalHeight, width);
    vector<Image::Pixel> pix;
    for (unsigned int i = 0; i < b.pixels.size(); i++) {
        pix.push_back(b.pixels[i]);
    }
    for (unsigned int i = 0; i < a.pixels.size(); i++) {
        pix.push_back(a.pixels[i]);
    }
    a.pixels = pix;
}

int main()
{
    Image car = Image("input/car"); //create all input images
    Image circles = Image("input/circles");
    Image layer_blue = Image("input/layer_blue");
    Image layer_green = Image("input/layer_green");
    Image layer_red = Image("input/layer_red");
    Image layer1 = Image("input/layer1");
    Image layer2 = Image("input/layer2");
    Image pattern1 = Image("input/pattern1");
    Image pattern2 = Image("input/pattern2");
    Image text = Image("input/text");
    Image text2 = Image("input/text2");
    Image part1 = Image("examples/EXAMPLE_part1"); //create all example images
    Image part2 = Image("examples/EXAMPLE_part2");
    Image part3 = Image("examples/EXAMPLE_part3");
    Image part4 = Image("examples/EXAMPLE_part4");
    Image part5 = Image("examples/EXAMPLE_part5");
    Image part6 = Image("examples/EXAMPLE_part6");
    Image part7 = Image("examples/EXAMPLE_part7");
    Image part8_b = Image("examples/EXAMPLE_part8_b");
    Image part8_g = Image("examples/EXAMPLE_part8_g");
    Image part8_r = Image("examples/EXAMPLE_part8_r");
    Image part9 = Image("examples/EXAMPLE_part9");
    Image part10 = Image("examples/EXAMPLE_part10");
    Image partec = Image("examples/EXAMPLE_extracredit");
    //testing
    cout << endl << endl << endl << "----TESTING----" << endl;

    Multiply(layer1, pattern1);
    layer1.WriteFile("part1.tga");

    cout << endl << "1: ";
    if (layer1.Equals(part1)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }

    layer1.ReloadImage();

    Subtract2(layer2, car);
    layer2.WriteFile("part2.tga");

    cout << endl << "2: ";
    if (layer2.Equals(part2)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }

    layer2.ReloadImage();

    Multiply(layer1, pattern2);
    Screen(text, layer1);

    text.WriteFile("part3.tga");

    cout << endl << "3: ";
    if (text.Equals(part3)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }
    layer1.ReloadImage();
    text.ReloadImage();
    Multiply(layer2, circles);
    Subtract(layer2, pattern2);
    layer2.WriteFile("part4.tga");
    cout << endl << "4: ";
    if (layer2.Equals(part4)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }

    layer2.ReloadImage();

    Overlay(pattern1, layer1);
    pattern1.WriteFile("part5.tga");


    cout << endl << "5: ";
    if (pattern1.Equals(part5)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }

    pattern1.ReloadImage();

    car.AddToColor("green", 200);
    car.WriteFile("part6.tga");

    cout << endl << "6: ";
    if (car.Equals(part6)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }
    car.ReloadImage();
    car.Scale("red", 4);
    car.Scale("blue", 0);

    car.WriteFile("part7.tga");

    cout << endl << "7: ";
    if (car.Equals(part7)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }


    car.ReloadImage();
    car.ToColor("blue");
    car.WriteFile("part8_b.tga");

    cout << endl << "8(blue): ";
    if (car.Equals(part8_b)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }


    car.ReloadImage();

    car.ToColor("red");
    car.WriteFile("part8_r.tga");

    cout << endl << "8(red): ";
    if (car.Equals(part8_r)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }

    car.ReloadImage();

    car.ToColor("green");
    car.WriteFile("part8_g.tga");

    cout << endl << "8(green): ";
    if (car.Equals(part8_g)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }


    car.ReloadImage();
    Combine3(layer_red, layer_green, layer_blue);
    layer_red.WriteFile("part9.tga");

    cout << endl << "9: ";
    if (layer_red.Equals(part9)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }


    layer_red.ReloadImage();
    Image temp = text2;
    Rotate180(text2, temp);
    temp.WriteFile("part10.tga");

    cout << endl << "10: ";
    if (temp.Equals(part10)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }


    text2.ReloadImage();
    CollateHorizontal(car, circles);
    CollateHorizontal(text, pattern1);

    CollateVertical(car, text);

    car.WriteFile("extracredit.tga");

    cout << endl << "extra: ";
    if (car.Equals(partec)) {

        cout << "passed";

    }
    else {
        cout << "failed";
    }

    text.ReloadImage();
    car.ReloadImage();
    cout << endl;

    return 0;
}