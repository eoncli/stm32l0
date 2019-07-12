#include <stdint.h>
#include "gpio.h"

typedef struct
{
  void (*init)(uint32_t baudrate, pin_t tx, pin_t rx);
  void (*off)(void);
  void (*write)(unsigned char c);
  uint8_t (*available)(void);
  int (*read)(void);
  int (*peek)(void);
  void (*readUntil)(char buffer[], uint8_t terminator);
  void (*print)(const char *s);
  void (*printArray)(char s[]);
  void (*println)(const char *s);
  void (*printNum)(unsigned long long n);
  void (*printNumBase)(unsigned long long n, uint8_t base);
  void (*printFloat)(double n, uint8_t decimals);
  void (*printlnNumber)(unsigned long long n);
  void (*printlnNumBase)(unsigned long long n, uint8_t base);
  void (*printlnFloat)(double n, uint8_t decimals);
} ObjUart_t;