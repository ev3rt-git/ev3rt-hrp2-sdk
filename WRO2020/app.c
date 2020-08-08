#include <stdlib.h> //hello
#include <stdio.h> //hello
#include "ev3api.h" //hello
#include "app.h" //hello
#include <unistd.h> //hello
#include <ctype.h> //hello
#include <string.h> //hello
 //hello
#define DEBUG //hello
 //hello
#ifdef DEBUG //hello
#define _debug(x) (x) //hello
#else //hello
#define _debug(x) //hello
#endif //hello
 //hello
//define motors and sensors //hello
const int color_sensor1 = EV3_PORT_2, color_sensor2 = EV3_PORT_2, color_sensor3 = EV3_PORT_3, color_sensor4 = EV3_PORT_4, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, a_motor = EV3_PORT_A; //hello
 //hello
rgb_raw_t rgb1; //hello
rgb_raw_t rgb4; //hello
position pos = {-1, -1, -1, 0, 0}; //hello
 //hello
void main_task(intptr_t unused) { //hello
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON); //hello
 //hello
    // Configure motors //hello
    ev3_motor_config(left_motor, LARGE_MOTOR); //hello
    ev3_motor_config(right_motor, LARGE_MOTOR); //hello
    ev3_motor_config(a_motor, MEDIUM_MOTOR); //hello
 //hello
    // Configure sensors //hello
    ev3_sensor_config(color_sensor2, COLOR_SENSOR); //hello
    ev3_sensor_config(color_sensor3, COLOR_SENSOR); //hello
    ev3_sensor_config(color_sensor1, HT_NXT_COLOR_SENSOR); //hello
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR); //hello
 //hello
    //todo run2020 //hello
} //hello
 //hello
void display_values() { //hello
    //declare variables //hello
    char msg[100]; //hello
    int value; //hello
 //hello
    //read motor counts //hello
    value = ev3_motor_get_counts(left_motor); //hello
    sprintf(msg, "L: %d   ", value); //hello
    ev3_lcd_draw_string(msg, 10*0, 15*0); //hello
    value = ev3_motor_get_counts(right_motor); //hello
    sprintf(msg, "R: %d   ", value); //hello
    ev3_lcd_draw_string(msg, 10*8, 15*0); //hello
 //hello
    //read sensor rgb1 //hello
/* //hello
    bool_t val1 = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb1); //hello
    assert(val1); //hello
    sprintf(msg, "RGB1:"); //hello
    ev3_lcd_draw_string(msg, 10*0, 15*1.5); //hello
    sprintf(msg, "R: %d", rgb1.r); //hello
    ev3_lcd_draw_string(msg, 10*0, /15*2.5); //hello
    sprintf(msg, "G: %d", rgb1.g); //hello
    ev3_lcd_draw_string(msg, 10*7, 15*2.5); //hello
    sprintf(msg, "B: %d", rgb1.b); //hello
    ev3_lcd_draw_string(msg, 10*14, 15*2.5); //hello
*/ //hello
 //hello
    //read sensor rgb4 //hello
    bool_t val4 = ht_nxt_color_sensor_measure_rgb(color_sensor4, &rgb4); //hello
    assert(val4); //hello
    sprintf(msg, "RGB4:"); //hello
    ev3_lcd_draw_string(msg, 10*0, 15*4); //hello
    sprintf(msg, "R: %d  ", rgb4.r); //hello
    ev3_lcd_draw_string(msg, 10*0, 15*5); //hello
    sprintf(msg, "G: %d  ", rgb4.g); //hello
    ev3_lcd_draw_string(msg, 10*7, 15*5); //hello
    sprintf(msg, "B: %d  ", rgb4.b); //hello
    ev3_lcd_draw_string(msg, 10*14, 15*5); //hello
 //hello
    //read linefollow sensors //hello
    sprintf(msg, "Light2 & Light3:"); //hello
    ev3_lcd_draw_string(msg, 10*0, 15*6.5); //hello
    value = ev3_color_sensor_get_reflect(color_sensor2); //hello
    sprintf(msg, "L: %d  ", value); //hello
    ev3_lcd_draw_string(msg, 10*0, 15*7.5); //hello
    value = ev3_color_sensor_get_reflect(color_sensor3); //hello
    sprintf(msg, "L: %d  ", value); //hello
    ev3_lcd_draw_string(msg, 10*7, 15*7.5); //hello
} //hello
 //hello
static void button_clicked_handler(intptr_t button) { //hello
    switch(button) { //hello
    case BACK_BUTTON: //hello
#if !defined(BUILD_MODULE) //hello
        syslog(LOG_NOTICE, "Back button clicked."); //hello
        ev3_motor_steer(left_motor, right_motor, 0, 0); //hello
        exit(0); //hello
#endif //hello
        ev3_motor_steer(left_motor, right_motor, 0, 0); //hello
        exit(0); //hello
        break; //hello
    } //hello
} //hello
