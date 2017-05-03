/**
 * This is sample program for Bluetooth class in CPP EV3RT API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

using ev3cxx::File;
using ev3cxx::display;
using ev3cxx::format;
    
void main_task(intptr_t unused) {
    //File file("test_file.txt");
    ev3cxx::Bluetooth file; // same workflow as with file
    file.open();

    display.format("Test File\n");
    display.format("isOpen(): % \n") % file.isOpen();

    if(!file.isOpen())
        std::exit(1); // if is not open, exit()

    display.format("write('a')\n");
    file.write('a');

    display.format("format(file,...)\n");

    format(file, "Test File\n");
    format(file, "\ntest write to file \n\tdata: %  % \n") % 125 % 98.76;

    file.close();
    display.format("close(): % \n") % file.isOpen();

    tslp_tsk(2000);
    display.resetScreen();


    // Open new file
    file.open("new_file.txt");
    display.format("%  - new_file\n") % "File";
    display.format("isOpen(): % \n") % file.isOpen();
    
    if(!file.isOpen())
        std::exit(1); // if is not open, exit()
    
    //format(file, "text %  %  % ") % 12 % 98.876 % 55; 
    format(file,    "text 125 56 \n 98.76 66,55\n");
    display.format( "text 123 56 \n 98.76 66,55\n");
    
    file.rewind(); // set position indicator to start => read from start

    char text[5] = {'\0'};
    text[0] = file.readChar();
    text[1] = file.readChar();
    text[2] = file.readChar();
    text[3] = file.readChar();
    display.format("text: % \n") % text;

    uint8_t ui8 = 0; 
    int er = file.readNumber(ui8);
    display.format("rN(ui8)=% : % \n") % er % ui8;

    float f = -1; 
    er = file.readNumber(f);
    display.format("rN(f)=% : % \n") % er % f;

    double d = -1; 
    er = file.readNumber(d);
    display.format("rN(f)=% : % \n") % er % d;

    f = -1; 
    er = file.readNumber(f);
    display.format("rN(f)=% : % \n") % er % f;

    tslp_tsk(2000);

    int i = -1; 
    er = file.readNumber(i);
    display.format("rN(i)=% : % \n") % er % i;

    tslp_tsk(2000);

    i = -1; 
    er = file.readNumber(i);
    display.format("rN(i)=% : % \n") % er % i;
}
