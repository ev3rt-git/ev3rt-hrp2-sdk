/**
 * Go straight until an obstacle is detected by ultrasonic sensor, capture it and return to the start position
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::UltrasonicSensor ultrasonic(ev3cxx::SensorPort::S4);
    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);
    ev3cxx::Motor claw_motor(ev3cxx::MotorPort::A);

    claw_motor.onForTime(30, 500);
    motors.on(50, 50);

    while (ultrasonic.Centimeters() >= 4) {};

    motors.off(true);
    claw_motor.onForTime(-30, 500);
    motors.onForDegrees(-50, -50, motors.leftMotor().degrees());
}
