
#include "ev3api.h"
#include "app.h"
#include "stdlib.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif


position pos = {-1, -1, -1, 0, 0};
const int color_sensor2 = EV3_PORT_2, color_sensor4 = EV3_PORT_4, color_sensor3 = EV3_PORT_3, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, a_motor = EV3_PORT_A;

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
#if !defined(BUILD_MODULE)
        syslog(LOG_NOTICE, "Back button clicked.");
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);
#endif
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);
        break;
    }
}

//int snow1[4][2] = {{7,-300},{34,300},{96,-300},{110,300}};
//int snow1[4][2] = {{8,-300},{38,300},{109,-300},{131,300}};
//int snow1[4][2] = {{11,150},{17,-150},{121,-150},{139,150}};
//int snow1[4][2] = {{17,-300},{70,-350}};
int snow1[1][2] = {{17,-300}};
int index1 = 0;
int isTurning = 0;
int turnReturn = 0;
int dashes = 0;
int isWhite = 1;
int lastDash = 0;
char msgbuf[100];
rgb_raw_t rgb;
float wheelDistance = 1;
float lasterror = 0, integral = 0;
float detected[3] = {0,0,0};//1 = red, 2 = yellow
float values[8] = {0,0,0,0,0,0,0,0};
float instructions[4] = {0,0,0,0};
int indexx = 0;
int err = 0;
int isReading = 0;
void main_task(intptr_t unused) {
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_motor_config(a_motor, MEDIUM_MOTOR);

    ev3_sensor_config(color_sensor2, COLOR_SENSOR);
    ev3_sensor_config(color_sensor3, COLOR_SENSOR);
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR);

    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_steer(left_motor,right_motor,10,1);
    while(wheelDistance < 70){
        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb);
        assert(val);
        displayValues();
        if(indexx == 0 && rgb.g > 40 && rgb.r > 40 && wheelDistance > 25 && wheelDistance < 31){
            indexx += 1;
            detected[0] = 2;
            ev3_speaker_play_tone(NOTE_A6, 60);
        }
        else if(indexx == 0 && rgb.r > 45 && wheelDistance > 25 && wheelDistance < 31){
            indexx += 1;
            detected[0] = 1;
            ev3_speaker_play_tone(NOTE_A4, 60);
        }
        else if(rgb.r > 55 && isReading < 0 && wheelDistance > 31 && indexx > 0){
            isReading = 50;
            detected[indexx] = wheelDistance;
            indexx += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else if(rgb.g > 55 && isReading < 0 && wheelDistance > 31 && indexx > 0){
            isReading = 50;
            detected[indexx] = wheelDistance;
            indexx += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else if(rgb.b > 55 && isReading < 0 && wheelDistance > 31 && indexx > 0){
            isReading = 50;
            detected[indexx] = wheelDistance;
            indexx += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else{
            isReading = isReading - 1;
        }
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
                    err = 1;
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
    tslp_tsk(800);
    ev3_motor_steer(left_motor,right_motor,15,-45);
    tslp_tsk(700);
    ev3_motor_steer(left_motor,right_motor,15,45);
    tslp_tsk(775);
    ev3_motor_steer(left_motor,right_motor,0,0);
    wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2 * (3.1415926535 * 9.5) / 360;
    pos.distance = wheelDistance;
    displayValues();
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2 * (3.1415926535 * 9.5) / 360;
    tslp_tsk(2000);
    
    
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
        
    }
}

void displayValues(){
    //sprintf(msgbuf, "Red:   %-4d", rgb.r);
    //ev3_lcd_draw_string(msgbuf, 0, 15 * 1);
    //sprintf(msgbuf, "Green: %-4d", rgb.g);
    //ev3_lcd_draw_string(msgbuf, 0, 15 * 2);
    //sprintf(msgbuf, "Blue:  %-4d", rgb.b);
    //ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
    sprintf(msgbuf, "Color %9f          " ,detected[0]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
    sprintf(msgbuf, "Street %d          " ,pos.street);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 2);
    sprintf(msgbuf, "distance %d          " ,pos.distance);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
    sprintf(msgbuf, " %9f          " ,instructions[0]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 4);
    sprintf(msgbuf, " %9f          " ,instructions[1]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 5);
    sprintf(msgbuf, " %9f          " ,instructions[2]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 6);
    sprintf(msgbuf, " %9f          " ,instructions[3]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 7);
}
void dashPID(int distance,int[4] snow1){
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    while (wheelDistance < 160) {
        if((wheelDistance >= snow1[index1][0] - 3) && (isTurning == 0) && index1 < 4){
            isTurning = 1;
            turnReturn = snow1[index1][1] * -1;
            ev3_motor_rotate(a_motor,snow1[index1][1],50,false);
            ev3_speaker_play_tone(NOTE_C4, 60);
            if(index1 == 4){

            }
            else{
                index1 = index1 + 1;
            }
        }
        if((isTurning == 1) && wheelDistance >= snow1[index1 - 1][0] + 3){
            isTurning = 0;
            ev3_motor_rotate(a_motor,turnReturn,50,false);
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        if(ev3_color_sensor_get_reflect(color_sensor2) > 60 && ev3_color_sensor_get_reflect(color_sensor3) > 60 && isWhite == 0 && wheelDistance > lastDash + 3){
            isWhite = 1;
            ev3_speaker_play_tone(NOTE_C5, 100);

            dashes += 1;
            lastDash = wheelDistance;
        }
        if(ev3_color_sensor_get_reflect(color_sensor2) < 60 && ev3_color_sensor_get_reflect(color_sensor3) < 60 && isWhite == 1 && wheelDistance > lastDash + 3){
            isWhite = 0;
            ev3_speaker_play_tone(NOTE_C4, 100);

            lastDash = wheelDistance;
            dashes += 1;
        }        
        float error = ev3_color_sensor_get_reflect(color_sensor2) - ev3_color_sensor_get_reflect(color_sensor3);

        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        integral = error + integral * 0.5;
        float steer = 0.04 * error + 0.55 * integral + 0.2 * (error - lasterror);
        ev3_motor_steer(left_motor, right_motor, 15, steer);
        lasterror = error;
        tslp_tsk(1);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb);
        assert(val);
    }
    
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    return;
}
void linePID(int distance){
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_rotate(a_motor,-500,15,true);
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
