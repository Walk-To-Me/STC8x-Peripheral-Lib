/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : AD9854.h
  - Author        : Walk-To-Me
  - Update date   : 2021年9月30日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)
 * This file is a AD9854 driver h-file.

 * C language AD9854 DDS driver, parallel interface
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of Arbitrary Waveform Generator function on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/

#ifndef __AD9854_H__
#define __AD9854_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "STC8Gx_REG.h"

/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
/* 2^48 / 300,000,000(系统时钟) = 938250 */
#define Freq_Mult 				938250
#define PLL_Mult				15

#define AD9854_ADDR_BUS				P1
#define AD9854_DATA_BUS				P2

#define AD9854_RST				P00
#define AD9854_WR				P01
#define AD9854_RD				P02
#define AD9854_UCLK				P03


/*-----------------------------------------------------------------------
|                             CMD CONFIG                                |
-----------------------------------------------------------------------*/
#define AD9854_FTW1_ADDR			0x04
#define AD9854_CTRL1_ADDR			0x1d
#define AD9854_CTRL2_ADDR			0x1e
#define AD9854_CTRL3_ADDR			0x1f
#define AD9854_CTRL4_ADDR			0x20
#define AD9854_OSK_I_ADDR			0x21
#define AD9854_OSK_Q_ADDR			0x23

/* 如果系统时钟频率超过200M(最大300M)，PLL Range Bit设为1，反之 */
#define PLL_Mult_Value				((PLL_Mult>10)?(0x40|PLL_Mult):(0x00|PLL_Mult))


/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void AD9854_Init(void);
void AD9854_SetSine(uint32_t freq, uint16_t shape);
void AD9854_Write_Byte(uint8_t addr, uint8_t dat);

#endif

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/



