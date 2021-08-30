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
 * @brief     	AHT10��ʼ������, ����д��ַ������Ϊ0(�ɲ���ʼ��)
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
 * @brief     	AHT10д���ݺ���(�����С, �ٶ��Ż�)
 * @param[in] 	addr		д���ݴ����ʼ�ֽڵ�ַ
 * @param[in] 	dat		д����ָ��
 * @param[in] 	n		д���ݸ���
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
 * @brief     	AHT10����ֽں���
 * @param[in] 	page		������ݴ����ʼҳ
 * @param[in] 	dat		�������
 * @param[in] 	n		�������ҳ��
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
 * @brief     	AHT10д�ֽں���
 * @param[in] 	addr		д���ݴ����ʼ�ֽڵ�ַ
 * @param[in] 	dat		�ֽ�����
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
 * @brief     	AHT10������ҳд����(��дһ����������ҳʱ, ͨ��ҳдģʽ, ����д�������)
 * @param[in] 	addr		д���ݴ����ʼ�ֽڵ�ַ
 * @param[in] 	dat		д����ָ��
 * @param[in] 	n		д���ݸ���
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
 * @brief     	AHT10ҳд����(��ҳд, һ��дһ��ҳ)
 * @param[in] 	page		д���ݴ����ʼҳ
 * @param[in] 	dat		д����ָ��
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
 * @brief     	AHT10�����ݺ���(����ַ, ������)
 * @param[in] 	addr		�����ݴ����ʼ�ֽڵ�ַ
 * @param[in] 	dat		������ָ��
 * @param[in] 	n		�����ݸ���
 * @return	void
**/
void AT24CXX_Read_Data(uint16_t addr, uint8_t *dat, uint16_t n)
{
	if (!addr)	AT24CXX_Read_Byte(AT24CXX_SIZE - 1);	// ȷ������ַ(�ȶ�ǰһ����ַ, �����в���, ���µ�ַ������)
	else		AT24CXX_Read_Byte(addr - 1);
	AT24CXX_Seq_Read_Bytes(dat, n);
}


/**
 * @brief     	AHT10�������ֽں���(������ǰ��ַ���ֽ�����)
 * @param[in] 	void
 * @return	dat		���ֽ�����
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
 * @brief     	AHT10���ֽں���
 * @param[in] 	addr		���ֽ����ݵ�ַ
 * @param[in] 	dat		���ֽ�����
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
 * @brief     	AHT10�������ֽں���(�ӵ�ǰ��ַ��ʼ��)
 * @param[in] 	dat		���ֽ����ݵ�ַ
 * @param[in] 	n		���ֽ����ݸ���
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


