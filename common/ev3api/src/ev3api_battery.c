/*
 * EV3.c
 *
 *  Created on: Oct 17, 2013
 *      Author: liyixiao
 */

#include <kernel.h>
#include "ev3api.h"
#include "platform_interface_layer.h"
#include "api_common.h"

int ev3_battery_current_mA() {
    return adc_count_to_battery_current_mA(*_global_ev3_brick_info.battery_current);
}

int ev3_battery_voltage_mV() {
    return adc_count_to_battery_voltage_mV(*_global_ev3_brick_info.battery_current, *_global_ev3_brick_info.battery_voltage);
}
