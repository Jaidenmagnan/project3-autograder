// Copyright 2023 Dogan Torosdagli

#include <iostream>
#include "Tests.h"

int main() {
    Tests * tests = new Tests();
    tests->testall();
    delete tests;
}
