/****************************************Copyright (c)*************************
**                               Copyright (C) 2014-2020, Tuya Inc., All Rights Reserved
**
**                                 http://www.tuya.com
**
**--------------File Information-------------------------------------------------------

** file name :protocol.h
** description: 
* * instructions for use

                  *******important, be sure to see it!********

* * 1. Users implement the function of sending / reporting data in this file
* * 2. The ID/TYPE and data processing functions of DP need to be implemented according to the actual definition.
* * 3. There is a # err prompt inside the function that requires the user to implement the code after starting some macro definitions. Delete the # err after completing the function.
**
**--------------Revision record---------------------------------------------------
** version: v1.0
** date : may 3, 2017 
description: Initial version
**

**version::v2.0
** date: March 23, 2020
** description: 
1. Added module unbinding interface support, command code 0x09.
2.Add rf RF test interface support, command code 0x0e.
3.Add record-based data reporting interface support,command code 0xe0.
4. Added access to real-time time API support,command code 0xe1.
5. Added support for modifying sleep mode state bluetooth broadcast interval,command code 0xe2.
6. Added support for turning off system clock,command code 0xe4.
7. Increase low power consumption to enable support,commadn code 0xe5.
8. Add dynamic password authentication interface support,command code 0xe6.
9. Added support for disconnecting Bluetooth connection,command code 0xe7.
10. Added support for querying MCU version number,command code 0xe8.
11. Added support for MCU to actively send version Numbers,command code 0xe9.
12. Add OTA upgrade request support,command code 0xea.
13. Add OTA update file information support,command 0xeb.
14. Add OTA upgrade file migration request support,command code 0xec.
15. Add OTA upgrade data support,command code 0xed.
16. Add OTA upgrade end support,command code 0xee.
17. Added support for MCU to acquire module version information,commadn code 0xa0.
18. Added support for resuming factory Settings notifications,command code 0xa1.
19. Add MCU OTA demo code.
20. Optimized bt_uart_service.
**
**-----------------------------------------------------------------------------
******************************************************************************/

#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_



/******************************************************************************
							Configuration of user-related information
******************************************************************************/
/******************************************************************************
							1: modify product information
******************************************************************************/
#define PRODUCT_KEY "gvudhnkw"    //开发平台创建产品后生成的16位字符产品唯一标识

#include "bluetooth.h"

#define MCU_VER "1.0.0"                                 //User's software version, used for MCU firmware upgrade, MCU upgrade version needs to be modified
#define MCU_APP_VER_NUM      0x010000					//User's software version, used for MCU firmware upgrade, MCU upgrade version needs to be modified eg.1.0.0
#define MCU_HARD_VER_NUM     0x010000					//The hardware version of the user is currently of no practical use


/******************************************************************************
                          2:Does MCU need firmware upgrade?
If you need to support MCU firmware upgrade, please open this macro
                        ********WARNING!!!**********
The current receive buffer is the size of turning off the firmware update function, and the firmware upgrade package is 256 bytes.
If you need to turn on this function, the serial port receiving buffer will become larger.
******************************************************************************/
#define         SUPPORT_MCU_FIRM_UPDATE                 //Enable MCU firmware upgrade (default)
/******************************************************************************
						 3: define the sending and receiving cache:
					If there is not enough RAM for MCU, it can be changed to 24.
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define BT_UART_QUEUE_LMT             16              //The size of the data receiving queue can be reduced if the RAM of MCU is not enough.
#define BT_UART_RECV_BUF_LMT          128              //According to the size of the user's DP data, it must be greater than 32
#else
#define BT_UART_QUEUE_LMT             100             //The size of the data receiving queue can be reduced if the RAM of MCU is not enough.
#define BT_UART_RECV_BUF_LMT          100             //Firmware upgrade buffer, large cache required, must be greater than 260
#endif

#define BT_UART_SEND_BUF_LMT         512              //According to the size of the user's DP data, it must be greater than 32
/******************************************************************************
						4: define how the module works
Module self-processing:
		  Bt indicator and BT Reset button are connected to the BT module (turn on the BT_CONTROL_SELF_MODE macro)
		  and define BT_STATE_KEY and BT_RESET_KEY correctly
MCU self-processing:
		  The BT indicator and bt Reset button are connected to the MCU (turn off the BT_CONTROL_SELF_MODE macro)
		  The MCU calls the McU_reset_bt () function in the McU_api.c file where the bt reset needs to be handled, and can call the McU_get_reset_bt_flag () function to return the bt reset result
		  Or call the McU_set_bt_mode (BT_CONFIG_E mode) function in the file McU_api.c to set BT mode, and call the McU_get_bt_work_state () function to return the result of setting BT
******************************************************************************/
//#define         BT_CONTROL_SELF_MODE                       / / bt self-processing button and LED indicator; if it is MCU external button / LED indicator, please turn off the macro
#ifdef          BT_CONTROL_SELF_MODE                      // Module self-processing
  #define     BT_STATE_KEY            14                    //Bt module status indication button, please set according to the actual GPIO pin
  #define     BT_RESERT_KEY           0                     //Bt module reset button, please set according to the actual GPIO pin
#endif

/******************************************************************************
					  5: clock function configuration

******************************************************************************/
#define         SUPPORT_MCU_RTC_CHECK                //open

/******************************************************************************
						1:dp data point serial number redefinition
		  * * this is automatically generated code. If there are any changes on the development platform, please download MCU_SDK again.**
******************************************************************************/
//频率(可下发可上报)
//备注:
#define DPID_FREQUENCY 101
//电池电量(只上报)
//备注:
#define DPID_VOLTAGE 102





/*****************************************************************************
Function name: all_data_update
Function description: upload all dp information of the system to achieve data synchronization between APP and muc
Input parameters: none
Return parameter: none
Instructions for use: this function needs to be called internally in SDK
		   MCU must implement the data reporting function within this function, including reporting only and downloadable hairstyle data.
*****************************************************************************/

void all_data_update(void);



/*****************************************************************************
Function name: DP_download_handle
Function description: DP release processing function
Input parameter: DPID :DP serial number
           value:Dp data buffer address
           length:dp data length
Return parameters:SUCCESS/ERRO
Instructions: This function cannot be modified by the user
*****************************************************************************/

unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);
/*****************************************************************************
Function name: get_download_cmd_total
Function description: Gets the sum of all DP commands
Input parameters: None
Return parameter: issue the sum of commands
Instructions: This function cannot be modified by the user
*****************************************************************************/

unsigned char get_download_cmd_total(void);


//////////////////////////////////The current version of MCU SDK has a new support protocol interface over the previous version////////////////////
/*****************************************************************************
Function name: bt_unbound_req
Function description: Send the unbind request to the module, and the module will unbind the Bluetooth connection after receiving the instruction
Input parameters: None
Return parameter: none
Instructions: The MCU calls for active untying
*****************************************************************************/

void bt_unbound_req(void);
/*****************************************************************************
Function name: bt_rf_test_req
Function description: transmit frequency test request to the module
Input parameters: None
Return parameter: none
Instructions for use:
*****************************************************************************/

void bt_rf_test_req(void);
/*****************************************************************************
Function name: bt_send_recordable_DP_data
Function description: report the recorded data
Input parameters: Type-1: Bluetooth module built-in time report -2: original data only report, no time -3: MCU built-in time report
		Dpid: former datapoint serial number
		Dptype: Corresponds to a datapoint specific data type on the open platform
		value:
		len:
Return parameter: none
Instructions: the MCU needs to improve the function itself
	It is recommended to use the cache queue. All data to be sent to the module should be put into the MCU cache queue, and the next data should be reported after one has been reported successfully. The recorded data should ensure that each data has been reported successfully
*****************************************************************************/
void bt_rf_test_result(unsigned char result,signed char rssi);
/*****************************************************************************
Function name: bt_send_recordable_DP_data
Function description: report the recorded data
Input parameters: Type-1: Bluetooth module built-in time report -2: original data only report, no time -3: MCU built-in time report
		Dpid: former datapoint serial number
		Dptype: Corresponds to a datapoint specific data type on the open platform
		value:
		len:
Return parameter: none
Instructions: the MCU needs to improve the function itself
	It is recommended to use the cache queue. All data to be sent to the module should be put into the MCU cache queue, and the next data should be reported after one has been reported successfully. The recorded data should ensure that each data has been reported successfully
*****************************************************************************/

void bt_send_recordable_dp_data(unsigned char snedType,unsigned char dpid,unsigned char dpType, unsigned char value[],unsigned short len);
/*****************************************************************************
Function name: bt_send_recordable_dp_data_result
Function description: report the recorded data
Input parameter :result: 0 storage success, 1 storage failure
Return parameter: none
Instructions: the MCU needs to improve the function itself
*****************************************************************************/

void bt_send_recordable_dp_data_result(unsigned char result);
/*****************************************************************************
Function name: bt_modify_adv_interval_req
Function description: send a request to the module to modify the broadcast interval of the module at low power consumption
Input parameter: value * 100ms equals the broadcast interval, value (0-20 to be modified)
Return parameter: none
Instructions for use:
*****************************************************************************/

void bt_modify_adv_interval_req(unsigned char value);
/*****************************************************************************
Function name: bt_modify_adv_interval_result
Function description:Processing the result of modifying the broadcast interval
Input parameters: result synchronization result 0 successful, other failed

Return parameter: none
Instructions: MCU needs to improve the function on its own.
*****************************************************************************/
void bt_modify_adv_interval_result(unsigned char result);
/*****************************************************************************
Function name: bt_close_timer_req
Function description: send a request to the module to turn off the system clock (currently available on telink platform only)
Input parameters: value 0 off, 1 on
Return parameter: none
Instructions for use:
*****************************************************************************/

void bt_close_timer_req(unsigned char value);
/*****************************************************************************
Function name: bt_close_timer_result
Function description: processing result
Input parameters: 0 successful, other failed

Return parameter: none
Instructions: MCU needs to improve the function on its own.
*****************************************************************************/

void bt_close_timer_result(unsigned char result);
/*****************************************************************************
Function name: bt_enable_lowpoer_req
Function description: send a request to enable low power consumption to the module (currently only applicable to telink platform)
Input parameters: value 0 off, 1 on
Return parameter: none
*****************************************************************************/

void bt_enable_lowpoer_req(unsigned char value);
/*****************************************************************************
Function name: bt_enable_lowpoer_req
Function description: send a request to enable low power consumption to the module (currently only applicable to telink platform)
Input parameters: value 0 off, 1 on
Return parameter: none
*****************************************************************************/

void bt_enable_lowpoer_result(unsigned char result);
/*****************************************************************************
Function name: bt_send_one_time_password_token_result
Function description: get the result of one-time dynamic password matching from the module
Input parameters:0x00 password check passed, 0x01 password check failed
Return parameter: none
Instructions: MCU needs to improve the function on its own.
*****************************************************************************/

unsigned char bt_send_one_time_password_token(unsigned char value[],unsigned char len);
/*****************************************************************************
Function name: bt_send_one_time_password_token_result
Function description: get the result of one-time dynamic password matching from the module
Input parameters:0x00 password check passed, 0x01 password check failed
Return parameter: none
Instructions: MCU needs to improve the function on its own.
*****************************************************************************/

void bt_send_one_time_password_token_result(unsigned char result);
/*****************************************************************************
Function name: bt_disconnect_req
Function description: send a request to disconnect the Bluetooth connection to the module
Input parameters: value 0 off, 1 on
Return parameter: none
Instructions for use:
*****************************************************************************/

void bt_disconnect_req(void);
/*****************************************************************************
Function name: bt_disconnect_result
Function description: receive the result that the module is disconnected from Bluetooth
Input parameters: result result 0 successful, other failed

Return parameter: none
Instructions: MCU needs to improve the function on its own.
*****************************************************************************/

void bt_disconnect_result(unsigned char result);
/*****************************************************************************
Function name: bt_factor_reset_notify
Function description: notification sent to mcu after the module resumes factory settings

Return parameter: none
Instructions for use: MCU can complete the operation of restoring factory settings with MCU here.
*****************************************************************************/

void bt_factor_reset_notify(void);



/*****************************************************************************
Function name: bt_send_mcu_ver
Function description: send the MCU version number to the module actively, mainly in order that the module can obtain the MCU version information in a more timely manner.

Return parameter: none
Instructions: MCU can be called once after initialization of serial port.
*****************************************************************************/
void bt_send_mcu_ver(void);

/*****************************************************************************
Function name: bt_time_sync_result
Function description: send the result of time synchronization to the module
Input parameters: result synchronization result 0 successful, other failed
		sync_time_type :time format
		Bt_time: Custom time (valid if time format 0 or 1)
		Time_zone_100: time zone
		Time_stamp_ms: timestamp (valid if it is in time format 1)
Return parameter: none
Instructions: MCU needs to improve the function on its own.
*****************************************************************************/
#include "system.h"
void bt_time_sync_result(unsigned char result,unsigned char sync_time_type,bt_time_struct_data_t bt_time,unsigned short time_zone_100, long time_stamp_ms);


void bt_send_time_sync_req(unsigned char sync_time_type);


#endif

