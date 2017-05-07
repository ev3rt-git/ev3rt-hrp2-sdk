/**
 * This sample program test function for controlling two motors by ev3_motor_rotate()
 *      EV3RT sometime skip ev3_motor_rotate() command 
 *      => required tslp_tsk() after each blocking ev3_motor_rotate()
 */

#include "app.h"
#include <stdlib.h>

void main_task(intptr_t unused) {
    motor_port_t left_motor = EV3_PORT_B;
    motor_port_t right_motor = EV3_PORT_C;
    
    uint32_t speed = 50;
    int degrees_straight = 360;
    int degrees_turn = 400;

    uint32_t sleep = 60; 
    // more than 30 work good, less occasionally skip some movements
    // more than 60 seems to work well, no skip movements

    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);

    ev3_lcd_set_font(EV3_FONT_MEDIUM);

    char lcd_text[100];
    
    // example with different sleep
    for(sleep = 10; sleep < 70; ++sleep) {

        sprintf(lcd_text, "sleep %lu", sleep);
        ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
        ev3_lcd_draw_string(lcd_text, 10, 20);

        for(int i = 0; i != 4; ++i) {

            // straight
            ev3_motor_rotate( left_motor, degrees_straight, speed, false);
            ev3_motor_rotate(right_motor, degrees_straight, speed, true);

            tslp_tsk(sleep);

            // turn
            ev3_motor_rotate( left_motor, degrees_turn, speed, true);

            tslp_tsk(sleep);
        }

        tslp_tsk(2000);
    }
}
