/**
 * This is sample program for Sensor class in CPP EV3RT API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

using ev3cxx::display;
using ev3cxx::Bluetooth;
using ev3cxx::format;

using ev3cxx::Sensor;
using ev3cxx::UltrasonicSensor;
    
void main_task(intptr_t unused) {
    Bluetooth bt{true};

    format(bt, "\n\n\t EV3RT ev3cxx \n\t Sensors test\n\n"); 
    display.format("  EV3RT ev3cxx \n  Sensors test\n");         

    UltrasonicSensor UltraS{Sensor::Port::S4};

    while(true) {
        format(bt, "uS-metric:%4 cm -%5 mm     uS-imperial:%4 in -%5 ln\n") 
            % UltraS.Centimeters() % UltraS.Millimeters() 
            % UltraS.Inches() % UltraS.InchesLine();
        
        display.setTextLine(2);
        display.format( "Metric%4 -%5\nImper %4 -%5") 
            % UltraS.Centimeters() % UltraS.Millimeters() 
            % UltraS.Inches() % UltraS.InchesLine();;

        tslp_tsk(50);
    }
    

}
