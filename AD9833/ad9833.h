/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : ad9833.h
  - Author        : Walk-To-Me
  - Update date   : 2021年9月9日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)
 * This file is a AD9833 driver h-file.

 * C language AD9833 DDS driver, SPI interface
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of Arbitrary Waveform Generator function on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#ifndef __AD9833_H__
#define __AD9833_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "STC8Gx_REG.h"


/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
#define AD9833_FMCLK					25000000			// 25MHz
#define AD9833_RESOLUTION				268435456			// 2^28
#define AD9833_FREQ_WORDS_GET(x)			(uint32_t)(x*((float)AD9833_RESOLUTION/AD9833_FMCLK))	// 获取频率字

#define AD9833_FSYNC					P20
#define AD9833_SCLK					P21
#define AD9833_SDATA					P22


/*-----------------------------------------------------------------------
|                             TYPE DEFINE                               |
-----------------------------------------------------------------------*/
typedef enum
{
	SINUSOID	= 0x0000,			/* SINUSOID */
	TRIANGLE	= 0x0002,			/* TRIANGLE */
	SQUARE		= 0x0020			/* SQUARE */
} AD9833_Wave_Shape_Type;

typedef enum
{
	FREQ0		= 0x0000,			/* FREQ0 */
	FREQ1		= 0x0800,			/* FREQ1 */
} AD9833_Freq_Register_Type;

/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void AD9833_Init(void);
void AD9833_Set_Wave(uint32_t freq, AD9833_Wave_Shape_Type shape, AD9833_Freq_Register_Type reg);
void AD9833_Set_Wave_Shape(AD9833_Wave_Shape_Type shape);
void AD9833_Set_Freq_Register(AD9833_Freq_Register_Type reg);

#endif

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/


