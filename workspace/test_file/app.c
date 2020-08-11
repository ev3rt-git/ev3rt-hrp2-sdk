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

const int left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, color_sensor4=EV3_PORT_4, color_sensor2 = EV3_PORT_2, color_sensor3 = EV3_PORT_3;

rgb_raw_t rgb4;
position pos = {-1, -1, -1, 0, 0};

int ready = true;

void main_task(intptr_t unused) {

    init();

    //run program
    readCode();
    tslp_tok(50000000);
}

void readCode() {
    //define variables
    int bit1 = -1;
    int bit2 = -1;

    //leave start
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, 30, 1);
    while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < 250) {
        display_values();
    }

    //record instructions
    ev3_motor_steer(left_motor, right_motor, 10, 1);
    while (rgb4.g > 30 && rgb4.b > 25) {
        display_values();
    }
    ev3_motor_stop(left_motor, true);
    ev3_motor_stop(right_motor, true);
    if (rgb4.g < 30) {
        pos.street = RED_STREET;
    } else {
        pos.street = YELLOW_STREET;
    }
    tslp_tok(5);
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, -10, 0);
    while (((abs(ev3_motor_get_counts(EV3_PORT_B)) + abs(ev3_motor_get_counts(EV3_PORT_C))) / 2) < 20) {
        display_values();
    }
    ev3_motor_stop(left_motor, true);
    ev3_motor_stop(right_motor, true);
    tslp_tok(5);

    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, 10, 1);

    int index;
    for (index = 0; index < 4; index++) {

        //read instructions
        while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < ((index+1) * 55)) {
            display_values();
        }
        if (((rgb4.r + rgb4.g + rgb4.b) / 3) > 25) {
            bit1 = 1;
        } else {
            bit1 = 0;
        }
        while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < ((index + 2) * 55)) {
            display_values();
        }
        if (((rgb4.r + rgb4.g + rgb4.b) / 3) > 25) {
            bit2 = 1;
        } else {
            bit2 = 0;
        }

        // decode instructions
        if (bit1 == 1) {
            if (bit2 == 1) {
                exit(1);
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

    //detect line
    ev3_motor_steer(left_motor, right_motor, 10, 1);
    while (ev3_color_sensor_get_reflect(color_sensor2) > 20 && ev3_color_sensor_get_reflect(color_sensor3) > 20) {
        display_values();
    }
    ev3_motor_stop(left_motor, true);
    ev3_motor_stop(right_motor, true);
    tslp_tok(5);

    //align robot
    ev3_motor_rotate(left_motor, -100, 15, true);
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_steer(left_motor, right_motor, 10, 0);
    while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < 140) {
        display_values();
    }
    ev3_motor_rotate(right_motor, -90, 15, true);
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_steer(left_motor, right_motor, -10, 0);
    while (((abs(ev3_motor_get_counts(EV3_PORT_B)) + abs(ev3_motor_get_counts(EV3_PORT_C))) / 2) < 10) {
        display_values();
    }
    ev3_motor_stop(left_motor, true);
    ev3_motor_stop(right_motor, true);

    //display things in a very medium font
    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE);
    char lcdstr[100];
    sprintf(lcdstr, "%d, %d", tasks[BLUE_STREET], tasks[GREEN_STREET]);
    ev3_lcd_draw_string(lcdstr, 0, 0);
    sprintf(lcdstr, "%d, %d", tasks[YELLOW_STREET], tasks[RED_STREET]);
    ev3_lcd_draw_string(lcdstr, 0, 15);

    //record position
    pos.section = 1;
    pos.distance = 51;
    pos.dash = 0;
    pos.facing = 0;
}

void init() {
    // Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);
    
    // Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    //ev3_motor_config(a_motor, MEDIUM_MOTOR);
    
    // Configure sensors
    ev3_sensor_config(color_sensor2, COLOR_SENSOR);
    ev3_sensor_config(color_sensor3, COLOR_SENSOR);
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR);
    
    // Set up sensors
    ev3_color_sensor_get_reflect(color_sensor2);
    ev3_color_sensor_get_reflect(color_sensor3);
    //bool_t val1 = ht_nxt_color_sensor_measure_rgb(color_sensor1, &rgb1);
    //assert(val1);
    bool_t val4 = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb4);
    assert(val4);

    // Configure brick
    ev3_lcd_set_font(EV3_FONT_MEDIUM);

    //reset snow/car collector
    //ev3_motor_set_power(a_motor, -100);
    //ev3_motor_rotate(a_motor, 300, 50);

    //wait for button press
    ev3_lcd_draw_string("Press OK to run", 14, 45);
    while (1) {
        if (ev3_button_is_pressed(ENTER_BUTTON)) {
            while (ev3_button_is_pressed(ENTER_BUTTON));
            break;
        }
    }
    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE);
}

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

void display_values() {
    //declare variables
    char msg[100];
    int value;

    //wait for values to be refreshed
    tslp_tok(3);

    //read motor counts
    value = ev3_motor_get_counts(left_motor);
    sprintf(msg, "L: %d   ", value);
    ev3_lcd_draw_string(msg, 10*0, 15*0);
    value = ev3_motor_get_counts(right_motor);
    sprintf(msg, "R: %d   ", value);
    ev3_lcd_draw_string(msg, 10*8, 15*0);

    //read sensor rgb1
/*
    bool_t val1 = ht_nxt_color_sensor_measure_rgb(color_sensor1, &rgb1);
    assert(val1);
    sprintf(msg, "RGB1:");
    ev3_lcd_draw_string(msg, 10*0, 15*1.5);
    sprintf(msg, "R: %d", rgb1.r);
    ev3_lcd_draw_string(msg, 10*0, /15*2.5);
    sprintf(msg, "G: %d", rgb1.g);
    ev3_lcd_draw_string(msg, 10*7, 15*2.5);
    sprintf(msg, "B: %d", rgb1.b);
    ev3_lcd_draw_string(msg, 10*14, 15*2.5);
*/

    //read sensor rgb4
    bool_t val4 = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb4);
    assert(val4);
    sprintf(msg, "RGB4:");
    ev3_lcd_draw_string(msg, 10*0, 15*4);
    sprintf(msg, "R: %d  ", rgb4.r);
    ev3_lcd_draw_string(msg, 10*0, 15*5);
    sprintf(msg, "G: %d  ", rgb4.g);
    ev3_lcd_draw_string(msg, 10*7, 15*5);
    sprintf(msg, "B: %d  ", rgb4.b);
    ev3_lcd_draw_string(msg, 10*14, 15*5);

    //read linefollow sensors
    sprintf(msg, "Light2 & Light3:");
    ev3_lcd_draw_string(msg, 10*0, 15*6.5);
    value = ev3_color_sensor_get_reflect(color_sensor2);
    sprintf(msg, "L: %d  ", value);
    ev3_lcd_draw_string(msg, 10*0, 15*7.5);
    value = ev3_color_sensor_get_reflect(color_sensor3);
    sprintf(msg, "L: %d  ", value);
    ev3_lcd_draw_string(msg, 10*7, 15*7.5);
}

void tslp_tok(int time){
    char oof_how_did_you_find_this[100];
    sprintf(oof_how_did_you_find_this,"The time is %d",time);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 1);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 2);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 3);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 4);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 5);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 6);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 7);
    ev3_lcd_draw_string(oof_how_did_you_find_this, 0, 15 * 8);
}