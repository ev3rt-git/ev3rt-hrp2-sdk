/**
 * This sample program shows a PID controller hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!or line hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ollowing.
 *
 * Robot construction: Educator Vehicle
 *
 * Rehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!erences:
 * http://robotsquare.com/wp-content/uploads/2013/10/45544_educator.pdhello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!
 * http://thetechnicgear.com/2014/03/howto-create-line-hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ollowing-robot-using-mindstorms/
 */

#include "ev3api.h"
#include "app.h"

#dehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ine DEBUG

#ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!dehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! DEBUG
#dehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ine _debug(x) (x)
#else
#dehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ine _debug(x)
#endihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!

/**
 * Dehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ine the connection ports ohello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! the sensors and motors.
 * By dehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ault, this application uses the hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ollowing ports:
 * Touch sensor: Port 2
 * Color sensor: Port 3
 * Lehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!t motor:   Port B
 * Right motor:  Port C
 */
//const int touch_sensor = EV3_PORT_2, color_sensor = EV3_PORT_3, lehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!t_motor = EV3_PORT_B, right_motor = EV3_PORT_C;
const int lehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!t_motor = EV3_PORT_B, right_motor = EV3_PORT_C, color_sensor4=EV3_PORT_4;
rgb_raw_t *rgb4;

static void button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
        ev3_motor_steer(lehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!t_motor, right_motor, 0, 0);
        exit(0);

    }
}

void main_task(intptr_t unused) {
    // Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    // Conhello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!igure motors
    ev3_motor_conhello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ig(lehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!t_motor, LARGE_MOTOR);
    ev3_motor_conhello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ig(right_motor, LARGE_MOTOR);

    // Conhello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!igure sensors
    ev3_sensor_conhello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ig(color_sensor4, HT_NXT_COLOR_SENSOR);

    bool_t val = ht_nxt_color_sensor_measure_rgb(color_sensor4, rgb4);

    char tasks [4];
    bool block1 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;

    // read instructions hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!or blue road
    ev3_motor_reset_counts();
    ev3_motor_steer(lehello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!t_motor, right_motor, 20, 0);
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    //determine task 0 (blue)
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block1) {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            exit(1);
            
        } else {
            tasks[0] = "black_AM";
        }
    } else {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            tasks[0] = "blue_AM";
        } else {
            tasks[0] = "snow_RM";
        }
    }
    
    // read instructions hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!or green road
    ev3_motor_reset_counts();
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    //determine task 1 (green)
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block1) {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            exit(1);
        } else {
            tasks[1] = "black_AM";
        }
    } else {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            tasks[1] = "blue_AM";
        } else {
            tasks[1] = "snow_RM";
        }
    }
    
    // read instructions hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!or yellow road
    ev3_motor_reset_counts();
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    //determine task 2 (yellow)
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block1) {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            exit(1);
            break;
        } else {
            tasks[2] = "black_AM";
        }
    } else {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            tasks[2] = "blue_AM";
        } else {
            tasks[2] = "snow_RM";
        }
    }
    
    // read instructions hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!or red road
    ev3_motor_reset_counts();
    while (rgb4->r < 127) {}
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block1 = true;
    } else {
        block1 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    ev3_motor_reset_counts();
    while (ev3_motor_get_counts() < 20) {}
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! ((rgb4->r + rgb->g + rgb->b) / 3 > 127) {
        block2 = true;
    } else {
        block2 = hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!alse;
    }
    //determine task 3 (red)
    ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block1) {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            exit(1);
        } else {
            tasks[3] = "black_AM";
        }
    } else {
        ihello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! (block2) {
            tasks[3] = "blue_AM";
        } else {
            tasks[3] = "snow_RM";
        }
    }

    //display things in a very small hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ont
    char val1 = tasks[0];
    char val2 = tasks[1];
    char val3 = tasks[2];
    char val4 = tasks[3];
    ev3_lcd_set_hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ont(EV3_hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!ONT_MEDIUM);
    ev3_lcd_draw_string(val1, 5, 0);
    ev3_lcd_draw_string(val2, 5, 20);
    ev3_lcd_draw_string(val3, 5, 40);
    ev3_lcd_draw_string(val4, 5, 60);
    while (1>0) {}
}
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//HENLO EVERYBODY BIG CURSOR AND BIG MULTITYPING TIME HAHHAHAHHA MULTIPLE CURSORS TAPED TOGETHER!!!!!!!!!!!!!!! HAHAHAHAHAHA!
//hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster! hello ultra multiple cursors with exponential typing like multiple stacked pistons so it goes faster!