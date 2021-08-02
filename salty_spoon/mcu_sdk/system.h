/****************************************Copyright (c)*************************
**                               Copyright (C) 2014-2020, Tuya Inc., All Rights Reserved
**
**                                 http://www.tuya.com
**
**--------------File Information-------------------------------------------------------

**file name :system.h
**description:data processing function.
**instructions for use : The user does not need to care about the content of the file.
**
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
#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "bluetooth.h"

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//Byte order of the frame
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//Data frame type
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //Heartbeat package
#define         PRODUCT_INFO_CMD                1                               //product information
#define         WORK_MODE_CMD                   2                               //Query module working mode set by MCU
#define         BT_STATE_CMD                    3                               //bluetooth work state
#define         BT_RESET_CMD                    4                               //reset thr bluetooth
#define         DATA_QUERT_CMD                  6                               //dp data form bt moudle
#define         STATE_UPLOAD_CMD                7                               //report dp data to bt moudle
#define         STATE_QUERY_CMD                 8                               //query dp state

//////////////////////////////////Compared with the previous version, the current MCU SDK version supports new protocol interfaces////////////////////
//If you don’t need a certain command interface, just comment out the definition of the command macro, and the related code will not be compiled to reduce the code space
#define TUYA_BCI_UART_COMMON_UNBOUND_REQ					0x09				//unbind
#define TUYA_BCI_UART_COMMON_RF_TEST	            	    0x0E				//rf radio frequency test
#define TUYA_BCI_UART_COMMON_SEND_STORAGE_TYPE              0xE0				//Recorded dp data report (offline cache)
#define TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE            0xE1				//Get real time
#define TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL		    0xE2				//Modify the sleep mode broadcast interval
#define TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME		    0xE4				//Turn off the system clock function
#define TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER		    0xE5				//Low power enable
#define TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN 	0xE6				//Dynamic password verification
#define TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT			    0xE7				//Disconnect device Bluetooth connection

#define TUYA_BCI_UART_COMMON_QUERY_MCU_VERSION			    0xE8				//Query MCU version number
#define TUYA_BCI_UART_COMMON_MCU_SEND_VERSION			    0xE9				//MCU actively sends the version number

#define TUYA_BCI_UART_COMMON_MCU_OTA_REQUEST			    0xEA				//OTA upgrade request
#define TUYA_BCI_UART_COMMON_MCU_OTA_FILE_INFO			    0xEB				//OTA upgrade request
#define TUYA_BCI_UART_COMMON_MCU_OTA_FILE_OFFSET	        0xEC				//OTA upgrade file offset request
#define TUYA_BCI_UART_COMMON_MCU_OTA_DATA 			        0xED				//OTA upgrade data
#define TUYA_BCI_UART_COMMON_MCU_OTA_END			        0xEE				//OTA upgrade is over


//#define TUYA_BCI_UART_COMMON_QUERY_MOUDLE_VERSION			0xA0				//MCU obtains module version information

#define TUYA_BCI_UART_COMMON_FACTOR_RESET_NOTIFY			0xA1				//Factory reset notification


//#ifdef TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE 
typedef struct
{
    unsigned short nYear;
    unsigned char nMonth;
    unsigned char nDay;
    unsigned char nHour;
    unsigned char nMin;
    unsigned char nSec;
    unsigned char DayIndex; /* 0 = Sunday */
} bt_time_struct_data_t;
//#endif





//=============================================================================
#define         VERSION                 0x00                                            //Protocol version number
#define         PROTOCOL_HEAD           0x07                                            //Fixed protocol header length
#define         FIRM_UPDATA_SIZE        200                                            //Upgrade package size
#define         FRAME_FIRST             0x55
#define         FRAME_SECOND            0xaa
//============================================================================= 
SYSTEM_EXTERN unsigned char volatile bt_queue_buf[PROTOCOL_HEAD + BT_UART_QUEUE_LMT];  //Serial queue buffer
SYSTEM_EXTERN unsigned char bt_uart_rx_buf[PROTOCOL_HEAD + BT_UART_RECV_BUF_LMT];         //Serial receive buffer
SYSTEM_EXTERN unsigned char bt_uart_tx_buf[PROTOCOL_HEAD + BT_UART_SEND_BUF_LMT];        //Serial transmission buffer
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;

SYSTEM_EXTERN unsigned char stop_update_flag;

#ifndef BT_CONTROL_SELF_MODE
SYSTEM_EXTERN unsigned char reset_bt_flag;                                                  
SYSTEM_EXTERN unsigned char set_btmode_flag;                                                
SYSTEM_EXTERN unsigned char bt_work_state;                                                  
#endif


/*****************************************************************************
Function name: set_bt_uart_byte
Function description: Writes 1 byte to BT_UART
Input parameters: dest: the actual address of the buffer area;
           byte:Write byte value
Return parameter: the total length after writing
*****************************************************************************/
unsigned short set_bt_uart_byte(unsigned short dest, unsigned char byte);

/*****************************************************************************
Function name: set_bt_uart_buffer
Function description: Writes a buffer to BT_UART
Input parameter: dest: destination address
           src:source address
           len:Data length
Return parameter: none
*****************************************************************************/
unsigned short set_bt_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/*****************************************************************************
Function name: bt_uart_write_frame
Function description: Send a frame of data to the bt uart port
Input parameter: fr_type:frame type
           len:data length
Return parameter: none
*****************************************************************************/
void bt_uart_write_frame(unsigned char fr_type, unsigned short len);
/*****************************************************************************
Function name:get_check_sum
Function description: calculate checksum
Input parameter: pack: data source pointer
           pack_len:data length
Return parameter: checksum
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/*****************************************************************************
Function name: data_handle
Function description: data frame processing
Input parameter: 
	Offset: Data start bit
Return parameter: none
*****************************************************************************/
void data_handle(unsigned short offset);

/*****************************************************************************
Function name:get_queue_total_data
Function description: read data in the queue
Input parameters: none
Return parameter: none
*****************************************************************************/
unsigned char get_queue_total_data(void);

/*****************************************************************************
Function name:Queue_Read_Byte
Function description: Read 1 byte data of queue
Input parameters: none
Return parameter: none
*****************************************************************************/
unsigned char Queue_Read_Byte(void);

#endif
  
  