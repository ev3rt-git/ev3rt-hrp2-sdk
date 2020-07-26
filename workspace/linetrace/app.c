
#include "ev3api.h"
#include "app.h"
#include "stdlib.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

const int color_sensor2 = EV3_PORT_2, color_sensor3 = EV3_PORT_3, left_motor = EV3_PORT_B, right_motor = EV3_PORT_C, a_motor = EV3_PORT_A;

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

void main_task(intptr_t unused) {
    // Register button handlers
    ev3_button_set_on_clicked(BACK_BUTTON, button_clicked_handler, BACK_BUTTON);

    // Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_motor_config(a_motor, MEDIUM_MOTOR);

    // Configure sensors
    ev3_sensor_config(color_sensor2, COLOR_SENSOR);
    ev3_sensor_config(color_sensor3, COLOR_SENSOR);

    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);
    float distance = 0;
    /**
     * PID controller
     */
    float lasterror = 0, integral = 0;
    while (distance < 3000) {
        distance = ev3_motor_get_counts(left_motor) / 2 + ev3_motor_get_counts(right_motor) / 2;
        float error = ev3_color_sensor_get_reflect(color_sensor2) - ev3_color_sensor_get_reflect(color_sensor3);
        integral = error + integral * 0.5;
        float steer = 0.07 * error + 0.4 * integral + 1 * (error - lasterror);
        ev3_motor_steer(left_motor, right_motor, 15, steer);
        lasterror = error;
        tslp_tsk(1);
    }
    ev3_motor_steer(left_motor, right_motor, 0, 0);
    ev3_motor_rotate(a_motor,200,10,true);
}
