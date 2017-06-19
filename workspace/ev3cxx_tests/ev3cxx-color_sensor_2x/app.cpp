/**
 * This is sample program for testing two Color sensors in EV3RT C++ API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

#define ATOMS_NO_EXCEPTION
#include "avakar.h"

using ev3cxx::display;
using ev3cxx::format;
using atoms::AvakarPacket;    

void main_task(intptr_t unused) {
    ev3cxx::Bluetooth bt{true};

    char welcomeString[] = "\tEV3RT ev3cxx \n\tColorSen2x test\nInitialization...\n";

    format(bt, "\n\n% ") % welcomeString;
    display.format("% ") % welcomeString;

    ev3cxx::ColorSensor leftS{ev3cxx::SensorPort::S1};
    ev3cxx::ColorSensor rightS{ev3cxx::SensorPort::S4};

    AvakarPacket packetOut;
    int lVal, rVal;

    while(true) {
        lVal = leftS.reflected();
        rVal = rightS.reflected();

        display.format("L: %4  R: %4\n") % lVal % rVal;
        
        packetOut.set_command(0);
        packetOut.push<uint8_t>(lVal);
        packetOut.push<uint8_t>(rVal);
        packetOut.push<uint8_t>(lVal + rVal);

        for(char ch: packetOut) {
           bt.write(ch); 
        }
        packetOut.clear();

        ev3cxx::delayMs(50);
    }
    

}
