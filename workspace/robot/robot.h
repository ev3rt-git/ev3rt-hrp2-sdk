#ifndef _ROBOT_H
#define _ROBOT_H

/* Robot configurations */
#define ROBOT_WHEEL_SIZE 6.24
#define ROBOT_TRACK_WIDTH 10.8
#define ROBOT_DEGREE_PER_CM (360 / (M_PI * ROBOT_WHEEL_SIZE))
#define ROBOT_CM_PER_DEGREE ((M_PI * ROBOT_WHEEL_SIZE) / 360)
#define ROBOT_REVERSED 1 /* change to -1 if revered */

/* PID controller consts */
#define ROBOT_KP 0.05
#define ROBOT_KI 0.5
#define ROBOT_KD 0.25

/* Maximum power the robot can use */
#define ROBOT_MAX_POWER 80

/* Power when robot is cruising around */
#define ROBOT_CRUISE_POWER 50

/* Power when robot is slowly approaching something */
#define ROBOT_INCHING_POWER 10

/* Add 1 power per 10ms, which means robot can accelerate from 0 power to 80 power in 800ms */
#define ROBOT_ACCELLERATION_STEP 1

/*
 * Trip
 */
typedef enum {
	TRIP_A,
	TRIP_B,
	LIFE_TIME
} robot_trip_t;

/*
 * Side
 */
typedef enum {
	NEITHER_SIDE,
	LEFT_SIDE,
	RIGHT_SIDE,
	BOTH_SIDE	
} robot_side_t;

#endif

/*
 * Get robot_move distance in CM since last call to robot_reset_move_count().
 */
extern float robot_get_move_distance();

/*
 * Get robot_move degree since last call to robot_reset_move_count().
 */
extern int robot_get_move_degree();

/*
 * Reset robot_move distance/degree readings.
 */
extern void robot_reset_move_count();

/*
 * Get trip distance in CM since last call to robot_reset_trip().
 */
extern float robot_get_trip_distance();

/*
 * Get trip degree since last call to robot_reset_trip().
 */
extern int robot_get_trip_degree();

/*
 * Reset trip distance/degree readings.
 */
extern void robot_reset_trip_();

/*
 * Stop both left and right wheels.
 */
extern void robot_stop();

/*
 * Turn robot around its left wheel.
 */
extern void robot_left_turn(int degree);

/*
 * Turn robot around its right wheel.
 */
extern void robot_right_turn(int degree);

/*
 * Rotate robot around its vertical axis using both left and righ wheel.
 */
extern void robot_pivot_turn(int degree);

/*
 * Move robot straightforward (or backword if distance < 0) and stop. 
 * If side is not NONE, robot should stop at line intersection.
 */
extern void robot_move(int distance, robot_side_t side);

/*
 * Moves robot along an robot_arc with a specified radius and angle.
 * radius - of the robot_arc path. If positive, the left side of the robot is on the inside of the turn. If negative, the left side of the robot is on the outside of the turn.
 * angle - The sign of the angle determines the direction of robot motion. Positive drives the robot forward, negative drives it backward.
 */
extern void robot_arc(float radius, float angle);

/*
 * Set the target power of moving. If current power is less than/greater 
 * than target power, robot should accelerate/decelerate to target power.
 */
extern void robot_set_power(int power);

/*
 * Set next trip point where a specific handler will be tiggered when robot 
 * passed that point. If distance equals to or less then 0, no event will
 * be triggered.
 */
extern void robot_set_trip_point(int index, int distance, ISR handler);

/*
 * Register on clicked event handler of BACKBUTTON.
 */
extern void robot_add_back_button_on_clicked(); 

extern void robot_