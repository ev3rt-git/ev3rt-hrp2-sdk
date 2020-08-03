/**
 * This sample program shows a PID controller for line following.
 *
 * Robot construction: Educator Vehicle
 *
 * References:
 * http://robotsquare.com/wp-content/uploads/2013/10/45544_educator.pdf
 * http://thetechnicgear.com/2014/03/howto-create-line-following-robot-using-mindstorms/
 */

#include "ev3api.h"
#include "app.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/**
 * Define the connection ports of the sensors and motors.
 * By default, this application uses the following ports:
 * Touch sensor: Port 2
 * Color sensor: Port 3
 * Left motor:   Port B
 * Right motor:  Port C
 */
//const int touch_sensor = EV3_PORT_2, color_sensor = EV3_PORT_3, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C;
const int left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, color_sensor4=EV3_PORT_4;
rgb_raw_t *rgb4;

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);

    }
}

void main_task(intptr_t unused) {
    // Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    // Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);

    // Configure sensors
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR);

    bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4, rgb4);

    char tasks [4];
    bool block1 = false;

    // read instructions for blue road
    ev3_motor_reset_counts();
    ev3_motor_steer(left_motor, right_motor, 20, 0);
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = false;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = false;
    }
    //determine task 0 (blue)
    if (block1) {
        if (block2) {
            exit(1);
            
        } else {
            tasks[0] = "black_AM";
        }
    } else {
        if (block2) {
            tasks[0] = "blue_AM";
        } else {
            tasks[0] = "snow_RM";
        }
    }
    
    // read instructions for green road
    ev3_motor_reset_counts();
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = false;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = false;
    }
    //determine task 1 (green)
    if (block1) {
        if (block2) {
            exit(1);
        } else {
            tasks[1] = "black_AM";
        }
    } else {
        if (block2) {
            tasks[1] = "blue_AM";
        } else {
            tasks[1] = "snow_RM";
        }
    }
    
    // read instructions for yellow road
    ev3_motor_reset_counts();
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = false;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = false;
    }
    //determine task 2 (yellow)
    if (block1) {
        if (block2) {
            exit(1);
            break;
        } else {
            tasks[2] = "black_AM";
        }
    } else {
        if (block2) {
            tasks[2] = "blue_AM";
        } else {
            tasks[2] = "snow_RM";
        }
    }
    
    // read instructions for red road
    ev3_motor_reset_counts();
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = false;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    if ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = false;
    }
    //determine task 3 (red)
    if (block1) {
        if (block2) {
            exit(1);
        } else {
            tasks[3] = "black_AM";
        }
    } else {
        if (block2) {
            tasks[3] = "blue_AM";
        } else {
            tasks[3] = "snow_RM";
        }
    }

    //display things in a very small font
    char val1 = tasks[0];
    char val2 = tasks[1];
    char val3 = tasks[2];
    char val4 = tasks[3];
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_lcd_draw_string(val1, 5, 0);
    ev3_lcd_draw_string(val2, 5, 20);
    ev3_lcd_draw_string(val3, 5, 40);
    ev3_lcd_draw_string(val4, 5, 60);
    while (1>0) {}
}