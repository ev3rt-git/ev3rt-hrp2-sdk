/**
 * Lights green LED if TouchSensor is pressed, otherwise lights red LED.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

void main_task(intptr_t unused) {
    ev3cxx::TouchSensor touch(ev3cxx::SensorPort::S1);
    
    while (true) {
        if (touch.isPressed())
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::GREEN);
        else
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::RED);
    }
}
