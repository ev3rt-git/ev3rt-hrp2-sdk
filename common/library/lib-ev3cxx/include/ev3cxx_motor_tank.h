/**
 * \file    ev3cxx_motor.h
 * \brief	EV3RT CPP API for motors
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include "ev3cxx_motor.h"

namespace ev3cxx {

/**
 * \~English
 * \brief    Class MotorTank. API for working with two motors together.
 */
class MotorTank
    : public detail::MotorConstants
{
public:

    MotorTank() = delete;

    /**
     * \~English
     * \brief 	    Constructor of class MotorTank.
     * \param left_motor_port   Port number
     * \param right_motor_port  Port number
     * \param type              Type of motor ports: \a motor_type_t
     */
    MotorTank(MotorPort left_motor_port, MotorPort right_motor_port, MotorType type = MotorType::LARGE)
    : m_left_motor(left_motor_port, type), 
      m_right_motor(right_motor_port, type)
    {}

    /**
     * \~English
     * \brief 	    Stop motors.
     * \details	    Set the motors power or speed to 0 and depending on param \a brake will brake the motors. 
     * \param brake Start braking the motors (true = braking, false = not braking). Default value is \a true.
     */
    void off(bool brake = true) {
        m_left_motor.off(brake);
        m_right_motor.off(brake);
    }

    /**
     * \~English
     * \brief 	            Set speed on regulated motors.
     * \details	            Using regulated motors driver (motors should has constant speed) and param \a power is equivalent to speed. 
     *                      Motor controls real power to keep constant speed.
     * \param left_speed    Left motor speed. Range: -100 to +100. A negative value turns the wheel backwards. Default value is 50. 
     *                      If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     * \param right_speed   Right motor speed. Range: -100 to +100. A negative value turns the wheel backwards. Default value is 50. 
     *                      If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */
    void on(int left_speed = 50, int right_speed = 50) {
        m_left_motor.on(left_speed);
        m_right_motor.on(right_speed);
    }

    /**
     * \~English
     * \brief 	            Set power on unregulated motors.
     * \details	            Motors work with constant power, NOT speed. Speed changes depend on load. 
     * \param left_power    Left motor power. Range: -100 to +100. A negative value turns the wheel backwards. Default value is 50. 
     *                      If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     * \param right_power   Right motor power. Range: -100 to +100. A negative value turns the wheel backwards. Default value is 50. 
     *                      If a out-of-range value is given, it will be clipped to the minimum (-100) or maximum (100) value.
     */   
    void unregulated(int left_power = 50, int right_power = 50) { 
        m_left_motor.unregulated(left_power);
        m_right_motor.unregulated(right_power);
    }
    
    /**
     * \~English
     * \brief 	            Set number of degrees to rotate with regulated motors. 
     * \param left_speed    \a Left motor speed. \a Range: -100 to +100. Default value is 50.
     * \param right_speed   \a Right motor speed. \a Range: -100 to +100. Default value is 50.
     * \param degrees       Number of degrees for rotation of the forward or faster motor. Default value is 360. 
     * \param brake         If \a true, then motor start braking after reach the position (= degrees).
     * \param blocking      \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                      Default value is \a false.
     * \param wait_after_ms Adds wait after the function executes. Default wait is 60 ms. It is important because of some race condition
     *                      when another motor rotation follows immediately
     */   
    void onForDegrees(int left_speed = 50, int right_speed = 50, int degrees = 360, bool_t brake = true, bool_t blocking = true, unsigned int wait_after_ms = 60) {
        int labs = abs(left_speed);
        int rabs = abs(right_speed);
        if (labs > rabs) {
            const float sr = float(rabs) / float(labs);
            m_right_motor.onForDegrees(right_speed, degrees * sr, brake, false, 0);
            m_left_motor.onForDegrees (left_speed, degrees, brake, blocking, wait_after_ms);
        } else {
            const float sr = rabs != 0 ? float(labs) / float(rabs) : 0;
            m_left_motor.onForDegrees (left_speed, degrees * sr, brake, false, 0);
            m_right_motor.onForDegrees(right_speed, degrees, brake, blocking, wait_after_ms);
        }
    }

    /**
     * \~English
     * \brief 	            Set number of rotations to rotate with regulated motors. 
     * \param left_speed    \a Left motor speed. \a Range: -100 to +100. Default value is 50.
     * \param right_speed   \a Right motor speed. \a Range: -100 to +100. Default value is 50.
     * \param rotations     Number of rotations for rotation of the forward or faster motor. Default value is 1. 
     * \param brake         If \a true, then motor start braking after reach the position (= degrees).
     * \param blocking      \a true (The function will be blocked until the move is finished), or \a false (The function will not be blocked). 
     *                      Default value is \a false.
     * \param wait_after_ms Adds wait after the function executes. Default wait is 60 ms. It is important because of some race condition
     *                      when another motor rotation follows immediately
     */   
    void onForRotations(int left_speed = 50, int right_speed = 50, float rotations = 1, bool_t brake = true, bool_t blocking = true, unsigned int wait_after_ms = 60) {
        onForDegrees(left_speed, right_speed, rotations * NUMBER_OF_DEGREES_PER_ROTATION, brake, blocking, wait_after_ms);
    }

    /**
     * \~English
     * \brief 	            Rotates the motor for a given time. 
     * \param left_speed    Speed of left motor. Range: -100 to +100. Default value is 50. 
     * \param right_speed   Speed of right motor. Range: -100 to +100. Default value is 50. 
     * \param ms            Number of miliseconds for rotate with motor. Default value is 1000. 
     * \param brake         If \a true, then motor start braking after reach the position (= degrees).
     */   
    void onForSeconds(int left_speed = 50, int right_speed = 50, unsigned int time_ms = 1000, bool_t brake = true) {
        on(left_speed, right_speed);
        tslp_tsk(time_ms);
        off(brake);
    }

    /**
    * \~English
    * \brief 	Reset the angular position of the motors to zero
    * \details  Setting the value of the angular position sensor of the motor does not affect the actual power and position of the motor.
    */
    void resetPosition()
    {
        m_left_motor.resetPosition();
        m_right_motor.resetPosition();
    }

    /**
     * \~English
     * \brief  Return reference to the left motor. 
     * \return Return Motor&.
     */   
    Motor& leftMotor() { return m_left_motor; }

    /**
     * \~English
     * \brief  Return const reference to the left motor. 
     * \return Return const Motor&.
     */   
    const Motor& leftMotor() const { return m_left_motor; }

    /**
     * \~English
     * \brief  Return reference to the right motor. 
     * \return Return Motor&.
     */   
    Motor& rightMotor() { return m_right_motor; }

    /**
     * \~English
     * \brief  Return const reference to the right motor. 
     * \return Return const Motor&.
     */   
    const Motor& rightMotor() const { return m_right_motor; }

private:
    Motor m_left_motor;
    Motor m_right_motor;
}; // class MotorTank
} // namespace ev3cxx
