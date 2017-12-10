/*
 * Motor.c
 *
 *  Created on: Oct 17, 2013
 *      Author: liyixiao
 */

#include "ev3api.h"
#include "platform_interface_layer.h"
#include "api_common.h"
#include <string.h>

/**
 * TODO: Undocumented source code from 'lms2012.h'
 */

typedef int8_t DATA8;
typedef int16_t DATA16;
typedef int32_t DATA32;

typedef   enum
{
//  TYPE_KEEP                     =   0,  //!< Type value that won't change type in byte codes
  TYPE_NXT_TOUCH                =   1,  //!< Device is NXT touch sensor
  TYPE_NXT_LIGHT                =   2,  //!< Device is NXT light sensor
  TYPE_NXT_SOUND                =   3,  //!< Device is NXT sound sensor
  TYPE_NXT_COLOR                =   4,  //!< Device is NXT color sensor

  TYPE_TACHO                    =   7,  //!< Device is a tacho motor
  TYPE_MINITACHO                =   8,  //!< Device is a mini tacho motor
  TYPE_NEWTACHO                 =   9,  //!< Device is a new tacho motor

  TYPE_THIRD_PARTY_START        =  50,
  TYPE_THIRD_PARTY_END          =  99,

  TYPE_IIC_UNKNOWN              = 100,

  TYPE_NXT_TEST                 = 101,  //!< Device is a NXT ADC test sensor

  TYPE_NXT_IIC                  = 123,  //!< Device is NXT IIC sensor
  TYPE_TERMINAL                 = 124,  //!< Port is connected to a terminal
  TYPE_UNKNOWN                  = 125,  //!< Port not empty but type has not been determined
  TYPE_NONE                     = 126,  //!< Port empty or not available
  TYPE_ERROR                    = 127,  //!< Port not empty and type is invalid
}
TYPE;

typedef   enum
{
  opOUTPUT_GET_TYPE           = 0xA0, //     00000
  opOUTPUT_SET_TYPE           = 0xA1, //     00001
  opOUTPUT_RESET              = 0xA2, //     00010
  opOUTPUT_STOP               = 0xA3, //     00011
  opOUTPUT_POWER              = 0xA4, //     00100
  opOUTPUT_SPEED              = 0xA5, //     00101
  opOUTPUT_START              = 0xA6, //     00110
  opOUTPUT_POLARITY           = 0xA7, //     00111
  opOUTPUT_READ               = 0xA8, //     01000
  opOUTPUT_TEST               = 0xA9, //     01001
  opOUTPUT_READY              = 0xAA, //     01010
  opOUTPUT_POSITION           = 0xAB, //     01011
  opOUTPUT_STEP_POWER         = 0xAC, //     01100
  opOUTPUT_TIME_POWER         = 0xAD, //     01101
  opOUTPUT_STEP_SPEED         = 0xAE, //     01110
  opOUTPUT_TIME_SPEED         = 0xAF, //     01111

  opOUTPUT_STEP_SYNC          = 0xB0, //     10000
  opOUTPUT_TIME_SYNC          = 0xB1, //     10001
  opOUTPUT_CLR_COUNT          = 0xB2, //     10010
  opOUTPUT_GET_COUNT          = 0xB3, //     10011

  opOUTPUT_PRG_STOP           = 0xB4, //     10100

}
OP;

typedef enum {
    STOP_FLOAT = 0,
    STOP_BRAKE = 1,
} stop_type;

typedef struct {
  DATA8   Cmd;
  DATA8   Nos;
  DATA8   PowerOrSpeed;
  DATA32  StepOrTime1;
  DATA32  StepOrTime2;
  DATA32  StepOrTime3;
  DATA8   Brake;
} cmd_step_time;

typedef struct {
  DATA8   Cmd;
  DATA8   Nos;
  DATA8   Speed;
  DATA16  Turn;
  DATA32  StepOrTime;
  DATA8   Brake;
} cmd_sync;

typedef struct
{
  DATA8   Cmd;
  DATA8   Nos;
  DATA8   Speed;
  DATA32  Step1;
  DATA32  Step2;
  DATA32  Step3;
  DATA8   Brake;
} STEPSPEED;

typedef struct
{
  DATA8   Cmd;
  DATA8   Nos;
  DATA8   Speed;
  DATA16  Turn;
  DATA32  Step;
  DATA8   Brake;
} STEPSYNC;

/**
 * Check whether a port number is valid
 */
#define CHECK_PORT(port) CHECK_COND((port) >= EV3_PORT_A && (port) <= EV3_PORT_D, E_ID)
#define CHECK_MASK(mask) CHECK_COND((mask & ~0x0F) == 0, E_ID)
#define CHECK_PORT_DIFFERENT(portA,portB) CHECK_COND(portA != portB, E_ILUSE)

/**
 * Check whether a port is connected (or initialized)
 */
#define CHECK_PORT_CONN(port) CHECK_COND(getDevType(mts[(port)]) != TYPE_NONE, E_OBJ)
#define CHECK_MASK_CONN(mask) do {                          \
        for(int _i = EV3_PORT_A; _i <= EV3_PORT_D; _i++) {  \
            if (mask & (1 << _i)) {                         \
                CHECK_PORT_CONN(_i);                        \
            }                                               \
        }                                                   \
    } while (0)

#define CHECK_MOTOR_TYPE(type) CHECK_COND(type >= NONE_MOTOR && type <= (NONE_MOTOR + TNUM_MOTOR_TYPE - 1), E_PAR)
#define CHECK_RDACCESS(pointer) CHECK_COND(pointer != NULL, E_PAR)
#define CHECK_WRACCESS(pointer) CHECK_COND(pointer != NULL, E_PAR)
#define CLIP(dst,src,min,max) do {  \
    if ((src) < (min)) {            \
        (dst) = (min);              \
    } else if ((src) > (max)) {     \
        (dst) = (max);              \
    } else {                        \
        (dst) = (src);              \
    }                               \
} while(0)

/**
 * Type of motors
 */
static motor_type_t mts[TNUM_MOTOR_PORT];

static const motor_data_t *pMotorData = NULL;
static volatile const uint8_t *pMotorReadyStatus = NULL;

static inline
int getDevType(motor_type_t type) {
	switch(type) {
	case NONE_MOTOR:
		return TYPE_NONE;
		break;

	case MEDIUM_MOTOR:
		return TYPE_MINITACHO;
		break;

	case LARGE_MOTOR:
	case UNREGULATED_MOTOR: // TODO: this is just wrong, but let's keep it here for compatibility
		return TYPE_TACHO;
		break;

	default:
		API_ERROR("Invalid motor type %d", type);
		return TYPE_NONE;
	}
}

void _initialize_ev3api_motor() {
	// TODO: Thread safe
	assert(pMotorData == NULL);
	if (pMotorData == NULL) {
		mts[EV3_PORT_A]   = NONE_MOTOR;
		mts[EV3_PORT_B]   = NONE_MOTOR;
		mts[EV3_PORT_C]   = NONE_MOTOR;
		mts[EV3_PORT_D]   = NONE_MOTOR;
		brickinfo_t brickinfo;
		ER ercd = fetch_brick_info(&brickinfo);
		assert(ercd == E_OK);
		pMotorReadyStatus = brickinfo.motor_ready;
		pMotorData        = brickinfo.motor_data;
		assert(pMotorData != NULL);
		assert(pMotorReadyStatus != NULL);
	}
}

ER loadMotors(motor_type_t *types) {
    ER           ercd  = E_SYS;
    motor_type_t utype;
    int          ktype;
    int          prt;
    char         buf[1 + TNUM_MOTOR_PORT];

    CHECK_RDACCESS(types);

    buf[0] = opOUTPUT_SET_TYPE;

    for (prt = EV3_PORT_A; prt <= EV3_PORT_D; prt++) {
        utype = types[prt];
        CHECK_MOTOR_TYPE(utype);
    }

    for (prt = EV3_PORT_A; prt <= EV3_PORT_D; prt++) {
        utype = types[prt];
        ktype = getDevType(utype);

        mts[prt]   = utype;
        buf[prt+1] = ktype;
    }
    motor_command(buf, sizeof(buf));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_config(motor_port_t port, motor_type_t type) {
    ER ercd;

    motor_type_t copy[TNUM_MOTOR_PORT];
    memcpy(copy, mts, TNUM_MOTOR_PORT * sizeof(motor_type_t));
    copy[port] = type;

    ercd = loadMotors(copy);

    if (ercd != E_OK)
        goto error_exit;

    ercd = ev3_motor_ex_stop(1 << port, false);

error_exit:
    return ercd;
}

ER ev3_motor_config_all(motor_type_t *types) {
    ER ercd;

    ercd = loadMotors(types);

    if (ercd != E_OK)
        goto error_exit;
    /*
     * Set initial state to IDLE
     */
    ercd = ev3_motor_ex_stop(0x0F, false);

error_exit:
    return ercd;
}



ER_UINT ev3_motor_get_type(motor_port_t port) {
	ER ercd;

	CHECK_PORT(port);

//	lazy_initialize();

	return mts[port];

error_exit:
	return ercd;
}

int32_t ev3_motor_get_counts(motor_port_t port) {
	// TODO: Should use ER & pointer instead ?
	ER ercd;

//	lazy_initialize();

	CHECK_PORT(port);
	CHECK_PORT_CONN(port);

	return *pMotorData[port].tachoSensor;

error_exit:
	assert(ercd != E_OK);
	syslog(LOG_ERROR, "%s(): Failed to get motor counts, ercd: %d", __FUNCTION__, ercd);
	return 0;
}

int ev3_motor_get_power(motor_port_t port) {
	// TODO: Should use ER & pointer instead ?
	ER ercd;

//	lazy_initialize();

	CHECK_PORT(port);
	CHECK_PORT_CONN(port);

	return *pMotorData[port].speed;

error_exit:
	assert(ercd != E_OK);
	syslog(LOG_ERROR, "%s(): Failed to get motor power, ercd: %d", __FUNCTION__, ercd);
	return 0;
}

ER ev3_motor_reset_counts(motor_port_t port) {
    return ev3_motor_ex_reset_counts(1 << port);
}

ER ev3_motor_get_velocity(motor_port_t port, int *pVal) {
    ER ercd;

    CHECK_PORT(port);
    CHECK_PORT_CONN(port);
    CHECK_WRACCESS(pVal);

    /**
     * Use the shmem-provided tacho derivative
     */
    *pVal = *pMotorData[port].speed;

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_set_power(motor_port_t port, int power) {
    ER ercd;

    CHECK_PORT(port);
    CHECK_PORT_CONN(port);

    motor_type_t mt = mts[port];
    if (mt == UNREGULATED_MOTOR) {
        ercd = ev3_motor_ex_setpower(1 << port, power);
    } else {
        ercd = ev3_motor_ex_setspeed(1 << port, power);
    }

    if (ercd != E_OK)
        goto error_exit;

    ercd = ev3_motor_ex_start(1 << port);

error_exit:
    return ercd;
}

ER ev3_motor_stop(motor_port_t port, bool_t brake) {
    return ev3_motor_ex_stop(1 << port, brake);
}

ER ev3_motor_rotate(motor_port_t port, int degrees, uint32_t speed_abs, bool_t blocking) {
    ER ercd;
    ev3_motor_ex_moveparams_t params;

    CHECK_PORT(port);
    CHECK_PORT_CONN(port);

    motor_mask_t mask = 1 << port;

    ER_UINT run = ev3_motor_ex_running(mask);
    if (run < 0) {
        ercd = run;
    } else if (run) {
        ev3_motor_ex_stop(mask, false);
    }

    int sgnDeg = (degrees < 0) ? -1 : 1;
    int absDeg = degrees * sgnDeg;

    params.speed    = speed_abs * sgnDeg;
    params.rampup   = 0;
    params.sustain  = absDeg;
    params.rampdown = 0;
    params.brake    = true;
    ercd = ev3_motor_ex_reg_step(mask, &params);

    if (ercd != E_OK)
        goto error_exit;

    if (blocking) {
        ercd = ev3_motor_ex_poll(mask);
    } else {
        ercd = E_OK;
    }

error_exit:
    return ercd;
}

ER ev3_motor_steer(motor_port_t left_motor, motor_port_t right_motor, int power, int turn_ratio) {
    ev3_motor_ex_syncparams_t params;

    params.speed      = power;
    CLIP(params.turn_ratio, turn_ratio, INT16_MIN, INT16_MAX);
    params.length     = 0;
    params.brake      = false;

    return ev3_motor_ex_sync_step(left_motor, right_motor, &params);
}

/*
 * Extended API
 */

ER ev3_motor_ex_reset_counts(motor_mask_t mask) {
    ER ercd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);

    char buf[2];

    /**
     * Reset the counts when used as a motor.
     * Useful when the position of a motor is changed by hand.
     */
    buf[0] = opOUTPUT_RESET;
    buf[1] = mask;
    motor_command(buf, sizeof(buf));

    /**
     * Reset then counts when used as a tacho sensor.
     */
    buf[0] = opOUTPUT_CLR_COUNT;
    buf[1] = mask;
    motor_command(buf, sizeof(buf));

    ercd = E_OK;

error_exit:
    return ercd;
}

ER ev3_motor_ex_setpower(motor_mask_t mask, int8_t power) {
    char buf[3];
    ER ercd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);
    CLIP(power, power, -100, 100);

    buf[0] = opOUTPUT_POWER;
    buf[1] = mask;
    buf[2] = power;
    motor_command(buf, sizeof(buf));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_ex_setspeed(motor_mask_t mask, int8_t speed) {
    ER ercd;
    char buf[3];

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);
    CLIP(speed, speed, -100, 100);

    buf[0] = opOUTPUT_SPEED;
    buf[1] = mask;
    buf[2] = speed;
    motor_command(buf, sizeof(buf));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_ex_start(motor_mask_t mask) {
    ER ercd;
    char buf[2];

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);

    buf[0] = opOUTPUT_START;
    buf[1] = mask;
    motor_command(buf, sizeof(buf));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_ex_stop(motor_mask_t mask, bool_t brake) {
    ER ercd;
    char buf[3];

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);

    buf[0] = opOUTPUT_STOP;
    buf[1] = mask;
    buf[2] = brake ? 0x01 : 0x00;
    motor_command(buf, sizeof(buf));

    ercd = E_OK;
error_exit:
    return ercd;
}

/*
 * Motor state
 */
ER_UINT ev3_motor_ex_running(motor_mask_t mask) {
    ER ercd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);

    return ~(*pMotorReadyStatus) & mask;

error_exit:
    return ercd;
}

ER ev3_motor_ex_poll(motor_mask_t mask) {
    ER ercd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);

    while((*pMotorReadyStatus & mask) != 0) {
        // busy loop
    }

    ercd = E_OK;
error_exit:
    return ercd;
}

/*
 * Regulated moves
 */

#ifndef ABS
#define ABS(v)  ((v) < 0 ? -(v) : (v))
#endif

void fillMoveStruct(uint8_t op, motor_mask_t mask, const const ev3_motor_ex_moveparams_t *in, cmd_step_time *out) {
    int sgnSpd = (in->speed >= 0) ? 1 : -1;

    out->Cmd          = op;
    out->Nos          = mask;
    CLIP(out->PowerOrSpeed, in->speed, -100, 100);
    CLIP(out->StepOrTime1,  in->rampup,   0, INT32_MAX);
    CLIP(out->StepOrTime2,  in->sustain,  0, INT32_MAX);
    CLIP(out->StepOrTime3,  in->rampdown, 0, INT32_MAX);
    out->StepOrTime1 *= sgnSpd;
    out->StepOrTime2 *= sgnSpd;
    out->StepOrTime3 *= sgnSpd;
    out->Brake        = in->brake ? 0x01 : 0x00;
}

/*
 * TODO: documented this
 * -100 <= speed <= 100, -100(left) <= turn_ration <= 100(right)
 * Motor with smaller port number will be treated as left motor.
 *
 * JakubVanek: I don't agree with the limits of turn_ratio.
 */

void fillSyncStruct(uint8_t op, motor_port_t left, motor_port_t right, const const ev3_motor_ex_syncparams_t *in, cmd_sync *out) {
    int sgnSpd = (in->speed >= 0) ? 1 : -1;

    // TODO: check if this is correct
    out->Cmd          = op;
    out->Nos          = (1 << left) | (1 << right);
    CLIP(out->Speed, in->speed, -100, 100);
    out->Turn         = (right > left) ? -in->turn_ratio : in->turn_ratio;
    CLIP(out->StepOrTime, in->length, 0, INT32_MAX);
    out->StepOrTime *= sgnSpd;
    out->Brake        = in->brake ? 0x01 : 0x00;
}


// Unregulated move
ER ev3_motor_ex_unreg_time(motor_mask_t mask, const ev3_motor_ex_moveparams_t *params) {
    ER ercd;
    cmd_step_time cmd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);
    CHECK_RDACCESS(params);

    fillMoveStruct(opOUTPUT_TIME_POWER, mask, params, &cmd);
    motor_command(&cmd, sizeof(cmd));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_ex_unreg_step(motor_mask_t mask, const ev3_motor_ex_moveparams_t *params) {
    ER ercd;
    cmd_step_time cmd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);
    CHECK_RDACCESS(params);

    fillMoveStruct(opOUTPUT_STEP_POWER, mask, params, &cmd);
    motor_command(&cmd, sizeof(cmd));

    ercd = E_OK;
error_exit:
    return ercd;
}

// Regulated move
ER ev3_motor_ex_reg_time  (motor_mask_t mask, const ev3_motor_ex_moveparams_t *params) {
    ER ercd;
    cmd_step_time cmd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);
    CHECK_RDACCESS(params);

    fillMoveStruct(opOUTPUT_TIME_SPEED, mask, params, &cmd);
    motor_command(&cmd, sizeof(cmd));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_ex_reg_step  (motor_mask_t mask, const ev3_motor_ex_moveparams_t *params) {
    ER ercd;
    cmd_step_time cmd;

    CHECK_MASK(mask);
    CHECK_MASK_CONN(mask);
    CHECK_RDACCESS(params);

    fillMoveStruct(opOUTPUT_STEP_SPEED, mask, params, &cmd);
    motor_command(&cmd, sizeof(cmd));

    ercd = E_OK;
error_exit:
    return ercd;
}

// Regulated & synced move
ER ev3_motor_ex_sync_time (motor_port_t left, motor_port_t right, ev3_motor_ex_syncparams_t *params) {
    ER ercd;
    cmd_sync cmd;

    CHECK_PORT(left);
    CHECK_PORT(right);
    CHECK_PORT_DIFFERENT(left,right);
    CHECK_PORT_CONN(left);
    CHECK_PORT_CONN(right);
    CHECK_RDACCESS(params);

    fillSyncStruct(opOUTPUT_TIME_SYNC, left, right, params, &cmd);
    motor_command(&cmd, sizeof(cmd));

    ercd = E_OK;
error_exit:
    return ercd;
}

ER ev3_motor_ex_sync_step (motor_port_t left, motor_port_t right, ev3_motor_ex_syncparams_t *params) {
    ER ercd;
    cmd_sync cmd;

    CHECK_PORT(left);
    CHECK_PORT(right);
    CHECK_PORT_DIFFERENT(left,right);
    CHECK_PORT_CONN(left);
    CHECK_PORT_CONN(right);
    CHECK_RDACCESS(params);

    fillSyncStruct(opOUTPUT_STEP_SYNC, left, right, params, &cmd);
    motor_command(&cmd, sizeof(cmd));

    ercd = E_OK;
error_exit:
    return ercd;
}
