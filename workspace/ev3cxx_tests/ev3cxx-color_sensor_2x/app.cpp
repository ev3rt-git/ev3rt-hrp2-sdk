/**
 * This is sample program for testing two Color sensors in EV3RT C++ API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include <cstdlib>

#include "ev3cxx.h"
#include "app.h"

#define ATOMS_NO_EXCEPTION
#include <atoms/communication/avakar.h>
#include <atoms/numeric/rolling_average.h> 

using ev3cxx::display;
using ev3cxx::format;
using atoms::AvakarPacket;
using atoms::RollingAverage;

void calibrateSensor(int val, int& min, int& max) {
    if(max < val)
        max = val;
    if(min > val)
        min = val;
}

int getCalibratedValue(int val, int vMin, int vMax, int min, int max, bool clamp = false) {
    int result = ((val - vMin) * (float(max - min) / (vMax - vMin))) + min; 
    if(clamp) {
        if(min > result)
            return min;
        if(max < result)
            return max;
    }
    return result;
}

void main_task(intptr_t unused) {
    ev3cxx::Bluetooth bt{true};

    char welcomeString[] = "\tEV3RT ev3cxx \n\tColorSen2x test\nInitialization...\n";

    format(bt, "\n\n% ") % welcomeString;
    display.format("% ") % welcomeString;

    ev3cxx::ColorSensor leftS{ev3cxx::SensorPort::S1};
    ev3cxx::ColorSensor rightS{ev3cxx::SensorPort::S4};
    ev3cxx::BrickButton btnEnter(ev3cxx::BrickButtons::ENTER);

    AvakarPacket packetOut;
    int lVal, rVal, lMin, lMax, rMin, rMax;
    RollingAverage<int, 20> lAvgVal, rAvgVal;

    lMin = rMin = 100;
    lMax = rMax = 0;
    
    int error;
    const int threshold = 70;

    display.format("Sensor calibration\n");
    while(!btnEnter.isPressed()) {
        lVal = leftS.reflected();
        rVal = rightS.reflected();
        calibrateSensor(lVal, lMin, lMax);
        calibrateSensor(rVal, rMin, rMax);
        ev3cxx::delayMs(50);

        display.format("\r%3-%3 : %3-%3") % lMin % lMax % rMin % rMax;
        format(bt, "lMin:%3  lMax:%3\t") % lMin % lMax;
        format(bt, "rMin:%3  rMax:%3\n") % rMin % rMax; 
    }

    while(true) {
        lVal = leftS.reflected();
        rVal = rightS.reflected();
        int lCalVal = getCalibratedValue(lVal, lMin, lMax, 0, 100, true);
        int rCalVal = getCalibratedValue(rVal, rMin, rMax, 0, 100, true);

        lAvgVal.push(lCalVal);
        rAvgVal.push(rCalVal);

        lCalVal = lAvgVal.get_average();
        rCalVal = rAvgVal.get_average();

        display.format("\rL%3-%3 R%3-%3") % lVal % lCalVal % rVal % rCalVal;
        
        if(rCalVal > threshold || lCalVal > threshold)
            error = 100;
        else
            error = rCalVal - lCalVal;

        packetOut.set_command(0);
        // packetOut.push<uint8_t>(lVal);
        // packetOut.push<uint8_t>(rVal);
        // packetOut.push<uint8_t>(lVal + rVal);
        packetOut.push<uint8_t>(lCalVal);
        packetOut.push<uint8_t>(rCalVal);
        packetOut.push<uint8_t>(lCalVal + rCalVal);
        packetOut.push<uint8_t>(error);

        for(char ch: packetOut) {
           bt.write(ch); 
        }
        packetOut.clear();

        ev3cxx::delayMs(50);
    }
    

}
