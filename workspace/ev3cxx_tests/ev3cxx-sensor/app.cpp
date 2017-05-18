/**
 * This is sample program for Sensor class in EV3RT C++ API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

using ev3cxx::display;
using ev3cxx::format;
    
void main_task(intptr_t unused) {
    ev3cxx::Bluetooth bt{true};

    format(bt, "\n\n\t EV3RT ev3cxx \n\t Sensors test\n\nInitialization...\n"); 
    display.format("\tEV3RT ev3cxx \n\tSensors test\nInitialization...\n");         

    ev3cxx::TouchSensor TouchS{ev3cxx::SensorPort::S1};
    ev3cxx::GyroSensor GyroS{ev3cxx::SensorPort::S2};
    ev3cxx::ColorSensor ColorS{ev3cxx::SensorPort::S3};
    ev3cxx::UltrasonicSensor UltraS{ev3cxx::SensorPort::S4};

    format(bt, "Waiting for click on Touch Sensor\n"); 
    display.format("Click => TouchS\n");
    
    TouchS.waitForClick();    
    
    format(bt, "Clicked\n"); 
    display.format("Clicked\n");
    
    ev3cxx::delayMs(2000);

    ev3cxx::MotorTank motors(ev3cxx::MotorPort::B, ev3cxx::MotorPort::B);
    motors.on(70, 70);

    while(true) {
        // BT
        format(bt, "touchS: %1\t") % (TouchS.isPressed() ? 'Y' : 'N');
        
        // Gyro - you can't use angle() and rate() in same time => call rate() reset initial position in angle()
        format(bt, "Gyro-angle:%5\t") % GyroS.angle();
        //format(bt, "Gyro-rate:%5\t") % GyroS.rate();
        
        format(bt, "Cs-ref:%3 Cs-amb:%3\t") 
            % ColorS.reflected() % ColorS.ambient();
        
        format(bt, "Us-metric:%4 cm -%5 mm\tUs-imperial:%4 in -%5 ln\n") 
            % UltraS.centimeters() % UltraS.millimeters() 
            % UltraS.inches() % UltraS.inchesLine();
        
        // DISPLAY
        display.setTextLine(2);
        display.format("touchS: %1 \n") % (TouchS.isPressed() ? 'Y' : 'N');
        
        display.format("gyroS-angle:%5\n") % GyroS.angle();
        //display.format("Gs-rate:%5\n") % GyroS.rate();
        
        display.format("Cs-r:%3 Cs-a:%3\n") 
            % ColorS.reflected() % ColorS.ambient();
        
        display.format("U-metric%4 %4\nU-imper %4 %4") 
            % UltraS.centimeters() % UltraS.millimeters() 
            % UltraS.inches() % UltraS.inchesLine();;

        ev3cxx::delayMs(50);
    }
    

}
