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
using ev3cxx::TouchSensor;
    
void main_task(intptr_t unused) {
    Bluetooth bt{true};

    format(bt, "\n\n\t EV3RT ev3cxx \n\t Sensors test\n\n"); 
    display.format("\tEV3RT ev3cxx \n\tSensors test\n");         

    TouchSensor TouchS{Sensor::Port::S1};
    UltrasonicSensor UltraS{Sensor::Port::S4};

    format(bt, "Waiting for click on Touch Sensor\n"); 
    display.format("Click => TouchS\n");
    
    TouchS.waitForClick();    
    
    format(bt, "Clicked\n"); 
    display.format("Clicked\n");
    
    tslp_tsk(2000);

    while(true) {
        format(bt, "touchS: %1\t") % (TouchS.isPressed() ? 'Y' : 'N');
        format(bt, "uS-metric:%4 cm -%5 mm\tuS-imperial:%4 in -%5 ln\n") 
            % UltraS.Centimeters() % UltraS.Millimeters() 
            % UltraS.Inches() % UltraS.InchesLine();
        
        display.setTextLine(2);
        display.format("touchS: %1 \n") % (TouchS.isPressed() ? 'Y' : 'N');
        display.format("uSmetric%4 %4\nuSimper %4 %4") 
            % UltraS.Centimeters() % UltraS.Millimeters() 
            % UltraS.Inches() % UltraS.InchesLine();;

        tslp_tsk(50);
    }
    

}
