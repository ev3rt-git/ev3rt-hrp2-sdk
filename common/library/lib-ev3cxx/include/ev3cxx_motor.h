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
* \brief    Enum with index of motor port (A - D)
*/
enum class MotorPort {
    A = 0, 	//!< \~English Motor port A
    B,		//!< \~English Motor port B
    C,		//!< \~English Motor port C
    D    	//!< \~English Motor port D
};

/**
* \~English
* \brief    Enum with index of motor type
*/
enum class MotorType {
    MEDIUM = 1,	    //!< \~English Medium servo motor    \~Japanese サーボモータM
    LARGE,	        //!< \~English Large servo motor     \~Japanese サーボモータL
    UNREGULATED,    //!< \~English Unregulated motor     \~Japanese 未調整モータ    
};

namespace detail {
struct MotorConstants
{
    static const int POWER_MAX = 100;   //!< \~English Max power on motor
    static const int POWER_MIN = -100;  //!< \~English Min power on motor
    static constexpr float NUMBER_OF_DEGREES_PER_ROTATION = 360.0;
}; // struct MotorConstants
} // namespace detail

/**
 * \~English
 * \brief    Class Motor. API for working with motor.
 */
class Motor
    : public detail::MotorConstants
{
public:
    
    /**
     * \~English
     * \brief 	    Constructor of class motor.
     * \param port  Port number
     * \param type  Type of port: \a motor_type_t
     */
    Motor(MotorPort port, MotorType type = MotorType::LARGE)
    : m_port(static_cast<motor_port_t>(port)), 
      m_type(static_cast<motor_type_t>(type)) 
    {
        ev3_motor_config(m_port, m_type);
    }

    /**
     * \~English
     * \brief 	    Destructor of class motor.
     * \details     Turns off the motor and lefts it coasts.
     */
    ~Motor() {
        Motor::off(false);
    }

    /**
     * \~English
     * \brief 	    Stop motor.
     * \details	    Set the motor power or speed to 0 and depending on param \a brake will brake the motor. 
     * \param brake Start braking the motor (true = braking, false = not braking). Default value is \a true.
     */
    void off(bool brake = true) {
        ev3_motor_stop(m_port, brake);
    }

    /**
     * \~English
     * \brief 	    Set speed on regulated motor.
     * \details	    Using regulated motor driver (motor should has constant speed) and param \a power is equivalent to speed. Motor control real power to keep constant speed.
     * \param power Motor speed. Range: -100 to +100. A negative value moves the robot backwards. Default value is 50. 
     *              If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */
    void on(int power = 50) {
        // change type of motor: UNREGULATED_MOTOR => LARGE_MOTOR/MEDIUM_MOTOR 
        if(m_type != getType()) {
            ev3_motor_config(m_port, m_type);
        }
        ev3_motor_set_power(m_port, power);
    }

    /**
     * \~English
     * \brief 	    Set power on unregulated motor. [TODO: fix - same behavior as on() -> problem in EV3RT]
     * \details	    Motor works with constant power, NOT speed. Speed changes depend on load. 
     * \param power Motor power. Range: -100 to +100. A negative value moves the robot backwards. Default value is 50. 
     *              If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */   
    void unregulated(int power = 50) { 
        // change type of motor: LARGE_MOTOR/MEDIUM_MOTOR => UNREGULATED_MOTOR
        if(UNREGULATED_MOTOR != getType()) {
            ev3_motor_config(m_port, UNREGULATED_MOTOR);
        }
        ev3_motor_set_power(m_port, power);
    }
    
    /**
     * \~English
     * \brief 	            Set number of degrees to rotate with regulated motor. 
     * \param speed         Speed of motor. Range: -100 to +100. Default value is 50. 
     * \param degrees       Number of degrees for rotation of motor. A negative value moves the robot backwards. Default value is 360. 
     * \param brake         If \a true, then motor start braking after reach the position (= degrees).
     * \param blocking      \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                      Default value is \a false.
     * \param wait_after_ms Adds wait after the functuion executes. Default wait is 60 ms. It is important because of some race condition
     *                      when another motor rotation follows immediately
     */   
    void onForDegrees(int speed = 50, int degrees = 360, bool_t brake = true, bool_t blocking = true, uint32_t wait_after_ms = 60) {
        if (speed == 0)
            return;
        if (speed < 0) {
            speed = -speed;
            degrees = -degrees;
        }
        ev3_motor_rotate_brake(m_port, degrees, speed, blocking, brake);
        tslp_tsk(wait_after_ms);
    }

    /**
     * \~English
     * \brief 	            Set number of rotation to rotate with regulated motor. 
     * \param speed         Speed of motor. Range: -100 to +100. Default value is 50. 
     * \param rotations     Number of rotation for rotate with motor. A negative value moves the robot backwards. Default value is 1. 
     * \param brake         If \a true, then motor start braking after reach the position (= degrees).
     * \param blocking      \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                      Default value is \a false.
     * \param wait_after_ms Adds wait after the functuion executes. Default wait is 60 ms. It is important because of some race condition
     *                      when another motor rotation follows immediately
     */   
    void onForRotations(int speed = 50, float rotations = 1, bool_t brake = true, bool_t blocking = true, uint32_t wait_after_ms = 60) {
        if (speed == 0)
            return;
        if (speed < 0) {
            speed = -speed;
            rotations = -rotations;
        }
        ev3_motor_rotate_brake(m_port, NUMBER_OF_DEGREES_PER_ROTATION * rotations, speed, blocking, brake);
        tslp_tsk(wait_after_ms);
    }

    /**
     * \~English
     * \brief 	    Rotates the motor for a given time. 
     * \param speed Speed of motor. Range: -100 to +100. Default value is 50. 
     * \param time  Number of miliseconds for rotate with motor. Default value is 1000. 
     * \param brake If \a true, then motor start braking after reach the position (= degrees).
     */   
    void onForTime(int speed = 50, uint32_t ms = 1000, bool_t brake = true) {
        on(speed);
        tslp_tsk(ms);
        off(brake);
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

private:
    motor_port_t m_port;
    motor_type_t m_type;
}; // class Motor
} // namespace ev3cxx
