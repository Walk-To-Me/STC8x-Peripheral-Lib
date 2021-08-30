/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : aht10.h
  - Author        : Walk-To-Me
  - Update date   : 2021年8月30日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)
 * This file is a AT24CXX driver h-file.

 * C language AT24CXX EEPROM driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of EEPROM function on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "soft_i2c.h"
#include "STC8x_I2C.h"
#include "STC8x_DELAY.h"

/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
#define AT24CXX_I2C_SET					1				// 0 硬件I2C  1 软件I2C
#define AT24CXX_ADDR					0x57				// AT24CXX器件地址(前7位器件地址, A2, A1, A0高电平)
#define AT24CXX_TYPE					32				// AT24C32   32K bits(4096 bytes) EEPROM
#define AT24CXX_SIZE					(AT24CXX_TYPE / 8 * 1024)	// SIZE OF EEPROM

#if (AT24CXX_I2C_SET)
	#define AT24CXX_I2C_Send_Start()			Soft_I2C_Send_Start()
	#define AT24CXX_I2C_Send_Stop()			Soft_I2C_Send_Stop()
	#define AT24CXX_I2C_Read_ACK()			Soft_I2C_Read_ACK()
	#define AT24CXX_I2C_Send_ACK()			Soft_I2C_Send_ACK()
	#define AT24CXX_I2C_Send_NACK()			Soft_I2C_Send_NACK()
	#define AT24CXX_I2C_Send_Byte(dat)		Soft_I2C_Send_Byte(dat)
	#define AT24CXX_I2C_Read_Byte()			Soft_I2C_Read_Byte()
#endif

#if (!AT24CXX_I2C_SET)
	#define AT24CXX_I2C_Send_Start()			I2C_Send_Start()
	#define AT24CXX_I2C_Send_Stop()			I2C_Send_Stop()
	#define AT24CXX_I2C_Read_ACK()			I2C_Read_ACK()
	#define AT24CXX_I2C_Send_ACK()			I2C_Send_ACK()
	#define AT24CXX_I2C_Send_NACK()			I2C_Send_NACK()
	#define AT24CXX_I2C_Send_Byte(dat)		I2C_Send_Byte(dat)
	#define AT24CXX_I2C_Read_Byte()			I2C_Read_Byte()
#endif


/*-----------------------------------------------------------------------
|                             CMD CONFIG                                |
-----------------------------------------------------------------------*/
#define AT24CXX_WR_ADDR					(AT24CXX_ADDR << 1)		// 写命令
#define AT24CXX_RD_ADDR					((AT24CXX_ADDR << 1) | 1)	// 读命令
#define AT24CXX_WR_TIME					5				// 写周期时间

/*目前仅支持24C32, 其他容量后续更新*/
#if (AT24CXX_TYPE == 32)
	#define AT24CXX_PAGE_SIZE				32			// 一页32字节
/*-------updating-------*/
#endif

/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void AT24CXX_Init(void);
void AT24CXX_Write_Data(uint16_t addr, uint8_t *dat, uint16_t n);
void AT24CXX_Fill_Data(uint16_t page, uint8_t dat, uint16_t n);
void AT24CXX_Write_Byte(uint16_t addr, uint8_t dat);
void AT24CXX_Write_Page(uint16 page, uint8_t *dat);
void AT24CXX_Write_NotFull_Page(uint16 addr, uint8_t *dat, uint8_t n);
void AT24CXX_Read_Data(uint16_t addr, uint8_t *dat, uint16_t n);
uint8_t AT24CXX_Read_Next_Byte(void);
uint8_t AT24CXX_Read_Byte(uint16_t addr);
void AT24CXX_Seq_Read_Bytes(uint8_t *dat, uint16_t n);

#endif


/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/


