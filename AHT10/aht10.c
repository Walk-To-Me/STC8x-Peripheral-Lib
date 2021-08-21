/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : aht10.c
  - Author        : Walk-To-Me
  - Update date   : 2021年8月21日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a aht10 driver c-file.

 * C language AHT10 temperature and humidity sensor driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of Temperature and humidity measurement functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "aht10.h"
/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void AHT10_Trigger_Measurement(void);
static uint8_t AHT10_Get_State(void);
static FSCSTATE AHT10_Get_Data(uint8_t *dat);
/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/

/**
 * @brief     	AHT10初始化函数
 * @param[in] 	void
 * @return		FSC_SUCCESS	初始化成功
 * @return		FSC_FAIL	初始化失败
**/
FSCSTATE AHT10_Init(void)
{
	DELAY_Set_Ms(45);									// 上电延时
	EAXFR_ENABLE();										// (!important)
	I2C_Send_Start();
	I2C_Send_Btye(AHT10_WR_ADDR);
	I2C_Read_ACK();
	I2C_Send_Btye(AHT10_CMD_INITIALIZE);
	I2C_Read_ACK();
	I2C_Send_Btye(0x08);
	I2C_Read_ACK();
	I2C_Send_Btye(0x00);
	I2C_Read_ACK();
	I2C_Send_Stop();
	EAXFR_DISABLE();									// (!important)
	if (AHT10_Get_State() & AHTX0_STATUS_CALIBRATED)	// 检测校准状态标志位
		return FSC_FAIL;
	return FSC_SUCCESS;
}



/**
 * @brief     AHT10软复位函数
 * @param[in] void
 * @return    void
**/
void AHT10_Rst(void)
{
	EAXFR_ENABLE();
	I2C_Send_Start();
	I2C_Send_Btye(AHT10_WR_ADDR);
	I2C_Read_ACK();
	I2C_Send_Btye(AHT10_CMD_SOFTRST);
	I2C_Read_ACK();
	I2C_Send_Stop();
	EAXFR_DISABLE();
}


/**
 * @brief     AHT10获取状态函数
 * @param[in] void
 * @return    void
**/
static uint8_t AHT10_Get_State(void)
{
	uint8_t state = 0;
	
	EAXFR_ENABLE();				// (!important)
	I2C_Send_Start();
	I2C_Send_Btye(AHT10_RD_ADDR);
	I2C_Read_ACK();
	state = I2C_Read_Byte();
	I2C_Send_NACK();
	I2C_Send_Stop();
	EAXFR_DISABLE();			// (!important)
	return state;
}


/**
 * @brief     AHT10触发测量函数
 * @param[in] void
 * @return    void
**/
static void AHT10_Trigger_Measurement(void)
{
	EAXFR_ENABLE();				// (!important)
	I2C_Send_Start();
	I2C_Send_Btye(AHT10_WR_ADDR);
	I2C_Read_ACK();
	I2C_Send_Btye(AHT10_CMD_TRIGGER);
	I2C_Read_ACK();
	I2C_Send_Btye(0x33);
	I2C_Read_ACK();
	I2C_Send_Btye(0x00);
	I2C_Read_ACK();
	I2C_Send_Stop();	
	EAXFR_DISABLE();			// (!important)
}


/**
 * @brief     	AHT10获取数据函数
 * @param[in] 	dat				数据存放指针
 * @return		FSC_SUCCESS		获取数据成功
 * @return		FSC_FAIL		获取数据失败
**/
static FSCSTATE AHT10_Get_Data(uint8_t *dat)
{
	uint8_t state = 0;
	uint8_t n = 0;
	AHT10_Trigger_Measurement();
	DELAY_Set_Ms(80);						// 等待测量
	
	EAXFR_ENABLE();
	I2C_Send_Start();
	I2C_Send_Btye(AHT10_RD_ADDR);
	I2C_Read_ACK();
	state = I2C_Read_Byte();
	I2C_Send_ACK();
	while (state & AHTX0_STATUS_BUSY)		// 检测忙状态标志位
	{
		DELAY_Set_Ms(1);
		n++;
		if (n >= 100)
			return FSC_FAIL;
	}
	for (n = 0; n < 5; n++)
	{
		dat[n] = I2C_Read_Byte();
		if (n < 4)	I2C_Send_ACK();
		else	I2C_Send_NACK();
	}
	I2C_Send_Stop();
	EAXFR_DISABLE();
	return FSC_SUCCESS;
}


/**
 * @brief     AHT10获取温度函数
 * @param[in] void
 * @return    ret_val	温度数据，分辨率0.01，单位℃
**/
float AHT10_Get_Temperature(void)
{
	uint8_t dat[5] = {0};
	uint32_t Data = 0;
	float ret_val;
	if (AHT10_Get_Data(dat) == FSC_SUCCESS)
	{
		Data = ((dat[2]&0x0f)<<8) | dat[3];
		Data = (Data<<8) | dat[4];
	}
	ret_val = (uint16_t)(Data*200.0*100.0/1024.0/1024.0 - 50.0*100) / 100.0;
	return ret_val;
}


/**
 * @brief     AHT10获取湿度函数
 * @param[in] void
 * @return    ret_val	湿度数据，分辨率0.001，单位x100%
**/
float AHT10_Get_Humidity(void)
{
	uint8_t dat[5] = {0};
	uint32_t Data = 0;
	float ret_val;
	if (AHT10_Get_Data(dat) == FSC_SUCCESS)
	{
		Data += dat[0];
		Data = (Data<<8) | dat[1];
		Data = ((Data<<8) | dat[2]) >> 4;
	}
	ret_val = (uint16_t)(Data*1000.0/1024.0/1024.0) / 1000.0;
	return ret_val;
}

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/


