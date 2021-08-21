/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : soft_iic.c
  - Author        : Walk-To-Me
  - Update date   : 2021��8��22��
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
#include "soft_iic.h"
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
 * @brief     ���IIC��ʼ������
 * @param[in] void
 * @return    void
**/
void Soft_IIC_Init(void)
{
	Soft_IIC_SCL = 1;
	Soft_IIC_SDA = 1;
}

/**
* @brief     ���IIC���Ϳ�ʼ����
 * @param[in] void
 * @return    void
**/
void Soft_IIC_Send_Start(void)
{
	Soft_IIC_Out();
	Soft_IIC_SDA = 1;
	Soft_IIC_SCL = 1;
	Delay4us();
	Soft_IIC_SDA = 0;
	Delay4us();
	Soft_IIC_SCL = 0;
}

/**
 * @brief     ���IIC���ͽ�������
 * @param[in] void
 * @return    void
**/
void Soft_IIC_Send_Stop(void)
{
	Soft_IIC_Out();
	Soft_IIC_SCL = 0;
	Soft_IIC_SDA = 0;
	Delay4us();
	Soft_IIC_SCL = 1;
	Soft_IIC_SDA = 1;
	Delay4us();
}

/**
 * @brief     ���IIC��ȡACK����
 * @param[in] void
 * @return    FSC_SUCCESS	��ȡACK�ɹ�
 * @return    FSC_FAIL		��ȡACKʧ��
**/
FSCSTATE Soft_IIC_Read_ACK(void)
{
	uint8_t n = 0;
	Soft_IIC_In();
	Soft_IIC_SDA = 1;
	Delay1us();
	Soft_IIC_SCL = 1;
	Delay1us();
	while (Soft_IIC_SDA)
	{
		n++;
		if (n > 250)
		{
			Soft_IIC_Send_Stop();
			return FSC_FAIL;
		}
	}
	Soft_IIC_SCL = 0;
	return FSC_SUCCESS;
}

/**
* @brief     ���IIC����ACK����
 * @param[in] void
 * @return    void
**/
void Soft_IIC_Send_ACK(void)
{
	Soft_IIC_SCL = 0;
	Soft_IIC_Out();
	Soft_IIC_SDA = 0;
	Delay2us();
	Soft_IIC_SCL = 1;
	Delay2us();
	Soft_IIC_SCL = 0;
}

/**
* @brief     ���IIC����NACK����
 * @param[in] void
 * @return    void
**/
void Soft_IIC_Send_NACK(void)
{
	Soft_IIC_SCL = 0;
	Soft_IIC_Out();
	Soft_IIC_SDA = 1;
	Delay2us();
	Soft_IIC_SCL = 1;
	Delay2us();
	Soft_IIC_SCL = 0;
}

/**
* @brief     ���IIC�����ֽں���
* @param[in] dat	���͵��ֽ�����
 * @return    void
**/
void Soft_IIC_Send_Byte(uint8_t dat)
{
	uint8_t i;
	Soft_IIC_Out();
	Soft_IIC_SCL = 0;
	for (i = 0; i < 8; i++)
	{
		Soft_IIC_SDA = (dat&0x80) >> 7;
		dat <<= 1;
		Delay2us();
		Soft_IIC_SCL = 1;
		Delay2us();
		Soft_IIC_SCL = 0;
		Delay2us();
	}
}

/**
 * @brief     ���IIC��ȡ�ֽں���
 * @param[in] void
 * @return    ret_val	��ȡ���ֽ�����
**/
uint8_t Soft_IIC_Read_Byte(void)
{
	uint8_t i;
	uint8_t ret_val;
	Soft_IIC_In();
	for (i = 0; i < 8; i++)
	{
		Soft_IIC_SCL = 0;
		Delay2us();
		Soft_IIC_SCL = 1;
		ret_val <<= 1;
		if (Soft_IIC_SDA) ret_val++;
		Delay1us();
	}
	return ret_val;
}

/**
 * @brief     1us��ʱ����
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
 * @brief     2us��ʱ����
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
 * @brief     4us��ʱ����
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







