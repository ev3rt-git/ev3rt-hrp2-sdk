/**
 * This sample program for CPP EV3RT API.
 */

#include "ev3api.h"
#include "ev3cxx.h"
#include "app.h"

float multi(float a, float b){
    return a*b; 
}

char lcd_text[100];
int32_t LCD_FONT_WIDTH, LCD_FONT_HEIGHT;

void lcd_debug_output(int cnt, int &lcd_row, ev3cxx::Motor &motor) {
    sprintf(lcd_text, "Degrees: %3.3f", motor.rotations());
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);

    sprintf(lcd_text, "Power: %i", motor.currentPower());
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);

    sprintf(lcd_text, "CNT: %f", multi(float(cnt), 1.25));
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);
}

void main_task(intptr_t unused) {
    // Set LCD font and get width and height of font
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_font_get_size(EV3_FONT_MEDIUM, &LCD_FONT_WIDTH, &LCD_FONT_HEIGHT);
    int char_size_width = EV3_LCD_WIDTH/LCD_FONT_WIDTH;
    int lcd_row = 0;

    // ev3_motor_config(EV3_PORT_B, LARGE_MOTOR);
    // ev3_motor_set_power(EV3_PORT_B, 50);

    // Title on LCD
    sprintf(lcd_text, "EV3CXX test");
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);
    
    ev3cxx::Motor left_motor(EV3_PORT_B, LARGE_MOTOR);

    // sprintf(lcd_text, "Set_port: %i", left_motor.getPort());
    // ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);

    sprintf(lcd_text, "Set_type: %i", left_motor.getType());
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);
    
    sprintf(lcd_text, "Const_er: %i", left_motor.getError());
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);
    
    left_motor.on(50);

    sprintf(lcd_text, "ON_er: %i", left_motor.getError());
    ev3_lcd_draw_string(lcd_text, 0, LCD_FONT_HEIGHT * lcd_row++);

    ev3_lcd_draw_string("on(50)      ", 0, LCD_FONT_HEIGHT * 7);
    int cnt = 0;  
    while(cnt < 500) {
        lcd_debug_output(cnt, lcd_row, left_motor);
        lcd_row -= 3;
        cnt++;
        tslp_tsk(10);
    }

    ev3_lcd_draw_string("oForD(30,-2000)    ", 0, LCD_FONT_HEIGHT *76);
    left_motor.onForDegrees(30, -2000);
    cnt = 0;  
    while(cnt < 500) {
        lcd_debug_output(cnt, lcd_row, left_motor);
        lcd_row -= 3;
        cnt++;
        tslp_tsk(10);
    }

    ev3_lcd_draw_string("oForD(70,1000,true)", 0, LCD_FONT_HEIGHT * 7);
    left_motor.onForDegrees(70, 1000, true);
    cnt = 0;  
    while(cnt < 500) {
        lcd_debug_output(cnt, lcd_row, left_motor);
        lcd_row -= 3;
        cnt++;
        tslp_tsk(10);
    }

    ev3_lcd_draw_string("oForRot(50,-10)    ", 0, LCD_FONT_HEIGHT * 7);
    //left_motor.onForRotations(50, -10);
    //left_motor.onForRotations(50, 10.5, false);
    cnt = 0;  
    while(cnt < 500) {
        lcd_debug_output(cnt, lcd_row, left_motor);
        lcd_row -= 3;
        cnt++;
        tslp_tsk(10);
    }
    ev3_lcd_draw_string("END               ", 0, LCD_FONT_HEIGHT * 7);
}
