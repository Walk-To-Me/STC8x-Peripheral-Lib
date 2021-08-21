/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : aht10.c
  - Author        : Walk-To-Me
  - Update date   : 2021��8��21��
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
 * @brief     	AHT10��ʼ������
 * @param[in] 	void
 * @return		FSC_SUCCESS	��ʼ���ɹ�
 * @return		FSC_FAIL	��ʼ��ʧ��
**/
FSCSTATE AHT10_Init(void)
{
	DELAY_Set_Ms(45);									// �ϵ���ʱ
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
	if (AHT10_Get_State() & AHTX0_STATUS_CALIBRATED)	// ���У׼״̬��־λ
		return FSC_FAIL;
	return FSC_SUCCESS;
}



/**
 * @brief     AHT10��λ����
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
 * @brief     AHT10��ȡ״̬����
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
 * @brief     AHT10������������
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
 * @brief     	AHT10��ȡ���ݺ���
 * @param[in] 	dat				���ݴ��ָ��
 * @return		FSC_SUCCESS		��ȡ���ݳɹ�
 * @return		FSC_FAIL		��ȡ����ʧ��
**/
static FSCSTATE AHT10_Get_Data(uint8_t *dat)
{
	uint8_t state = 0;
	uint8_t n = 0;
	AHT10_Trigger_Measurement();
	DELAY_Set_Ms(80);						// �ȴ�����
	
	EAXFR_ENABLE();
	I2C_Send_Start();
	I2C_Send_Btye(AHT10_RD_ADDR);
	I2C_Read_ACK();
	state = I2C_Read_Byte();
	I2C_Send_ACK();
	while (state & AHTX0_STATUS_BUSY)		// ���æ״̬��־λ
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
 * @brief     AHT10��ȡ�¶Ⱥ���
 * @param[in] void
 * @return    ret_val	�¶����ݣ��ֱ���0.01����λ��
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
 * @brief     AHT10��ȡʪ�Ⱥ���
 * @param[in] void
 * @return    ret_val	ʪ�����ݣ��ֱ���0.001����λx100%
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


