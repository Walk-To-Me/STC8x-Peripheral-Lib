/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : aht10.c
  - Author        : Walk-To-Me
  - Update date   : 2021��8��30��
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)
 * This file is a AD9833 driver c-file.

 * C language AD9833 DDS driver, SPI interface
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of Arbitrary Waveform Generator function on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "ad9833.h"

/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
uint16_t ctrl_word;						// ���ƼĴ�������

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void AD9833_Write_Word(uint16_t dat); 

/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/
/**
 * @brief     	AD9833��ʼ������, ���δʹ�øó�ʼ������, ��Ĭ��Ϊ����д��14λģʽ(D13λΪ0 D12λΪ0)
 * @param[in] 	void
 * @return	void
**/
void AD9833_Init(void)
{
	AD9833_Write_Word(0x2100);				// �ڲ��Ĵ�������
	ctrl_word = 0x2000;					// ������������дģʽ(����дģʽΪ0x0000)
}

/**
 * @brief     	AD9833���ò���
 * @param[in] 	freq		Ƶ�ʷ�Χ(0-AD9833_FMCLK)
 * @param[in] 	shape		������״(����, ����, ����)
 * @param[in] 	reg		Ƶ�ʼĴ���(Ƶ�ʼĴ���0, Ƶ�ʼĴ���1)
 * @return	void
**/
void AD9833_Set_Wave(uint32_t freq, AD9833_Wave_Shape_Type shape, AD9833_Freq_Register_Type reg)
{
	uint32_t freq_words;
	freq_words = AD9833_FREQ_WORDS_GET(freq);
	AD9833_Write_Word(0x2100);
	AD9833_Write_Word(((reg==FREQ0) ? 0x4000:0x8000) | (freq_words&0x3fff));		// д���14λ
	AD9833_Write_Word(((reg==FREQ0) ? 0x4000:0x8000) | ((freq_words>>14)&0x3fff));		// д���14λ
	
	ctrl_word &= ~(1<<1 | 1<<5 | 1<<11);			// ������κ�Ƶ�ʼĴ������λ
	ctrl_word |= (shape | reg);				// ���ò��κ�Ƶ�ʼĴ������λ
	AD9833_Write_Word(ctrl_word);				// ���ò��κ�Ƶ�ʼĴ���
}


/**
 * @brief     	AD9833���ò�����״
 * @param[in] 	shape		������״(����, ����, ����)
 * @return	void
**/
void AD9833_Set_Wave_Shape(AD9833_Wave_Shape_Type shape)
{
	ctrl_word &= ~(1<<1 | 1<<5);				// ����������λ
	ctrl_word |= shape;					// ���ò������λ
	AD9833_Write_Word(ctrl_word);				// ���ò���
}


/**
 * @brief     	AD9833����Ƶ�ʼĴ���
 * @param[in] 	shape		Ƶ�ʼĴ���(Ƶ�ʼĴ���0, Ƶ�ʼĴ���1)
 * @return	void
**/
void AD9833_Set_Freq_Register(AD9833_Freq_Register_Type reg)
{
	ctrl_word &= ~(1<<11);					// ���Ƶ�ʼĴ������λ
	ctrl_word |= reg;					// ����Ƶ�ʼĴ������λ
	AD9833_Write_Word(ctrl_word);
}


/**
 * @brief     	AD9833дһ��������
 * @param[in] 	dat		д��������
 * @return	void
**/
static void AD9833_Write_Word(uint16_t dat)
{
	uint8_t i;
	AD9833_SCLK = 1;
	AD9833_FSYNC = 1;
	AD9833_FSYNC = 0;
	
	for (i = 0; i < 16; i++)
	{
		AD9833_SDATA = dat & 0x8000;
		AD9833_SCLK = 0;
		AD9833_SCLK = 1;
		dat <<= 1;
	}
	AD9833_FSYNC = 1;
	AD9833_SCLK = 0;
}

/* functions update: 2021��9��9��*/
/* ��λ����, ���ߵȻ��������д����� */
/* Ŀǰ����ʵ�����Ⲩ�η�������, �����߼�������FSK�д����� */

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/



