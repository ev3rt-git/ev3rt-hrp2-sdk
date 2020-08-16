#include <stdlib.h>
#include <stdio.h>
#include "ev3api.h"
#include "app.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

// define motors and sensors
const int color_sensor1 = EV3_PORT_2, color_sensor2 = EV3_PORT_2, color_sensor3 = EV3_PORT_3, color_sensor4 = EV3_PORT_4, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, a_motor = EV3_PORT_A, d_motor = EV3_PORT_D;

rgb_raw_t rgb1;
rgb_raw_t rgb4;
position pos = {-1, -1, -1, 0, 0};

int allTasks[4][3][6][3] = {
    //blue
    {
        //color_4
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //a_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //d_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
    },
    //green
    {
        //color_4
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //a_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //d_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
    },
    //yellow
    {
        //color_4
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //a_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //d_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
    },
    //red
    {
        //color_4
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //a_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
        //d_motor
        {
            //index 0
            {
                0,0,0
            },
            //index 1
            {
                0,0,0
            },
            //index 2
            {
                0,0,0
            },
            //index 3
            {
                0,0,0
            },
            //index 4
            {
                0,0,0
            },
            //index 5
            {
                0,0,0
            },
        },
    },
}

int color_4_index = 0;
int next_color_4_task[3] = allTasks[pos.street][0][color_4_index];
int a_motor_index = 0;
int next_color_a_task[3] = allTasks[pos.street][0][a_motor_index];
int d_motor_index = 0;
int next_color_d_task[3] = allTasks[pos.street][0][s_motor_index];


void main_task(intptr_t unused) {
    init();

    readCode();
    // TODO run2020
}

void readCode() {
    // define variables
    int bit1 = -1;
    int bit2 = -1;

    // leave start
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, 30, 1);
    while (abs(((ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2)) < 250) {
        display_values();
    }

    // detect line
    ev3_motor_steer(left_motor, right_motor, 10, 1);
    while (rgb4.g > 30 && rgb4.b > 25) {
        display_values();
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    if (rgb4.g < 30) {
        pos.street = RED_STREET;
    } else {
        pos.street = YELLOW_STREET;
    }
    tslp_tsk(5);
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, -10, 0);
    while (((abs(ev3_motor_get_counts(EV3_PORT_B)) + abs(ev3_motor_get_counts(EV3_PORT_C))) / 2) < 20) {
        display_values();
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);

    tslp_tsk(5);
    // record instructions
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
    ev3_motor_steer(left_motor, right_motor, 10, 1);
    int index;
    for (index = 0; index < 4; index++) {
        // read instructions
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
                ev3_motor_steer(left_motor, left_motor, 0, 0);
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

    // detect line
    ev3_motor_steer(left_motor, right_motor, 10, 1);
    while (ev3_color_sensor_get_reflect(color_sensor3) > 20) {
        display_values();
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    tslp_tsk(5);

    /*
    // align robot
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
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    */

    // display things in a very medium font
    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE);
    char lcdstr[100];
    sprintf(lcdstr, "%d, %d", tasks[BLUE_STREET], tasks[GREEN_STREET]);
    ev3_lcd_draw_string(lcdstr, 0, 0);
    sprintf(lcdstr, "%d, %d", tasks[YELLOW_STREET], tasks[RED_STREET]);
    ev3_lcd_draw_string(lcdstr, 0, 15);

    // record position
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
    ev3_motor_config(a_motor, MEDIUM_MOTOR);
    ev3_motor_config(d_motor, MEDIUM_MOTOR);
    
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

    // reset snow/car collector
    ev3_motor_set_power(a_motor, -100);
    tslp_tsk(1500);
    ev3_motor_rotate(a_motor, 500, 50, true);

    // reset abrasive material dispenser
    ev3_motor_set_power(d_motor, 100);
    tslp_tsk(1500);
    ev3_motor_stop(d_motor, true);

    // wait for button press
    ev3_lcd_draw_string("Press OK to run", 14, 45);
    ev3_lcd_fill_rect(77, 87, 24, 20, EV3_LCD_BLACK);
    ev3_lcd_fill_rect(79, 89, 20, 1, EV3_LCD_WHITE);
    ev3_lcd_draw_string("OK", 79, 90);
    while (1) {
        if (ev3_button_is_pressed(ENTER_BUTTON)) {
            while (ev3_button_is_pressed(ENTER_BUTTON));
            break;
        }
    }
    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE);
}

void display_values() {
    // declare variables
    char msg[100];
    int value;

    // wait for values to be refreshed
    tslp_tsk(3);

    // read motor counts
    value = ev3_motor_get_counts(left_motor);
    sprintf(msg, "L: %d   ", value);
    ev3_lcd_draw_string(msg, 10*0, 15*0);
    value = ev3_motor_get_counts(right_motor);
    sprintf(msg, "R: %d   ", value);
    ev3_lcd_draw_string(msg, 10*8, 15*0);

    // read sensor rgb1
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

    // read sensor rgb4
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

    // read linefollow sensors
    sprintf(msg, "Light2 & Light3:");
    ev3_lcd_draw_string(msg, 10*0, 15*6.5);
    value = ev3_color_sensor_get_reflect(color_sensor2);
    sprintf(msg, "L: %d  ", value);
    ev3_lcd_draw_string(msg, 10*0, 15*7.5);
    value = ev3_color_sensor_get_reflect(color_sensor3);
    sprintf(msg, "L: %d  ", value);
    ev3_lcd_draw_string(msg, 10*7, 15*7.5);
}

void readColorCode(){
    float wheelDistance = 0;
    float detected[3] = {0,0,0};//1 = red, 2 = yellow
    float values[8] = {0,0,0,0,0,0,0,0};
    int instructions[4] = {0,0,0,0};
    int readIndex = 0;
    int isReading = 0;
    ev3_motor_steer(left_motor,right_motor,10,5);
    while(wheelDistance < 70){
        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb4);
        assert(val);
        if(readIndex == 0 && rgb4.g > 40 && rgb4.r > 40 && wheelDistance > 30 && wheelDistance < 35){
            readIndex += 1;
            detected[0] = 2;
            ev3_speaker_play_tone(NOTE_A6, 60);
        }
        else if(readIndex == 0 && rgb4.r > 45 && wheelDistance > 30 && wheelDistance < 35){
            readIndex += 1;
            detected[0] = 1;
            ev3_speaker_play_tone(NOTE_A4, 60);
        }
        /*else if(rgb4.r > 55 && isReading < 2 && wheelDistance > 35 && readIndex > 0){
            isReading = 50;
            detected[readIndex] = wheelDistance;
            readIndex += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else if(rgb4.g > 55 && isReading < 2 && wheelDistance > 35 && readIndex > 0){
            isReading = 50;
            detected[readIndex] = wheelDistance;
            readIndex += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else if(rgb4.b > 55 && isReading < 2 && wheelDistance > 35 && readIndex > 0){
            isReading = 50;
            detected[readIndex] = wheelDistance;
            readIndex += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }*/
        //else if(isReading > 1){
            //isReading = isReading - 1;
        //}
        int sddsfsd = round((detected[1] - 31) / 5);
        int sddsfsd2 = round((detected[2] - 31) / 5);
        values[sddsfsd] = 1;
        values[sddsfsd2] = 1;
        for(int i = 0; i < 7; i +=2){
            if(values[i] == 0){
                if(values[i + 1] == 0){
                    instructions[i/2] = 0;
                    tasks[i/2] = 0;
                }
                else{
                    instructions[i/2] = 1;
                    tasks[i/2] = 1;
                }
            }
            else{
                if(values[i + 1] == 0){
                    instructions[i/2] = 2;
                    tasks[i/2] = 2;
                }
                else{
                }
            }
        }
        if(detected[0] = 2){
            pos.street = 2;
        }
        if(detected[0] = 1){
            pos.street = 3;
        }
        pos.section = 1;
        pos.distance = wheelDistance;
        pos.dash = 0;
        pos.facing = 0;
        tslp_tsk(10);
    }
    ev3_motor_steer(left_motor,right_motor,15,0);
    tslp_tsk(1500);
    ev3_motor_steer(left_motor,right_motor,0,0);
    wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2 * (3.1415926535 * 9.5) / 360;
    pos.distance = wheelDistance;
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2 * (3.1415926535 * 9.5) / 360;
}

void execute_moving_the_robot_based_on_the_color_code(){
    //0:b,1:g,2:y,3:r
    if(tasks[0] == 0 && tasks[1] == 0 && pos.street == 2){
        
    }
    if(tasks[0] == 0 && tasks[2] == 0 && pos.street == 2){
        
    }
    if(tasks[0] == 0 && tasks[3] == 0 && pos.street == 2){
        
    }
    if(tasks[1] == 0 && tasks[2] == 0 && pos.street == 2){
        
    }
    if(tasks[1] == 0 && tasks[3] == 0 && pos.street == 2){
        
    }
    if(tasks[2] == 0 && tasks[3] == 0 && pos.street == 2){
        int snowValues[6][3] = {{17,-300},{121,-300},{139,0},{1000,0},{1000,0},{1000,0}};
        wallFollow(160,snowValues);
    }
    //red
    if(tasks[0] == 0 && tasks[1] == 0 && pos.street == 3){

    }
    if(tasks[0] == 0 && tasks[2] == 0 && pos.street == 3){
        
    }
    if(tasks[0] == 0 && tasks[3] == 0 && pos.street == 3){
        
    }
    if(tasks[1] == 0 && tasks[2] == 0 && pos.street == 3){
        
    }
    if(tasks[1] == 0 && tasks[3] == 0 && pos.street == 3){
        
    }
    if(tasks[2] == 0 && tasks[3] == 0 && pos.street == 3){
        int snowValues[6][3] = {{25,350,5},{70,275,10},{97,350,6},{1000,0,0},{1000,0,0},{1000,0,0}};
        wallFollow(136,snowValues,3);
        ev3_motor_rotate(a_motor,500,50,true);
        ev3_motor_steer(left_motor,right_motor,-15,0);
        tslp_tsk(1000);
        ev3_motor_steer(left_motor,right_motor,0,0);
        tslp_tsk(1050);
        ev3_motor_steer(left_motor,right_motor,-15,90);
        tslp_tsk(900);
        ev3_motor_steer(left_motor,right_motor,0,0);
        ev3_motor_steer(left_motor,right_motor,-15,0);
        tslp_tsk(900);
        ev3_motor_steer(left_motor,right_motor,0,0);
        ev3_motor_rotate(a_motor,-500,50,true);
        tslp_tsk(1000);
        int snowValues1[6][3] = {{23,500,10},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0}};
        wallFollow(60,snowValues1,0);
        ev3_motor_steer(left_motor,right_motor,20,0);
        tslp_tsk(1600);
        ev3_motor_steer(left_motor,right_motor,0,0);
        ev3_motor_steer(left_motor,right_motor,10,-45);
        tslp_tsk(2400);
        ev3_motor_steer(left_motor,right_motor,0,0);
        int snowValues2[6][3] = {{20,500,5},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0}};
        wallFollow(160,snowValues2,3);
    }
}

void wallFollow(int distance,int snow[6][3],int steer){
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_reset_counts(d_motor);
    int dashes = 0;
    int isWhite = 1;
    int lastDash = 0;
    int snowIndex = 0;
    int isTurning = 0;
    float wheelDistance = 0;
    while (wheelDistance < distance) {
        if(wheelDistance >= snow[snowIndex][0] - snow[snowIndex][2] && isTurning == 0 && snowIndex < 6){
            isTurning = 1;
            ev3_motor_rotate(a_motor,snow[snowIndex][1],50,false);
            ev3_speaker_play_tone(NOTE_C4, 60);
            if(snowIndex == 6){

            }
            else{
                snowIndex = snowIndex + 1;
            }
        }
        if(isTurning == 1 && wheelDistance >= snow[snowIndex - 1][0] + snow[snowIndex - 1][2]){
            isTurning = 0;
            ev3_motor_rotate(a_motor,snow[snowIndex - 1][1] * -1,50,false);
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        /*if(ev3_color_sensor_get_reflect(color_sensor2) > 60 && isWhite == 0 && wheelDistance > lastDash + 3){
            isWhite = 1;
            ev3_speaker_play_tone(NOTE_C5, 100);

            dashes += 1;
            lastDash = wheelDistance;
        }
        if(ev3_color_sensor_get_reflect(color_sensor2) < 60 && isWhite == 1 && wheelDistance > lastDash + 3){
            isWhite = 0;
            ev3_speaker_play_tone(NOTE_C4, 100);

            lastDash = wheelDistance;
            dashes += 1;
        }*/
        //float error = ev3_color_sensor_get_reflect(color_sensor2) - ev3_color_sensor_get_reflect(color_sensor3);

        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        ev3_motor_steer(left_motor, right_motor, 15, steer);
        tslp_tsk(1);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb4);
        assert(val);
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    return;
}

void linePID(int distance){
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_reset_counts(d_motor);
    int snowIndex = 0;
    int isTurning = 0;
    float wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2;
    float lasterror = 0, integral = 0;
    while (wheelDistance < distance) {
        if(ev3_motor_get_counts(a_motor) > 490){
            ev3_motor_reset_counts(a_motor);
            ev3_motor_rotate(a_motor,-500,13,false);
            
        }
        if(ev3_motor_get_counts(a_motor) < -490){
            ev3_motor_reset_counts(a_motor);
            ev3_motor_rotate(a_motor,500,13,false);
        }
        wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2;
        float error = ev3_color_sensor_get_reflect(color_sensor2) - ev3_color_sensor_get_reflect(color_sensor3);
        integral = error + integral * 0.5;
        float steer = 0.04 * error + 0.5 * integral + 0.25 * (error - lasterror);
        ev3_motor_steer(left_motor, right_motor, 30, steer);
        lasterror = error;  
        tslp_tsk(1);
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    return;
}

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
            ev3_motor_stop(left_motor, false);
            ev3_motor_stop(right_motor, false);
            ev3_motor_stop(a_motor, false);
            ev3_motor_stop(d_motor, false);
        exit(0);
        break;
    }
}
/*

// task_4: read_color
// task_a: cm, rotation_1, rotation_2
// task_d: cm, rotation_1, rotation_2

// all_tasks contains all tasks for all threets, some tasks will be wiped our after reading codes
all_tasks[street][motor][index][task];
all_tasks[4     ][3    ][6 / 3][3   ];

street_tasks = all_tasks[street];
c4_tasks = street_tasks[street];
a_tasks = street_tasks[street];
d_tasks = street_tasks[street];
c4_task_index = 0;
a_task_index = 0;
d_task_index = 0;

void main() {
	init();

	read_tasks();
	
	while (1) {
		if (street = YELLOW) {
			while (follow_wall()) { // turn, follow_wall, turn, go_to_line
				do_tasks();
			}
			street = RED; // can be GREEN based on route plan
		} else if (street = RED) {
			while (follow_wall()) { // push_snow, back, turn, follow_wall (pick up abrasive material if applies), turn, go_to_line
				do_tasks();
			}
			street = YELLOW; // can be BLUE based on route plan
		} else if (street = BLUE) {
			// follow wall, sharp turn
			while (follow_line()) { // turn, go_to_line
				do_tasks();
			}
			street = YELLOW;
		} else if (street = BLUE) {
			// follow wall, sharp turn
			while (follow_line()) { // turn, go_to_line
				do_tasks();
			}
			stree = RED;
		}
	}
}

void read_tasks() {
	// read code 
	// wipe out tasks no longer needed
	// plan route
}

void do_tasks() {
	if (distance between c4_tasks[cm_1] and c4_tasks[cm_2]) {
		do_4_task();
		// move to next task
		c4_task_index++;
	}
	if (distance > a_tasks[a_task_index][cm]) {
		do_a_task();
		a_task_index++;
	}
	if (distance > d_tasks[d_task_index][cm]) {
		do_d_task();
		d_task_index++;
	}
}

*/

