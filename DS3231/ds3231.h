/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : ssd1306.c
  - Author        : Walk-To-Me
  - Update date   : 2021年8月26日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a DS3231 driver h-file.

 * C language DS3231 driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of RTC functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#ifndef __DS3231_H__
#define __DS3231_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "soft_i2c.h"
#include "STC8x_I2C.h"

/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
#define DS3231_I2C_SET					0				// 0 硬件I2C  1 软件I2C
#define DS3231_ADDR					0x68				// DS3231器件地址

#if (DS3231_I2C_SET)
	#define DS3231_I2C_Send_Start()			Soft_I2C_Send_Start()
	#define DS3231_I2C_Send_Stop()			Soft_I2C_Send_Stop()
	#define DS3231_I2C_Read_ACK()			Soft_I2C_Read_ACK()
	#define DS3231_I2C_Send_ACK()			Soft_I2C_Send_ACK()
	#define DS3231_I2C_Send_NACK()			Soft_I2C_Send_NACK()
	#define DS3231_I2C_Send_Byte(dat)		Soft_I2C_Send_Byte(dat)
	#define DS3231_I2C_Read_Byte()			Soft_I2C_Read_Byte()
#endif

#if (!DS3231_I2C_SET)
	#define DS3231_I2C_Send_Start()			I2C_Send_Start()
	#define DS3231_I2C_Send_Stop()			I2C_Send_Stop()
	#define DS3231_I2C_Read_ACK()			I2C_Read_ACK()
	#define DS3231_I2C_Send_ACK()			I2C_Send_ACK()
	#define DS3231_I2C_Send_NACK()			I2C_Send_NACK()
	#define DS3231_I2C_Send_Byte(dat)		I2C_Send_Byte(dat)
	#define DS3231_I2C_Read_Byte()			I2C_Read_Byte()
#endif


/*-----------------------------------------------------------------------
|                             CMD CONFIG                                |
-----------------------------------------------------------------------*/
#define DS3231_WR_ADDR					(DS3231_ADDR << 1)		// 写命令
#define DS3231_RD_ADDR					((DS3231_ADDR << 1) | 1)	// 读命令

#define DS3231_TIME_ADDR				0x00				// 时间寄存器起始地址
#define DS3231_ALARM1_ADDR				0x07				// 闹钟1寄存器起始地址
#define DS3231_ALARM2_ADDR				0x0b				// 闹钟2寄存器起始地址
#define DS3231_CON_ADDR					0x0e				// 控制寄存器地址
#define DS3231_STA_ADDR					0x0f				// 状态寄存器地址
#define DS3231_AO_ADDR					0x10				// 晶体老化补偿寄存器地址





/*-----------------------------------------------------------------------
|                             TYPE DEFINE                               |
-----------------------------------------------------------------------*/
typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t sys_12_24hour;								// (0) 24小时制  (1) 12小时制
	uint8_t am_pm;									// (0) AM  (1) PM
} DS3231_TIME_Type;

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day_or_date;
	uint8_t sys_12_24hour;								// (0) 24小时制  (1) 12小时制
	uint8_t am_pm;									// (0) AM  (1) PM
	uint8_t dy_dt_set;								// (0) date模式  (1) day模式
} DS3231_ALARM1_Type;

typedef struct {
	uint8_t minute;
	uint8_t hour;
	uint8_t day_or_date;
	uint8_t sys_12_24hour;								// (0) 24小时制  (1) 12小时制
	uint8_t am_pm;									// (0) AM  (1) PM
	uint8_t dy_dt_set;								// (0) date模式  (1) day模式
} DS3231_ALARM2_Type;

typedef enum
{
	DS3231_ALARM1_Mode_1		= 0x0f,						// Alarm once per second
	DS3231_ALARM1_Mode_2		= 0x0e,						// Alarm when seconds match
	DS3231_ALARM1_Mode_3		= 0x0c,						// Alarm when minutes and seconds match
	DS3231_ALARM1_Mode_4		= 0x08,						// Alarm when hours, minutes, and seconds match
	DS3231_ALARM1_Mode_5		= 0x00						// Alarm when date/day, hours, minutes, and seconds match
}	DS3231_ALARM1_Mode_Type;

typedef enum
{
	DS3231_ALARM2_Mode_1		= 0x07,						// Alarm once per minute (00 seconds of every minute)
	DS3231_ALARM2_Mode_2		= 0x06,						// Alarm when minutes match
	DS3231_ALARM2_Mode_3		= 0x04,						// Alarm when hours and minutes match
	DS3231_ALARM2_Mode_4		= 0x00						// Alarm when date/day, hours, and minutes match
}	DS3231_ALARM2_Mode_Type;

typedef enum
{
	DS3231_SQW_1			= 0,						// 1Hz
	DS3231_SQW_2			= 1,						// 1.024kHz
	DS3231_SQW_3			= 2,						// 4.096kHz
	DS3231_SQW_4			= 3						// 8.192kHz
}	DS3231_SQW_Type;


/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void DS3231_Init(void);
FSCSTATE DS3231_Set_Time(DS3231_TIME_Type *time);
void DS3231_Get_Time(DS3231_TIME_Type *time);
FSCSTATE DS3231_Set_Alarm1(DS3231_ALARM1_Type *time, DS3231_ALARM1_Mode_Type mode);
FSCSTATE DS3231_Set_Alarm2(DS3231_ALARM2_Type *time, DS3231_ALARM2_Mode_Type mode);
void DS3231_EN32kHz_On(void);
void DS3231_EN32kHz_Off(void);
void DS3231_Set_SQW(DS3231_SQW_Type sqw);
void DS3231_SQW_On(void);
void DS3231_INT_On(void);
void DS3231_A1IE_ON(void);
void DS3231_A2IE_ON(void);
void DS3231_Set_Cal(int8 t);
void DS3231_CONV_On(void);
void DS3231_Write_Data(uint8_t addr, uint8_t *dat, uint8_t n);
void DS3231_Read_Data(uint8_t addr, uint8_t *dat, uint8_t n);

#endif


/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/

