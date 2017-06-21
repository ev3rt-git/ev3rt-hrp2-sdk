/*
 * Sensor.c
 *
 *  Created on: Oct 17, 2013
 *      Author: liyixiao
 */

#include "ev3api.h"

#include <t_stddef.h>
#include <t_syslog.h>
#include <string.h>
#include "platform_interface_layer.h"
#include "api_common.h"

/**
 * Check whether a port number is valid
 */
#define CHECK_PORT(port) CHECK_COND((port) >= EV3_PORT_1 && (port) <= EV3_PORT_4, E_ID)

/*
 * Device type of sensors
 * NONE_SENSOR = not connected
 */
static sensor_type_t sensors[TNUM_SENSOR_PORT];

static const uart_data_t *pUartSensorData = NULL;
static const analog_data_t *pAnalogSensorData = NULL;
static const i2c_data_t *pI2CSensorData = NULL;

/**
 * Functions to get data from analog sensor
 */

static inline
int16_t analog_sensor_get_pin1(sensor_port_t port) {
	return pAnalogSensorData[port].pin1[*pAnalogSensorData[port].actual];
}

static inline
int16_t analog_sensor_get_pin6(sensor_port_t port) {
	return pAnalogSensorData[port].pin6[*pAnalogSensorData[port].actual];
}

/**
 * Fetch data from a UART sensor. If size is 0, switch mode only.
 */
static
void uart_sensor_fetch_data(sensor_port_t port, uint8_t mode, void *dest, SIZE size) {
	ER ercd;

	// Switch mode
//	uart_sensor_switch_mode(port, mode);
	ercd = uart_sensor_config(port, mode);
	assert(ercd == E_OK);
//	while(!uart_sensor_data_ready(port));
	while(!((*pUartSensorData[port].status) & UART_DATA_READY)); // TODO: time out

	// Copy data
	if (size > 0) {
		void *raw = (void*)(pUartSensorData[port].raw[*pUartSensorData[port].actual]); // TODO: check this, discard volatile
		memcpy(dest, raw, size);
//		memcpy(dest, (void*)uart_sensor_get_raw(port), size);
	}
}

void _initialize_ev3api_sensor() {
	// TODO: Thread safe
	if (pUartSensorData == NULL) {
		sensors[EV3_PORT_1]   = NONE_SENSOR;
		sensors[EV3_PORT_2]   = NONE_SENSOR;
		sensors[EV3_PORT_3]   = NONE_SENSOR;
		sensors[EV3_PORT_4]   = NONE_SENSOR;
		brickinfo_t brickinfo;
		ER ercd = fetch_brick_info(&brickinfo);
		assert(ercd == E_OK);
		pUartSensorData = brickinfo.uart_sensors;
		pAnalogSensorData = brickinfo.analog_sensors;
		pI2CSensorData = brickinfo.i2c_sensors;
		assert(pUartSensorData != NULL);
		assert(pAnalogSensorData != NULL);
		assert(pI2CSensorData != NULL);
	}
}

ER ev3_sensor_config(sensor_port_t port, sensor_type_t type) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);

	sensors[port] = type;

	switch (type) {
	case NONE_SENSOR:
        // Do nothing
		break;

	case TOUCH_SENSOR: {
        // It seems analog sensor can't work correctly in I2C mode
    	ercd = uart_sensor_config(port, 0);
    	assert(ercd == E_OK);
        /* Busy wait 2ms to ensure that sensor value is updated */
        SYSTIM start, now;
        get_tim(&start);
        do { get_tim(&now); } while (now - start < 2);
        }
        break;

    case ULTRASONIC_SENSOR:
    case GYRO_SENSOR:
    case COLOR_SENSOR:
    case INFRARED_SENSOR:
        // Configure UART sensor
    	ercd = uart_sensor_config(port, 0);
    	assert(ercd == E_OK);
        // Wait UART sensor to finish initialization
//    	while(!uart_sensor_data_ready(port));
		break;

    case HT_NXT_ACCEL_SENSOR:
    case NXT_TEMP_SENSOR:
    	ercd = uart_sensor_config(port, 0xFF /* TODO:MODE_NONE_UART_SENSOR */);
    	assert(ercd == E_OK);
        break;

	default:
		API_ERROR("Invalid sensor type %d", type);
		return E_PAR;
	}

	ercd = E_OK;

error_exit:
	return ercd;
}

ER_UINT ev3_sensor_get_type(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);

	return sensors[port];

error_exit:
	return ercd;
}

typedef enum {
	COL_REFLECT = 0,
	COL_AMBIENT = 1,
	COL_COLOR   = 2,
	COL_RGBRAW  = 4,
} COLOR_SENSOR_MODES;

colorid_t ev3_color_sensor_get_color(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == COLOR_SENSOR, E_OBJ);

	colorid_t val;
	uart_sensor_fetch_data(port, COL_COLOR, &val, sizeof(val));
	assert(val >= COLOR_NONE && val < TNUM_COLOR);
    return val;

error_exit:
	syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return COLOR_NONE;
}

uint8_t ev3_color_sensor_get_reflect(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == COLOR_SENSOR, E_OBJ);

	uint8_t val;
	uart_sensor_fetch_data(port, COL_REFLECT, &val, sizeof(val));
    return val;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return 0;
}

uint8_t ev3_color_sensor_get_ambient(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == COLOR_SENSOR, E_OBJ);

	uint8_t val;
	uart_sensor_fetch_data(port, COL_AMBIENT, &val, sizeof(val));
    return val;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return 0;
}

void ev3_color_sensor_get_rgb_raw(sensor_port_t port, rgb_raw_t *val) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == COLOR_SENSOR, E_OBJ);

	uart_sensor_fetch_data(port, COL_RGBRAW, val, sizeof(rgb_raw_t));

    return;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
}

typedef enum {
	GYRO_ANG  = 0,
	GYRO_RATE = 1,
	GYRO_GnA  = 3,
	GYRO_CAL  = 4,
} GYRO_SENSOR_MODES;

int16_t ev3_gyro_sensor_get_angle(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == GYRO_SENSOR, E_OBJ);

	int16_t val;
	uart_sensor_fetch_data(port, GYRO_ANG, &val, sizeof(val));
    return val;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return 0;
}

int16_t ev3_gyro_sensor_get_rate(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == GYRO_SENSOR, E_OBJ);

	int16_t val;
	uart_sensor_fetch_data(port, GYRO_RATE, &val, sizeof(val));
    return val;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return 0;
}

ER ev3_gyro_sensor_reset(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == GYRO_SENSOR, E_OBJ);

	//uart_sensor_switch_mode(port, GYRO_CAL);
	uart_sensor_fetch_data(port, GYRO_CAL, NULL, 0);

	return E_OK;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return ercd;
}

typedef enum {
	US_DIST_CM = 0,
	US_DIST_IN = 1,
	US_LISTEN  = 2,
} ULTRASONIC_SENSOR_MODES;

int16_t ev3_ultrasonic_sensor_get_raw_data(sensor_port_t port, ULTRASONIC_SENSOR_MEASURE_MODES mode) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == ULTRASONIC_SENSOR, E_OBJ);

#if 0
    return ev3_uart_sensor_get_short(port) / 10;
#endif
	int16_t val;
	uart_sensor_fetch_data(port, mode, &val, sizeof(val));
    return val;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return 0;
}

int16_t ev3_ultrasonic_sensor_get_distance(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == ULTRASONIC_SENSOR, E_OBJ);

#if 0
    return ev3_uart_sensor_get_short(port) / 10;
#endif
	int16_t val;
	uart_sensor_fetch_data(port, US_DIST_CM, &val, sizeof(val));
    return val / 10;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return 0;
}

bool_t ev3_ultrasonic_sensor_listen(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == ULTRASONIC_SENSOR, E_OBJ);

	// TODO: TEST THIS API!
	uint8_t val;
	uart_sensor_fetch_data(port, US_LISTEN, &val, sizeof(val));
    return val;

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return false;
}

typedef enum {
	IR_DIST   = 0,
	IR_SEEK   = 1,
	IR_REMOTE = 2,
} INFRARED_SENSOR_SENSOR_MODES;

int8_t ev3_infrared_sensor_get_distance(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == INFRARED_SENSOR, E_OBJ);

	int8_t val;
	uart_sensor_fetch_data(port, IR_DIST, &val, sizeof(val));
	return val;

error_exit:
	syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
	return 0;
}

ir_seek_t ev3_infrared_sensor_seek(sensor_port_t port) {
	ir_seek_t result;
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == INFRARED_SENSOR, E_OBJ);

	int8_t val[8];
	uart_sensor_fetch_data(port, IR_SEEK, &val, 8 * sizeof(int8_t));
	for (int i = 0; i < 4; i++) {
		result.heading [i] = val[2 * i];
		result.distance[i] = val[2 * i + 1];
	}
	return result;

error_exit:
	syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
	for (int i = 0; i < 4; i++) {
		result.heading [i] = 0;
		result.distance[i] = -128;
	}
	return result;
}

ir_remote_t ev3_infrared_sensor_get_remote(sensor_port_t port) {
	ir_remote_t result;
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == INFRARED_SENSOR, E_OBJ);

	uint8_t val[4];
	uart_sensor_fetch_data(port, IR_REMOTE, &val, 4 * sizeof(uint8_t));
	for (int i = 0; i < 4; i++) {
		switch(val[i])
		{
		case 0:  // no buttons pressed
			result.channel[i] = 0;
			continue;
		case 1:  // red up
			result.channel[i] = IR_RED_UP_BUTTON;
			continue;
		case 2:  // red down
			result.channel[i] = IR_RED_DOWN_BUTTON;
			continue;
		case 3:  // blue up
			result.channel[i] = IR_BLUE_UP_BUTTON;
			continue;
		case 4:  // blue down
			result.channel[i] = IR_BLUE_DOWN_BUTTON;
			continue;
		case 5:  // red up and blue up
			result.channel[i] = IR_RED_UP_BUTTON + IR_BLUE_UP_BUTTON;
			continue;
		case 6:  // red up and blue down
			result.channel[i] = IR_RED_UP_BUTTON + IR_BLUE_DOWN_BUTTON;
			continue;
		case 7:  // red down and blue up
			result.channel[i] = IR_RED_DOWN_BUTTON + IR_BLUE_UP_BUTTON;
			continue;
		case 8:  // red down and blue down
			result.channel[i] = IR_RED_DOWN_BUTTON + IR_BLUE_DOWN_BUTTON;
			continue;
		case 9:  // beacon mode on
			result.channel[i] = IR_BEACON_BUTTON;
			continue;
		case 10: // red up and red down
			result.channel[i] = IR_RED_UP_BUTTON + IR_RED_DOWN_BUTTON;
			continue;
		case 11: // blue up and blue down
			result.channel[i] = IR_BLUE_UP_BUTTON + IR_BLUE_DOWN_BUTTON;
		}
	}
	return result;

error_exit:
	syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
	for (int i = 0; i < 4; i++) {
		result.channel[i] = 0;
	}
	return result;
}

bool_t ev3_touch_sensor_is_pressed(sensor_port_t port) {
	ER ercd;

//	lazy_initialize();
	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == TOUCH_SENSOR, E_OBJ);

	int16_t val = analog_sensor_get_pin6(port);
    return val > (ADC_RES / 2);

error_exit:
    syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
    return false;
}

bool_t ht_nxt_accel_sensor_measure(sensor_port_t port, int16_t axes[3]) {
	ER ercd;

	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == HT_NXT_ACCEL_SENSOR, E_OBJ);
	CHECK_COND(*pI2CSensorData[port].status == I2C_TRANS_IDLE, E_OBJ);

	for (int i = 0; i < 3; i++) {
		axes[i] = pI2CSensorData[port].raw[i];
		axes[i] = ((axes[i] < 128 ? axes[i] : axes[i] - 256) << 2) | (pI2CSensorData[port].raw[i + 3] & 0x3);
	}

	ercd = start_i2c_transaction(port, 0x1, "\x42", 1, 6);
	assert(ercd == E_OK);

	return true;

error_exit:
	syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
	return false;
}

bool_t nxt_temp_sensor_measure(sensor_port_t port, float *temp) {
	ER ercd;

	CHECK_PORT(port);
	CHECK_COND(ev3_sensor_get_type(port) == NXT_TEMP_SENSOR, E_OBJ);
	CHECK_COND(*pI2CSensorData[port].status == I2C_TRANS_IDLE, E_OBJ);

    //syslog(LOG_EMERG, "TEMP RAW: 0x%x 0x%x", pI2CSensorData[port].raw[0], pI2CSensorData[port].raw[1]);
    int16_t raw = pI2CSensorData[port].raw[0];
    raw = ((raw < 128 ? raw : raw - 256) << 4) | ((pI2CSensorData[port].raw[1] >> 4) & 0xF);
    *temp = raw * 0.0625f;

	ercd = start_i2c_transaction(port, 0x4c, "\x0", 1, 2);
	assert(ercd == E_OK);

	return true;

error_exit:
	syslog(LOG_WARNING, "%s(): ercd %d", __FUNCTION__, ercd);
	return false;
}

#if 0 // Legacy code

short ev3_uart_sensor_get_short(sensor_port_t port) {
    volatile const int8_t* raw = uart_sensor_get_raw(port);
    short res = (unsigned short)raw[0];
    res |= ((unsigned short)raw[1]) << 8;
    return res;
}

void ev3_sensors_init(sensor_type_t type1, sensor_type_t type2, sensor_type_t type3, sensor_type_t type4) {
	ER ercd;

	if (has_inited) {
		API_ERROR("Sensor ports already initialized");
		return;
	} else
		has_inited = true;

	sensors[Port1] = type1;
	sensors[Port2] = type2;
	sensors[Port3] = type3;
	sensors[Port4] = type4;

	for (int i = Port1; i < TNUM_SENSOR_PORT; ++i) {
		switch (sensors[i]) {
		case NONE_SENSOR:
		case TOUCH_SENSOR:
			// Do nothing for analog sensor or no sensor
			break;

	    case ULTRASONIC_SENSOR:
	    case INFRARED_SENSOR:
	    case GYRO_SENSOR:
	    case COLOR_SENSOR:
	        // Configure UART sensor
	    	ercd = uart_sensor_config(i, 0);
	    	assert(ercd == E_OK);
			break;

		default:
			API_ERROR("Invalid sensor type %d", sensors[i]);
			return;
		}
	}

	pUartSensorData = driver_data_pointer(DPID_UART_SENSOR);
	pAnalogSensorData = driver_data_pointer(DPID_ANALOG_SENSOR);

	for (int i = Port1; i < TNUM_SENSOR_PORT; ++i) {
		switch (sensors[i]) {
	    case ULTRASONIC_SENSOR:
	    case GYRO_SENSOR:
	    case COLOR_SENSOR:
	        // Wait UART sensor to finish initialization
	    	while(!uart_sensor_data_ready(i));
			break;

	    default:
	    	break;
		}
	}

//	dump_uart_sensor_types();
}

void ev3_sensor_config(sensor_port_t port, sensor_type_t type) {
	ER ercd;

	if (!has_inited) {
		API_ERROR("Sensor ports must be initialized before reconfiguration");
		return;
	}

	sensors[port] = type;

	switch (type) {
	case NONE_SENSOR:
	case TOUCH_SENSOR:
		// Do nothing for analog sensor or no sensor
		break;

    case ULTRASONIC_SENSOR:
    case INFRARED_SENSOR:
    case GYRO_SENSOR:
    case COLOR_SENSOR:
        // Configure UART sensor
    	ercd = uart_sensor_config(port, 0);
    	assert(ercd == E_OK);
        // Wait UART sensor to finish initialization
    	while(!uart_sensor_data_ready(port));
		break;

	default:
		API_ERROR("Invalid sensor type %d", type);
		return;
	}
}


/**
 * Interface to check the data-ready status of a UART sensor port.
 * @param port a sensor port
 * @retval true  UART_DATA_READY
 * @retval false !UART_DATA_READY
 */
static inline
bool_t uart_sensor_data_ready(sensor_port_t port) {
	return (*pUartSensorData[port].status) & UART_DATA_READY;
}

static inline
void uart_sensor_switch_mode(sensor_port_t port, uint8_t mode) {
	ER ercd = uart_sensor_config(port, mode);
	assert(ercd == E_OK);
	while(!uart_sensor_data_ready(port));
}

/**
 * Interface to get the latest raw data of a UART sensor port.
 * @param port a sensor port
 * @return a pointer to the raw data
 */
static inline
volatile const int8_t* uart_sensor_get_raw(sensor_port_t port) {
	return pUartSensorData[port].raw[*pUartSensorData[port].actual];
}


#endif
