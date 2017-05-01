/*
 * Motor.c
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx_motor.h"

using namespace ev3cxx;

Motor::Motor(motor_port_t port, motor_type_t type)
:m_port(port),
 m_type(type)
{
    m_error = ev3_motor_config(m_port, type);
}

Motor::~Motor(void){
    Motor::off(true);
}

ER Motor::off(bool brake){
    return m_error = ev3_motor_stop(m_port, brake);
}

ER Motor::on(int power){
    m_error = ev3_motor_set_power(m_port, power);
    // change type of motor: UNREGULATED_MOTOR => LARGE_MOTOR/MEDIUM_MOTOR
    if( m_type != getType()) {
        m_error = ev3_motor_config(m_port, m_type);
    }

    if(m_error == E_OK)
        m_error = ev3_motor_set_power(m_port, power);
    else
        assert(true);//API_ERROR(m_error); // TODO: API_ERROR

    return m_error;
}

ER Motor::unregulated(int power){ 
    // change type of motor: LARGE_MOTOR/MEDIUM_MOTOR => UNREGULATED_MOTOR
    if(UNREGULATED_MOTOR != getType()) {
        m_error = ev3_motor_config(m_port, UNREGULATED_MOTOR);
    }

    if(m_error == E_OK)
        m_error = ev3_motor_set_power(m_port, power);
    else
        assert(true);//API_ERROR(m_error); // TODO: API_ERROR

    return m_error;
}

void Motor::onForDegrees(uint32_t power_abs, int degrees, bool_t blocking){ // TODO: add param brake
    ev3_motor_rotate( m_port, degrees, power_abs, blocking);
}

void Motor::onForRotations(uint32_t power_abs, float rotations, bool_t blocking){ // TODO: add param brake
    ev3_motor_rotate( m_port, NUMBER_OF_DEGREES_PER_ROTATION * rotations, power_abs, blocking);
}

int Motor::degrees() const { return ev3_motor_get_counts(m_port); }

float Motor::rotations() const { 
    return ev3_motor_get_counts(m_port) / NUMBER_OF_DEGREES_PER_ROTATION;
 }

int Motor::currentPower() const { return ev3_motor_get_power(m_port); }

void Motor::resetPosition() { ev3_motor_reset_counts(m_port); }

motor_port_t Motor::getPort() const { return m_port; }

ER_UINT Motor::getType() const { 
    ER_UINT motor_type = ev3_motor_get_type(m_port);

    if(motor_type < 0 || motor_type >= TNUM_MOTOR_PORT)
        return E_ID;
    return motor_type; 
}

ER Motor::getError() const { return m_error; } 
