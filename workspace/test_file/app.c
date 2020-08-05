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

const int left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, color_sensor4=EV3_PORT_4;
rgb_raw_t rgb4;
position pos = {-1, -1, -1, 0, 0};

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);
        break;
    }
}

/*
static void exit_program_with_exception() {
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    exit(1);
}
*/

void main_task(intptr_t unused) {
    // Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    // Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);

    // Configure sensors
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR);

    bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb4);

    ev3_motor_steer(left_motor, right_motor, 30, 0);

    int bit1 = -1;
    int bit2 = -1;

    //leave start
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 300) {}
    ev3_motor_steer(left_motor, right_motor, 0, 0);

    //decode instructions
    ev3_motor_steer(left_motor, right_motor, 10, 0);
    while (rgb->g>30 && rgb->b>25) {}
    ev3_motor_steer(left_motor, right_motor, 0, 0);

    int index;
    for (index = 0; index < 4; index++) {
        //read instruction
        ev3_motor_steer(left_motor, right_motor, 10, 0);
        ev3_motor_reset_counts(EV3_PORT_B);
        ev3_motor_reset_counts(EV3_PORT_C);
        while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 60) {}
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        if (((rgb4->r + rgb4->g + rgb4->b) / 3) > 25) {
            bit1 = 1;
        } else {
            bit1 = 0;
        }
        ev3_motor_steer(left_motor, right_motor, 10, 0);
        ev3_motor_reset_counts(EV3_PORT_B);
        ev3_motor_reset_counts(EV3_PORT_C);
        while (((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2) < 60) {}
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        if (((rgb4->r + rgb4->g + rgb4->b) / 3) > 25) {
            bit2 = 1;
        } else {
            bit2 = 0;
        }

        // decode instruction
        if (bit1 == 1) {
            tasks[index] = BLACKMATERIAL;
        } else {
            if (bit2 == 1) {
                tasks[index] = BLUEMATERIAL;
            } else {
                tasks[index] = REMOVESNOW;
            }
        }
    }
    //display things in a very small font
    char lcdstr[100];
    sprintf(lcdstr, "%d, %d, %d, %d", tasks[BLUE_STREET], tasks[RED_STREET], tasks[GREEN_STREET], tasks[YELLOW_STREET]);
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, COLOR_WHITE);
    ev3_lcd_draw_string(lcdstr, 0, 0);
    tslp_tsk(5000);
}
