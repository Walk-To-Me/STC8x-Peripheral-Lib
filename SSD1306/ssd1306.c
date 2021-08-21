/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : ssd1306.c
  - Author        : Walk-To-Me
  - Update date   : 2021年8月20日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a ssd1306 driver c-file.

 * C language SSD1306 OLED driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of OLED display functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "ssd1306.h"
#include "font.h"
/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
static void SSD1306_Write_Cmd(uint8_t cmd);
static void SSD1306_Write_Data(uint8_t dat);
static void SSD1306_Write_Byte(uint8_t dat, SSD1306_Type state);
/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/

/**
 * @brief     SSD1306初始化函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Init(void)
{
	SSD1306_Write_Byte(SET_DISP_OFF, SSD1306_Tran_Cmd);			// Display Off
	SSD1306_Write_Byte(SET_SCROLL_OFF, SSD1306_Tran_Cmd);			// Scroll off	(if you need to use scroll functions, it is important to turn off the scroll before use it)
	SSD1306_Write_Byte(SET_MUX_RATIO, SSD1306_Tran_Cmd);			// Set MUX Ratio
	SSD1306_Write_Byte(SET_MUX_RATIO_VAL, SSD1306_Tran_Cmd);		// MUX: 63(maxium)
	SSD1306_Write_Byte(SET_DISP_OFFSET, SSD1306_Tran_Cmd);			// Set Display Offset
	SSD1306_Write_Byte(SET_DISP_OFFSET_VAL, SSD1306_Tran_Cmd);		// Offset: 0
	SSD1306_Write_Byte(SET_DISP_START_LINE, SSD1306_Tran_Cmd);		// Set Display Start Line
	SSD1306_Write_Byte(SET_MEM_ADDR, SSD1306_Tran_Cmd);			// Set memory address mode
	SSD1306_Write_Byte(SET_MEM_ADDR_VAL, SSD1306_Tran_Cmd);			// mem_addr mode: 0x02
	SSD1306_Write_Byte(SET_SEG_REMAP, SSD1306_Tran_Cmd);			// Set Segment re-map 0 - > 127
	SSD1306_Write_Byte(SET_COM_OUT_DIR, SSD1306_Tran_Cmd);			// Set COM Output Scan Direction 
	SSD1306_Write_Byte(SET_COM_PIN_CFG, SSD1306_Tran_Cmd);			// Set COM Pins hardware configuration
	SSD1306_Write_Byte(SET_COM_PIN_CFG_VAL, SSD1306_Tran_Cmd);		// Y_WIDTH = 64 -> 0x12
	SSD1306_Write_Byte(SET_CONTRAST, SSD1306_Tran_Cmd);			// Set Contrast Control	
	SSD1306_Write_Byte(SET_CONTRAST_VAL, SSD1306_Tran_Cmd);			// Contrast: 0x7F
	SSD1306_Write_Byte(SET_PRECHARGE, SSD1306_Tran_Cmd);			// SET_PRECHARGE
	SSD1306_Write_Byte(SET_PRECHARGE_VAL, SSD1306_Tran_Cmd);		// PRECHARGE: 0xf1
	SSD1306_Write_Byte(SET_VCOM_DESEL, SSD1306_Tran_Cmd);			// Setting vcomh voltage multiplier
	SSD1306_Write_Byte(SET_VCOM_DESEL_VAL, SSD1306_Tran_Cmd);		// 0.83*vcc -> 0x30
	SSD1306_Write_Byte(SET_ENTIRE_ON, SSD1306_Tran_Cmd);			// Entire Display On
	SSD1306_Write_Byte(SET_NORM_INV, SSD1306_Tran_Cmd);			// Set Normal Display
	SSD1306_Write_Byte(SET_DISP_CLK_DIV, SSD1306_Tran_Cmd);			// Set Osc Frequency 
	SSD1306_Write_Byte(SET_DISP_CLK_DIV_VAL, SSD1306_Tran_Cmd);		// CLK_DIV: 0x80
	SSD1306_Write_Byte(SET_CHARGE_PUMP, SSD1306_Tran_Cmd);			// Enable charge pump regulator
	SSD1306_Write_Byte(SET_CHARGE_PUMP_VAL, SSD1306_Tran_Cmd); 		// CHARGE_PUMP OPEN -> 0x14
	SSD1306_Write_Byte(SET_DISP_ON, SSD1306_Tran_Cmd);			// Display On
}


/**
 * @brief     SSD1306打开显示功能函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Display_On(void)
{
	SSD1306_Write_Byte(SET_CHARGE_PUMP, SSD1306_Tran_Cmd);
	SSD1306_Write_Byte(SET_CHARGE_PUMP_VAL, SSD1306_Tran_Cmd);
	SSD1306_Write_Byte(SET_DISP_ON, SSD1306_Tran_Cmd);
}


/**
 * @brief     SSD1306关闭显示功能函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Display_Off(void)
{
	SSD1306_Write_Byte(SET_CHARGE_PUMP, SSD1306_Tran_Cmd);
	SSD1306_Write_Byte(SET_CHARGE_PUMP_VAL & 0xf0, SSD1306_Tran_Cmd);
	SSD1306_Write_Byte(SET_DISP_OFF, SSD1306_Tran_Cmd);
}


/**
 * @brief     SSD1306正常显示模式（默认）
 * @param[in] void
 * @return    void
**/
void SSD1306_Normal_Display(void)
{
	SSD1306_Write_Byte(SET_NORM_INV, SSD1306_Tran_Cmd);
}


/**
 * @brief     SSD1306反转显示模式
 * @param[in] void
 * @return    void
**/
void SSD1306_Inverse_Display(void)
{
	SSD1306_Write_Byte(SET_NORM_INV | 0x01, SSD1306_Tran_Cmd);
}

/**
 * @brief     SSD1306激活滚动函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Scroll_On(void)
{
	SSD1306_Write_Byte(SET_SCROLL_ON, SSD1306_Tran_Cmd);
}


/**
 * @brief     SSD1306关闭滚动函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Scroll_Off(void)
{
	SSD1306_Write_Byte(SET_SCROLL_OFF, SSD1306_Tran_Cmd);
}


/**
 * @brief     SSD1306水平滚动函数
 * @param[in] page_start	起始页
 * @param[in] page_end		终止页
 * @param[in] time		每次滚屏时间(X frames)
 * @param[in] mode		0 向右滚动  1 向左滚动
 * @return    void
**/
void SSD1306_Horizontal_Scroll(uint8_t page_start, uint8_t page_end, SCROLL_TIME_Type time, uint8_t mode)
{
	SSD1306_Write_Byte(SET_SCROLL_OFF, SSD1306_Tran_Cmd);				// 先关闭滚动
	SSD1306_Write_Byte(SET_HORIZONTAL_SCROLL|mode, SSD1306_Tran_Cmd);		// 设置滚动方向
	SSD1306_Write_Byte(0x00, SSD1306_Tran_Cmd);					// Dummy byte 
	SSD1306_Write_Byte(PAGE-1-page_end, SSD1306_Tran_Cmd);				// 设置起始页
	SSD1306_Write_Byte(time, SSD1306_Tran_Cmd);					// 设置每次滚屏时间
	SSD1306_Write_Byte(PAGE-1-page_start, SSD1306_Tran_Cmd);			// 设置终止页
	SSD1306_Write_Byte(0x00, SSD1306_Tran_Cmd);					// Dummy byte 
	SSD1306_Write_Byte(0xff, SSD1306_Tran_Cmd);					// Dummy byte 
	SSD1306_Write_Byte(SET_SCROLL_ON, SSD1306_Tran_Cmd);				// 激活滚动
}


/**
 * @brief     SSD1306设置点坐标函数
 * @param[in] x    横坐标
 * @param[in] y    纵坐标(按页分隔)
 * @return    void
**/
void SSD1306_Set_Pos(uint8_t x, uint8_t y) 
{
	if (x < X_WIDTH && y < PAGE)
	{
		SSD1306_Write_Byte(0xB0 + PAGE - 1 - y, SSD1306_Tran_Cmd);
		SSD1306_Write_Byte(((x&0xF0)>>4)|0x10, SSD1306_Tran_Cmd);
		SSD1306_Write_Byte(x&0x0F, SSD1306_Tran_Cmd); 
	}
}   

/**
 * @brief     SSD1306画点函数
 * @param[in] x    横坐标
 * @param[in] y    纵坐标(按页分隔)
 * @param[in] dot  画点数据 1 亮, 0 暗
 * @return    void
**/
void SSD1306_Draw_Point(uint8_t x, uint8_t y, uint8_t dot)
{
	SSD1306_Set_Pos(x, y);
	if (!dot) SSD1306_Write_Byte(0x00 ,SSD1306_Tran_Data);
	else SSD1306_Write_Byte(0xff ,SSD1306_Tran_Data);
}


/**
 * @brief     SSD1306矩形填充函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t dot)
{
	uint8_t i, j;
	for (i = x0; i <= x1; i++)
		for (j = y0; j <= y1; j++)
			SSD1306_Draw_Point(i, j, dot);
}

/**
 * @brief     SSD1306点亮全屏函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Fill_All(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		SSD1306_Write_Byte(0xB0 + i, SSD1306_Tran_Cmd);
		SSD1306_Write_Byte(0x00, SSD1306_Tran_Cmd);
		SSD1306_Write_Byte(0x10, SSD1306_Tran_Cmd);
		for (n = 0; n < 128; n++) SSD1306_Write_Byte(0xff, SSD1306_Tran_Data);
	}
}

/**
 * @brief     SSD1306清屏函数
 * @param[in] void
 * @return    void
**/
void SSD1306_Clr(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		SSD1306_Write_Byte(0xB0+i, SSD1306_Tran_Cmd);
		SSD1306_Write_Byte(0x00, SSD1306_Tran_Cmd);
		SSD1306_Write_Byte(0x10, SSD1306_Tran_Cmd);
		for (n = 0; n < 128; n++) SSD1306_Write_Byte(0x00, SSD1306_Tran_Data);
	}
}

/**
 * @brief     SSD1306显示字符函数
 * @param[in] x		横坐标
 * @param[in] y		纵坐标(按页分隔)
 * @param[in] chr	字符
 * @param[in] Size	字体尺寸
 * @return    void
**/
void SSD1306_Show_Char(uint8_t x, uint8_t y, uint8_t chr, FONT_SIZE_Type Size)
{
	uint8_t i, size_x;
	size_x = Size / 2;		// 字符宽度为字体尺寸的1/2
	chr = chr - ' ';
	SSD1306_Set_Pos(x, y);
	for (i = 0; i < size_x; i++)
		SSD1306_Write_Byte(ASCII_16_08[chr][i], SSD1306_Tran_Data);
	SSD1306_Set_Pos(x, y+1);
	for (i = 0; i < size_x; i++)
		SSD1306_Write_Byte(ASCII_16_08[chr][i+size_x], SSD1306_Tran_Data);
}

/**
 * @brief     SSD1306显示字符串函数
 * @param[in] x    横坐标
 * @param[in] y    纵坐标(按页分隔)
 * @param[in] str  字符串
 * @param[in] Size 字体尺寸
 * @return    void
**/
void SSD1306_Show_String(uint8_t x, uint8_t y, uint8_t *str, FONT_SIZE_Type Size)
{
	while (*str != '\0')
	{
		SSD1306_Show_Char(x, y, *(str++), Size);
		x += Size / 2;		// 偏移一个字符宽度(1/2字体尺寸)
	}
}

/**
 * @brief     SSD1306显示整数函数
 * @param[in] x    横坐标
 * @param[in] y    纵坐标(按页分隔)
 * @param[in] num  显示的整数(支持正负数识别)
 * @param[in] len  显示的整数数字长度(不包括符号)
 * @param[in] Size 字体尺寸
 * @return    void
**/
void SSD1306_Show_Num(uint8_t x, uint8_t y, int32_t num, uint8_t len, FONT_SIZE_Type Size)
{
	uint8_t i, dat, sign;
	sign = (num >= 0) ? 0 : 1;
	if (sign)
	{
		SSD1306_Show_Char(x, y, '-', Size);
		num = -num;
	}
	for (i = 0; i < len; i++)
	{
		dat = num % 10;
		if (num == 0 && i != 0)
		{
			SSD1306_Show_Char(x-(8*i)+(len-1+sign)*8, y, ' ', Size);
			continue;
		}
		num = num / 10;
		SSD1306_Show_Char(x-(8*i)+(len-1+sign)*8, y, dat+'0', Size);
	}
}

/**
 * @brief     SSD1306显示浮点数函数
 * @param[in] x    横坐标
 * @param[in] y    纵坐标(按页分隔)
 * @param[in] num  显示的浮点数(支持正负数识别)
 * @param[in] len  显示的浮点数中数字长度(不包括符号和小数点)
 * @param[in] acc  显示的浮点数中小数长度
 * @param[in] Size 字体尺寸
 * @return    void
**/
void SSD1306_Show_Float(uint8_t x, uint8_t y, float num, uint8_t len, uint8_t acc, FONT_SIZE_Type Size)
{
	uint8_t i, dat, sign;
	uint32_t Num;
	sign = (num >= 0) ? 0 : 1;
	if (sign)
	{
		SSD1306_Show_Char(x, y, '-', Size);
		num = -num;
	}
	
	for (i = 0; i < acc; i++)
		num *= 10;
	Num = num;
	for (i = 0; i <= len; i++)
	{
		dat = Num % 10;
		if (i != acc)
		{
			if (Num == 0 && dat != 0)
			{
				SSD1306_Show_Char(x-(8*i)+(len+sign)*8, y, ' ', Size);
				continue;
			}
			Num = Num / 10;
			SSD1306_Show_Char(x-(8*i)+(len+sign)*8, y, dat+'0', Size);
		}
		else
			SSD1306_Show_Char(x-(8*i)+(len+sign)*8, y, '.', Size);
	}
}



/**
 * @brief     SSD1306显示汉字函数
 * @param[in] x		横坐标
 * @param[in] y		纵坐标(按页分隔)
 * @param[in] pos	显示的汉字索引
 * @param[in] Size	字体尺寸
 * @return    void
**/
void SSD1306_Show_Chinese(uint8_t x, uint8_t y, uint8_t pos, FONT_SIZE_Type Size)
{
	uint8_t i;
	SSD1306_Set_Pos(x, y);
	for (i = 0; i < Size; i++)
		SSD1306_Write_Byte(HanZi_16_16[pos][i], SSD1306_Tran_Data);
	SSD1306_Set_Pos(x, y + 1);
	for (i = 0; i < Size; i++)
		SSD1306_Write_Byte(HanZi_16_16[pos][i + Size], SSD1306_Tran_Data);
}


/**
 * @brief     SSD1306显示图片函数
 * @param[in] x      横坐标
 * @param[in] y      纵坐标(按页分隔)
 * @param[in] width  图片宽度
 * @param[in] height 图片长度
 * @param[in] pic    图片数据
 * @return    void
**/
void SSD1306_Show_Picture(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *pic)
{
	uint8_t i, j, page_width;
	page_width = height/8 + ((height%8)?1:0);		// 图片所占page宽度
	
	for (i = 0; i < page_width; i++)
	{
		SSD1306_Set_Pos(x, y+i);
		for (j = 0; j < width; j++)
			SSD1306_Write_Byte(pic[j+i*width], SSD1306_Tran_Data);
	}
}


/**
 * @brief     SSD1306写字节函数
 * @param[in] dat	一个字节数据
 * @param[in] state	数据处理类型，命令/数据  
 * @return    void
**/
static void SSD1306_Write_Byte(uint8_t dat, SSD1306_Type state)
{
	EAXFR_ENABLE();						// !important  XFR寄存器使能
	if (!state)
	{
		SSD1306_Write_Cmd(dat);
	}
	else
	{
		SSD1306_Write_Data(dat);
	}
	EAXFR_DISABLE();					// 关闭XFR寄存器使能
}


/**
 * @brief     IIC写命令
 * @param[in] cmd    一个字节数据
 * @return    void
**/
static void SSD1306_Write_Cmd(uint8_t cmd)
{
	I2C_Send_Start();
	I2C_Send_Btye(0x78);
	I2C_Read_ACK();
	I2C_Send_Btye(0x00);
	I2C_Read_ACK();
	I2C_Send_Btye(cmd);
	I2C_Read_ACK();
	I2C_Send_Stop();
}


/**
 * @brief     IIC写数据
 * @param[in] dat    一个字节数据
 * @return    void
**/
static void SSD1306_Write_Data(uint8_t dat)
{
	I2C_Send_Start();
	I2C_Send_Btye(0x78);
	I2C_Read_ACK();
	I2C_Send_Btye(0x40);
	I2C_Read_ACK();
	I2C_Send_Btye(dat);
	I2C_Read_ACK();
	I2C_Send_Stop();
}

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/



