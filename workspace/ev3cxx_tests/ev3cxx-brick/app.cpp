/**
 * This is sample program for Brick components (LEDs, buttons) in EV3RT C++ API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

using ev3cxx::display;
using ev3cxx::format;
    
void main_task(intptr_t unused) {
    ev3cxx::BrickButton btnUP(ev3cxx::BrickButtons::UP);
    
    while (true) {
        ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::RED);

        if(btnUP.isPressed()) {
            ev3cxx::statusLight.setColor(ev3cxx::StatusLightColor::GREEN);
        }
        ev3cxx::delayMs(50);
    }

}
