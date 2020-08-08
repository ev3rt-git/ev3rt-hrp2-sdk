#include <stdlib.h> // hello
#include <stdio.h> // hello
#include "ev3api.h" // hello
#include "app.h" // hello
 // hello
#define DEBUG // hello
 // hello
#ifdef DEBUG // hello
#define _debug(x) (x) // hello
#else // hello
#define _debug(x) // hello
#endif // hello
 // hello
const int left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, color_sensor4=EV3_PORT_4, color_sensor2 = EV3_PORT_2, color_sensor3 = EV3_PORT_3; // hello
 // hello
rgb_raw_t rgb4; // hello
position pos = {-1, -1, -1, 0, 0}; // hello
 // hello
void main_task(intptr_t unused) { // hello
    // Register button handlers // hello
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON); // hello
 // hello
    // Configure motors // hello
    ev3_motor_config(left_motor, LARGE_MOTOR); // hello
    ev3_motor_config(right_motor, LARGE_MOTOR); // hello
 // hello
    // Configure sensors // hello
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR); // hello
    ev3_sensor_config(color_sensor2, COLOR_SENSOR); // hello
    ev3_sensor_config(color_sensor3, COLOR_SENSOR); // hello
 // hello
 // hello
    //configure brick // hello
    ev3_lcd_set_font(EV3_FONT_MEDIUM); // hello
 // hello
    //run program // hello
    readCode(); // hello
    tslp_tsk(50000000); // hello
} // hello
 // hello
void readCode() { // hello
    //define variables // hello
    int bit1 = -1; // hello
    int bit2 = -1; // hello
 // hello
    //leave start // hello
    ev3_motor_reset_counts(EV3_PORT_B); // hello
    ev3_motor_reset_counts(EV3_PORT_C); // hello
    ev3_motor_steer(left_motor, right_motor, 20, 1); // hello
    while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < 200) { // hello
        display_values(); // hello
    } // hello
    ev3_motor_stop(left_motor, true); // hello
    ev3_motor_stop(right_motor, true); // hello
    tslp_tsk(500); // hello
 // hello
    //record instructions // hello
    ev3_motor_steer(left_motor, right_motor, 10, 1); // hello
    while (rgb4.g > 30 && rgb4.b > 25) { // hello
        display_values(); // hello
    } // hello
    ev3_motor_stop(left_motor, true); // hello
    ev3_motor_stop(right_motor, true); // hello
    if (rgb4.g < 30) { // hello
        pos.street = RED_STREET; // hello
    } else { // hello
        pos.street = YELLOW_STREET; // hello
    } // hello
    tslp_tsk(250); // hello
 // hello
    int index; // hello
    for (index = 0; index < 4; index++) { // hello
        //read instructions // hello
        ev3_motor_reset_counts(EV3_PORT_B); // hello
        ev3_motor_reset_counts(EV3_PORT_C); // hello
        ev3_motor_steer(left_motor, right_motor, 10, 1); // hello
        while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < 55) { // hello
            display_values(); // hello
        } // hello
        ev3_motor_stop(left_motor, true); // hello
        ev3_motor_stop(right_motor, true); // hello
        if (((rgb4.r + rgb4.g + rgb4.b) / 3) > 25) { // hello
            bit1 = 1; // hello
        } else { // hello
            bit1 = 0; // hello
        } // hello
        ev3_motor_reset_counts(EV3_PORT_B); // hello
        ev3_motor_reset_counts(EV3_PORT_C); // hello
        ev3_motor_steer(left_motor, right_motor, 10, 1); // hello
        while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < 55) { // hello
            display_values(); // hello
        } // hello
        ev3_motor_stop(left_motor, true); // hello
        ev3_motor_stop(right_motor, true); // hello
        if (((rgb4.r + rgb4.g + rgb4.b) / 3) > 25) { // hello
            bit2 = 1; // hello
        } else { // hello
            bit2 = 0; // hello
        } // hello
 // hello
        // decode instructions // hello
        if (bit1 == 1) { // hello
            tasks[index] = BLACKMATERIAL; // hello
        } else { // hello
            if (bit2 == 1) { // hello
                tasks[index] = BLUEMATERIAL; // hello
            } else { // hello
                tasks[index] = REMOVESNOW; // hello
            } // hello
        } // hello
    } // hello
    tslp_tsk(250); // hello
 // hello
    //detect line // hello
    ev3_motor_steer(left_motor, right_motor, 10, 1); // hello
    while (ev3_color_sensor_get_reflect(color_sensor2) > 15 && ev3_color_sensor_get_reflect(color_sensor3) > 15) { // hello
        display_values(); // hello
    } // hello
    ev3_motor_stop(left_motor, true); // hello
    ev3_motor_stop(right_motor, true); // hello
 // hello
    //display things in a very small font // hello
    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE); // hello
    char lcdstr[100]; // hello
    sprintf(lcdstr, "%d, %d", tasks[BLUE_STREET], tasks[GREEN_STREET]); // hello
    ev3_lcd_draw_string(lcdstr, 0, 0); // hello
    sprintf(lcdstr, "%d, %d", tasks[YELLOW_STREET], tasks[RED_STREET]); // hello
    ev3_lcd_draw_string(lcdstr, 0, 0); // hello
 // hello
    //record position // hello
    pos.section = 1; // hello
    pos.distance = 51; // hello
    pos.dash = 0; // hello
    pos.facing = 0; // hello
} // hello
 // hello
static void button_clicked_handler(intptr_t button) { // hello
    switch(button) { // hello
    case BACK_BUTTON: // hello
        ev3_motor_steer(left_motor, right_motor, 0, 0); // hello
        exit(0); // hello
        break; // hello
    } // hello
} // hello
 // hello
/* // hello
static void exit_program_with_exception() { // hello
    ev3_motor_steer(left_motor, right_motor, 0, 0); // hello
    exit(1); // hello
} // hello
*/ // hello
 // hello
void display_values() { // hello
    //declare variables // hello
    char msg[100]; // hello
    int value; // hello
 // hello
    //read motor counts // hello
    value = ev3_motor_get_counts(left_motor); // hello
    sprintf(msg, "L: %d   ", value); // hello
    ev3_lcd_draw_string(msg, 10*0, 15*0); // hello
    value = ev3_motor_get_counts(right_motor); // hello
    sprintf(msg, "R: %d   ", value); // hello
    ev3_lcd_draw_string(msg, 10*8, 15*0); // hello
 // hello
    //read sensor rgb1 // hello
/* // hello
    bool_t val1 = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb1); // hello
    assert(val1); // hello
    sprintf(msg, "RGB1:"); // hello
    ev3_lcd_draw_string(msg, 10*0, 15*1.5); // hello
    sprintf(msg, "R: %d", rgb1.r); // hello
    ev3_lcd_draw_string(msg, 10*0, /15*2.5); // hello
    sprintf(msg, "G: %d", rgb1.g); // hello
    ev3_lcd_draw_string(msg, 10*7, 15*2.5); // hello
    sprintf(msg, "B: %d", rgb1.b); // hello
    ev3_lcd_draw_string(msg, 10*14, 15*2.5); // hello
*/ // hello
 // hello
    //read sensor rgb4 // hello
    bool_t val4 = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb4); // hello
    assert(val4); // hello
    sprintf(msg, "RGB4:"); // hello
    ev3_lcd_draw_string(msg, 10*0, 15*4); // hello
    sprintf(msg, "R: %d  ", rgb4.r); // hello
    ev3_lcd_draw_string(msg, 10*0, 15*5); // hello
    sprintf(msg, "G: %d  ", rgb4.g); // hello
    ev3_lcd_draw_string(msg, 10*7, 15*5); // hello
    sprintf(msg, "B: %d  ", rgb4.b); // hello
    ev3_lcd_draw_string(msg, 10*14, 15*5); // hello
 // hello
    //read linefollow sensors // hello
    sprintf(msg, "Light2 & Light3:"); // hello
    ev3_lcd_draw_string(msg, 10*0, 15*6.5); // hello
    value = ev3_color_sensor_get_reflect(color_sensor2); // hello
    sprintf(msg, "L: %d  ", value); // hello
    ev3_lcd_draw_string(msg, 10*0, 15*7.5); // hello
    value = ev3_color_sensor_get_reflect(color_sensor3); // hello
    sprintf(msg, "L: %d  ", value); // hello
    ev3_lcd_draw_string(msg, 10*7, 15*7.5); // hello
} // hello
