/**
  ******************************************************************************
  * @file    system_eeprom.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   System EEPROM Functions
  ******************************************************************************
*/

#include "System.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

// FLASH Timeout
#define FLASH_TIMEOUT_VALUE (50000U) // 50 s

// FLASH Keys
#define FLASH_PEKEY1 ((uint32_t)0x89ABCDEFU) /*!< Flash program erase key1 */
#define FLASH_PEKEY2 ((uint32_t)0x02030405U) /*!< Flash program erase key: used with FLASH_PEKEY2          \ \ \ \ \ \ \ \
																								 to unlock the write access to the FLASH_PECR register and \ \ \ \ \ \ \ \
																								 data EEPROM */

#define IS_FLASH_EEPROM_ADDRESS(__ADDRESS__) (((__ADDRESS__) >= DATA_EEPROM_BASE) && ((__ADDRESS__) <= DATA_EEPROM_END))

/** 
 ===============================================================================
              ##### Private functions #####
 ===============================================================================
 */

static uint8_t System_EE_waitForLastOperation(uint32_t timeout)
{
	uint32_t exit_time = millis() + timeout;
	while ((FLASH->SR & FLASH_SR_BSY) != 0)
	{ //  Wait till no operation is on going
		if (millis() > exit_time)
			return 0; // Timeout
	}
	if ((FLASH->SR & FLASH_SR_EOP) != 0)
	{
		FLASH->SR = FLASH_SR_EOP;
	}
	if (((FLASH->SR & FLASH_SR_WRPERR) != 0) ||
			((FLASH->SR & FLASH_SR_PGAERR) != 0) ||
			((FLASH->SR & FLASH_SR_SIZERR) != 0) ||
			((FLASH->SR & FLASH_SR_OPTVERR) != 0) ||
			((FLASH->SR & FLASH_SR_RDERR) != 0) ||
			((FLASH->SR & FLASH_SR_FWWERR) != 0) ||
			((FLASH->SR & FLASH_SR_NOTZEROERR) != 0))
	{
		FLASH->SR = (FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_OPTVERR |
								 FLASH_SR_RDERR | FLASH_SR_FWWERR | FLASH_SR_NOTZEROERR);

		return 0; // Error
	}
	return 1;
}

/** 
 ===============================================================================
              ##### Public functions #####
 ===============================================================================
 */

void eeprom_unlock(void)
{
	if ((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
	{
		// Unlocking the Data memory and FLASH_PECR register access
		FLASH->PEKEYR = FLASH_PEKEY1;
		FLASH->PEKEYR = FLASH_PEKEY2;
	}
}

void eeprom_lock(void)
{
	// Set the PELOCK Bit to lock the data memory and FLASH_PECR register access
	SET_BIT(FLASH->PECR, FLASH_PECR_PELOCK);
}

uint8_t eeprom_writeByte(uint32_t address, uint8_t data)
{
	uint8_t status;
	address = DATA_EEPROM_BASE + address;
	if (address > DATA_EEPROM_END)
		return 0;
	status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	if (status == 1)
	{
		*(__IO uint8_t *)address = (uint8_t)data; // Program byte (8-bit) at a specified address.
		status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	}
	return status;
}

uint8_t eeprom_writeHalfWord(uint32_t address, uint16_t data)
{
	uint8_t status;
	address = DATA_EEPROM_BASE + address;
	if (address > DATA_EEPROM_END)
		return 0;
	status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	if (status == 1)
	{
		*(__IO uint16_t *)address = (uint16_t)data; // Program halfword (16-bit) at a specified address.
		status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	}
	return status;
}

uint8_t eeprom_writeWord(uint32_t address, uint32_t data)
{
	uint8_t status;
	address = DATA_EEPROM_BASE + address;
	if (address > DATA_EEPROM_END)
		return 0;
	status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	if (status == 1)
	{
		*(__IO uint32_t *)address = (uint32_t)data;
		; // Program word (32-bit) at a specified address.
		status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	}
	return status;
}

uint8_t eeprom_writeFloat(uint32_t address, float *data)
{
	uint8_t status;
	uint32_t *p = (uint32_t *)data;
	address = DATA_EEPROM_BASE + address;
	if (address > DATA_EEPROM_END)
		return 0;
	status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	if (status == 1)
	{
		*(__IO uint32_t *)address = (uint32_t)*p;
		; // Program word (32-bit) at a specified address.
		status = System_EE_waitForLastOperation(FLASH_TIMEOUT_VALUE);
	}
	return status;
}

uint8_t eeprom_readByte(uint32_t address)
{
	uint8_t r;
	address = DATA_EEPROM_BASE + address;
	r = *(__IO uint8_t *)address;
	return r;
}

uint16_t eeprom_readHalfWord(uint32_t address)
{
	uint16_t r;
	address = DATA_EEPROM_BASE + address;
	r = *(__IO uint16_t *)address;
	return r;
}

uint32_t eeprom_readWord(uint32_t address)
{
	uint32_t r;
	address = DATA_EEPROM_BASE + address;
	r = *(__IO uint32_t *)address;
	return r;
}

void eeprom_readFloat(uint32_t address, float *rdata)
{
	uint32_t *int_data = (uint32_t *)rdata;
	address = DATA_EEPROM_BASE + address;
	(*int_data) = *(__IO uint32_t *)address;
}
