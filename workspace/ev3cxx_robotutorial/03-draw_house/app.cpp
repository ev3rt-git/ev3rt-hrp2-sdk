/**
 * This is sample program for draw a house by a simple two-wheel robot, holding a pen
 * 
 * Don't forget set position of pen_motor UP!
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);
    ev3cxx::Motor pen_motor(ev3cxx::MotorPort::A);
    
    motors.onForRotations(50, 50, 1);
    for (int i = 0; i != 4; ++i) {
        pen_motor.onForDegrees(50, -55);
        motors.onForDegrees(50, 50, 320);
        pen_motor.onForDegrees(50, 55);
        motors.onForDegrees(50,  50,  265);
        motors.onForDegrees(50, -50,  190);
        motors.onForDegrees(50,  50, -265);
    }
    motors.onForDegrees(50,  50,  265);
    motors.onForDegrees(50, -50,  100);
    motors.onForDegrees(50,  50, -265);
    pen_motor.onForDegrees(50, -55);
    motors.onForDegrees(50, 50, 450);
    pen_motor.onForDegrees(50, 55);
    for (int i = 0; i != 2; ++i) {
        motors.onForDegrees(50,  50,  265);
        motors.onForDegrees(50, -50, -200);
        motors.onForDegrees(50,  50, -265);
        pen_motor.onForDegrees(50, -55);
        motors.onForDegrees(50, 50, 225);
        pen_motor.onForDegrees(50, 55);
    }
    motors.onForDegrees(50,  50,  265);
    motors.onForDegrees(50, -50, -200);
    motors.onForDegrees(50,  50, -265);
    pen_motor.onForDegrees(50, -55);
    motors.onForDegrees(50, 50, 450);
    pen_motor.onForDegrees(50, 55);
}
