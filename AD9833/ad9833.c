/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : aht10.c
  - Author        : Walk-To-Me
  - Update date   : 2021年8月30日
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
uint16_t ctrl_word;						// 控制寄存器数据

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void AD9833_Write_Word(uint16_t dat); 

/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/
/**
 * @brief     	AD9833初始化函数, 如果未使用该初始化函数, 将默认为单次写低14位模式(D13位为0 D12位为0)
 * @param[in] 	void
 * @return	void
**/
void AD9833_Init(void)
{
	AD9833_Write_Word(0x2100);				// 内部寄存器重启
	ctrl_word = 0x2000;					// 设置连续两次写模式(单次写模式为0x0000)
}

/**
 * @brief     	AD9833设置波形
 * @param[in] 	freq		频率范围(0-AD9833_FMCLK)
 * @param[in] 	shape		波形形状(正弦, 三角, 矩形)
 * @param[in] 	reg		频率寄存器(频率寄存器0, 频率寄存器1)
 * @return	void
**/
void AD9833_Set_Wave(uint32_t freq, AD9833_Wave_Shape_Type shape, AD9833_Freq_Register_Type reg)
{
	uint32_t freq_words;
	freq_words = AD9833_FREQ_WORDS_GET(freq);
	AD9833_Write_Word(0x2100);
	AD9833_Write_Word(((reg==FREQ0) ? 0x4000:0x8000) | (freq_words&0x3fff));		// 写入低14位
	AD9833_Write_Word(((reg==FREQ0) ? 0x4000:0x8000) | ((freq_words>>14)&0x3fff));		// 写入高14位
	
	ctrl_word &= ~(1<<1 | 1<<5 | 1<<11);			// 清除波形和频率寄存器相关位
	ctrl_word |= (shape | reg);				// 设置波形和频率寄存器相关位
	AD9833_Write_Word(ctrl_word);				// 设置波形和频率寄存器
}


/**
 * @brief     	AD9833设置波形形状
 * @param[in] 	shape		波形形状(正弦, 三角, 矩形)
 * @return	void
**/
void AD9833_Set_Wave_Shape(AD9833_Wave_Shape_Type shape)
{
	ctrl_word &= ~(1<<1 | 1<<5);				// 清除波形相关位
	ctrl_word |= shape;					// 设置波形相关位
	AD9833_Write_Word(ctrl_word);				// 设置波形
}


/**
 * @brief     	AD9833设置频率寄存器
 * @param[in] 	shape		频率寄存器(频率寄存器0, 频率寄存器1)
 * @return	void
**/
void AD9833_Set_Freq_Register(AD9833_Freq_Register_Type reg)
{
	ctrl_word &= ~(1<<11);					// 清除频率寄存器相关位
	ctrl_word |= reg;					// 设置频率寄存器相关位
	AD9833_Write_Word(ctrl_word);
}


/**
 * @brief     	AD9833写一个字数据
 * @param[in] 	dat		写入字数据
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

/* functions update: 2021年9月9日*/
/* 相位设置, 休眠等基本函数有待更新 */
/* 目前仅仅实现任意波形发生功能, 其他高级功能如FSK有待完善 */

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/



