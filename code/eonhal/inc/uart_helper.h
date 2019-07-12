/**
  ******************************************************************************
  * @file    uart_helper.h 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   Uart Helper
  ******************************************************************************
*/

#ifndef __UART_HELPER_H_
#define __UART_HELPER_H_

#include <stdint.h>
#include "gpio.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_bus.h"

#define UART_IT_PE ((uint32_t)0x0028)   /*!< UART parity error interruption                 */
#define UART_IT_TXE ((uint32_t)0x0727)  /*!< UART transmit data register empty interruption */
#define UART_IT_TC ((uint32_t)0x0626)   /*!< UART transmission complete interruption        */
#define UART_IT_RXNE ((uint32_t)0x0525) /*!< UART read data register not empty interruption */
#define UART_IT_IDLE ((uint32_t)0x0424) /*!< UART idle interruption                         */
#define UART_IT_LBD ((uint32_t)0x0846)  /*!< UART LIN break detection interruption          */
#define UART_IT_CTS ((uint32_t)0x096A)  /*!< UART CTS interruption                          */
#define UART_IT_CM ((uint32_t)0x112E)   /*!< UART character match interruption              */
#define UART_IT_WUF ((uint32_t)0x1476)  /*!< UART wake-up from stop mode interruption       */

#define UART_GET_IT(__UARTX__, __IT__) ((__UARTX__)->ISR & ((uint32_t)1U << ((__IT__) >> 0x08U)))
#define UART_GET_FLAG(__UARTX__, __FLAG__) (((__UARTX__)->ISR & (__FLAG__)) == (__FLAG__))

#define UART_BUFFER_SIZE 64

typedef struct
{
  uint8_t buffer[UART_BUFFER_SIZE];
  volatile uint8_t head;
  volatile uint8_t tail;
} UARTRingBuff_t;

__STATIC_INLINE void uart_rb_insert(UARTRingBuff_t *rb, uint8_t b)
{
  uint8_t i = (uint8_t)(rb->head + 1) % UART_BUFFER_SIZE;
  if (i != rb->tail)
  {
    rb->buffer[rb->head] = b;
    rb->head = i;
  }
}

#endif