
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
const int color_sensor2 = EV3_PORT_2, color_sensor4 = EV3_PORT_4, color_sensor3 = EV3_PORT_3, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, a_motor = EV3_PORT_A, d_motor = EV3_PORT_D;

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
        ev3_motor_steer(left_motor, right_motor, 0, 0);
        exit(0);
        break;
    }
}

//snow
int snowIndex = 0;
int isTurning = 0;
//dashes
int dashes = 0;
int isWhite = 1;
int lastDash = 0;
//display values
char msgbuf[100];
//rgb
rgb_raw_t rgb1;
rgb_raw_t rgb4;
//distance and PID
float wheelDistance = 0;
//read code
float detected[3] = {0,0,0};//1 = red, 2 = yellow
float values[8] = {0,0,0,0,0,0,0,0};
int instructions[4] = {0,0,0,0};
int readIndex = 0;
int isReading = 0;
void main_task(intptr_t unused) {
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_motor_config(a_motor, MEDIUM_MOTOR);
    ev3_motor_config(d_motor, MEDIUM_MOTOR);

    ev3_sensor_config(color_sensor2, COLOR_SENSOR);
    ev3_sensor_config(color_sensor3, COLOR_SENSOR);
    ev3_sensor_config(color_sensor4, HT_NXT_COLOR_SENSOR);

    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_reset_counts(d_motor);
    //ev3_motor_steer(left_motor,right_motor,10,5);
    /*while(wheelDistance < 70){
        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb4);
        assert(val);
        displayValues();
        if(readIndex == 0 && rgb4.g > 40 && rgb4.r > 40 && wheelDistance > 25 && wheelDistance < 31){
            readIndex += 1;
            detected[0] = 2;
            ev3_speaker_play_tone(NOTE_A6, 60);
        }
        else if(readIndex == 0 && rgb4.r > 45 && wheelDistance > 25 && wheelDistance < 31){
            readIndex += 1;
            detected[0] = 1;
            ev3_speaker_play_tone(NOTE_A4, 60);
        }
        else if(rgb4.r > 55 && isReading < 0 && wheelDistance > 31 && readIndex > 0){
            isReading = 50;
            detected[readIndex] = wheelDistance;
            readIndex += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else if(rgb4.g > 55 && isReading < 0 && wheelDistance > 31 && readIndex > 0){
            isReading = 50;
            detected[readIndex] = wheelDistance;
            readIndex += 1;
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else if(rgb4.b > 55 && isReading < 0 && wheelDistance > 31 && readIndex > 0){
            isReading = 50;
            detected[readIndex] = wheelDistance;
            readIndex += 1;
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
    displayValues();
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2 * (3.1415926535 * 9.5) / 360;
    tslp_tsk(1000);
    
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
        int snowValues[6][3] = {{25,350,5},{70,275,10},{100,350,6},{1000,0,0},{1000,0,0},{1000,0,0}};
        wallFollow(136,snowValues,3);
        ev3_motor_rotate(a_motor,500,50,true);
        ev3_motor_steer(left_motor,right_motor,-15,0);
        tslp_tsk(1000);
        ev3_motor_steer(left_motor,right_motor,0,0);
        tslp_tsk(1050);
        ev3_motor_steer(left_motor,right_motor,-15,90);
        tslp_tsk(900);
        ev3_motor_steer(left_motor,right_motor,0,0);
        ev3_motor_rotate(a_motor,-500,50,true);
        tslp_tsk(1000);
        int snowValues1[6][3] = {{20,500,5},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0}};
        wallFollow(60,snowValues1,0);
        ev3_motor_steer(left_motor,right_motor,20,0);
        tslp_tsk(2150);
        ev3_motor_steer(left_motor,right_motor,0,0);
        ev3_motor_steer(left_motor,right_motor,-15,90);
        tslp_tsk(900);
        ev3_motor_steer(left_motor,right_motor,0,0);
        int snowValues2[6][3] = {{20,500,5},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0}};
        wallFollow(160,snowValues2,3);
    }*/
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
    tslp_tsk(2000);
    ev3_motor_steer(left_motor,right_motor,0,0);
    ev3_motor_steer(left_motor,right_motor,-10,45);
    tslp_tsk(2400);
    ev3_motor_steer(left_motor,right_motor,0,0);
    int snowValues2[6][3] = {{20,500,5},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0},{1000,0,0}};
    wallFollow(160,snowValues2,3);

}

void displayValues(){
    //sprintf(msgbuf, "Red:   %-4d", rgb4.r);
    //ev3_lcd_draw_string(msgbuf, 0, 15 * 1);
    //sprintf(msgbuf, "Green: %-4d", rgb4.g);
    //ev3_lcd_draw_string(msgbuf, 0, 15 * 2);
    //sprintf(msgbuf, "Blue:  %-4d", rgb4.b);
    //ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
    /*sprintf(msgbuf, "isTurning %9f          " ,isTurning);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 1);
    sprintf(msgbuf, "wheelDistance %9f          " ,wheelDistance);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 2);
    sprintf(msgbuf, "distance %d          " ,pos.distance);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
    sprintf(msgbuf, " %d          " ,instructions[0]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 4);
    sprintf(msgbuf, " %d          " ,instructions[1]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 5);
    sprintf(msgbuf, " %d          " ,instructions[2]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 6);
    sprintf(msgbuf, " %d          " ,instructions[3]);
    ev3_lcd_draw_string(msgbuf, 0, 15 * 7);*/
}
void wallFollow(int distance,int snow[6][3],int steer){
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_reset_counts(d_motor);
    snowIndex = 0;
    wheelDistance = 0;
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
        //lasterror = error;
        tslp_tsk(1);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb4);
        assert(val);
        displayValues();
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    return;
}
void linePID(int distance){
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(a_motor);
    ev3_motor_reset_counts(d_motor);
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
