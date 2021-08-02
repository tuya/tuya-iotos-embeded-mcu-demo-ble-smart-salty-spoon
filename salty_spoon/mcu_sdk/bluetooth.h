/****************************************Copyright (c)*************************
**                               Copyright (C) 2014-2020, Tuya Inc., All Rights Reserved
**
**                                 http://www.tuya.com
**
**--------------File Information-------------------------------------------------------
** file name: bluetooth.h
** description: bluetooth file header definition 
** instructions for use: users do not need to care about the content of this file
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
#ifndef __BLUETOOTH_H_
#define __BLUETOOTH_H_


//=============================================================================
#ifndef TRUE
#define         TRUE                1
#endif
//
#ifndef FALSE
#define         FALSE               0
#endif
//
#ifndef NULL
#define         NULL                ((void *) 0)
#endif

#ifndef SUCCESS
#define         SUCCESS             1
#endif

#ifndef ERROR
#define         ERROR               0
#endif

#ifndef INVALID
#define         INVALID             0xFF
#endif

#ifndef ENABLE
#define         ENABLE                1
#endif
//
#ifndef DISABLE
#define         DISABLE               0
#endif
//=============================================================================
//dp data point type
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW
#define         DP_TYPE_BOOL                    0x01	                        //Bool
#define         DP_TYPE_VALUE                   0x02	                        //Value
#define         DP_TYPE_STRING                  0x03				//String
#define         DP_TYPE_ENUM                    0x04				//Enum
#define         DP_TYPE_BITMAP                  0x05				//Bitmap

//=============================================================================
//BT work state
//=============================================================================
#define         BT_UN_BIND                      0x00                            //Unbound state
#define         BT_NOT_CONNECTED                0x01                            //Bound but not connected
#define         BT_CONNECTED                    0x02                            //Bound and connected
#define         BT_SATE_UNKNOW                  0xff

//=============================================================================
//bt reset state
//=============================================================================
#define         RESET_BT_ERROR                  0
#define         RESET_BT_SUCCESS                1

//=============================================================================
//MCU firmware upgrade status
//=============================================================================
#define         FIRM_STATE_UN_SUPPORT           0x00                            //MCU upgrade is not supported
#define         FIRM_STATE_BT_UN_READY          0x01                            //Module not ready
#define         FIRM_STATE_GET_ERROR            0x02                            //Failed to query cloud upgrade information
#define         FIRM_STATE_NO                   0x03                            //No upgrade required (no updated version in the cloud)
#define         FIRM_STATE_START                0x04                            //Need to upgrade. Wait for the module to initiate the upgrade operation.

//=============================================================================
//The way BT and mcu work
//=============================================================================
#define         UNION_WORK                      0x0                             //Mcu module cooperates with bt
#define         BT_ALONE                      0x1                             //Bt module self-processing

//=============================================================================
//System working mode
//=============================================================================
#define         NORMAL_MODE             0x00                                    //Normal working state
#define         FACTORY_MODE            0x01                                    //Factory model
#define         UPDATE_MODE             0X02                                    //Upgrade mode

#include "protocol.h"
#include "system.h"
#include "mcu_api.h"
//#include "mcu_ota_handler.h"
#include "main.h"
//=============================================================================
//download dp cmd
//=============================================================================
typedef struct {
  unsigned char dp_id;                        //dp id
  unsigned char dp_type;                          //dp type
} DOWNLOAD_CMD_S;

#endif
