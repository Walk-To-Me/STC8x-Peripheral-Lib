/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : soft_i2c.h
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
#ifndef __I2C_H__
#define __I2C_H__



#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "STC8Gx_REG.h"



/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
#define Soft_I2C_SCL		P20						// 设置SCL IO口
#define Soft_I2C_SDA		P21						// 设置SDA IO口


/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void Soft_I2C_Init(void);
void Soft_I2C_Send_Start(void);
void Soft_I2C_Send_Stop(void);
FSCSTATE Soft_I2C_Read_ACK(void);
void Soft_I2C_Send_ACK(void);
void Soft_I2C_Send_NACK(void);
void Soft_I2C_Send_Byte(uint8_t dat);
uint8_t Soft_I2C_Read_Byte(void);



#endif


/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/

