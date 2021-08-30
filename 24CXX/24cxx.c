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
 * This file is a AT24CXX driver c-file.

 * C language AT24CXX EEPROM driver, I2C interfaces
 * base on 8051 ELL low-layer libraries by zeweni
 * appy to efficient and fast construction of EEPROM function on STC8x
 * For more information on this, contact me by e-mail	(Air_Code@outlook.com)
**/

/*-----------------------------------------------------------------------
|                               INCLUDES                                |
-----------------------------------------------------------------------*/
#include "24cxx.h"
/*-----------------------------------------------------------------------
|                                 DATA                                  |
-----------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------
|                      STATIC FUNCTION DECLARATION                      |
-----------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------
|                               FUNCTION                                |
-----------------------------------------------------------------------*/


/**
 * @brief     	AHT10初始化函数, 重置写地址计数器为0(可不初始化)
 * @param[in] 	void
 * @return	void
**/
void AT24CXX_Init(void)
{
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_WR_ADDR);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(0x00);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(0x00);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
	DELAY_Set_Ms(AT24CXX_WR_TIME);				// (!important)
}


/**
 * @brief     	AHT10写数据函数(任意大小, 速度优化)
 * @param[in] 	addr		写数据存放起始字节地址
 * @param[in] 	dat		写数据指针
 * @param[in] 	n		写数据个数
 * @return	void
**/
void AT24CXX_Write_Data(uint16_t addr, uint8_t *dat, uint16_t n)
{
	uint8_t i;
	uint8_t temp, rest, page;
	if (addr % AT24CXX_PAGE_SIZE == 0)
	{
		page = addr / AT24CXX_PAGE_SIZE;
		if (n / AT24CXX_PAGE_SIZE == 0)
			AT24CXX_Write_NotFull_Page(addr, dat, (uint8_t)n);
		else
		{
			for (i = 0; i < n / AT24CXX_PAGE_SIZE; i++)
			{
				AT24CXX_Write_Page(page++, dat);
				dat += AT24CXX_PAGE_SIZE;
			}
			rest = n % AT24CXX_PAGE_SIZE;
			if (rest)	AT24CXX_Write_NotFull_Page(page*AT24CXX_PAGE_SIZE, dat, rest);
		}
	}
	else
	{
		temp = addr%AT24CXX_PAGE_SIZE + n;
		if (temp < AT24CXX_PAGE_SIZE)
			AT24CXX_Write_NotFull_Page(addr, dat, (uint8_t)n);
		else
		{
			AT24CXX_Write_NotFull_Page(addr, dat, AT24CXX_PAGE_SIZE - addr%AT24CXX_PAGE_SIZE);
			dat += (AT24CXX_PAGE_SIZE - addr%AT24CXX_PAGE_SIZE);
			addr += (AT24CXX_PAGE_SIZE - addr%AT24CXX_PAGE_SIZE);
			page = addr / AT24CXX_PAGE_SIZE;
			rest = temp - AT24CXX_PAGE_SIZE;
			if (rest / AT24CXX_PAGE_SIZE == 0)
				AT24CXX_Write_NotFull_Page(addr, dat, rest % AT24CXX_PAGE_SIZE);
			else
			{
				for (i = 0; i < rest / AT24CXX_PAGE_SIZE; i++)
				{
					AT24CXX_Write_Page(page++, dat);
					dat += AT24CXX_PAGE_SIZE;
				}
				rest = rest % AT24CXX_PAGE_SIZE;
				if (rest)	AT24CXX_Write_NotFull_Page(page*AT24CXX_PAGE_SIZE, dat, rest);
			}
			
		}
	}
}



/**
 * @brief     	AHT10填充字节函数
 * @param[in] 	page		填充数据存放起始页
 * @param[in] 	dat		填充数据
 * @param[in] 	n		填充数据页数
 * @return	void
**/
void AT24CXX_Fill_Data(uint16_t page, uint8_t dat, uint16_t n)
{
	uint16_t addr;
	uint8_t addr_low, addr_high;
	uint16_t i;
	uint8_t j;
	
	for (i = 0; i < n; i++)
	{
		addr = page * AT24CXX_PAGE_SIZE;
		addr_low = (uint8_t)addr;
		addr_high = (uint8_t)(addr>>8);
		
		#if (!AT24CXX_I2C_SET)
			EAXFR_ENABLE();
		#endif
		AT24CXX_I2C_Send_Start();
		AT24CXX_I2C_Send_Byte(AT24CXX_WR_ADDR);
		AT24CXX_I2C_Read_ACK();
		AT24CXX_I2C_Send_Byte(addr_high);
		AT24CXX_I2C_Read_ACK();
		AT24CXX_I2C_Send_Byte(addr_low);
		AT24CXX_I2C_Read_ACK();
		for (j = 0; j < AT24CXX_PAGE_SIZE; j++)
		{
			AT24CXX_I2C_Send_Byte(dat);
			AT24CXX_I2C_Read_ACK();
		}
		AT24CXX_I2C_Send_Stop();
		#if (!AT24CXX_I2C_SET)
			EAXFR_DISABLE();
		#endif
		page++;
		DELAY_Set_Ms(AT24CXX_WR_TIME);			// (!important)
	}
}


/**
 * @brief     	AHT10写字节函数
 * @param[in] 	addr		写数据存放起始字节地址
 * @param[in] 	dat		字节数据
 * @return	void
**/
void AT24CXX_Write_Byte(uint16_t addr, uint8_t dat)
{
	uint8_t addr_low, addr_high;
	addr_low = (uint8_t)addr;
	addr_high = (uint8_t)(addr>>8);
	
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_WR_ADDR);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_high);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_low);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(dat);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
	DELAY_Set_Ms(AT24CXX_WR_TIME);				// (!important)
}


/**
 * @brief     	AHT10非完整页写函数(在写一个不完整的页时, 通过页写模式, 快速写多个数据)
 * @param[in] 	addr		写数据存放起始字节地址
 * @param[in] 	dat		写数据指针
 * @param[in] 	n		写数据个数
 * @return	void
**/
void AT24CXX_Write_NotFull_Page(uint16 addr, uint8_t *dat, uint8_t n)
{
	uint8_t addr_low, addr_high, rest;
	uint8_t i;
	rest = AT24CXX_PAGE_SIZE - addr % AT24CXX_PAGE_SIZE;
	addr_low = (uint8_t)addr;
	addr_high = (uint8_t)(addr>>8);
	if (n > rest)	n = rest;
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_WR_ADDR);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_high);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_low);
	AT24CXX_I2C_Read_ACK();
	for (i = 0; i < n; i++)
	{
		AT24CXX_I2C_Send_Byte(*dat++);
		AT24CXX_I2C_Read_ACK();
	}
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
	DELAY_Set_Ms(AT24CXX_WR_TIME);				// (!important)
}



/**
 * @brief     	AHT10页写函数(按页写, 一次写一整页)
 * @param[in] 	page		写数据存放起始页
 * @param[in] 	dat		写数据指针
 * @return	void
**/
void AT24CXX_Write_Page(uint16 page, uint8_t *dat)
{
	uint16_t addr;
	uint8_t addr_low, addr_high;
	uint8_t i;
	addr = page * AT24CXX_PAGE_SIZE;
	addr_low = (uint8_t)addr;
	addr_high = (uint8_t)(addr>>8);
	
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_WR_ADDR);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_high);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_low);
	AT24CXX_I2C_Read_ACK();
	for (i = 0; i < AT24CXX_PAGE_SIZE; i++)
	{
		AT24CXX_I2C_Send_Byte(*dat++);
		AT24CXX_I2C_Read_ACK();
	}
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
	DELAY_Set_Ms(AT24CXX_WR_TIME);				// (!important)
}


/**
 * @brief     	AHT10读数据函数(按地址, 连续读)
 * @param[in] 	addr		读数据存放起始字节地址
 * @param[in] 	dat		读数据指针
 * @param[in] 	n		读数据个数
 * @return	void
**/
void AT24CXX_Read_Data(uint16_t addr, uint8_t *dat, uint16_t n)
{
	if (!addr)	AT24CXX_Read_Byte(AT24CXX_SIZE - 1);	// 确定读地址(先读前一个地址, 不进行操作, 更新地址计数器)
	else		AT24CXX_Read_Byte(addr - 1);
	AT24CXX_Seq_Read_Bytes(dat, n);
}


/**
 * @brief     	AHT10立即读字节函数(读出当前地址中字节数据)
 * @param[in] 	void
 * @return	dat		读字节数据
**/
uint8_t AT24CXX_Read_Next_Byte(void)
{
	uint8_t dat = 0;
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_RD_ADDR);
	AT24CXX_I2C_Read_ACK();
	dat = AT24CXX_I2C_Read_Byte();
	AT24CXX_I2C_Send_NACK();
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
	return dat;
}



/**
 * @brief     	AHT10读字节函数
 * @param[in] 	addr		读字节数据地址
 * @param[in] 	dat		读字节数据
 * @return	void
**/
uint8_t AT24CXX_Read_Byte(uint16_t addr)
{
	uint8_t addr_low, addr_high;
	uint8_t dat = 0;
	addr_low = (uint8_t)addr;
	addr_high = (uint8_t)(addr>>8);
	
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_WR_ADDR);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_high);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Byte(addr_low);
	AT24CXX_I2C_Read_ACK();
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_RD_ADDR);
	AT24CXX_I2C_Read_ACK();
	dat =AT24CXX_I2C_Read_Byte();
	AT24CXX_I2C_Send_NACK();
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
	return dat;
}


/**
 * @brief     	AHT10连续读字节函数(从当前地址开始读)
 * @param[in] 	dat		读字节数据地址
 * @param[in] 	n		读字节数据个数
 * @return	void
**/
void AT24CXX_Seq_Read_Bytes(uint8_t *dat, uint16_t n)
{
	uint8_t i;
	#if (!AT24CXX_I2C_SET)
		EAXFR_ENABLE();
	#endif
	AT24CXX_I2C_Send_Start();
	AT24CXX_I2C_Send_Byte(AT24CXX_RD_ADDR);
	AT24CXX_I2C_Read_ACK();
	for(i = 0; i < n; i++)
	{
		*dat++ = AT24CXX_I2C_Read_Byte();
		if (i < n-1)	AT24CXX_I2C_Send_ACK();
		else	AT24CXX_I2C_Send_NACK();
	}
	AT24CXX_I2C_Send_Stop();
	#if (!AT24CXX_I2C_SET)
		EAXFR_DISABLE();
	#endif
}

/*-----------------------------------------------------------------------
|                   END OF FLIE.  (C) COPYRIGHT Walk-To-Me              |
-----------------------------------------------------------------------*/


