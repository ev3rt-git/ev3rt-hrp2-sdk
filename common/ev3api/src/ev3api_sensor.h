/**
 * \file    sensor.h
 * \brief	API for sensors
 * \author	ertl-liyixiao
 */

/**
 * \~English
 * \defgroup ev3api-sensor Sensor
 * \brief    Definitions of API for controlling sensors.
 * @{
 *
 * \~Japanese
 * \defgroup ev3sensor 各種センサ
 * \brief    各種センサに関するAPI．
 * @{
 */

#pragma once

/**
 * \~English
 * \brief Enumeration type for supported sensor ports
 *
 * \~Japanese
 * \brief センサポートを表す番号
 */
typedef enum {
    EV3_PORT_1 = 0,		  //!< \~English Port 1 			    \~Japanese ポート1
    EV3_PORT_2 = 1,	      //!< \~English Port 2 	            \~Japanese ポート2
    EV3_PORT_3 = 2,		  //!< \~English Port 3 	 	        \~Japanese ポート3
    EV3_PORT_4 = 3,       //!< \~English Port 4	 			    \~Japanese ポート4
    TNUM_SENSOR_PORT = 4, //!< \~English Number of sensor ports \~Japanese センサポートの数
} sensor_port_t;

/**
 * \~English
 * \brief Enumeration type for supported sensor types
 *
 * \~Japanese
 * \brief サポートするセンサタイプ
 */
typedef enum {
    NONE_SENSOR = 0,     //!< \~English Not connected			 		  \~Japanese センサ未接続
    ULTRASONIC_SENSOR,   //!< \~English Ultrasonic sensor 	 			  \~Japanese 超音波センサ
    GYRO_SENSOR,	     //!< \~English Gyroscope sensor 		 		  \~Japanese ジャイロセンサ
    TOUCH_SENSOR,	     //!< \~English Touch sensor			 		  \~Japanese タッチセンサ
    COLOR_SENSOR,	     //!< \~English Color sensor					  \~Japanese カラーセンサ
    INFRARED_SENSOR,     //!< \~English Infra-Red sensor				  \~Japanese 赤外線センサー
	HT_NXT_ACCEL_SENSOR, //!< \~English HiTechnic NXT acceleration sensor \~Japanese 加速度センサ（HiTechnic社製）
	HT_NXT_COLOR_SENSOR, //!< \~English HiTechnic NXT color sensor        \~Japanese カラーセンサ（HiTechnic社製）
	NXT_TEMP_SENSOR,     //!< \~English NXT temperature sensor            \~Japanese NXT温度センサ
    TNUM_SENSOR_TYPE     //!< \~English Number of sensor types 			  \~Japanese センサタイプの数
} sensor_type_t;

/**
 * \~English
 * \brief Enumeration type for colors that can be detected by color sensor
 *
 * \~Japanese
 * \brief カラーセンサで識別できるカラーの番号
 */
typedef enum {
    COLOR_NONE   = 0, //!< \~English None 			  \~Japanese 無色
    COLOR_BLACK  = 1, //!< \~English Black 			  \~Japanese 黒
    COLOR_BLUE   = 2, //!< \~English Blue  			  \~Japanese 青
    COLOR_GREEN  = 3, //!< \~English Green  		  \~Japanese 緑
    COLOR_YELLOW = 4, //!< \~English Yellow 		  \~Japanese 黄
    COLOR_RED    = 5, //!< \~English Red  			  \~Japanese 赤
    COLOR_WHITE  = 6, //!< \~English White 		      \~Japanese 白
    COLOR_BROWN  = 7, //!< \~English Brown 			  \~Japanese 茶
    TNUM_COLOR		  //!< \~English Number of colors \~Japanese 識別できるカラーの数
} colorid_t;

/**
 * \~English
 * \brief Structure for an RGB raw value
 *
 * \~Japanese
 * \brief RGB Raw値を格納する構造体
 */
typedef struct {
    uint16_t r; //!< \~English Red value   \~Japanese 赤
    uint16_t g; //!< \~English Green value \~Japanese 緑
    uint16_t b; //!< \~English Blue value  \~Japanese 青
} rgb_raw_t;

/** 
 * \~English
 * \brief 	     Configure a sensor port.
 * \details      Always returns 0  when an invalid sensor number is specified.
 * \param port   Sensor port to be configured
 * \param type   Sensor type for the specified sensor port
 * \retval E_OK  Successful completion
 * \retval E_ID  Illegal sensor port number 
 * \retval E_PAR Illegal sensor type
 *
 * \~Japanese
 * \brief 	     センサポートを設定する．
 * \details      センサポートに接続しているセンサのタイプを設定する．既に設定した場合も新しいセンサタイプを指定できる．
 * \param  port  センサポート番号
 * \param  type  センサタイプ
 * \retval E_OK  正常終了
 * \retval E_ID  不正のセンサポート番号
 * \retval E_PAR 不正のセンサタイプ
 */
ER ev3_sensor_config(sensor_port_t port, sensor_type_t type);

/**
 * \~English
 * \brief 	    Get the type of a sensor port.
 * \param port  Sensor port to be inquired
 * \retval >=0  Specified sensor type of supported sensor 
 * \retval E_ID Incorrect sensor port number
 *
 * \~Japanese
 * \brief 	    センサポートのセンサタイプを取得する．
 * \param  port センサポート番号
 * \retval >=0  指定したセンサポートのセンサタイプ
 * \retval E_ID 不正のセンサポート番号
 */
ER_UINT ev3_sensor_get_type(sensor_port_t port);

/**
 * \~English
 * \brief 	   Get the color by a color sensor.
 * \details    Always return COLOR_NONE (error  is outputted) when an incorrect sensor number is specified.
 * \param port Sensor port to be inquired
 * \return     Color detected
 *
 * \~Japanese
 * \brief 	    カラーセンサでカラーを識別する．
 * \details     不正のセンサポート番号を指定した場合，常にCOLOR_NONEを返す（エラーログが出力される）．
 * \param  port センサポート番号
 * \return      識別したカラー
 */
colorid_t ev3_color_sensor_get_color(sensor_port_t port);

/**
 * \~English
 * \brief 	   Get the reflect light intensity by a color sensor.
 * \details    Always returns 0 (error log is outputted) when an invalid sensor number is specified.
 * \param port Sensor port to be inquired
 * \return     Reflect light intensity, ranging from 0 to 100
 *
 * \~Japanese
 * \brief 	    カラーセンサで反射光の強さを測定する．
 * \details     不正のセンサポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param port  センサポート番号
 * \return      反射光の強さ（0〜100）
 */
uint8_t ev3_color_sensor_get_reflect(sensor_port_t port);

/**
 * \~English
 * \brief 	   Get the ambient light intensity by a color sensor.
 * \details    Always returns 0 (error log is outputted) when an invalid sensor number is specified.
 * \param port Sensor port to be inquired
 * \return     Ambient light intensity, ranging from 0 to 100
 *
 * \~Japanese
 * \brief 	    カラーセンサで環境光の強さを測定する．
 * \details     不正のセンサポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param port  センサポート番号
 * \return      環境光の強さ（0〜100）
 */
uint8_t ev3_color_sensor_get_ambient(sensor_port_t port);

/**
 * \~English
 * \brief 	   Get the RGB raw value by a color sensor.
 * \details    If an invalid sensor number is specified, val is not updated (error log is output).
 * \param port Sensor port to be inquired
 * \param val  Pointer for storing sensor value
 *
 * \~Japanese
 * \brief 	    カラーセンサでRGB Raw値を測定する．
 * \details     不正のセンサポート番号を指定した場合，valは更新しない（エラーログが出力される）．
 * \param port  センサポート番号
 * \param val   取得した値を格納する変数のポインタ
 */
void ev3_color_sensor_get_rgb_raw(sensor_port_t port, rgb_raw_t *val);

/**
 * \~English
 * \brief 	   Get the angular position by a gyroscope sensor.
 * \details    Always returns 0 (error log is outputted) when an invalid sensor number is specified.
 * \param port Sensor port to be inquired
 * \return     Angular position in degrees
 *
 * \~Japanese
 * \brief 	    ジャイロセンサで角位置を測定する．
 * \details     不正のセンサポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param port  センサポート番号
 * \return      角位置（単位は度）
 */
int16_t ev3_gyro_sensor_get_angle(sensor_port_t port);

/**
 * \~English
 * \brief 	   Get the angular speed by a gyroscope sensor.
 * \details    Always returns 0 (error log is outputted) when an invalid sensor number is specified.
 * \param port Sensor port to be inquired
 * \return     Angular speed in degrees/second.
 *
 * \~Japanese
 * \brief 	    ジャイロセンサで角速度を測定する
 * \details     不正のセンサポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param  port センサポート番号
 * \return      角位置（単位は度/秒）
 */
int16_t ev3_gyro_sensor_get_rate(sensor_port_t port);

/**
 * \~English
 * \brief 	    Reset the angular position of a gyroscope sensor to zero.
 * \param port  Sensor port to be reset
 * \retval E_OK Successful completion
 * \retval E_ID Illegal sensor port number 
 *
 * \~Japanese
 * \brief 	   ジャイロセンサの角位置をゼロにリセットする．
 * \param port センサポート番号
 * \retval E_OK 正常終了
 * \retval E_ID 不正のセンサポート番号
 */
ER ev3_gyro_sensor_reset(sensor_port_t port);

/**
 * \~English
 * \brief 	   Get the distance by a ultrasonic sensor.
 * \details    Always returns 0 (error log is outputted) when an invalid sensor number is specified.
 * \param port Sensor port to be inquired
 * \return     Distance in centimeters.
 *
 * \~Japanese
 * \brief 	    超音波センサで距離を測定する．
 * \details     不正のセンサポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param  port センサポート番号
 * \return      距離（単位はセンチ）
 */
int16_t ev3_ultrasonic_sensor_get_distance(sensor_port_t port);

/**
 * \~English
 * \brief 	     Get a ultrasonic signal by a ultrasonic sensor.
 * \details      When an invalid sensor support number is specified, always returns false (error log is output).
 * \param port   Sensor port to be inquired
 * \retval true  A signal has been received
 * \retval false No signal has been received
 *
 * \~Japanese
 * \brief 	     超音波センサで超音波信号を検出する．
 * \details      不正のセンサポート番号を指定した場合，常に \a false を返す（エラーログが出力される）．
 * \param  port  センサポート番号
 * \retval true  超音波信号を検出した
 * \retval false 超音波信号を検出しなかった
 */
bool_t ev3_ultrasonic_sensor_listen(sensor_port_t port);

/**
 * \~English
 * \brief Structure for IR Seek values for all 4 channels
 *
 * \~Japanese
 * \brief IRビーコンの方位と距離を格納する構造体
 */ 
typedef struct {
    int8_t heading[4];  //!< \~English Heading  for channels 1-4 (-25 to 25)           \~Japanese 全て（４つ）のチャンネルの方位（-25～25）
    int8_t distance[4]; //!< \~English Distance for channels 1-4 (-128 and 0 to 100)   \~Japanese 全て（４つ）のチャンネルの距離（0〜100または-128）
} ir_seek_t; 

#define IR_RED_UP_BUTTON     1
#define IR_RED_DOWN_BUTTON   2
#define IR_BLUE_UP_BUTTON    4
#define IR_BLUE_DOWN_BUTTON  8
#define IR_BEACON_BUTTON     16

typedef struct {
    uint8_t channel[4];    //!< \~English IR Remote controller data for channels 1-4   \~Japanese 全て（４つ）のチャンネルのボタン入力のパタン
} ir_remote_t; 

/**
 * \~English
 * \brief      Get the distance using the infrared sensor.
 * \details    Always returns 0 (error log is outputted) when an invalid sensor number is specified.
 * \param port Sensor port to be inquired.
 * \return     Distance in percentage (0-100).
 *
 * \~Japanese
 * \brief      IRセンサで距離を測定する．
 * \details    不正のセンサポート番号を指定した場合，常に0を返す（エラーログが出力される）．
 * \param port センサポート番号
 * \return     距離（単位はパーセント）
 */
int8_t ev3_infrared_sensor_get_distance(sensor_port_t port);

/**
 * \~English
 * \brief      Gets values to seek a remote controller in beacon mode.
 * \details    When an invalid sensor support number is specified, always return 0 direction and distance of -128 (error log is output).
 * \param port Sensor port to be inquired.
 * \return     Struct with heading/distance for all (4) channels.
 *
 * \~Japanese
 * \brief      IRセンサでIRビーコンの方位と距離を測定する． 
 * \details    不正のセンサポート番号を指定した場合，常に0の方位と-128の距離を返す（エラーログが出力される）．
 * \param port センサポート番号
 * \return     全て（４つ）のチャンネルの方位と距離
 */
ir_seek_t ev3_infrared_sensor_seek(sensor_port_t port);

/**
 * \~English
 * \brief      Gets commands from IR remote controllers. 
 * \details    When an invalid sensor support number is specified, always return 0 pattern (error log is output).
 * \param port Sensor port to be inquired.
 * \return     Struct with details of the IR remote buttons pressed.
 *
 * \~Japanese
 * \brief      IRセンサでIRビーコンのボタン入力を検出する．
 * \details    不正のセンサポート番号を指定した場合，常に0のパタンを返す（エラーログが出力される）．
 * \param port センサポート番号
 * \return     全て（４つ）のチャンネルのボタン入力のパタン
 */
ir_remote_t ev3_infrared_sensor_get_remote(sensor_port_t port);

/**
 * \~English
 * \brief 	     Get the status of a touch sensor.
 * \details      When an invalid sensor support number is specified, always returns false (error log is output)
 * \param port   Sensor port to be inquired
 * \retval true  Pressed  pressed
 * \retval false Unpressed pressed
 *
 * \~Japanese
 * \brief 	     タッチセンサの状態を検出する．
 * \details      不正のセンサポート番号を指定した場合，常に \a false を返す（エラーログが出力される）．
 * \param port   センサポート番号
 * \retval true  押されている状態
 * \retval false 押されていない状態
 */
bool_t ev3_touch_sensor_is_pressed(sensor_port_t port);

/**
 * \~English
 * \brief 	     Measure acceleration with a HiTechnic NXT acceleration sensor.
 * \details      When an invalid sensor support number is specified, always returns false (error log is output)
 * \param port   Sensor port to be inquired
 * \param axes   Array to store the x/y/z axes data
 * \retval true  Axes [] is  updated
 * \retval false Axes[] is unchanged due to I2C busy
 *
 * \~Japanese
 * \brief 	     加速度センサ（HiTechnic社製）で加速度を測定する．
 * \details      不正のセンサポート番号を指定した場合，常に \a false を返す（エラーログが出力される）．
 * \param  port  センサポート番号
 * \param  axes  x軸、y軸、z軸の3方向の加速度を格納するための配列
 * \retval true  axes[]は更新された
 * \retval false axes[]は変更されなかった（前回のI2C操作が完成していない）
 */
bool_t ht_nxt_accel_sensor_measure(sensor_port_t port, int16_t axes[3]);

/**
 * \~English
 * \brief        Measure color with a HiTechnic NXT color sensor (NCO1038).
 * \details      When an invalid sensor support number is specified, always returns false (error log is output)
 * \param port   Sensor port to be inquired
 * \param color  Pointer to store color number (0-17)
 * \retval true  color is updated
 * \retval false color is unchanged due to I2C busy
 *
 * \~Japanese
 * \brief        カラーセンサ（HiTechnic社製）でカラーを測定する．
 * \details      不正のセンサポート番号を指定した場合，常に \a false を返す（エラーログが出力される）．
 * \param  port  センサポート番号
 * \param  color カラーの番号（0-17）を格納するためのポインタ
 * \retval true  colorは更新された
 * \retval false colorは変更されなかった（前回のI2C操作が完成していない）
 */
bool_t ht_nxt_color_sensor_measure_color(sensor_port_t port, uint8_t *color);

/**
 * \~English
 * \brief        Measure RGB raw value with a HiTechnic NXT color sensor (NCO1038).
 * \details      When an invalid sensor support number is specified, always returns false (error log is output)
 * \param port   Sensor port to be inquired
 * \param val    Pointer to store RGB raw value
 * \retval true  val is updated
 * \retval false val is unchanged due to I2C busy
 *
 * \~Japanese
 * \brief        カラーセンサ（HiTechnic社製）でRGB Raw値を測定する．
 * \details      不正のセンサポート番号を指定した場合，常に \a false を返す（エラーログが出力される）．
 * \param  port  センサポート番号
 * \param  val   RGB Raw値を格納するためのポインタ
 * \retval true  valは更新された
 * \retval false valは変更されなかった（前回のI2C操作が完成していない）
 */
bool_t ht_nxt_color_sensor_measure_rgb(sensor_port_t port, rgb_raw_t *val);

/**
 * \~English
 * \brief 	     Measure temperature with a NXT temperature sensor (9749).
 * \details      When an invalid sensor support number is specified, always returns false (error log is output)
 * \param port   Sensor port to be inquired
 * \param temp   Variable to store the temperature value
 * \retval true  Temp is updated)
 * \retval false Temp is unchanged due to I2C busy
 *
 * \~Japanese
 * \brief 	     NXT温度センサ（9749）で温度を測定する．
 * \details      不正のセンサポート番号を指定した場合，常に \a false を返す（エラーログが出力される）．
 * \param  port  センサポート番号
 * \param  temp  温度データ（°C）を格納するための変数へのポインタ
 * \retval true  tempは更新された
 * \retval false tempは変更されなかった（前回のI2C操作が完成していない）
 */
bool_t nxt_temp_sensor_measure(sensor_port_t port, float *temp);

/**
 * @} // End of group
 */

