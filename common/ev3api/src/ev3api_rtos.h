/**
 * \file    ev3rtos.h
 * \brief	API for EV3RT-defined RTOS functions
 * \author	ertl-liyixiao
 */

#pragma once

/**
 * \~English
 * \defgroup ev3api-rtos Extended RTOS function
 * \brief    Definitions of EV3RT specific API for RTOS function.
 * @{
 *
 * \~Japanese
 * \defgroup ev3api-rtos 拡張RTOS機能
 * \brief    EV3RT独自のRTOS機能に関するAPI．
 * @{
 */

/**
 * \~English
 * \brief            Start an EV3 cyclic handler.
 * \param  ev3cycid  ID number of cyclic handler for EV 3 (specified by EV3_CRE_CYC）
 * \retval E_OK      Successful completion
 * \retval E_CTX     Call from non-task context
 * \retval E_ID      Invalid ID number
 *
 * \~Japanese
 * \brief            EV3用周期ハンドラの動作を開始する．
 * \param  ev3cycid  EV3用周期ハンドラのID番号（EV3_CRE_CYCで指定）
 * \retval E_OK      正常終了
 * \retval E_CTX     非タスクコンテキストからの呼出し
 * \retval E_ID      不正ID番号
 */
ER ev3_sta_cyc(ID ev3cycid);

/**
 * \~English
 * \brief            Stop an EV3 cyclic handler.
 * \param  ev3cycid  ID number of cyclic handler for EV 3 (specified by EV3_CRE_CYC）
 * \retval E_OK      Successful completion
 * \retval E_CTX     Call from non-task context
 * \retval E_ID      Invalid ID number
 *
 * \~Japanese
 * \brief            EV3用周期ハンドラの動作を停止する．
 * \param  ev3cycid  EV3用周期ハンドラのID番号（EV3_CRE_CYCで指定）
 * \retval E_OK      正常終了
 * \retval E_CTX     非タスクコンテキストからの呼出し
 * \retval E_ID      不正ID番号
 */
ER ev3_stp_cyc(ID ev3cycid);

/**
 * @} // End of group
 */
