/**
 * This is sample program for C++ STL (string, vector) on EV3RT.
 * It is necessary to set the maximum APP size in EV3RT higher because with C++ STL are APP file bigger then 1 MB (standard max size in EV3RT):
 *      In 'target/ev3/ev3.hs' multiply constants TMAX_APP_BINARY_SIZE, TMAX_APP_TEXT_SIZE, TMAX_APP_DATA_SIZE by 8.
 *      Rebuild uImage in 'base-workspace' by command 'make app=loader' and copy new uImage to SD card.
 *      After that you can upload APP with max size 8 MB.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

#include <iostream>
#include <string>
#include <vector>

using ev3cxx::File;
using ev3cxx::display;
using ev3cxx::format;

void main_task(intptr_t unused) {
    display.setFont(EV3_FONT_SMALL);

    std::string test_string = "Hello";
    test_string += " world";

    display.format("Test String\n");
    display.format("% \n") % test_string.c_str();

    std::vector<int> v = {7, 5, 16, 8};
    v.push_back(25);
    v.push_back(13);

   for(int num: v) {
        display.format("% \n") % num;
    }

}
