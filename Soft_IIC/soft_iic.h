/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : soft_iic.h
  - Author        : Walk-To-Me
  - Update date   : 2021年8月22日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a soft iic driver h-file.

 * C language soft IIC driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of IIC functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#ifndef __SOFT_IIC_H__
#define __SOFT_IIC_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "STC8Gx_REG.h"
#include "STC8x_GPIO.h"


/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
#define Soft_IIC_SCL		P20						// 设置SCL IO口
#define Soft_IIC_SDA		P21						// 设置SDA IO口
#define Soft_IIC_In()		GPIO_MODE_IN_FLOATING(GPIO_P2,Pin_1)		// 设置SDA浮空输入
#define Soft_IIC_Out()		GPIO_MODE_OUT_PP(GPIO_P2,Pin_1)			// 设置SDA推挽输出


/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void Soft_IIC_Init(void);
void Soft_IIC_Send_Start(void);
void Soft_IIC_Send_Stop(void);
FSCSTATE Soft_IIC_Read_ACK(void);
void Soft_IIC_Send_ACK(void);
void Soft_IIC_Send_NACK(void);
void Soft_IIC_Send_Byte(uint8_t dat);
uint8_t Soft_IIC_Read_Byte(void);



#endif


/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/

