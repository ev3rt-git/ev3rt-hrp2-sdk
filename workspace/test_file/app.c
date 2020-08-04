/**
 * This sample program shows a PID controller for line following.
 *
 * Robot conlcdstruction: Educator Vehicle
 *
 * References:
 * http://robotsquare.com/wp-content/uploads/2013/10/45544_educator.pdf
 * http://thetechnicgear.com/2014/03/howto-create-line-following-robot-using-mindstorms/
 */
#include <stdlib.h>
#include <stdio.h>
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
position pos = {-1, -1, -1, 0, 0};

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);

    }
}

static void exit_program_with_exception() {
    switch(button) {
    case BACK_BUTTON:
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(1);

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

    ht_nxt_color_sensor_measure_rgb(color_sensor4, rgb4);

    int bit1 = -1;
    int bit2 = -1;

    //decode instructions
    ev3_motor_steer(left_motor, right_motor, 20, 0);
    while (rgb4->r < 127) {}
    int index = 0;
    for (index < 4) {
        //read instruction
        ev3_motor_reset_counts(EV3_PORT_B);
        ev3_motor_reset_counts(EV3_PORT_C);
        while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
        if (((rgb4->r + rgb4->g + rgb4->b) / 3) > 127) {
            bit1 = 1;
        } else {
            bit1 = 0;
        }
        ev3_motor_reset_counts(EV3_PORT_B);
        ev3_motor_reset_counts(EV3_PORT_C);
        while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
        if (((rgb4->r + rgb4->g + rgb4->b) / 3) > 127) {
            bit2 = 1;
        } else {
            bit2 = 0;
        }
        // decode instruction
        if (bit1 == 1) {
            if (bit2 == 1) {
                exit_program_with_exception();
            } else {
                tasks[index] = BLACKMATERIAL;
            }
        } else {
            if (bit2 == 1) {
                tasks[index] = BLUEMATERIAL;
            } else {
                tasks[index] = REMOVESNOW;
            }
        }
    }
    // read instructions for blue road
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, 20, 0);
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit1 = 1;
    } else {
        bit1 = 0;
    }
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit2 = 1;
    } else {
        bit2 = 0;
    }
    //determine task 0 (blue)
    if (bit1) {
        if (bit2) {
            exit(1);
        } else {
            tasks[BLUE_STREET] = BLACKMATERIAL;
        }
    } else {
        if (bit2) {
            tasks[BLUE_STREET] = BLUEMATERIAL;
        } else {
            tasks[BLUE_STREET] = REMOVESNOW;
        }
    }
    
 /*   // read instructions for green road
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit1 = 1;
    } else {
        bit1 = 0;
    }
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit2 = 1;
    } else {
        bit2 = 0;
    }
    //determine task 1 (green)
    if (bit1) {
        if (bit2) {
            exit(1);
        } else {
            tasks[GREEN_STREET] = BLACKMATERIAL;
        }
    } else {
        if (bit2) {
            tasks[GREEN_STREET] = BLUEMATERIAL;
        } else {
            tasks[GREEN_STREET] = REMOVESNOW;
        }
    }
    
    // read inlcdstructions for yellow road
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit1 = 1;
    } else {
        bit1 = 0;
    }
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit2 = 1;
    } else {
        bit2 = 0;
    }
    //determine task 2 (yellow)
    if (bit1) {
        if (bit2) {
            exit(1);
        } else {
            tasks[YELLOW_STREET] = BLACKMATERIAL;
        }
    } else {
        if (bit2) {
            tasks[YELLOW_STREET] = BLUEMATERIAL;
        } else {
            tasks[YELLOW_STREET] = REMOVESNOW;
        }
    }
    
    // read inlcdstructions for red road
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit1 = 1;
    } else {
        bit1 = 0;
    }
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 20) {}
    if ((rgb4->r + rgb4->g + rgb4->b) / 3 > 127) {
        bit2 = 1;
    } else {
        bit2 = 0;
    }
    //determine task 3 (red)
    if (bit1) {
        if (bit2) {
            exit(1);
        } else {
            tasks[RED_STREET] = BLACKMATERIAL;
        }
    } else {
        if (bit2) {
            tasks[RED_STREET] = BLUEMATERIAL;
        } else {
            tasks[RED_STREET] = REMOVESNOW;
        }
    }

 */
   //display things in a very small font
    char lcdstr = char[100];
    sprintf(lcdstr, "%d, %d, %d, %d", tasks[BLUE_STREET], tasks[RED_STREET], tasks[GREEN_STREET], tasks[YELLOW_STREET]);
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, COLOR_WHITE);
    ev3_lcd_draw_lcdstring(lcdstr, 0, 0);
    while (1>0) {}
}