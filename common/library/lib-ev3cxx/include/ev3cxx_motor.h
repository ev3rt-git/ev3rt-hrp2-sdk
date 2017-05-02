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
     * \param port  Number of port
     * \param type  Type of port: \a motor_type_t
     */
    Motor(motor_port_t port, motor_type_t type = LARGE_MOTOR);
    
    /**
     * \~English
     * \brief 	    Destructor of class motor.
     */
    ~Motor();

    /**
     * \~English
     * \brief 	    Stop motor.
     * \details	    Set the motor power or speed to 0 and depending on param \a brake will brake the motor. 
     * \param brake Start breaking the motor (true = braking, false = not braking). Default value is \a true.
     */
    ER off(bool brake = true);

    /**
     * \~English
     * \brief 	    Set speed on regulated motor.
     * \details	    Using regulated motor driver and param \a power is equivalent to speed. 
     * \param power Motor speed. Range: -100 to +100. A negative value moves the robot backwards. Default value is 50. 
     *              If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */
    ER on(int power = 50);

    /**
     * \~English
     * \brief 	    Set power on unregulated motor. [TODO: fix - same behavior as on() -> problem in EV3RT]
     * \param power Motor power. Range: -100 to +100. A negative value moves the robot backwards. Default value is 50. 
     *              If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */   
    ER unregulated(int power = 50);
    
    /**
     * \~English
     * \brief 	        Set number of degrees to rotate with regulated motor. 
     * \param power_abs \a power_abs is equivalent to speed. \a power_abs is in absolute value. Range: 0 to +100. Default value is 50. 
     * \param degrees   Number of degrees for rotation of motor. A negative value moves the robot backwards. Default value is 360. 
     * \param blocking  \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                  Default value is \a false.
     */   
    void onForDegrees(uint32_t power_abs = 50, int degrees = 360, bool_t blocking = false);

    /**
     * \~English
     * \brief 	        Set number of rotation to rotate with regulated motor. 
     * \param power_abs \a power_abs is equivalent to speed. \a power_abs is in absolute value. Range: 0 to +100. Default value is 50. 
     * \param rotations Number of rotation for rotate with motor. A negative value moves the robot backwards. Default value is 1. 
     * \param blocking  \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                  Default value is \a false.
     */   
    void onForRotations(uint32_t power_abs = 50, float rotations = 1, bool_t blocking = false);

    /**
    * \~English
    * \brief   Get the actual position of a motor in degrees.
    * \details When an incorrect motor port number is specified, it always returns 0.
    * \return  Motor position in degrees. 
    */
    int degrees() const;

    /**
    * \~English
    * \brief   Get the actual position of a motor in rotations.
    * \details When an incorrect motor port number is specified, it always returns 0.
    * \return  Motor position in rotations (float value). 
    */
    float rotations() const;

    /**
    * \~English
    * \brief   Get the actual power (equivalent with speed) of a motor. [TODO: power/speed]
    * \details When an incorrect motor port number is specified, it always returns 0.
    * \return  Motor power 
    */
    int currentPower() const;

    /**
    * \~English
    * \brief 	Reset the angular position of the motor to zero
    * \details  Setting the value of the angular position sensor of the motor does not affect the actual power and position of the motor.
    */
    void resetPosition();

    /**
     * \~English
     * \brief  Get motor port. 
     * \return Return const motor port.
     */   
    motor_port_t getPort() const;

    /**
     * \~English
     * \brief  Get curent type of motor which is set in EV3RT (not set by constructor). 
     * \return Return const motor type or error.
     * \retval >=0  Motor type of specified motor port
     * \retval E_ID Illegal motor port number
     */   
    ER_UINT getType() const;

    /**
     * \~English
     * \brief         Get state of motor error.
     * \retval E_OK   All OK
     * \retval E_ID   Illegal motor port number
     * \retval E_PAR  Illegal motor type
     * \retval E_OBJ  Motor port has not been initialized
     */
    ER getError() const;

private:
    motor_port_t m_port;
    motor_type_t m_type;
    ER m_error;

}; // class Motor
} // namespace ev3cxx
