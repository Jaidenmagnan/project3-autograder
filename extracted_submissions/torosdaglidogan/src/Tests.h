// Copyright 2023 Dogan Torosdagli

#pragma once

#include "Headers.h"
#include "Reader.h"
#include "Writer.h"
#include "Processor.h"
#include <string>
#include <iostream>
#include <iomanip>

class Tests {
    Read * car_file, * circles_file, * layer_blue_file, * layer_green_file, * layer_red_file, * layer_1_file,
     * layer_2_file, * pattern_1_file, * pattern_2_file, * text_file, * text_2_file;
    Image car, circles, layer_blue, layer_green, layer_red, layer_1, layer_2, pattern_1, pattern_2, text, text_2;
    Read * example_1_file, * example_2_file, * example_3_file, * example_4_file, * example_5_file, * example_6_file,
     * example_7_file, * example_8r_file, * example_8g_file, * example_8b_file, * example_9_file, * example_10_file,
      *ec_example_file;


 public:
    Tests() {
        // dynamic analysis with Dr. Memory with:
        // g++ -gdwarf-2 .\src\Testing.cpp -o Debug
        // replace .. with the following to make absolute paths:
        // C:/Users/dogan/source/repos/COP3504-Proj3

        // import and set up all input and example files

        car_file = new Read("input/car.tga");
        circles_file = new Read("input/circles.tga");
        layer_blue_file = new Read("input/layer_blue.tga");
        layer_green_file = new Read("input/layer_green.tga");
        layer_red_file = new Read("input/layer_red.tga");
        layer_1_file = new Read("input/layer1.tga");
        layer_2_file = new Read("input/layer2.tga");
        pattern_1_file = new Read("input/pattern1.tga");
        pattern_2_file = new Read("input/pattern2.tga");
        text_file = new Read("input/text.tga");
        text_2_file = new Read("input/text2.tga");

        car = car_file->getImage();
        circles = circles_file->getImage();
        layer_blue = layer_blue_file->getImage();
        layer_green = layer_green_file->getImage();
        layer_red = layer_red_file->getImage();
        layer_1 = layer_1_file->getImage();
        layer_2 = layer_2_file->getImage();
        pattern_1 = pattern_1_file->getImage();
        pattern_2 = pattern_2_file->getImage();
        text = text_file->getImage();
        text_2 = text_2_file->getImage();

        example_1_file = new Read("examples/EXAMPLE_part1.tga");
        example_2_file = new Read("examples/EXAMPLE_part2.tga");
        example_3_file = new Read("examples/EXAMPLE_part3.tga");
        example_4_file = new Read("examples/EXAMPLE_part4.tga");
        example_5_file = new Read("examples/EXAMPLE_part5.tga");
        example_6_file = new Read("examples/EXAMPLE_part6.tga");
        example_7_file = new Read("examples/EXAMPLE_part7.tga");
        example_8r_file = new Read("examples/EXAMPLE_part8_r.tga");
        example_8g_file = new Read("examples/EXAMPLE_part8_g.tga");
        example_8b_file = new Read("examples/EXAMPLE_part8_b.tga");
        example_9_file = new Read("examples/EXAMPLE_part9.tga");
        example_10_file = new Read("examples/EXAMPLE_part10.tga");
        ec_example_file = new Read("examples/EXAMPLE_extracredit.tga");
    }

    ~Tests() {

        // delete all example files

        delete car_file;
        delete circles_file;
        delete layer_blue_file;
        delete layer_green_file;
        delete layer_red_file;
        delete layer_1_file;
        delete layer_2_file;
        delete pattern_1_file;
        delete pattern_2_file;
        delete text_file;
        delete text_2_file;

        delete example_1_file;
        delete example_2_file;
        delete example_3_file;
        delete example_4_file;
        delete example_5_file;
        delete example_6_file;
        delete example_7_file;
        delete example_8r_file;
        delete example_8g_file;
        delete example_8b_file;
        delete example_9_file;
        delete example_10_file;
        delete ec_example_file;
    }

    // Test Framework
    // 1. Clone the required input file as to not modify it
    // 2. Use Process class to make the needed changes to it
    // 3. Write to output file
    // 4. Compare result to example file
    // 5. Delete the cloned input file
    // 6. Return Result

    bool test1() {
        Read * tmpfile = new Read(*layer_1_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).multiply(pattern_1);
        Write(tmp, "output/part1.tga");
        bool result = *tmpfile == *example_1_file;
        delete tmpfile;
        return result;
    }

    bool test2() {
        Read * tmpfile = new Read(*car_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).subtract(layer_2);
        Write(tmp, "output/part2.tga");
        bool result = *tmpfile == *example_2_file;
        delete tmpfile;
        return result;
    }

    bool test3() {
        Read * tmpfile = new Read(*layer_1_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).multiply(pattern_2).screen(text);
        Write(tmp, "output/part3.tga");
        bool result = *tmpfile == *example_3_file;
        delete tmpfile;
        return result;
    }

    bool test4() {
        Read * tmpfile = new Read(*layer_2_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).multiply(circles).subtract(pattern_2);
        Write(tmp, "output/part4.tga");
        bool result = *tmpfile == *example_4_file;
        delete tmpfile;
        return result;
    }

    bool test5() {
        Read * tmpfile = new Read(*layer_1_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).overlay(pattern_1);
        Write(tmp, "output/part5.tga");
        bool result = *tmpfile == *example_5_file;
        delete tmpfile;
        return result;
    }

    bool test6() {
        Read * tmpfile = new Read(*car_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).add(Pixel{0, 200, 0});
        Write(tmp, "output/part6.tga");
        bool result = *tmpfile == *example_6_file;
        delete tmpfile;
        return result;
    }

    bool test7() {
        Read * tmpfile = new Read(*car_file);
        Image tmp = tmpfile->getImage();
        Process(tmp).scale(4U, 1U, 0U);
        Write(tmp, "output/part7.tga");
        bool result = *tmpfile == *example_7_file;
        delete tmpfile;
        return result;
    }

    bool test8() {
        Read * tmpfile = new Read(*car_file);
        bool result = true;

        Read * tmpRfile = new Read(*tmpfile);
        Process(tmpRfile->getImage()).matrix(1U, 0U, 0U, 1U, 0U, 0U, 1U, 0U, 0U);
        Write(tmpRfile->getImage(), "output/part8r.tga");
        result &= *tmpRfile == *example_8r_file;
        delete tmpRfile;

        Read * tmpGfile = new Read(*tmpfile);
        Process(tmpGfile->getImage()).matrix(0U, 1U, 0U, 0U, 1U, 0U, 0U, 1U, 0U);
        Write(tmpGfile->getImage(), "output/part8g.tga");
        result &= *tmpGfile == *example_8g_file;
        delete tmpGfile;

        Read * tmpBfile = new Read(*tmpfile);
        Process(tmpBfile->getImage()).matrix(0U, 0U, 1U, 0U, 0U, 1U, 0U, 0U, 1U);
        Write(tmpBfile->getImage(), "output/part8b.tga");
        result &= *tmpBfile == *example_8b_file;
        delete tmpBfile;

        delete tmpfile;
        return result;
    }

    bool test9() {
        Read * tmpR = new Read(*layer_red_file);
        Read * tmpG = new Read(*layer_green_file);
        Read * tmpB = new Read(*layer_blue_file);

        Process(tmpR->getImage()).scale(1U, 0U, 0U);
        Process(tmpG->getImage()).scale(0U, 1U, 0U);
        Process(tmpB->getImage()).scale(0U, 0U, 1U);

        Process(tmpR->getImage()).add(tmpG->getImage()).add(tmpB->getImage());
        Write(tmpR->getImage(), "output/part9.tga");
        bool result = *tmpR == *example_9_file;
        delete tmpR;
        delete tmpG;
        delete tmpB;
        return result;
    }

    bool test10() {
        Read * tmp = new Read(*text_2_file);
        Process(tmp->getImage()).rot180();
        Write(tmp->getImage(), "output/part10.tga");
        bool result = *tmp == *example_10_file;
        delete tmp;
        return result;
    }

    bool testEC() {
        Read * BL = new Read(*text_file, 2, 0, 0);
        Read * BR = new Read(*pattern_1_file, 2, 1, 0);
        Read * TL = new Read(*car_file, 2, 0, 1);
        Read * TR = new Read(*circles_file, 2, 1, 1);

        Process(BL->getImage()).add(BR->getImage()).add(TL->getImage()).add(TR->getImage());
        Write(BL->getImage(), "output/extracredit.tga");
        bool result = *BL == *ec_example_file;
        delete BL;
        delete BR;
        delete TL;
        delete TR;
        return result;
    }

    void testall() {
        // array of function pointers to the test functions
        static bool (Tests::*testlist[10])() {&Tests::test1, &Tests::test2, &Tests::test3, &Tests::test4, &Tests::test5, &Tests::test6, &Tests::test7, &Tests::test8, &Tests::test9, &Tests::test10}; // NOLINT
        int score = 0;
        std::cout << "Running All Tests:" << std::endl;
        bool pass;
        for (int i = 0; i < 10; i++) {
            pass = (this->*testlist[i])();
            std::cout << "Test #" << std::setw(3) << std::left << i + 1  <<
             "-----------  " << (pass?"PASSED":"FAILED") << std::endl;
            score += pass;
        }
        pass = testEC(); score += pass;
        std::cout << "Test #EC -----------  " << (pass?"PASSED":"FAILED") << std::endl;
        std::cout << "Test results: " << score << " / 11" << std::endl;
    }
};
