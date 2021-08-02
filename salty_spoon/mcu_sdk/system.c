/****************************************Copyright (c)*************************
**                               Copyright (C) 2014-2020, Tuya Inc., All Rights Reserved
**
**                                 http://www.tuya.com
**
**--------------File Information-------------------------------------------------------

**file name :system.c
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

#define SYSTEM_GLOBAL

#include "bluetooth.h"
//
//
extern const DOWNLOAD_CMD_S download_cmd[];

/*****************************************************************************
Function name: set_bt_uart_byte
Function description: Writes 1 byte to BT_UART
Input parameters: dest: the actual address of the buffer area;
           byte:Write byte value
Return parameter: the total length after writing
*****************************************************************************/
unsigned short set_bt_uart_byte(unsigned short dest, unsigned char byte)
{
  unsigned char *obj = (unsigned char *)bt_uart_tx_buf + DATA_START + dest;
  
  *obj = byte;
  dest += 1;
  
  return dest;
}
/*****************************************************************************
Function name: set_bt_uart_buffer
Function description: Writes a buffer to BT_UART
Input parameter: dest: destination address
           src:source address
           len:Data length
Return parameter: none
*****************************************************************************/
unsigned short set_bt_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len)
{
  unsigned char *obj = (unsigned char *)bt_uart_tx_buf + DATA_START + dest;
  
  my_memcpy(obj,src,len);
  
  dest += len;
  return dest;
}
/*****************************************************************************
Function name: bt_uart_write_data
Function description: Writes continuous data to BT UART
Input parameters: in:  buffer pointer
		   len: data length
Return parameter: none
*****************************************************************************/
static void bt_uart_write_data(unsigned char *in, unsigned short len)
{
  if((NULL == in) || (0 == len))
  {
    return;
  }
  while(len --)
  {
    uart_transmit_output(*in);
    in ++;
  }
}
/*****************************************************************************
Function name:get_check_sum
Function description: calculate checksum
Input parameter: pack: data source pointer
           pack_len:data length
Return parameter: checksum
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
  unsigned short i;
  unsigned char check_sum = 0;
  
  for(i = 0; i < pack_len; i ++)
  {
    check_sum += *pack ++;
  }
  
  return check_sum;
}
/*****************************************************************************
Function name: bt_uart_write_frame
Function description: Send a frame of data to the bt uart port
Input parameter: fr_type:frame type
           len:data length
Return parameter: none
*****************************************************************************/
void bt_uart_write_frame(unsigned char fr_type, unsigned short len)
{
  unsigned char check_sum = 0;
  
  bt_uart_tx_buf[HEAD_FIRST] = 0x55;
  bt_uart_tx_buf[HEAD_SECOND] = 0xaa;
  bt_uart_tx_buf[PROTOCOL_VERSION] = 0x00;
  bt_uart_tx_buf[FRAME_TYPE] = fr_type;
  bt_uart_tx_buf[LENGTH_HIGH] = len >> 8;
  bt_uart_tx_buf[LENGTH_LOW] = len & 0xff;
  
  len += PROTOCOL_HEAD;
  check_sum = get_check_sum((unsigned char *)bt_uart_tx_buf, len - 1);
  bt_uart_tx_buf[len - 1] = check_sum;
  //
  bt_uart_write_data((unsigned char *)bt_uart_tx_buf, len);
}
/*****************************************************************************
Function name: heat_beat_check
Function description: Heartbeat packet detection
Input parameters: none
Return parameter: none
*****************************************************************************/
static void heat_beat_check(void)
{
  unsigned char length = 0;
  static unsigned char mcu_reset_state = FALSE;
  
  if(FALSE == mcu_reset_state)
  {
    length = set_bt_uart_byte(length,FALSE);
    mcu_reset_state = TRUE;
  }
  else
  {
    length = set_bt_uart_byte(length,TRUE);
  }
  
  bt_uart_write_frame(HEAT_BEAT_CMD, length);
}
/*****************************************************************************
Function name: product_info_update
Function description: upload product information
Input parameters: none
Return parameter: none
*****************************************************************************/
static void product_info_update(void)
{
  unsigned char length = 0;
  
  length = set_bt_uart_buffer(length,(unsigned char *)PRODUCT_KEY,my_strlen((unsigned char *)PRODUCT_KEY));
  length = set_bt_uart_buffer(length,(unsigned char *)MCU_VER,my_strlen((unsigned char *)MCU_VER));
  
  bt_uart_write_frame(PRODUCT_INFO_CMD, length);
}
/*****************************************************************************
Function name: get_mcu_bt_mode
Function description: query the working mode of mcu and bt
Input parameters: none
Return parameter: none
*****************************************************************************/
static void get_mcu_bt_mode(void)
{
  unsigned char length = 0;
  
#ifdef BT_CONTROL_SELF_MODE                                   //Module self-processing
  length = set_bt_uart_byte(length, BT_STATE_KEY);
  length = set_bt_uart_byte(length, BT_RESERT_KEY);
#else                                                           
  //No need to process data
#endif
  
  bt_uart_write_frame(WORK_MODE_CMD, length);
}
/*****************************************************************************
Function name: get_update_dpid_index
Function description: Get the serial number of DPID in the array
Input parameters: dpid:dpid
Return parameter: index: DP serial number
*****************************************************************************/
static unsigned char get_dowmload_dpid_index(unsigned char dpid)
{
  unsigned char index;
  unsigned char total = get_download_cmd_total();
  
  for(index = 0; index < total; index ++)
  {
    if(download_cmd[index].dp_id == dpid)
    {
      break;
    }
  }
  
  return index;
}
/*****************************************************************************
Function name: data_point_handle
Function description: send data processing
Input parameter: 
	value: the pointer of the data source issued
Return parameter: 
	ret: return data processing result
*****************************************************************************/
static unsigned char data_point_handle(const unsigned char value[])
{
  unsigned char dp_id,index;
  unsigned char dp_type;
  unsigned char ret;
  unsigned short dp_len;
  
  dp_id = value[0];
  dp_type = value[1];
  dp_len = value[2] * 0x100;
  dp_len += value[3];
  
  index = get_dowmload_dpid_index(dp_id);

  if(dp_type != download_cmd[index].dp_type)
  {
    //Error message
    return FALSE;
  }
  else
  {
    ret = dp_download_handle(dp_id,value + 4,dp_len);
  }
  
  return ret;
}
/*****************************************************************************
Function name: data_handle
Function description: data frame processing
Input parameter: 
	Offset: Data start bit
Return parameter: none
*****************************************************************************/
void data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
  unsigned char *firmware_addr;
  static unsigned long firm_length;                                             //MCU upgrades file length
  static unsigned char firm_update_flag;                                        //MCU upgrade flag
  unsigned long dp_len;
#else
  unsigned short dp_len;
#endif
  unsigned char length = 0;
  
  unsigned char ret;
  unsigned short i,total_len;
  unsigned char cmd_type = bt_uart_rx_buf[offset + FRAME_TYPE];


  signed char bt_rssi;

#ifdef TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE 
  bt_time_struct_data_t bt_time;
  unsigned short time_zone_100;
  char current_timems_string[14] = "000000000000";
   long time_stamp_ms;
#endif

  switch(cmd_type)
  {
  case HEAT_BEAT_CMD:                                   //Heartbeat package
    heat_beat_check();
    break;
    
  case PRODUCT_INFO_CMD:                                //product information
    product_info_update();
    break;
    
  case WORK_MODE_CMD:                                   //Query module working mode set by MCU
    get_mcu_bt_mode();
    break;
    
#ifndef BT_CONTROL_SELF_MODE
  case BT_STATE_CMD:                                  //bt work state
    bt_work_state = bt_uart_rx_buf[offset + DATA_START];
    if(bt_work_state==0x01||bt_work_state==0x00)
    {
    	mcu_ota_init_disconnect();

    }
    bt_uart_write_frame(BT_STATE_CMD,0);
    break;

  case BT_RESET_CMD:                                  //Reset BT (BT returns success)
    reset_bt_flag = RESET_BT_SUCCESS;
    break;
#endif
    
  case DATA_QUERT_CMD:                                  //dp data handled
    total_len = bt_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    total_len += bt_uart_rx_buf[offset + LENGTH_LOW];
    
    for(i = 0;i < total_len;)
    {
      dp_len = bt_uart_rx_buf[offset + DATA_START + i + 2] * 0x100;
      dp_len += bt_uart_rx_buf[offset + DATA_START + i + 3];
      //
      ret = data_point_handle((unsigned char *)bt_uart_rx_buf + offset + DATA_START + i);
      
      if(SUCCESS == ret)
      {
        //Success tips
      }
      else
      {
        //Error message
      }
      
      i += (dp_len + 4);
    }
    
    break;
    
  case STATE_QUERY_CMD:                                 //Status query
    all_data_update();                               
    break;
    
#ifdef TUYA_BCI_UART_COMMON_RF_TEST 
	case TUYA_BCI_UART_COMMON_RF_TEST:
		if(my_memcmp((unsigned char *)bt_uart_rx_buf + offset + DATA_START+7,"true",4)==0)
		{
			bt_rssi = (bt_uart_rx_buf[offset + DATA_START+21]-'0')*10 + (bt_uart_rx_buf[offset + DATA_START+22]-'0');
			bt_rssi = -bt_rssi;
			bt_rf_test_result(1,bt_rssi);
		}
		else
		{
			bt_rf_test_result(0,0);
		}
		break;
#endif

#ifdef TUYA_BCI_UART_COMMON_SEND_STORAGE_TYPE 
	case TUYA_BCI_UART_COMMON_SEND_STORAGE_TYPE:
		bt_send_recordable_dp_data_result(bt_uart_rx_buf[offset + DATA_START]);
		break;
#endif

#ifdef TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE 
	case TUYA_BCI_UART_COMMON_SEND_TIME_SYNC_TYPE:
		ret = bt_uart_rx_buf[offset + DATA_START];
		if(ret==0)//Get time succeeded
		{
			if(bt_uart_rx_buf[offset + DATA_START+1]==0x00)//Time format 0 :Get 7 bytes of time and time type + 2 bytes of time zone information
			{
				bt_time.nYear = bt_uart_rx_buf[offset + DATA_START+2] + 2018;

				bt_time.nMonth = bt_uart_rx_buf[offset + DATA_START+3];
				bt_time.nDay = bt_uart_rx_buf[offset + DATA_START+4];
				bt_time.nHour = bt_uart_rx_buf[offset + DATA_START+5];
				bt_time.nMin = bt_uart_rx_buf[offset + DATA_START+6];
				bt_time.nSec = bt_uart_rx_buf[offset + DATA_START+7];
				bt_time.DayIndex = bt_uart_rx_buf[offset + DATA_START+8];
				time_zone_100 = ((unsigned short)bt_uart_rx_buf[offset + DATA_START+9]<<8)+bt_uart_rx_buf[offset + DATA_START+10];
			}
			else if(bt_uart_rx_buf[offset + DATA_START+1]==0x01)//Time format 1: Get 13 bytes of ms-level unix time + 2 bytes of time zone information
			{
				my_memcpy(current_timems_string,&bt_uart_rx_buf[offset + DATA_START+2],13);
				time_stamp_ms = my_atoll(current_timems_string);
				time_zone_100 = ((unsigned short)bt_uart_rx_buf[offset + DATA_START+15]<8)+bt_uart_rx_buf[offset + DATA_START+16];
			}
			else if(bt_uart_rx_buf[offset + DATA_START+1]==0x02)//Time format 2: Get 7 bytes of time and time type + 2 bytes of time zone information
			{
				bt_time.nYear = bt_uart_rx_buf[offset + DATA_START+2] + 2000;
				bt_time.nMonth = bt_uart_rx_buf[offset + DATA_START+3];
				bt_time.nDay = bt_uart_rx_buf[offset + DATA_START+4];
				bt_time.nHour = bt_uart_rx_buf[offset + DATA_START+5];
				bt_time.nMin = bt_uart_rx_buf[offset + DATA_START+6];
				bt_time.nSec = bt_uart_rx_buf[offset + DATA_START+7];
				bt_time.DayIndex = bt_uart_rx_buf[offset + DATA_START+8];
				time_zone_100 = ((unsigned short)bt_uart_rx_buf[offset + DATA_START+9]<<8)+bt_uart_rx_buf[offset + DATA_START+10];
			}
			bt_time_sync_result(0,bt_uart_rx_buf[offset + DATA_START+1],bt_time,time_zone_100,time_stamp_ms);
		}
		else//Failed to get time
		{
			bt_time_sync_result(1,bt_uart_rx_buf[offset + DATA_START+1],bt_time,time_zone_100,time_stamp_ms);
		}
		break;
#endif

#ifdef TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL
	case TUYA_BCI_UART_COMMON_MODIFY_ADV_INTERVAL:
		bt_modify_adv_interval_result(bt_uart_rx_buf[offset + DATA_START]);
		break;
#endif
#ifdef TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME
	case TUYA_BCI_UART_COMMON_TURNOFF_SYSTEM_TIME:
	  bt_close_timer_result(bt_uart_rx_buf[offset + DATA_START]);
	  break;
#endif
#ifdef TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER
	case TUYA_BCI_UART_COMMON_ENANBLE_LOWER_POWER:
		bt_enable_lowpoer_result(bt_uart_rx_buf[offset + DATA_START]);
		break;
#endif
#ifdef TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN
	case TUYA_BCI_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN:
	  bt_send_one_time_password_token_result(bt_uart_rx_buf[offset + DATA_START]);
	  break;
#endif
#ifdef TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT
	case TUYA_BCI_UART_COMMON_ACTIVE_DISCONNECT:
		bt_disconnect_result(bt_uart_rx_buf[offset + DATA_START]);
		break;
#endif
#ifdef TUYA_BCI_UART_COMMON_QUERY_MCU_VERSION
	case TUYA_BCI_UART_COMMON_QUERY_MCU_VERSION:  
	  length = set_bt_uart_buffer(length,(unsigned char *)MCU_APP_VER_NUM,3);
	  length = set_bt_uart_buffer(length,(unsigned char *)MCU_HARD_VER_NUM,3);
	  bt_uart_write_frame(TUYA_BCI_UART_COMMON_QUERY_MCU_VERSION,length);
	  break;
#endif
#ifdef TUYA_BCI_UART_COMMON_FACTOR_RESET_NOTIFY
	case TUYA_BCI_UART_COMMON_FACTOR_RESET_NOTIFY:	
		bt_factor_reset_notify();
		break;
#endif
#ifdef SUPPORT_MCU_FIRM_UPDATE
	  case TUYA_BCI_UART_COMMON_MCU_OTA_REQUEST:
	  case TUYA_BCI_UART_COMMON_MCU_OTA_FILE_INFO:
	  case TUYA_BCI_UART_COMMON_MCU_OTA_FILE_OFFSET:
	  case TUYA_BCI_UART_COMMON_MCU_OTA_DATA:
	  case TUYA_BCI_UART_COMMON_MCU_OTA_END:
		total_len = bt_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
		total_len += bt_uart_rx_buf[offset + LENGTH_LOW];
		//mcu_ota_proc(cmd_type,&bt_uart_rx_buf[offset + DATA_START],total_len);
	  	break;
#endif   

  default:
    break;
  }
}
/*****************************************************************************
Function name:get_queue_total_data
Function description: read data in the queue
Input parameters: none
Return parameter: none
*****************************************************************************/
unsigned char get_queue_total_data(void)
{
  if(queue_in != queue_out)
    return 1;
  else
    return 0;
}
/*****************************************************************************
Function name:Queue_Read_Byte
Function description: Read 1 byte data of queue
Input parameters: none
Return parameter: none
*****************************************************************************/
unsigned char Queue_Read_Byte(void)
{
  unsigned char value;
  
  if(queue_out != queue_in)
  {
    //Data is not empty
    if(queue_out >= (unsigned char *)(bt_queue_buf + sizeof(bt_queue_buf)))
    {
      //Data has reached the end
      queue_out = (unsigned char *)(bt_queue_buf);
    }
    
    value = *queue_out ++;   
  }
  
  return value;
}
