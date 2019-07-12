/**
  ******************************************************************************
  * @file    uart1.h 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   Header de UART Library
  ******************************************************************************
*/

#ifndef __UART1_H
#define __UART1_H

#include <stdint.h>
#include <stdbool.h>
#include "eon_string.h"
#include "pinmap_hal.h"

#if (defined(USART1) || defined(UART1))

/**
 * @brief Initiatize the UART
 * 
 * @param {baudrate}  Usually 9600
 * @param {tx}  TX1_Pin 
 * @param {rx}  RX1_Pin
 */
void uart1_init(uint32_t baudrate, pin_t tx, pin_t rx);

/**
 * @brief Turn off the UART
 * 
 */
void uart1_off(void);

/**
 * @brief Write a character
 * 
 * @param {c} Character to be written
 */
void uart1_write(unsigned char c);

/**
 * @brief Verify is there any character to be read
 * 
 * @return {uint8_t} Total bytes ready to be read
 */
uint8_t uart1_available(void);

/**
 * @brief Read a character
 * 
 * @return {int} Character read (-1) if fails
 */
int uart1_read(void);

/**
 * @brief Peek function
 * 
 * @return {int} Character (-1) if fails
 */
int uart1_peek(void);

/**
 * @brief Read until a terminator
 * 
 * @param {buffer} Buffer to be filled
 * @param {terminator} Terminator character
 */
void uart1_readUntil(char buffer[], uint8_t terminator);

/**
 * @brief Print text
 * 
 * @param {s} Message to print 
 */
void uart1_print(const char *s);

/**
 * @brief Print an array of characters
 * 
 * @param {s} Array of characters 
 */
void uart1_printArray(char s[]);

/**
 * @brief Print text and append a new line at the end
 * 
 * @param {s} Message to print 
 */
void uart1_println(const char *s);

/**
 * @brief Print an integer
 * 
 * @param {n} Integer 
 */
void uart1_printNum(unsigned long long n);

/**
 * @brief Print an integer specifiying the desired base
 * 
 * @param {n} Integer 
 * @param {base} Base
 */
void uart1_printNumBase(unsigned long long n, uint8_t base);

/**
 * @brief Print a float
 * 
 * @param {n} Float number
 * @param {decimals} Number of digits you want for decimal part
 */
void uart1_printFloat(double n, uint8_t decimals);

/**
 * @brief Print an integer and append a new line at the end
 * 
 * @param {n} Integer 
 */
void uart1_printlnNum(unsigned long long n);

/**
 * @brief Print an integer specifiying the desired base and append a new line at the end
 * 
 * @param {n} Integer 
 * @param {base} Base
 */
void uart1_printlnNumBase(unsigned long long n, uint8_t base);

/**
 * @brief Print a float and append a new line at the end
 * 
 * @param {n} Float number
 * @param {decimals} Number of digits you want for decimal part
 */
void uart1_printlnFloat(double n, uint8_t decimals);

#ifdef ENABLE_CLASSES
#include "oop_defs.h"
extern const ObjUart_t Serial1;
#endif

#endif

#endif
