/**
 * This sample program for CPP EV3RT API.
 */

#include <cstdlib>

#include "ev3api.h"
#include "ev3cxx.h"
#include "app.h"

using ev3cxx::display;

void main_task(intptr_t unused) {
    // // EV3API lib - C solution
    // ev3_motor_config(EV3_PORT_B, LARGE_MOTOR);
    // ev3_motor_set_power(EV3_PORT_B, 50);

    // // Set LCD font and get width and height of font
    // char lcdText[100];
    // ev3_lcd_set_font(EV3_FONT_MEDIUM);
    // int32_t LCD_FONT_HEIGHT, LCD_FONT_WIDTH;
    // ev3_font_get_size(EV3_FONT_MEDIUM, &LCD_FONT_WIDTH, &LCD_FONT_HEIGHT);

    // bt *bt = ev3_serial_open_bt(EV3_SERIAL_BT);   
    // ev3_lcd_draw_string(lcdText, 0, LCD_FONT_HEIGHT * 0);
    // sprintf(lcdText, "isOpen(): %i", (bt == NULL) ? 'Y' : 'N');
    // ev3_lcd_draw_string(lcdText, 0, LCD_FONT_HEIGHT * 1);
    // sprintf(lcdText, "isConnected(): %i", ev3_bluetooth_is_connected() ? 'Y' : 'N');
    // ev3_lcd_draw_string(lcdText, 0, LCD_FONT_HEIGHT * 2);
    
    // fputc('a', bt);
    // ev3_lcd_draw_string("fputc('a', bt)", 0, LCD_FONT_HEIGHT * 3);
    
    // ev3cxx lib - C++ solution
    //bt bt("test_bt.txt");
    ev3cxx::Bluetooth bt{}; // same workflow as with bt
    bt.open();

    display.setTextLine(4);
    display.format("Test bt\n");    
    display.format("isOpen(): % \n") % (bt.isOpen() ? 'Y' : 'N');
    display.format("isConnected(): % \n") % (bt.isConnected() ? 'Y' : 'N');

    if(!bt.isOpen())
        std::exit(1); // if is not bluetooth open => exit() => end program

    display.format("write('a')\n");
    bt.write('a');

    display.format("format(bt,...)\n");

    format(bt, "Test bt\n");
    format(bt, "\ntest write to bt \n\tdata: %  % \n") % 125 % 98.76;

    bt.close();
    display.format("close(): % \n") % bt.isOpen();

    tslp_tsk(2000);
    display.resetScreen();


    // Open new bt
    bt.open("new_bt.txt");
    display.format("%  - new_bt\n") % "bt";
    display.format("isOpen(): % \n") % bt.isOpen();
    
    if(!bt.isOpen())
        std::exit(1); // if is not open, exit()
    
    //format(bt, "text %  %  % ") % 12 % 98.876 % 55; 
    format(bt,    "text 125 56 \n 98.76 66,55\n");
    display.format( "text 123 56 \n 98.76 66,55\n");
    
    bt.rewind(); // set position indicator to start => read from start

    char text[5] = {'\0'};
    text[0] = bt.readChar();
    text[1] = bt.readChar();
    text[2] = bt.readChar();
    text[3] = bt.readChar();
    display.format("text: % \n") % text;

    uint8_t ui8 = 0; 
    int er = bt.readNumber(ui8);
    display.format("rN(ui8)=% : % \n") % er % ui8;

    float f = -1; 
    er = bt.readNumber(f);
    display.format("rN(f)=% : % \n") % er % f;

    double d = -1; 
    er = bt.readNumber(d);
    display.format("rN(f)=% : % \n") % er % d;

    f = -1; 
    er = bt.readNumber(f);
    display.format("rN(f)=% : % \n") % er % f;

    tslp_tsk(2000);

    int i = -1; 
    er = bt.readNumber(i);
    display.format("rN(i)=% : % \n") % er % i;

    tslp_tsk(2000);

    i = -1; 
    er = bt.readNumber(i);
    display.format("rN(i)=% : % \n") % er % i;
}
