
#include "ev3api.h"
#include "app.h"
#include "stdlib.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif


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

void dashPID(int distance){
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
        float steer = 0.05 * error + 0.5 * integral + 0.25 * (error - lasterror);
        ev3_motor_steer(left_motor, right_motor, 30, steer);
        lasterror = error;
        tslp_tsk(1);
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

void main_task(intptr_t unused) {
    //int snow1[4][2] = {{7,-300},{34,300},{96,-300},{110,300}};
    //int snow1[4][2] = {{8,-300},{38,300},{109,-300},{131,300}};

    //int snow1[4][2] = {{11,150},{17,-150},{121,-150},{139,150}};
    //int snow1[4][2] = {{17,-300},{70,-350}};
    int snow1[1][2] = {{17,-300}};
    int snow4[2][2] = {{32,-300},{36,300}};
    int snow5[4][2] = {{32,-150},{36,150},{121,-150},{139,150}};
    int snow6[2][2] = {{32,-300},{36,300}};
    int snow7[4][2] = {{32,-150},{36,150},{121,-150},{139,150}};
    int snow8[2][2] = {{32,-300},{36,300}};
    int index = 0;
    int isTurning = 0;
    int turnReturn = 0;
    int dashes = 0;
    int isWhite = 1;
    int lastDash = 0;
	char msgbuf[100];
    rgb_raw_t rgb;
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
    float wheelDistance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2;
    float lasterror = 0, integral = 0;
    ev3_motor_steer(left_motor,right_motor,10,0);
    while(wheelDistance < 40){
        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4,  &rgb);
        assert(val);
        sprintf(msgbuf, "Red:   %-4d", rgb.r);
        ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
        sprintf(msgbuf, "Green: %-4d", rgb.g);
        ev3_lcd_draw_string(msgbuf, 0, 15 * 4);
        sprintf(msgbuf, "Blue:  %-4d", rgb.b);
        ev3_lcd_draw_string(msgbuf, 0, 15 * 5);
        if(rgb.r > 55 || rgb.g > 55 || rgb.b > 55){
            sprintf(msgbuf, "THERE IS A CAR!!!");
            ev3_lcd_draw_string(msgbuf, 0, 15 * 7);
            
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        else{
            sprintf(msgbuf, "what??            ");
            ev3_lcd_draw_string(msgbuf, 0, 15 * 7);
        }
        tslp_tsk(10);
    }
    while (wheelDistance < 1000) {
        /*if((wheelDistance >= snow1[index][0] - 3) && (isTurning == 0) && index < 2){
            isTurning = 1;
            turnReturn = snow1[index][1] * -1;
            ev3_motor_rotate(a_motor,snow1[index][1],50,false);
            ev3_speaker_play_tone(NOTE_C4, 60);
            if(index == 4){

            }
            else{
                index = index + 1;
            }
        }
        if((isTurning == 1) && wheelDistance >= snow1[index - 1][0] + 3){
            isTurning = 0;
            ev3_motor_rotate(a_motor,turnReturn,50,false);
            ev3_speaker_play_tone(NOTE_C5, 60);
        }*/
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
        /*sprintf(msgbuf, "Red:   %-4d", rgb.r);
        ev3_lcd_draw_string(msgbuf, 0, 15 * 3);
        sprintf(msgbuf, "Green: %-4d", rgb.g);
        ev3_lcd_draw_string(msgbuf, 0, 15 * 4);
        sprintf(msgbuf, "Blue:  %-4d", rgb.b);
        ev3_lcd_draw_string(msgbuf, 0, 15 * 5);*/
        if(rgb.r > 55 || rgb.g > 55 || rgb.b > 55){
            //sprintf(msgbuf, "THERE IS A CAR!!!");
            //ev3_lcd_draw_string(msgbuf, 0, 15 * 7);
            
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        /*else{
            sprintf(msgbuf, "what??            ");
            ev3_lcd_draw_string(msgbuf, 0, 15 * 7);
        }*/
    }
    /*wheelDistance = 0;
    ev3_motor_steer(left_motor, right_motor, 15, 15);
    tslp_tsk(1000);
    
    ev3_motor_steer(left_motor, right_motor, 15, -15);
    while (wheelDistance < 58) {
        if((wheelDistance >= snow1[index][0] - 3) && (isTurning == 0) && index < 2){
            isTurning = 1;
            turnReturn = snow1[index][1] * -1;
            ev3_motor_rotate(a_motor,snow1[index][1],50,false);
            ev3_speaker_play_tone(NOTE_C4, 60);
            if(index == 4){

            }
            else{
                index = index + 1;
            }
        }
        if((isTurning == 1) && wheelDistance >= snow1[index - 1][0] + 3){
            isTurning = 0;
            ev3_motor_rotate(a_motor,turnReturn,50,false);
            ev3_speaker_play_tone(NOTE_C5, 60);
        }
        wheelDistance = (ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2) * ((3.1415926535 * 9.5) / 360);
        float error = ev3_color_sensor_get_reflect(color_sensor2) - ev3_color_sensor_get_reflect(color_sensor3);
        integral = error + integral * 0.5;
        float steer = 0.04 * error + 0.55 * integral + 0.2 * (error - lasterror);
        ev3_motor_steer(left_motor, right_motor, 15, steer);
        lasterror = error;
        tslp_tsk(1);
    }
    
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    dashPID(1950);
    ev3_motor_steer(left_motor, right_motor, 20, 100);
    tslp_tsk(480);
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    tslp_tsk(1000);
    //ev3_speaker_play_tone(NOTE_C4, 100);
    linePID(1000);
    //ev3_speaker_play_tone(NOTE_C4, 100);

    int color1 = 0;
    int color2 = 0;
    //1:r,2:y,3:g,4:b
    if(color1 == 1){
        if(color2 == 2){
            //collect snow from yellow
            //collect 4 snow from red on way to snow containers
        }
        if(color2 == 3){
            
        }
        if(color2 == 4){
            
        }
    }
    if(color1 == 2){
        if(color2 == 1){
            //collect snow from yellow
            //collect 4 snow from red on way to snow containers
        }
        if(color2 == 3){
            //collect snow from yellow
            //collect snow from green
        }
        if(color2 == 4){
            //collect snow from yellow
        }
    }
    if(color1 == 3){
        if(color2 == 1){
            
        }
        if(color2 == 2){
            //collect snow from yellow
            //collect snow from green
        }
        if(color2 == 4){
            
        }
    }
    if(color1 == 4){
        if(color2 == 1){
            
        }
        if(color2 == 2){
            
        }
        if(color2 == 3){
            
        }
    }*/
	//char msgbuf[100];
    //rgb_raw_t rgb;

}
