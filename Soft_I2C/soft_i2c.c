/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : soft_i2c.c
  - Author        : Walk-To-Me
  - Update date   : 2021年8月22日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a soft iic driver c-file.

 * C language soft IIC driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of IIC functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "soft_i2c.h"
/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
/* None */
/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void Delay1us(void);
static void Delay2us(void);
static void Delay4us(void);
/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/

/**
 * @brief     软件IIC初始化函数
 * @param[in] void
 * @return    void
**/
void Soft_I2C_Init(void)
{
	Soft_I2C_SCL = 1;
	Soft_I2C_SDA = 1;
}

/**
* @brief     软件IIC发送开始函数
 * @param[in] void
 * @return    void
**/
void Soft_I2C_Send_Start(void)
{
	Soft_I2C_SDA = 1;
	Soft_I2C_SCL = 1;
	Delay4us();
	Soft_I2C_SDA = 0;
	Delay4us();
	Soft_I2C_SCL = 0;
}

/**
 * @brief     软件IIC发送结束函数
 * @param[in] void
 * @return    void
**/
void Soft_I2C_Send_Stop(void)
{
	Soft_I2C_SCL = 0;
	Soft_I2C_SDA = 0;
	Delay4us();
	Soft_I2C_SCL = 1;
	Soft_I2C_SDA = 1;
	Delay4us();
}

/**
 * @brief     软件IIC获取ACK函数
 * @param[in] void
 * @return    FSC_SUCCESS	获取ACK成功
 * @return    FSC_FAIL		获取ACK失败
**/
FSCSTATE Soft_I2C_Read_ACK(void)
{
	uint8_t n = 0;
	Soft_I2C_SDA = 1;		// SDA引脚拉高， WEAKPULL 模式下允许输入
	Delay1us();
	Soft_I2C_SCL = 1;
	Delay1us();
	while (Soft_I2C_SDA)		// 使用 WEAKPULL 模式下输入功能，须先将IO口拉高
	{
		n++;
		if (n > 250)
		{
			Soft_I2C_Send_Stop();
			return FSC_FAIL;
		}
	}
	Soft_I2C_SCL = 0;
	return FSC_SUCCESS;
}

/**
* @brief     软件IIC发送ACK函数
 * @param[in] void
 * @return    void
**/
void Soft_I2C_Send_ACK(void)
{
	Soft_I2C_SCL = 0;
	Soft_I2C_SDA = 0;
	Delay2us();
	Soft_I2C_SCL = 1;
	Delay2us();
	Soft_I2C_SCL = 0;
}

/**
* @brief     软件IIC发送NACK函数
 * @param[in] void
 * @return    void
**/
void Soft_I2C_Send_NACK(void)
{
	Soft_I2C_SCL = 0;
	Soft_I2C_SDA = 1;
	Delay2us();
	Soft_I2C_SCL = 1;
	Delay2us();
	Soft_I2C_SCL = 0;
}

/**
* @brief     软件IIC发送字节函数
* @param[in] dat	发送的字节数据
 * @return    void
**/
void Soft_I2C_Send_Byte(uint8_t dat)
{
	uint8_t i;
	Soft_I2C_SCL = 0;
	for (i = 0; i < 8; i++)
	{
		Soft_I2C_SDA = (dat&0x80) >> 7;
		dat <<= 1;
		Delay2us();
		Soft_I2C_SCL = 1;
		Delay2us();
		Soft_I2C_SCL = 0;
		Delay2us();
	}
}

/**
 * @brief     软件IIC获取字节函数
 * @param[in] void
 * @return    ret_val	获取的字节数据
**/
uint8_t Soft_I2C_Read_Byte(void)
{
	uint8_t i;
	uint8_t ret_val;
	Soft_I2C_SDA = 1;	// WEAKPULL 模式下须拉高引脚才能启用输入功能 (!important)
	for (i = 0; i < 8; i++)
	{
		Soft_I2C_SCL = 0;
		Delay2us();
		Soft_I2C_SCL = 1;
		ret_val <<= 1;
		if (Soft_I2C_SDA) ret_val++;
		Delay1us();
	}
	return ret_val;
}

/**
 * @brief     1us延时函数
 * @param[in] void
 * @return    void
**/
static void Delay1us(void)		//@24.000MHz
{
	unsigned char i;

	i = 6;
	while (--i);
}

/**
 * @brief     2us延时函数
 * @param[in] void
 * @return    void
**/
static void Delay2us(void)		//@24.000MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}

/**
 * @brief     4us延时函数
 * @param[in] void
 * @return    void
**/
static void Delay4us(void)		//@24.000MHz
{
	unsigned char i;

	i = 30;
	while (--i);
}


/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/







