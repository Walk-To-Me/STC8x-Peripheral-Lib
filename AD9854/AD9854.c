/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : AD9854.c
  - Author        : Walk-To-Me
  - Update date   : 2021��9��30��
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)
 * This file is a AD9854 driver c-file.

 * C language AD9854 DDS driver, parallel interface
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of Arbitrary Waveform Generator function on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "AD9854.h"

/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
uint8_t FTW[6];						// Ƶ�ʿ�����FTW

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void Freq_Convert(uint32_t freq);


/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/

/**
 * @brief     	AD9854��ʼ������
 * @param[in] 	void
 * @return	void
**/
void AD9854_Init(void)
{
	AD9854_WR = 1;
	AD9854_RD = 1;
	AD9854_UCLK = 0;
	AD9854_RST = 1;
	AD9854_RST = 0;
	
	AD9854_Write_Byte(AD9854_CTRL1_ADDR, 0x10);		// �رձȽ�������
	AD9854_Write_Byte(AD9854_CTRL2_ADDR, PLL_Mult_Value);	// ϵͳʱ�ӱ�Ƶ
	AD9854_Write_Byte(AD9854_CTRL3_ADDR, 0x00);		// �ⲿ����ʱ��
	AD9854_Write_Byte(AD9854_CTRL4_ADDR, 0x60);		// ����OSK����
	
	AD9854_UCLK = 1;					// �������
	AD9854_UCLK = 0;
}

/**
 * @brief     	Ƶ����ת������
 * @param[in] 	freq		���Ƶ��ֵ(��󲻳���sys_clk/2)
 * @return	void
**/
static void Freq_Convert(uint32_t freq)
{
	uint32_t freq_buf;
	uint8_t array_freq[4];
	
	
	array_freq[0] = (uint8_t)freq;
	array_freq[1] = (uint8_t)(freq>>8);
	array_freq[2] = (uint8_t)(freq>>16);
	array_freq[3] = (uint8_t)(freq>>24);

	freq_buf = Freq_Mult*array_freq[0];
	FTW[0] = freq_buf;
	freq_buf >>= 8;

	freq_buf += Freq_Mult*array_freq[1];
	FTW[1] = freq_buf;
	freq_buf >>= 8;

	freq_buf += Freq_Mult*array_freq[2];
	FTW[2] = freq_buf;
	freq_buf >>= 8;

	freq_buf += Freq_Mult*array_freq[3];
	FTW[3] = freq_buf;
	freq_buf >>= 8;

	FTW[4] = freq_buf;
	FTW[5] = freq_buf>>8;
}


/**
 * @brief     	AD9854���ò��κ���
 * @param[in] 	freq		���Ƶ��ֵ(��󲻳���sys_clk/2)
 * @param[in] 	freq		������η���(��ΧΪ0-4065)
 * @return	void
**/
void AD9854_SetSine(uint32_t freq, uint16_t shape)
{
	uint8_t i;
	
	Freq_Convert(freq);
	
	for (i = 0; i < 6; i++)
	{
		AD9854_Write_Byte(AD9854_FTW1_ADDR+i, FTW[5-i]);
	}
	AD9854_Write_Byte(AD9854_OSK_I_ADDR, shape>>8);
	AD9854_Write_Byte(AD9854_OSK_I_ADDR+1, shape);
	
	AD9854_Write_Byte(AD9854_OSK_Q_ADDR, shape>>8);
	AD9854_Write_Byte(AD9854_OSK_Q_ADDR+1, shape);
	
	AD9854_UCLK = 1;
	AD9854_UCLK = 0;
}


/**
 * @brief     	AD9854д�ֽں���(����)
 * @param[in] 	addr		д���ַ
 * @param[in] 	dat		д������
 * @return	void
**/
void AD9854_Write_Byte(uint8_t addr, uint8_t dat)
{
	AD9854_ADDR_BUS = (addr&0x3f) | (AD9854_ADDR_BUS&0xc0);
	AD9854_DATA_BUS = dat;
	AD9854_WR = 0;
	AD9854_WR = 1;
}


/* functions update: 2021��9��30��*/
/* ��λ����, ���ߣ��Ȼ��������д����� */
/* Ŀǰ����ʵ�����Ⲩ�η�������, �����߼�������FSK��BPSK��OSK��AM��RFSK�д����� */

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/


