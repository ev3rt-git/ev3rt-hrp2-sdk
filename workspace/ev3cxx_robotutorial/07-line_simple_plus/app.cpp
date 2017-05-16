/**
 * Follows black line on white background + add waiting for press touch sensor on start and after press touch sensor againt that stop loop 
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::TouchSensor touch(ev3cxx::SensorPort::S1);
    ev3cxx::ColorSensor color(ev3cxx::SensorPort::S3);
    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);

    touch.waitForPress();
    ev3cxx::wait(500); // 500 = 500 ms = 0.5 s

    while (touch.isPressed() == false) {
        if (color.getReflected() < 50) {
            motors.on(25, 0);
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::GREEN);
        } else {
            motors.on(0, 25);
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::RED);
        }
    }
}
