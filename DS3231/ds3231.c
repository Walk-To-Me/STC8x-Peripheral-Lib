/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : ssd1306.c
  - Author        : Walk-To-Me
  - Update date   : 2021��8��27��
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a DS3231 driver c-file.

 * C language DS3231 driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of RTC functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "ds3231.h"
/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void bin_to_bcd(uint8_t *dat, uint8_t n);
static void bcd_to_bin(uint8_t *dat, uint8_t n);


/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/

/**
 * @brief     DS3231��ʼ������
 * @param[in] void
 * @return    void
**/
void DS3231_Init(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~((1<<7) | (1<<1) | (1<<0));				// �ϵ����OSF, A2F, A1F��־λ
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231����ʱ�亯��
 * @param[in] time  ����ʱ�����ݵĽṹ��ָ��
 * @return    FSC_FAIL     ����ʧ��
 * @return    FSC_SUCCESS  ���óɹ�
**/
FSCSTATE DS3231_Set_Time(DS3231_TIME_Type *time)
{
	if (time->hour >= 0 && time->hour <= 23)
	{
		if (time->sys_12_24hour)
		{
			if (time->hour < 12)
				if (time->am_pm)	time->hour += 12;
			else
				return FSC_FAIL;
		}
		else
			if (time->hour < 12)
				if (time->am_pm)	return FSC_FAIL;
			else
				if (!time->am_pm)	return FSC_FAIL;
	}
	else
		return FSC_FAIL;
	
	bin_to_bcd((uint8_t *)time, 7);
	time->hour &= ~(1 << 6);			 		// 12/24Сʱ�Ʊ�־λ��0
	DS3231_Write_Data(DS3231_TIME_ADDR, (uint8_t *)time, 7);
	return FSC_SUCCESS;
}


/**
 * @brief     DS3231��ȡʱ�亯��
 * @param[in] time  ��ȡʱ�����ݵĽṹ��ָ��
 * @return    void
**/
void DS3231_Get_Time(DS3231_TIME_Type *time)
{
	DS3231_Read_Data(DS3231_TIME_ADDR, (uint8_t *)time, 7);
	bcd_to_bin((uint8_t *)time, 7);
	if (time->hour >= 12)
	{
		time->am_pm = 1;					// am_pm��־��1  PMģʽ
		if (time->sys_12_24hour)
			time->hour -= 12;				// 12Сʱ�Ƹ�ʽ����
	}
	else
	{
		time->am_pm = 0;					// am_pm��־��1  AMģʽ
	}
}


/**
 * @brief     DS3231��������1����
 * @param[in] time  ��������1��ʱ���ݵĽṹ��ָ��
 * @param[in] mode  ����1��ʱģʽ
 * @return    FSC_FAIL     ����ʧ��
 * @return    FSC_SUCCESS  ���óɹ�
**/
FSCSTATE DS3231_Set_Alarm1(DS3231_ALARM1_Type *time, DS3231_ALARM1_Mode_Type mode)
{
	if (time->hour >= 0 && time->hour <= 23)
	{
		if (time->sys_12_24hour)
		{
			if (time->hour < 12)
				if (time->am_pm)	time->hour += 12;
			else
				return FSC_FAIL;
		}
		else
			if (time->hour < 12)
				if (time->am_pm)	return FSC_FAIL;
			else
				if (!time->am_pm)	return FSC_FAIL;
	}
	else
		return FSC_FAIL;
	
	if (time->dy_dt_set)
		if (time->day_or_date < 1 || time->day_or_date > 7)	return FSC_FAIL;
	else
		if (time->day_or_date < 1 || time->day_or_date > 31)	return FSC_FAIL;

	bin_to_bcd((uint8_t *)time, 4);
	time->hour &= ~(1 << 6);					// 12/24Сʱ�Ʊ�־λ��0
	time->second |= ((mode&(1<<0))<<7);				// ����A1M1��־λ
	time->minute |= ((mode&(1<<1))<<6);				// ����A1M2��־λ
	time->hour |= ((mode&(1<<2))<<5);				// ����A1M3��־λ
	time->day_or_date |= ((mode&(1<<3))<<4);			// ����A1M4��־λ
	DS3231_Write_Data(DS3231_ALARM1_ADDR, (uint8_t *)time, 4);
	return FSC_SUCCESS;
}


/**
 * @brief     DS3231��������2����
 * @param[in] time  ��������2��ʱ���ݵĽṹ��ָ��
 * @param[in] mode  ����2��ʱģʽ
 * @return    FSC_FAIL     ����ʧ��
 * @return    FSC_SUCCESS  ���óɹ�
**/
FSCSTATE DS3231_Set_Alarm2(DS3231_ALARM2_Type *time, DS3231_ALARM2_Mode_Type mode)
{
	if (time->hour >= 0 && time->hour <= 23)
	{
		if (time->sys_12_24hour)
		{
			if (time->hour < 12)
				if (time->am_pm)	time->hour += 12;
			else
				return FSC_FAIL;
		}
		else
			if (time->hour < 12)
				if (time->am_pm)	return FSC_FAIL;
			else
				if (!time->am_pm)	return FSC_FAIL;
	}
	else
		return FSC_FAIL;
	
	if (time->dy_dt_set)
		if (time->day_or_date < 1 || time->day_or_date > 7)	return FSC_FAIL;
	else
		if (time->day_or_date < 1 || time->day_or_date > 31)	return FSC_FAIL;
	
	bin_to_bcd((uint8_t *)time, 3);
	time->hour &= ~(1 << 6);					// 12/24Сʱ�Ʊ�־λ��0
	time->minute |= ((mode&(1<<0))<<7);				// ����A2M2��־λ
	time->hour |= ((mode&(1<<1))<<6);				// ����A2M3��־λ
	time->day_or_date |= ((mode&(1<<2))<<5);			// ����A2M4��־λ
	DS3231_Write_Data(DS3231_ALARM2_ADDR, (uint8_t *)time, 3);
	return FSC_SUCCESS;
}


/**
 * @brief     DS3231ʹ��32kHz�������32.768kHz�����ź�
 * @param[in] void
 * @return    void
**/
void DS3231_EN32kHz_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat |= (1<<3);					// ʹ��EN32kHz�������
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231��ֹ32kHz�������32.768kHz�����ź�
 * @param[in] void
 * @return    void
**/
void DS3231_EN32kHz_Off(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~(1<<3);					// ��ֹEN32kHz�������
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231���ò�ʹ��INT/SQW����SQW����(��ֹINT����)
 * @param[in] sqw   SQW�������Ƶ��ģʽ
 * @return    void
**/
void DS3231_Set_SQW(DS3231_SQW_Type sqw)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (sqw<<3);				// ����Ƶ��ѡ��λ
	dat &= ~(1<<2);					// ʹ��SQW�����������(��ֹ�����жϹ���)
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231ʹ��INT/SQW����SQW����(��ֹINT����)
 * @param[in] void
 * @return    void
**/
void DS3231_SQW_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat &= ~(1<<2);					// ʹ��SQW�����������(��ֹ�����жϹ���)
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231ʹ��INT/SQW����INT����(��ֹSQW����)
 * @param[in] void
 * @return    void
**/
void DS3231_INT_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<2);					// ʹ�������ж��������(��ֹSQW�����������)
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}



/**
 * @brief     DS3231����1�ж�ʹ��
 * @param[in] void
 * @return    void
**/
void DS3231_A1IE_ON(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<0);					// ʹ������1�ж�
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231����2�ж�ʹ��
 * @param[in] void
 * @return    void
**/
void DS3231_A2IE_ON(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<1);					// ʹ������2�ж�
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231��ȡ����1��־
 * @param[in] void
 * @return    void
**/
uint8_t DS3231_Get_A1F(void)
{
	uint8_t ret_val = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &ret_val, 1);
	ret_val &= (1<<0);				// ��ȡA1F��־λ
	return ret_val;
}


/**
 * @brief     DS3231��ȡ����2��־
 * @param[in] void
 * @return    void
**/
uint8_t DS3231_Get_A2F(void)
{
	uint8_t ret_val = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &ret_val, 1);
	ret_val &= (1<<1);				// ��ȡA1F��־λ
	return ret_val;
}


/**
 * @brief     DS3231�������1��־
 * @param[in] void
 * @return    void
**/
void DS3231_Clr_A1F(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~(1<<0);					// ���A1F��־λ
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231�������2��־
 * @param[in] void
 * @return    void
**/
void DS3231_Clr_A2F(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~(1<<1);					// ���A2F��־λ
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231����У׼(ͨ�����������ϻ���������, �۲�32kHz�����������У׼)
 * @param[in] dat   �����ϻ���������(������λ)      + ������Ƶ��  - ������Ƶ��
 * @return    void
**/
void DS3231_Set_Cal(int8 dat)
{
	DS3231_Write_Data(DS3231_AO_ADDR, &dat, 1);		// ���þ����ϻ�����
	DS3231_CONV_On();				// �����¶�ת��, ִ��TCXO�㷨����������������
}


/**
 * @brief     DS3231ǿ�ƿ����¶�ת��(���ڵ��������ϻ�����������, ���̹۲��ϻ��Ĵ�����32kHz���Ƶ�ʵ�Ӱ��)
 * @param[in] void
 * @return    void
**/
void DS3231_CONV_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	while (dat & 0x04);				// BSYλ���
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<5);
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);		// ����CONVλΪ1, �����¶�ת��
}



/**
 * @brief     DS3231����дn�ֽ�����
 * @param[in] addr  д�����ݳ�ʼ��ַ
 * @param[in] dat   д������ָ��
 * @param[in] n     д�������ֽڸ���
 * @return    void
**/
void DS3231_Write_Data(uint8_t addr, uint8_t *dat, uint8_t n)
{
	uint8_t i;
	#if (!DS3231_I2C_SET)
		EAXFR_ENABLE();
	#endif
	DS3231_I2C_Send_Start();
	DS3231_I2C_Send_Byte(0xd0);
	DS3231_I2C_Read_ACK();
	DS3231_I2C_Send_Byte(addr);
	DS3231_I2C_Read_ACK();
	for (i = 0; i < n; i++)
	{
		DS3231_I2C_Send_Byte(*dat++);
		DS3231_I2C_Read_ACK();
	}
	DS3231_I2C_Send_Stop();
	#if (!DS3231_I2C_SET)
		EAXFR_DISABLE();
	#endif
}


/**
 * @brief     DS3231������n�ֽ�����
 * @param[in] addr  ��ȡ���ݳ�ʼ��ַ
 * @param[in] dat   ��ȡ����ָ��
 * @param[in] n     ��ȡ�����ֽڸ���
 * @return    void
**/
void DS3231_Read_Data(uint8_t addr, uint8_t *dat, uint8_t n)
{
	uint8_t i;
	#if (!DS3231_I2C_SET)
		EAXFR_ENABLE();
	#endif
	DS3231_I2C_Send_Start();
	DS3231_I2C_Send_Byte(DS3231_WR_ADDR);
	DS3231_I2C_Read_ACK();
	DS3231_I2C_Send_Byte(addr);
	DS3231_I2C_Read_ACK();
	DS3231_I2C_Send_Stop();
	
	DS3231_I2C_Send_Start();
	DS3231_I2C_Send_Byte(DS3231_RD_ADDR);
	DS3231_I2C_Read_ACK();
	for (i = 0; i < n; i++)
	{
		*dat++ = DS3231_I2C_Read_Byte();
		if (i < n-1)	DS3231_I2C_Send_ACK();
		else	DS3231_I2C_Send_NACK();
	}
	DS3231_I2C_Send_Stop();
	#if (!DS3231_I2C_SET)
		EAXFR_DISABLE();
	#endif
}


/**
 * @brief     ��ʮ����תBCD��ʽ
 * @param[in] dat   ����ָ��
 * @param[in] n     �����ֽڸ���
 * @return    void
**/
static void bin_to_bcd(uint8_t *dat, uint8_t n)
{
	uint8_t i, temp;
	for (i = 0; i < n; i++)
	{
		temp = *dat;
		*dat++ = (temp%10) | ((temp/10) << 4);
	}
}


/**
 * @brief     BCD��ʽת��ʮ����
 * @param[in] dat   ����ָ��
 * @param[in] n     �����ֽڸ���
 * @return    void
**/
static void bcd_to_bin(uint8_t *dat, uint8_t n)
{
	uint8_t i, temp;
	for (i = 0; i < n; i++)
	{
		temp = *dat;
		*dat++ = (temp&DS3231_STA_ADDR) + ((temp&0xf0)>>4)*10;
	}
}

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/

