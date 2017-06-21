/**
 * Follows black line on white background
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::ColorSensor color(ev3cxx::SensorPort::S3);
    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);

    while (true) {
        if (color.reflected() < 50) {
            motors.on(25, 0);
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::GREEN);
        } else {
            motors.on(0, 25);
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::RED);
        }
    }
}
