/**
 * \file    ev3cxx_motor.h
 * \brief	EV3RT CPP API for motors
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include "ev3cxx.h"

namespace ev3cxx {

/**
 * \~English
 * \brief    Class Motor
 * \details  API for working with motors.
 */
class Motor
{
public:
    static const int POWER_MAX = 100;   //!< \~English Max power on motor
    static const int POWER_MIN = -100;  //!< \~English Min power on motor
    static constexpr float NUMBER_OF_DEGREES_PER_ROTATION = 360.0;

    /**
     * \~English
     * \brief 	    Constructor of class motor.
     * \param port  Port number
     * \param type  Type of port: \a motor_type_t
     */
     
    Motor(motor_port_t port, motor_type_t type = LARGE_MOTOR)
    :m_port(port),
    m_type(type) 
    {
        m_error = ev3_motor_config(m_port, type);
    }

    /**
     * \~English
     * \brief 	    Destructor of class motor.
     */
    ~Motor() {
        Motor::off(true);
    }

    /**
     * \~English
     * \brief 	    Stop motor.
     * \details	    Set the motor power or speed to 0 and depending on param \a brake will brake the motor. 
     * \param brake Start braking the motor (true = braking, false = not braking). Default value is \a true.
     */
    ER off(bool brake = true) {
        return (m_error = ev3_motor_stop(m_port, brake));
    }

    /**
     * \~English
     * \brief 	    Set speed on regulated motor.
     * \details	    Using regulated motor driver (motor should has constant speed) and param \a power is equivalent to speed. Motor control real power to keep constant speed.
     * \param power Motor speed. Range: -100 to +100. A negative value moves the robot backwards. Default value is 50. 
     *              If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */
    ER on(int power = 50) {
        m_error = ev3_motor_set_power(m_port, power);
        // change type of motor: UNREGULATED_MOTOR => LARGE_MOTOR/MEDIUM_MOTOR
        if(m_type != getType()) {
            m_error = ev3_motor_config(m_port, m_type);
        }

        if(m_error == E_OK)
            m_error = ev3_motor_set_power(m_port, power);
        else
            assert(false);//API_ERROR(m_error); // TODO: API_ERROR

        return m_error;
    }
    /**
     * \~English
     * \brief 	    Set power on unregulated motor. [TODO: fix - same behavior as on() -> problem in EV3RT]
     * \details	    Motor works with constant power, NOT speed. Speed changes depend on load. 
     * \param power Motor power. Range: -100 to +100. A negative value moves the robot backwards. Default value is 50. 
     *              If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */   
    ER unregulated(int power = 50) { 
        // change type of motor: LARGE_MOTOR/MEDIUM_MOTOR => UNREGULATED_MOTOR
        if(UNREGULATED_MOTOR != getType()) {
            m_error = ev3_motor_config(m_port, UNREGULATED_MOTOR);
        }

        if(m_error == E_OK)
            m_error = ev3_motor_set_power(m_port, power);
        else
            assert(false);//API_ERROR(m_error); // TODO: API_ERROR

        return m_error;
    }
    
    /**
     * \~English
     * \brief 	        Set number of degrees to rotate with regulated motor. 
     * \param power_abs \a power_abs is equivalent to speed. \a power_abs is in absolute value. Range: 0 to +100. Default value is 50. 
     * \param degrees   Number of degrees for rotation of motor. A negative value moves the robot backwards. Default value is 360. 
     * \param blocking  \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                  Default value is \a false.
     */   
    void onForDegrees(uint32_t power_abs = 50, int degrees = 360, bool_t blocking = false) { // TODO: add param brake
        ev3_motor_rotate(m_port, degrees, power_abs, blocking);
    }

    /**
     * \~English
     * \brief 	        Set number of rotation to rotate with regulated motor. 
     * \param power_abs \a power_abs is equivalent to speed. \a power_abs is in absolute value. Range: 0 to +100. Default value is 50. 
     * \param rotations Number of rotation for rotate with motor. A negative value moves the robot backwards. Default value is 1. 
     * \param blocking  \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                  Default value is \a false.
     */   
    void onForRotations(uint32_t power_abs = 50, float rotations = 1, bool_t blocking = false) { // TODO: add param brake
        ev3_motor_rotate(m_port, NUMBER_OF_DEGREES_PER_ROTATION * rotations, power_abs, blocking);
    }

    /**
    * \~English
    * \brief   Get the actual position of a motor in degrees.
    * \details When an incorrect motor port number is specified, it always returns 0.
    * \return  Motor position in degrees. 
    */
    int degrees() const { return ev3_motor_get_counts(m_port); }

    /**
    * \~English
    * \brief   Get the actual position of a motor in rotations.
    * \details When an incorrect motor port number is specified, it always returns 0.
    * \return  Motor position in rotations (float value). 
    */
    float rotations() const { 
        return ev3_motor_get_counts(m_port) / NUMBER_OF_DEGREES_PER_ROTATION;
    }

    /**
    * \~English
    * \brief   Get the actual power (equivalent with speed) of a motor. [TODO: power/speed]
    * \details When an incorrect motor port number is specified, it always returns 0.
    * \return  Motor power 
    */
    int currentPower() const { return ev3_motor_get_power(m_port); }

    /**
    * \~English
    * \brief 	Reset the angular position of the motor to zero
    * \details  Setting the value of the angular position sensor of the motor does not affect the actual power and position of the motor.
    */
    void resetPosition() { ev3_motor_reset_counts(m_port); }
    /**
     * \~English
     * \brief  Get motor port. 
     * \return Return const motor port.
     */   
    motor_port_t getPort() const { return m_port; }

    /**
     * \~English
     * \brief  Get curent type of motor which is set in EV3RT (not set by constructor). 
     * \return Return const motor type or error.
     * \retval >=0  Motor type of specified motor port
     * \retval E_ID Illegal motor port number
     */   
    ER_UINT getType() const { 
        ER_UINT motor_type = ev3_motor_get_type(m_port);

        if(motor_type < 0 || motor_type >= TNUM_MOTOR_PORT)
            return E_ID;
        return motor_type; 
    }

    /**
     * \~English
     * \brief         Get state of motor error.
     * \retval E_OK   All OK
     * \retval E_ID   Illegal motor port number
     * \retval E_PAR  Illegal motor type
     * \retval E_OBJ  Motor port has not been initialized
     */
    ER getError() const { return m_error; }

private:
    motor_port_t m_port;
    motor_type_t m_type;
    ER m_error;

}; // class Motor
} // namespace ev3cxx
