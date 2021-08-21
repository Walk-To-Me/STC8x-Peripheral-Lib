/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : ssd1306.h
  - Author        : Walk-To-Me
  - Update date   : 2021年8月20日
  -	Copyright(C)  : 2021-2022 Walk-To_Me. All rights reserved.
-----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
|                            COPYRIGHT NOTICE                            |
------------------------------------------------------------------------*/
/*
 * Copyright (C) 2021, Walk-To-Me (Air_Code@outlook.com)

 * This file is a ssd1306 driver h-file.

 * C language SSD1306 OLED driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of OLED display functions on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "ELL_CORE.h"
#include "ELL_TYPE.h"
#include "STC8x_I2C.h"


/*-----------------------------------------------------------------------
|                             CMD CONFIG                                |
-----------------------------------------------------------------------*/
#define SET_CONTRAST						0x81
#define SET_ENTIRE_ON						0xa4
#define SET_NORM_INV						0xa6
#define SET_DISP_OFF						0xae
#define SET_DISP_ON						0xaf
#define	SET_MEM_ADDR						0x20
#define	SET_COL_ADDR						0x21
#define	SET_PAGE_ADDR						0x22
#define SET_DISP_START_LINE					0x40
#define SET_SEG_REMAP						0xa1
#define SET_MUX_RATIO						0xa8
#define SET_COM_OUT_DIR						0xc0
#define SET_DISP_OFFSET						0xd3
#define SET_COM_PIN_CFG						0xda
#define SET_DISP_CLK_DIV					0xd5
#define SET_PRECHARGE						0xd9
#define SET_VCOM_DESEL						0xdb
#define SET_CHARGE_PUMP						0x8d
#define SET_HORIZONTAL_SCROLL					0x26
#define SET_SCROLL_ON						0x2f
#define SET_SCROLL_OFF						0x2e

/*-----------------------------------------------------------------------
|                             USER CONFIG                               |
-----------------------------------------------------------------------*/
#define X_WIDTH 						128					// OLED屏幕宽度
#define Y_WIDTH							64					// OLED屏幕高度
#define PAGE							(Y_WIDTH >> 3)				// OLED屏幕页数
#define EXTERNAL_VCC						DISABLE					// 是否启用外部电源


/*-----------------------------------------------------------------------
|                             CMD_VAL CONFIG                            |
-----------------------------------------------------------------------*/
#define SET_MUX_RATIO_VAL					(Y_WIDTH - 1)
#define SET_DISP_OFFSET_VAL					0x00
#define SET_MEM_ADDR_VAL					0x02
#define SET_COM_PIN_CFG_VAL					(Y_WIDTH == 32 ? 0x02 : 0x12)
#define SET_CONTRAST_VAL					0x7f
#define SET_PRECHARGE_VAL					(EXTERNAL_VCC ? 0x22 : 0xf1)
#define SET_VCOM_DESEL_VAL					0x30
#define SET_DISP_CLK_DIV_VAL					0x80
#define SET_CHARGE_PUMP_VAL					(EXTERNAL_VCC ? 0x10 : 0x14)

/*-----------------------------------------------------------------------
|                             Type DEFINE                               |
-----------------------------------------------------------------------*/
typedef enum
{
	SSD1306_Tran_Cmd 	= 0x00,			/* SSD1306 write cmd  */
	SSD1306_Tran_Data 	= 0x01			/* SSD1306 write data */
}	SSD1306_Type;

typedef enum
{
	Scroll_Time_2 		= 0x07,			/* 2 frames */
	Scroll_Time_3 		= 0x04,			/* 3 frames */
	Scroll_Time_4		= 0x05,			/* 4 frames */
	Scroll_Time_5		= 0x00,			/* 5 frames */
	Scroll_Time_25		= 0x06,			/* 25 frames */
	Scroll_Time_64		= 0x01,			/* 64 frames */
	Scroll_Time_128		= 0x02,			/* 128 frames */
	Scroll_Time_256		= 0x03			/* 256 frames */
}	SCROLL_TIME_Type;

typedef enum
{
	FONT_SIZE_16 		= 16,			/* font size 16 */
}	FONT_SIZE_Type;



/*-----------------------------------------------------------------------
|                             API FUNCTION                              |
-----------------------------------------------------------------------*/
void SSD1306_Init(void);
void SSD1306_Display_On(void);
void SSD1306_Display_Off(void);
void SSD1306_Normal_Display(void);
void SSD1306_Inverse_Display(void);
void SSD1306_Scroll_Off(void);
void SSD1306_Scroll_On(void);
void SSD1306_Horizontal_Scroll(uint8_t page_start, uint8_t page_end, SCROLL_TIME_Type time, uint8_t mode);
void SSD1306_Draw_Point(uint8_t x, uint8_t y, uint8_t dot);
void SSD1306_Fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t dot);
void SSD1306_Fill_All(void);
void SSD1306_Clr(void);
void SSD1306_Show_Char(uint8_t x, uint8_t y, uint8_t chr, FONT_SIZE_Type Size);
void SSD1306_Show_String(uint8_t x, uint8_t y, uint8_t *str, FONT_SIZE_Type Size);
void SSD1306_Show_Num(uint8_t x, uint8_t y, int32_t num, uint8_t len, FONT_SIZE_Type Size);
void SSD1306_Show_Float(uint8_t x, uint8_t y, float num, uint8_t len, uint8_t acc, FONT_SIZE_Type Size);
void SSD1306_Show_Chinese(uint8_t x, uint8_t y, uint8_t pos, FONT_SIZE_Type Size);
void SSD1306_Show_Picture(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t *pic);

#endif

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/

