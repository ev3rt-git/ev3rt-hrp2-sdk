/**
 * This is sample program for drive a simple two-wheel robot along a square.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);
    
    for (int i = 0; i != 4; ++i) {
        motors.onForRotations(50, 50, 1);
        motors.leftMotor().onForRotations(50, 1.1);
    }
}
