/**
  ******************************************************************************
  * @file    uart1.c 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   UART Functions
  ******************************************************************************
*/

#include "uart1.h"
#include "uart_helper.h"

#if (defined(USART1) || defined(UART1))
/** 
 ===============================================================================
              ##### Global Static Variables #####
 ===============================================================================
 */

static UARTRingBuff_t urb;

/** 
 ===============================================================================
              ##### Interrupt #####
 ===============================================================================
 */

void USART1_IRQHandler(void)
{
	uint8_t ch = 0;
	if (UART_GET_IT(USART1, UART_IT_RXNE) != 0)
	{
		ch = (uint8_t)LL_USART_ReceiveData8(USART1);
		uart_rb_insert(&urb, ch);
	}
}

/** 
 ===============================================================================
              ##### Initialization Functions #####
 ===============================================================================
 */

void uart1_init(uint32_t baudrate, pin_t tx, pin_t rx)
{

	LL_USART_InitTypeDef USART_InitStruct;
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	gpio_modeUART(tx);
	gpio_modeUART(rx);

	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);

	USART_InitStruct.BaudRate = baudrate;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_8;
	LL_USART_Init(USART1, &USART_InitStruct);

	LL_USART_ConfigAsyncMode(USART1);

	LL_USART_Enable(USART1);

	LL_USART_EnableIT_RXNE(USART1);
}

void uart1_off(void)
{
	LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
	NVIC_DisableIRQ(USART1_IRQn);
	LL_USART_Disable(USART1);
	LL_USART_DisableIT_RXNE(USART1);
}

/** 
 ===============================================================================
              ##### Write Functions #####
 ===============================================================================
 */

void uart1_write(unsigned char c)
{
	while (UART_GET_FLAG(USART1, LL_USART_ISR_TXE) == 0)
		;
	LL_USART_TransmitData8(USART1, c);
}

/** 
 ===============================================================================
              ##### Print Functions #####
 ===============================================================================
 */

void uart1_print(const char *s)
{
	unsigned char i = 0;
	while (s[i] != '\0')
	{
		uart1_write(s[i++]);
	}
}

void uart1_printArray(char s[])
{
	unsigned char i = 0;
	while (s[i] != '\0')
	{
		uart1_write(s[i++]);
	}
}

void uart1_println(const char *s)
{
	uart1_print(s);
	uart1_write('\r');
	uart1_write('\n');
}

void uart1_printNum(unsigned long long n)
{
	unsigned char buf[10];
	unsigned long i = 0;
	if (n == 0)
	{
		uart1_write('0');
	}

	while (n > 0)
	{
		buf[i++] = n % 10;
		n /= 10;
	}

	for (; i > 0; i--)
	{
		uart1_write((char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
	}
}

void uart1_printNumBase(unsigned long long n, uint8_t base)
{
	unsigned char buf[10];
	unsigned long i = 0;
	if (n == 0)
	{
		uart1_write('0');
	}

	while (n > 0)
	{
		buf[i++] = n % base;
		n /= base;
	}

	for (; i > 0; i--)
	{
		uart1_write((char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
	}
}

void uart1_printFloat(double n, uint8_t decimals)
{
	int to_print;
	long long int_part;
	double remainder;
	double rounding = 0.5;
	uint8_t i = 0;

	// Handle negative numbers
	if (n < 0.0)
	{
		uart1_write('-');
		n = -n;
	}

	// Simplistic rounding strategy so that e.g. print(1.999, 2)
	// prints as "2.00"
	for (i = 0; i < decimals; i++)
	{
		rounding /= 10.0;
	}
	n += rounding;

	// Extract the integer part of the number and print it
	int_part = (long long)n;
	remainder = n - int_part;
	uart1_printNum(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (decimals > 0)
	{
		uart1_write('.');
	}

	// Extract digits from the remainder one at a time
	while (decimals-- > 0)
	{
		remainder *= 10.0;
		to_print = (int)remainder;
		uart1_printNum(to_print);
		remainder -= to_print;
	}
}

void uart1_printlnNum(unsigned long long n)
{
	uart1_printNum(n);
	uart1_write('\r');
	uart1_write('\n');
}

void uart1_printlnNumBase(unsigned long long n, uint8_t base)
{
	uart1_printNumBase(n, base);
	uart1_write('\r');
	uart1_write('\n');
}

void uart1_printlnFloat(double n, uint8_t decimals)
{
	uart1_printFloat(n, decimals);
	uart1_write('\r');
	uart1_write('\n');
}

/** 
 ===============================================================================
              ##### Read Functions #####
 ===============================================================================
 */

uint8_t uart1_available(void)
{
	return ((uint8_t)(UART_BUFFER_SIZE + urb.head - urb.tail)) % UART_BUFFER_SIZE;
}

int uart1_read(void)
{
	if (urb.head == urb.tail)
	{
		return -1;
	}
	else
	{
		uint8_t c = urb.buffer[urb.tail];
		urb.tail = (uint8_t)(urb.tail + 1) % UART_BUFFER_SIZE;
		return c;
	}
}

int uart1_peek(void)
{
	if (urb.head == urb.tail)
		return -1;
	else
		return urb.buffer[urb.tail];
}

void uart1_readUntil(char buffer[], uint8_t terminator)
{
	uint8_t i = 0;
	char c = 0;
	if (uart1_available())
		c = uart1_read();
	else
		return;
	while (c != terminator)
	{
		if (uart1_available() > 0)
		{
			buffer[i] = c;
			c = uart1_read();
			i++;
		}
	}
}

#ifdef ENABLE_CLASSES
const ObjUart_t Serial1 = {
		uart1_init,
		uart1_off,
		uart1_write,
		uart1_available,
		uart1_read,
		uart1_peek,
		uart1_readUntil,
		uart1_print,
		uart1_printArray,
		uart1_println,
		uart1_printNum,
		uart1_printNumBase,
		uart1_printFloat,
		uart1_printlnNum,
		uart1_printlnNumBase,
		uart1_printlnFloat,
}
#endif

#endif
