/**
 * \file    motor.h
 * \brief	API for motors
 * \author	ertl-liyixiao
 */

/**
 * \~English
 * \defgroup ev3motor Motor
 * \brief    Definitions of API for controlling motors.
 * @{
 *
 * \~Japanese
 * \defgroup ev3motor サーボモータ
 * \brief    モータ制御に関するAPI．
 * @{
 */

#pragma once

/**
 * \~English
 * \brief Structure containing move parameters.
 */
typedef struct {
    /**
     * \~English
     * \brief How fast the motor will rotate.
     * \details Units: percent of max power/speed; sign indicates direction
     */
    int8_t   speed;
    /**
     * \~English
     * \brief How long will the motor accelerate.
     * \details Units: degrees or milliseconds
     */
    uint32_t rampup;
    /**
     * \~English
     * \brief How long will the motor run at the provided power/speed.
     * \details Units: degrees or milliseconds
     */
    uint32_t sustain;
    /**
     * \~English
     * \brief How long will the motor decelerate.
     * \details Units: degrees or milliseconds
     */
    uint32_t rampdown;
    /**
     * \~English
     * \brief Whether the motor will be braked or left floating.
     */
    bool_t   brake;
} ev3_motor_ex_moveparams_t;

/**
 * \~English
 * \brief Structure containing sync-move parameters.
 */
typedef struct {
    /**
     * \~English
     * \brief How fast the motor will rotate.
     * \details Units: percent of max power/speed
     */
    int8_t  speed;
    /**
     * \brief How much will the robot turn. Direction unknown.
     * \details Integer (probably unbounded?); zero means straight, absolute value of 100 makes one motor stop.
     */
    int16_t turn_ratio;
    /**
     * \brief How long will the motors run.
     * \details Units: degrees or milliseconds
     */
    uint32_t length;
    /**
     * \~English
     * \brief Whether the motor will be braked or left floating.
     */
    bool_t   brake;
} ev3_motor_ex_syncparams_t;

/**
 * \~English
 * \brief ID for supported motor ports
 *
 * \~Japanese
 * \brief モータポートを表す番号
 */
typedef enum {
    EV3_PORT_A = 0,  	//!< \~English Port A				 \~Japanese ポートA
    EV3_PORT_B = 1,		//!< \~English Port B 				 \~Japanese ポートB
    EV3_PORT_C = 2,		//!< \~English Port C 				 \~Japanese ポートC
    EV3_PORT_D = 3, 	//!< \~English Port D 			     \~Japanese ポートD
    TNUM_MOTOR_PORT = 4 //!< \~English Number of motor ports \~Japanese モータポートの数
} motor_port_t;

/**
 * \~English
 * \brief ID mask for operating on multiple motors at once
 */
typedef enum {
    EV3_PORTBIT_A = 0x1,	//!< \~English Port A				 \~Japanese ポートA
    EV3_PORTBIT_B = 0x2,	//!< \~English Port B 				 \~Japanese ポートB
    EV3_PORTBIT_C = 0x4,	//!< \~English Port C 				 \~Japanese ポートC
    EV3_PORTBIT_D = 0x8,	//!< \~English Port D 			     \~Japanese ポートD
    TNUM_MOTOR_MASK = 4
} motor_mask_t;

/**
 * \~English
 * \brief Enumeration type for supported motor types
 *
 * \~Japanese
 * \brief サポートするモータタイプ
 */
typedef enum {
    NONE_MOTOR = 0,    //!< \~English Not connected         \~Japanese モータ未接続
    MEDIUM_MOTOR,      //!< \~English Medium servo motor    \~Japanese サーボモータM
    LARGE_MOTOR,       //!< \~English Large servo motor     \~Japanese サーボモータL
    UNREGULATED_MOTOR, //!< \~English Unregulated motor     \~Japanese 未調整モータ
    TNUM_MOTOR_TYPE    //!< \~English Number of motor types \~Japanese モータタイプの数
} motor_type_t;

/**
 * \~English
 * \brief 	   Configure a motor port.
 * \details	   Set the type of motor connected to the motor port. You can also specify a new motor type even if you have already set it.
 * \param port Motor port to be configured
 * \param type Motor type for the specified motor port
 * \retval E_OK  Successful completion
 * \retval E_ID  Illegal motor port number 
 * \retval E_PAR Illegal motor type
 *
 * \~Japanese
 * \brief 	     モータポートを設定する．
 * \details      モータポートに接続しているモータのタイプを設定する．既に設定した場合も新しいモータタイプを指定できる．
 * \param  port  モータポート番号
 * \param  type  モータタイプ
 * \retval E_OK  正常終了
 * \retval E_ID  不正のモータポート番号
 * \retval E_PAR 不正のモータタイプ
 */
ER ev3_motor_config(motor_port_t port, motor_type_t type);

/**
 * \~English
 * \brief       Configure all motor ports at once.
 * \details     Set the type of motors connected to all motor ports at once. You can also specify a new motor type even if you have already set it.
 * \param types Pointer to four motor types.
 * \retval E_OK  Successful completion
 * \retval E_PAR Illegal motor type
 */
ER ev3_motor_config_all(motor_type_t *types);

/**
 * \~English
 * \brief 	    Get the type of a motor port.
 * \param port  Motor port to be inquired
 * \retval >=0  Motor type of specified motor port
 * \retval E_ID Illegal motor port number
 *
 * \~Japanese
 * \brief 	    モータポートのモータタイプを取得する．
 * \param  port モータポート番号
 * \retval >=0  指定したモータポートのモータタイプ
 * \retval E_ID 不正のモータポート番号
 */
ER_UINT ev3_motor_get_type(motor_port_t port);

/**
 * \~English
 * \brief 	   Get the angular position of a motor port.
 * \details    When an incorrect motor port number is specified, it always returns 0.
 * \param port Motor port to be inquired
 * \return     Angular position in degrees. A negative value means the motor rotate has rotated backwards. [TODO: number of degrees per one revolution of motor]
 
 *
 * \~Japanese
 * \brief 	   モータの角位置を取得する．
 * \details    不正のモータポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param port モータポート番号
 * \return     モータの角位置（単位は度），マイナスの値は逆方向に回転されたことを指す
 */
int32_t ev3_motor_get_counts(motor_port_t port);

/**
 * \~English
 * \brief 	   Reset the angular position of the motor to zero
 * \details    Setting the value of the angular position sensor of the motor does not affect the actual power and position of the motor.
 * \param port Motor port to be inquired
 * \retval E_OK   Successful completion
 * \retval E_ID   Illegal motor port number
 * \retval E_OBJ  Motor port has not been initialized.
 *
 * \~Japanese
 * \brief 	      モータの角位置をゼロにリセットする．
 * \details       モータの角位置センサの値を設定するだけ，モータの実際のパワーと位置に影響を与えない．
 * \param  port   モータポート番号
 * \retval E_OK   正常終了
 * \retval E_ID   不正のモータポート番号
 * \retval E_OBJ  モータ未接続
 */
ER ev3_motor_reset_counts(motor_port_t port);

/**
 * \~English
 * \brief         Get the measured rotational speed of the motor
 * \details       Asks the kernel driver for the current derivation of the motor position.
 * \param port    Motor port to be inquired
 * \param pDst    Where to store the velocity
 * \retval E_OK   Successful completion
 * \retval E_ID   Illegal motor port number
 * \retval E_OBJ  Motor port has not been initialized
 * \retval E_PAR  Destination pointer is not accessible
 */
ER ev3_motor_get_velocity(motor_port_t port, int *pVal);

/**
 * \~English
 * \brief 	    Set the unregulated power for a motor port.
 * \param port  Motor port to be set
 * \param power The percentage of full power, ranging from -100 to +100. A negative value makes the motor rotate backwards.
 * \retval E_OK     Successful completion
 * \retval E_ID     Illegal motor port number
 * \retval E_OBJ    Motor port has not been initialized.
 *
 * \~Japanese
 * \brief 	     モータのパワーを設定する
 * \param  port  モータポート番号
 * \param  power モータのフルパワーのパーセント値．範囲：-100から+100．マイナスの値でモータを逆方向に回転させることができる．
 * \retval E_OK  正常終了
 * \retval E_ID  不正のモータポート番号
 * \retval E_OBJ モータ未接続
 */
ER ev3_motor_set_power(motor_port_t port, int power);

/**
 * \~English
 * \brief 	    Get the actual power of a motor port.
 * \details     When an incorrect motor port number is specified, it always returns 0.
 * \param port  Motor port to be set
 * \return      Motor power [TODO: range?]
 *
 * \~Japanese
 * \brief 	   モータのパワーを取得する
 * \details    不正のモータポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param port モータポート番号
 * \return     モータのパワー
 */
int ev3_motor_get_power(motor_port_t port);

/**
 * \~English
 * \brief 	    Stop a motor port.
 * \param port  Motor port to be stopped
 * \param brake Brake mode, \a true for braking, \a false for coasting.
 * \retval E_OK     Successful completion
 * \retval E_ID     Illegal motor port number
 * \retval E_OBJ    Motor port has not been initialized.
 *
 * \~Japanese
 * \brief 	     モータを停止する
 * \param  port  モータポート番号
 * \param  brake ブレーキモードの指定，\a true （ブレーキモード）, \a false （フロートモード）
 * \retval E_OK  正常終了
 * \retval E_ID  不正のモータポート番号
 * \retval E_OBJ モータ未接続
 */
ER ev3_motor_stop(motor_port_t port, bool_t brake);

/**
 * \~English
 * \brief 	        Rotate a motor port for specified degrees.
 * \param port      Motor port to be rotated
 * \param degrees   Degrees to be rotated. A negative value makes the motor rotate backwards.
 * \param speed_abs Speed for rotating. The value is a percentage of full speed, ranging from 0 to +100.
 * \param blocking  \a true (The function will be blocked until the rotation is finished), or \a false (The function will not be blocked).
 * \retval E_OK     Successful completion
 * \retval E_ID     Illegal motor port number
 * \retval E_OBJ    Motor port has not been initialized.
 *
 * \~Japanese
 * \brief 	         モータを指定した角度だけ回転させる
 * \param  port      モータポート番号
 * \param  degrees   回転角度，マイナスの値でモータを逆方向に回転させることができる
 * \param  speed_abs 回転速度，モータポートのフルスピードのパーセント値．範囲：0から+100．
 * \param  blocking  \a true (関数は回転が完了してからリターン)，\a false (関数は回転操作を待たずにリターン)
 * \retval E_OK      正常終了
 * \retval E_ID      不正のモータポート番号
 * \retval E_OBJ     モータ未接続
 */
ER ev3_motor_rotate(motor_port_t port, int degrees, uint32_t speed_abs, bool_t blocking);

/**
 * \~English
 * \brief              Move the robot along a curved path using two motors.
 * \param  left_motor  ID of the left motor port
 * \param  right_motor ID of the right motor port
 * \param  power       Power of motors. Range: -100 to +100. A negative value moves the robot backwards.
 * \param  turn_ratio  The sharpness of the turn. Range: -100 to +100. If \a turn_ratio is negative, the robot will turn left.
 *                     If \a turn_ratio is positive, the robot will turn right. More specifically, \a turn_ratio determines the ratio of
 *                     inner wheel speed as a percent. For example, if \a turn_ratio is +25, the right motor will move at 75% of the \a power,
 *                     which makes the robot turn right.
 * \retval E_OK        Success
 * \retval E_ID        Invalid ID of motor port
 * \retval E_OBJ       Motor port has not been initialized.
 *
 * \~Japanese
 * \brief              ２つのモータでロボットのステアリング操作を行う．
 * \param  left_motor  左モータのモータポート番号
 * \param  right_motor 右モータのモータポート番号
 * \param  power       モータのパワー．範囲：-100から+100．マイナスの値は後退．
 * \param  turn_ratio  ステアリングの度合い．範囲：-100から+100．マイナスの値は左への転回，プラスの値は右への転回になる．
 *                     具体的に言えば，このパラメータはこの左右モータのパワーの差の度合いである．例えば，\a turn_ratio は+25である場合，
 *                     左モータのパワーは\a power で，右モータのパワーは\a power の75\%になり，ロボットは右へ転回する．
 * \retval E_OK        正常終了
 * \retval E_ID        不正のモータID
 * \retval E_OBJ       モータ未接続
 */
ER ev3_motor_steer(motor_port_t left_motor, motor_port_t right_motor, int power, int turn_ratio);

/**
 * \~English
 * \brief         Reset tacho counts on multiple motors at once.
 * \param mask    Port bit mask.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_reset_counts(motor_mask_t mask);

/**
 * \~English
 * \brief         Set output power (unregulated) on multiple motors at once.
 * \param mask    Port bit mask.
 * \param power   Integer in range <-100; +100>, negative value indicates going backward.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_setpower(motor_mask_t mask, int8_t power);

/**
 * \~English
 * \brief         Set output speed (regulated) on multiple motors at once.
 * \param mask    Port bit mask.
 * \param speed   Integer in range <-100; +100>, negative value indicates going backward.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_setspeed(motor_mask_t mask, int8_t speed);

/**
 * \~English
 * \brief         Start unlimited move on multiple motors at once.
 * \param mask    Port bit mask.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_start   (motor_mask_t mask);

/**
 * \~English
 * \brief         Stop multiple motors at once.
 * \param mask    Port bit mask.
 * \param brake   Whether the motors will be braked (= true) or just left floated (= false) at the end of the move.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_stop    (motor_mask_t mask, bool_t brake);

/**
 * \~English
 * \brief         Start unregulated limited move on multiple motors at once.
 * \param mask    Port bit mask.
 * \param params  Move parameters (power + milliseconds).
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_unreg_time(motor_mask_t mask, const ev3_motor_ex_moveparams_t *params);

/**
 * \~English
 * \brief         Start unregulated limited move on multiple motors at once.
 * \param mask    Port bit mask.
 * \param params  Move parameters (power + degrees).
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_unreg_step(motor_mask_t mask, const ev3_motor_ex_moveparams_t *params);

/**
 * \~English
 * \brief         Start regulated limited move on multiple motors at once.
 * \param mask    Port bit mask.
 * \param params  Move parameters (speed + milliseconds).
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_reg_time  (motor_mask_t mask, const ev3_motor_ex_moveparams_t *params);

/**
 * \~English
 * \brief         Start regulated limited move on multiple motors at once.
 * \param mask    Port bit mask.
 * \param params  Move parameters (speed + degrees).
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_reg_step  (motor_mask_t mask, const ev3_motor_ex_moveparams_t *params);

/**
 * \~English
 * \brief         Start synced move.
 * \param left    Left motor to use.
 * \param right   Right motor to use.
 * \param params  Move parameters (speed + milliseconds).
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_sync_time (motor_port_t left, motor_port_t right, ev3_motor_ex_syncparams_t *params);

/**
 * \~English
 * \brief         Start synced move.
 * \param left    Left motor to use.
 * \param right   Right motor to use.
 * \param params  Move parameters (speed + degrees).
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER ev3_motor_ex_sync_step (motor_port_t left, motor_port_t right, ev3_motor_ex_syncparams_t *params);

/**
 * \~English
 * \brief         Check whether specified motors are running.
 * \param mask    Port bit mask.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER_UINT ev3_motor_ex_running(motor_mask_t mask);

/**
 * \~English
 * \brief         Wait until all of the specified motors are stopped.
 * \param mask    Port bit mask.
 * \retval E_OK   Success
 * \retval E_ID   Invalid ID of motor port
 * \retval E_OBJ  Motor port has not been initialized.
 */
ER      ev3_motor_ex_poll   (motor_mask_t mask);




/**
 * @} // End of group
 */
