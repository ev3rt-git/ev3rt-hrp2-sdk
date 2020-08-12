#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ev3api.h"
#include "robot.h"

#define DEBUG

#ifdef DEBUG
  #define _debug(x) (x)
#else
  #define _debug(x)
#endif

// TODO log _trip_degree in robot_move methods.
static int _robot_trip_degree = 0;

inline float robot_get_move_distance() {
    return robot_get_move_degree() * ROBOT_CM_PER_DEGREE;
}

inline int robot_get_move_degree() {
    return (ev3_motor_get_counts(EV3_PORT_B) + ev3_motor_get_counts(EV3_PORT_C)) / 2;
}

inline void robot_reset_move() {
    ev3_motor_reset_counts(EV3_PORT_B);
    ev3_motor_reset_counts(EV3_PORT_C);
}

inline float robot_get_trip_distance() {
    return _robot_trip_degree * ROBOT_CM_PER_DEGREE;
}

inline int robot_get_trip_degree() {
    return _robot_trip_degree;
}

inline void robot_reset_trip() {
    _robot_trip_degree = 0;
}

inline void robot_stop() {
	ev3_motor_stop(EV3_PORT_B, true);
	ev3_motor_stop(EV3_PORT_C, true);
}

void robot_left_turn(int degree) {
}

void robot_right_turn(int degree) {
}

void robot_pivot_turn(int degree) {
}

void robot_move(int distance, robot_side_t side) {
}

void robot_arc(float radius, float angle) {
}

inline void robot_set_power(int power) {
}

void robot_set_trip_point(int index, int distance, ISR handler) {
	//ROBOT_TRIP_POINTS[index] = distance * ROBOT_DEGREE_PER_CM;
	//ROBOT_TRIP_EVENT_HANDLERS[index] = handler;
}

/*
static void robot_trigger_trip_event() {
	int i = 0; 
	for (i = 0; i < ROBOT_NUM_EVENT_HANDLERS; i ++) {
		if (ROBOT_TRIP_POINTS[i] > 0 && ROBOT_TRIP_POINTS[i] >= _robot_trip_degree) {
			// trigger event handler
			ROBOT_TRIP_EVENT_HANDLERS[i](_robot_trip_degree * ROBOT_CM_PER_DEGREE);
			// reset trip point to avoid repeated events
			ROBOT_TRIP_POINTS[i] = 0;
		}
	}
}
*/

static void robot_back_button_clicked_handler(intptr_t button) {
    switch(button) {
    case BACK_BUTTON:
		ev3_motor_stop(EV3_PORT_A, true);
		ev3_motor_stop(EV3_PORT_B, true);
		ev3_motor_stop(EV3_PORT_C, true);
		ev3_motor_stop(EV3_PORT_C, true);
        exit(-1);
        break;
    }
}

void robot_add_back_button_on_clicked() {
	ev3_button_set_on_clicked(BACK_BUTTON, robot_back_button_clicked_handler, BACK_BUTTON);
}	
