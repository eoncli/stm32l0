
#include <stdint.h>
#include "lprint.h"

#if defined(__CC_ARM)
__weak void LPUTC(char c)
{
}
#elif defined(__GNUC__)
void LPUTC(char c) __attribute__((weak));
#endif

__STATIC_INLINE void _lprintIntBase(int64_t n, uint8_t base)
{
  unsigned char buf[10];
  uint16_t i = 0;
  if (n == 0)
  {
    LPUTC('0');
  }

  if (n < 0)
  {
    LPUTC('-');
    n = -n;
  }

  while (n > 0)
  {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
  {
    LPUTC((char)(buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
  }
}

__STATIC_INLINE void _lprintNum(int64_t n, uint8_t isfloat){
  uint32_t int_part;
	uint8_t remainder;

	// Handle negative numbers
	if (n < 0)
	{
		LPUTC('-');
		n = -n;
	}

	if (!isfloat)
	{
		lprintIntBase(n, 10);
		return;
	}

	remainder = n % 100;
	int_part = (uint32_t)((n - remainder) / 100);
	lprintIntBase(int_part, 10);
	LPUTC('.');
	lprintIntBase(remainder, 10);
}



void lprint(const char *format, ...)
{
  unsigned char buf[3] = {0, 0, 0};
  uint8_t startArg = 0;
  va_list va;
  va_start(va, format);

  while (*format)
  {

    if ((*format != '{') && (startArg == 0))
    {
      LPUTC(*format);
      format++;
      continue;
    }
    else if (startArg == 0)
    {
      startArg = 1;
    }

    if (startArg > 0)
    {
      buf[startArg - 1] = *format;
      startArg++;
      if (startArg == 4)
      {
        if (buf[2] == '}')
        {
          const int w = va_arg(va, int);
          switch (buf[1])
          {
          case 'x':
            LPUTC('0');
            LPUTC('x');
            lprintIntBase(w, 16);
            break;
          case 'd':
            lprintNum(w, 0);
            break;
          case 'f':
            lprintNum(w, 1);
            break;
          }
        }
        else
        {
          LPUTC(buf[0]);
          LPUTC(buf[1]);
          LPUTC(buf[2]);
        }
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 0;
        startArg = 0;
      }
    }

    format++;
  }

  va_end(va);
}