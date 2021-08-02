#ifndef MAIN_H
#define MAIN_H

#include "stc8g.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;


    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
//typedef   signed       __INT64 int64_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
//typedef unsigned       __INT64 uint64_t;

extern u16  fre_set;
extern u16 voltage;
void Uart_PutChar(unsigned char value);

#endif

