/**
 * This is sample program for Display class in CPP EV3RT API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

using ev3cxx::display;

void main_task(intptr_t unused) {
    display.setFont(EV3_FONT_MEDIUM); // default font

    int i_num = 65;
    display.format("Display test"); // show on zero text line

    display.format(1, "I1: % \n") % i_num; // show on first text line

    display.clearTextLine(1, display.black, display.getFontWidth() * 7);

    i_num = 123456;
    display.format("I2:%7\n") % i_num;
        
    i_num = 76;
    display.format("I3: % ") % i_num; // no space after number

    float f_num = 98.1234567;
    display.format("F1: % \n") % f_num;
    
    f_num = 9876.1234567;
    display.format("F2: % \n") % f_num;

    f_num = 987654321.1234567;
    display.format("F3: % \n") % f_num;

    tslp_tsk(1000);

    double d_num = 9876543210.0123456789;
    display.format("D1: %  ") % d_num; // one space after number
    
    tslp_tsk(2000);
    
    d_num = 1239876543210.0123456789;
    display.format("D2: % ") % d_num; 
    // with EV3_FONT_MEDIUM - overwrite zero text line on screen

    // TODO: add example for change port
}
