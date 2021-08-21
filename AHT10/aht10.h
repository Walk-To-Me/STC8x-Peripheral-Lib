/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : aht10.h
  - Author        : Walk-To-Me
  - Update date   : 2021��8��21��
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a aht10 driver h-file.

 * C language AHT10 temperature and humidity sensor driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of Temperature and humidity measurement functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#ifndef __AHT10_H__
#define __AHT10_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "STC8x_I2C.h"
#include "STC8x_DELAY.h"

/*-----------------------------------------------------------------------
|                             USER CONFIG	                              |
-----------------------------------------------------------------------*/
#define AHT10_ADDR								0x38																	// AHT10������ַ

/*-----------------------------------------------------------------------
|                             CMD CONFIG			                          |
-----------------------------------------------------------------------*/
#define AHT10_WR_ADDR							(AHT10_ADDR << 1)											// д����
#define AHT10_RD_ADDR							((AHT10_ADDR << 1) | 1)								// ������
#define AHT10_CMD_INITIALIZE			0xe1																	// ��ʼ������
#define AHT10_CMD_TRIGGER					0xac																	// ������������
#define AHT10_CMD_SOFTRST					0xba																	// �����λ����
#define AHTX0_STATUS_BUSY					0x80																	// æ״̬��־λ
#define AHTX0_STATUS_CALIBRATED		0x08																	// У׼״̬��־λ


/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
FSCSTATE AHT10_Init(void);
void AHT10_Rst(void);
float AHT10_Get_Temperature(void);
float AHT10_Get_Humidity(void);

#endif

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/

