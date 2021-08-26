/*-----------------------------------------------------------------------
|                            FILE DESCRIPTION                           |
-----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
  - File name     : ssd1306.c
  - Author        : Walk-To-Me
  - Update date   : 2021年8月27日
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
 * @brief     DS3231初始化函数
 * @param[in] void
 * @return    void
**/
void DS3231_Init(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~((1<<7) | (1<<1) | (1<<0));				// 上电清除OSF, A2F, A1F标志位
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231设置时间函数
 * @param[in] time  设置时间数据的结构体指针
 * @return    FSC_FAIL     设置失败
 * @return    FSC_SUCCESS  设置成功
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
	time->hour &= ~(1 << 6);			 		// 12/24小时制标志位清0
	DS3231_Write_Data(DS3231_TIME_ADDR, (uint8_t *)time, 7);
	return FSC_SUCCESS;
}


/**
 * @brief     DS3231获取时间函数
 * @param[in] time  获取时间数据的结构体指针
 * @return    void
**/
void DS3231_Get_Time(DS3231_TIME_Type *time)
{
	DS3231_Read_Data(DS3231_TIME_ADDR, (uint8_t *)time, 7);
	bcd_to_bin((uint8_t *)time, 7);
	if (time->hour >= 12)
	{
		time->am_pm = 1;					// am_pm标志置1  PM模式
		if (time->sys_12_24hour)
			time->hour -= 12;				// 12小时制格式修正
	}
	else
	{
		time->am_pm = 0;					// am_pm标志置1  AM模式
	}
}


/**
 * @brief     DS3231设置闹钟1函数
 * @param[in] time  设置闹钟1定时数据的结构体指针
 * @param[in] mode  闹钟1定时模式
 * @return    FSC_FAIL     设置失败
 * @return    FSC_SUCCESS  设置成功
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
	time->hour &= ~(1 << 6);					// 12/24小时制标志位清0
	time->second |= ((mode&(1<<0))<<7);				// 设置A1M1标志位
	time->minute |= ((mode&(1<<1))<<6);				// 设置A1M2标志位
	time->hour |= ((mode&(1<<2))<<5);				// 设置A1M3标志位
	time->day_or_date |= ((mode&(1<<3))<<4);			// 设置A1M4标志位
	DS3231_Write_Data(DS3231_ALARM1_ADDR, (uint8_t *)time, 4);
	return FSC_SUCCESS;
}


/**
 * @brief     DS3231设置闹钟2函数
 * @param[in] time  设置闹钟2定时数据的结构体指针
 * @param[in] mode  闹钟2定时模式
 * @return    FSC_FAIL     设置失败
 * @return    FSC_SUCCESS  设置成功
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
	time->hour &= ~(1 << 6);					// 12/24小时制标志位清0
	time->minute |= ((mode&(1<<0))<<7);				// 设置A2M2标志位
	time->hour |= ((mode&(1<<1))<<6);				// 设置A2M3标志位
	time->day_or_date |= ((mode&(1<<2))<<5);			// 设置A2M4标志位
	DS3231_Write_Data(DS3231_ALARM2_ADDR, (uint8_t *)time, 3);
	return FSC_SUCCESS;
}


/**
 * @brief     DS3231使能32kHz引脚输出32.768kHz方波信号
 * @param[in] void
 * @return    void
**/
void DS3231_EN32kHz_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat |= (1<<3);					// 使能EN32kHz引脚输出
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231禁止32kHz引脚输出32.768kHz方波信号
 * @param[in] void
 * @return    void
**/
void DS3231_EN32kHz_Off(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~(1<<3);					// 禁止EN32kHz引脚输出
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231设置并使能INT/SQW引脚SQW功能(禁止INT功能)
 * @param[in] sqw   SQW方波输出频率模式
 * @return    void
**/
void DS3231_Set_SQW(DS3231_SQW_Type sqw)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (sqw<<3);				// 设置频率选择位
	dat &= ~(1<<2);					// 使能SQW方波输出功能(禁止闹钟中断功能)
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231使能INT/SQW引脚SQW功能(禁止INT功能)
 * @param[in] void
 * @return    void
**/
void DS3231_SQW_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat &= ~(1<<2);					// 使能SQW方波输出功能(禁止闹钟中断功能)
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231使能INT/SQW引脚INT功能(禁止SQW功能)
 * @param[in] void
 * @return    void
**/
void DS3231_INT_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<2);					// 使能闹钟中断输出功能(禁止SQW方波输出功能)
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}



/**
 * @brief     DS3231闹钟1中断使能
 * @param[in] void
 * @return    void
**/
void DS3231_A1IE_ON(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<0);					// 使能闹钟1中断
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231闹钟2中断使能
 * @param[in] void
 * @return    void
**/
void DS3231_A2IE_ON(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<1);					// 使能闹钟2中断
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);
}


/**
 * @brief     DS3231获取闹钟1标志
 * @param[in] void
 * @return    void
**/
uint8_t DS3231_Get_A1F(void)
{
	uint8_t ret_val = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &ret_val, 1);
	ret_val &= (1<<0);				// 获取A1F标志位
	return ret_val;
}


/**
 * @brief     DS3231获取闹钟2标志
 * @param[in] void
 * @return    void
**/
uint8_t DS3231_Get_A2F(void)
{
	uint8_t ret_val = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &ret_val, 1);
	ret_val &= (1<<1);				// 获取A1F标志位
	return ret_val;
}


/**
 * @brief     DS3231清除闹钟1标志
 * @param[in] void
 * @return    void
**/
void DS3231_Clr_A1F(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~(1<<0);					// 清除A1F标志位
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231清除闹钟2标志
 * @param[in] void
 * @return    void
**/
void DS3231_Clr_A2F(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	dat &= ~(1<<1);					// 清除A2F标志位
	DS3231_Write_Data(DS3231_STA_ADDR, &dat, 1);
}


/**
 * @brief     DS3231设置校准(通过调整晶体老化补偿参数, 观察32kHz引脚输出进行校准)
 * @param[in] dat   晶体老化补偿参数(含符号位)      + 降低振荡频率  - 增大振荡频率
 * @return    void
**/
void DS3231_Set_Cal(int8 dat)
{
	DS3231_Write_Data(DS3231_AO_ADDR, &dat, 1);		// 设置晶体老化补偿
	DS3231_CONV_On();				// 开启温度转换, 执行TCXO算法更新振荡器电容阵列
}


/**
 * @brief     DS3231强制开启温度转换(用于调整晶体老化补偿参数后, 立刻观察老化寄存器对32kHz输出频率的影响)
 * @param[in] void
 * @return    void
**/
void DS3231_CONV_On(void)
{
	uint8_t dat = 0;
	DS3231_Read_Data(DS3231_STA_ADDR, &dat, 1);
	while (dat & 0x04);				// BSY位检测
	DS3231_Read_Data(DS3231_CON_ADDR, &dat, 1);
	dat |= (1<<5);
	DS3231_Write_Data(DS3231_CON_ADDR, &dat, 1);		// 设置CONV位为1, 开启温度转换
}



/**
 * @brief     DS3231连续写n字节数据
 * @param[in] addr  写入数据初始地址
 * @param[in] dat   写入数据指针
 * @param[in] n     写入数据字节个数
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
 * @brief     DS3231连续读n字节数据
 * @param[in] addr  读取数据初始地址
 * @param[in] dat   读取数据指针
 * @param[in] n     读取数据字节个数
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
 * @brief     二十进制转BCD格式
 * @param[in] dat   数据指针
 * @param[in] n     数据字节个数
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
 * @brief     BCD格式转二十进制
 * @param[in] dat   数据指针
 * @param[in] n     数据字节个数
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

