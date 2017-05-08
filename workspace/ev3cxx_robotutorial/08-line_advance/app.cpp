/**
 * Follows black line on white background
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::ColorSensor sensor(ev3cxx::SensorPort::S3);
    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);
    
    const int line_center = 50;
    const float turn_speed = 0.5;
    const int forward_speed = 20;
    int turn = 0;

    while (true) {
        turn = (line_center - sensor.getReflected()) * turn_speed;
        motors.on(forward_speed + turn, forward_speed - turn);
    }
}
