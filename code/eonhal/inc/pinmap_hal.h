/**
 ******************************************************************************
 * @file    pinmap_hal.h
 * @version V1.0.1
 * @date    2019
 * @brief   STM32 pin map
 ******************************************************************************
 */

#ifndef __PINMAP_HAL_H
#define __PINMAP_HAL_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32l0xx.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IRQ(__IRQ__) __IRQ__()

  // **** Types
  typedef uint8_t pin_t;
  typedef uint8_t mode_t;
  typedef uint8_t pull_t;
  typedef uint8_t speed_t;

  // **** Function that returns STM32 Pin Map
  typedef struct STM32_Pin_Info STM32_Pin_Info;
  STM32_Pin_Info *HAL_Pin_Map(void);

/***
 * Alternate Functions
 */
#define AF_0 ((uint8_t)0x00)
#define AF_1 ((uint8_t)0x01)
#define AF_2 ((uint8_t)0x02)
#define AF_3 ((uint8_t)0x03)
#define AF_4 ((uint8_t)0x04)
#define AF_5 ((uint8_t)0x05)
#define AF_6 ((uint8_t)0x06)
#define AF_7 ((uint8_t)0x07)
#define AF_NONE ((uint8_t)0xFF)

/***
 * Pin Definitions
 */
#if defined(STM32L031F6)
// **** Pins
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PA9 8
#define PA10 9
#define PA13 10
#define PA14 11

#define PB1 12

#define PC14 13
#define PC15 14

#endif

#if defined(STM32L031K6) || defined(STM32L031K4)
// **** Pins
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PA8 8
#define PA9 9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15

#define PB0 16
#define PB1 17
#define PB3 18
#define PB4 19
#define PB5 20
#define PB6 21
#define PB7 22

#define PC14 23
#define PC15 24

#endif

#if defined(STM32L031xx)
// **** Special Pins
#define AN_PA0 PA0
#define AN_PA1 PA1
#define AN_PA2 PA2
#define AN_PA3 PA3
#define AN_PA4 PA4
#define AN_PA5 PA5
#define AN_PA6 PA6
#define AN_PA7 PA7
#ifdef PB0
#define AN_PB0 PB0
#endif
#define AN_PB1 PB1

#define TIM2CH1_PA0 PA0
#define TIM2CH2_PA1 PA1
#define TIM2CH3_PA2 PA2
#define TIM2CH4_PA3 PA3
#define TIM2CH1_PA5 PA5
#define TIM22CH1_PA6 PA6
#define TIM22CH2_PA7 PA7
#ifdef PA8
#define TIM2CH1_PA8 PA8
#endif
#define TIM22CH1_PA9 PA9
#define TIM22CH2_PA10 PA10
#ifdef PA11
#define TIM21CH2_PA11 PA11
#endif
#ifdef PA15
#define TIM2CH1_PA15 PA15
#endif
#ifdef PB0
#define TIM2CH3_PB0 PB0
#endif
#define TIM2CH4_PB1 PB1
#ifdef PB3
#define TIM2CH2_PB3 PB3
#define TIM22CH1_PB4 PB4
#define TIM22CH2_PB5 PB5
#define TIM21CH1_PB6 PB6
#define TIM2CH3_PB10 PB10
#define TIM2CH4_PB11 PB11
#define TIM21CH1_PB13 PB13
#define TIM21CH2_PB14 PB14
#endif

#define CS1_PA4 PA4
#define SCK1_PA5 PA5
#define MISO1_PA6 PA6
#define MOSI1_PA7 PA7
#ifdef PA11
#define MISO1_PA11 PA11
#endif
#ifdef PA12
#define MOSI1_PA12 PA12
#endif
#ifdef PA15
#define CS1_PA15 PA15
#endif
#ifdef PB0
#define MISO1_PB0 PB0
#endif
#define MOSI1_PB1 PB1
#ifdef PB3
#define SCK1_PB3 PB3
#define MISO1_PB4 PB4
#define MOSI1_PB5 PB5
#define CS1_PB12 PB12
#define SCK1_PB13 PB13
#define MISO1_PB14 PB14
#define MOSI1_PB15 PB15
#endif

#define SCL1_PA9 PA9
#define SDA1_PA10 PA10
#ifdef PB6
#define SCL1_PB6 PB6
#define SDA1_PB7 PB7
#define SCL1_PB8 PB8
#define SDA1_PB9 PB9
#endif

#define TX2_PA2 PA2
#define RX2_PA3 PA3
#define TX2_PA9 PA9
#define RX2_PA10 PA10
#define TX2_PA14 PA14
#ifdef PA15
#define RX2_PA15 PA15
#endif
#ifdef PB6
#define TX2_PB6 PB6
#define RX2_PB7 PB7
#endif

#endif

#if defined(STM32L051C8)
// **** Pins
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PA8 8
#define PA9 9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15

#define PB0 16
#define PB1 17
#define PB2 18
#define PB3 19
#define PB4 20
#define PB5 21
#define PB6 22
#define PB7 23
#define PB8 24
#define PB9 25
#define PB10 26
#define PB11 27
#define PB12 28
#define PB13 29
#define PB14 30
#define PB15 31

#define PC13 32
#define PC14 33
#define PC15 34

#define PH0 35
#define PH1 36

#endif

#if defined(STM32L051xx)
// **** Special Pins
#ifdef PC0
#define AN_PC0 PC0
#endif
#ifdef PC1
#define AN_PC1 PC1
#endif
#ifdef PC2
#define AN_PC2 PC2
#endif
#ifdef PC3
#define AN_PC3 PC3
#endif
#define AN_PA0 PA0
#define AN_PA1 PA1
#define AN_PA2 PA2
#define AN_PA3 PA3
#define AN_PA4 PA4
#define AN_PA5 PA5
#define AN_PA6 PA6
#ifdef PC4
#define AN_PC4 PC4
#endif
#ifdef PC5
#define AN_PC5 PC5
#endif
#define AN_PB0 PB0
#define AN_PB1 PB1

#define TIM2CH1_PA0 PA0
#define TIM2CH2_PA1 PA1
#define TIM2CH3_PA2 PA2
#define TIM2CH4_PA3 PA3
#define TIM2CH1_PA5 PA5
#define TIM22CH1_PA6 PA6
#define TIM22CH2_PA7 PA7
#define TIM2CH1_PA15 PA15
#define TIM2CH2_PB3 PB3
#define TIM22CH1_PB4 PB4
#define TIM22CH2_PB5 PB5
#define TIM2CH3_PB10 PB10
#define TIM2CH4_PB11 PB11
#define TIM21CH1_PB13 PB13
#define TIM21CH2_PB14 PB14
#ifdef PC6
#define TIM22CH1_PC6 PC6
#endif
#ifdef PC7
#define TIM22CH2_PC7 PC7
#endif

#define CS1_PA4 PA4
#define SCK1_PA5 PA5
#define MISO1_PA6 PA6
#define MOSI1_PA7 PA7
#define MISO1_PA11 PA11
#define MOSI1_PA12 PA12
#define CS1_PA15 PA15
#define SCK1_PB3 PB3
#define MISO1_PB4 PB4
#define MOSI1_PB5 PB5
#define CS2_PB9 PB9
#define SCK2_PB10 PB10
#define CS2_PB12 PB12
#define SCK2_PB13 PB13
#define MISO2_PB14 PB14
#define MOSI2_PB15 PB15
#ifdef PC2
#define MISO2_PC2 PC2
#endif
#ifdef PC3
#define MOSI2_PC3 PC3
#endif

#define SCL1_PB6 PB6
#define SDA1_PB7 PB7
#define SCL1_PB8 PB8
#define SDA1_PB9 PB9
#define SCL2_PB10 PB10
#define SDA2_PB11 PB11
#define SCL2_PB13 PB13
#define SDA2_PB14 PB14

#define TX2_PA2 PA2
#define RX2_PA3 PA3
#define TX1_PA9 PA9
#define RX1_PA10 PA10
#define TX2_PA14 PA14
#define RX2_PA15 PA15
#define TX1_PB6 PB6
#define RX1_PB7 PB7

#endif

#if defined(CMWX1ZZABZ_091)
#define PA0 0
#define PA2 1
#define PA3 2
#define PA4 3
#define PA5 4
#define PA8 5
#define PA9 6
#define PA10 7
#define PA11 8
#define PA12 9
#define PA13 10
#define PA14 11

#define PB2 12
#define PB5 13
#define PB6 14
#define PB7 15
#define PB8 16
#define PB9 17
#define PB12 18
#define PB13 19
#define PB14 20
#define PB15 21

#define PH0 22
#define PH1 23

// Dedicated pins connected to SX1276 radio
#define RADIO_RST 24                 //PC0  -- this pin is not exposed
#define RADIO_MOSI 25                //PA7  -- this pin is not exposed
#define RADIO_MISO 26                //PA6  -- this pin is not exposed
#define RADIO_SCLK 27                //PB3  -- this pin is not exposed
#define RADIO_CS 28                  //PA15 -- this pin is not exposed
#define RADIO_DIO0 29                //PB4 -- exposed but it must not be a good idea manipulate this pin for other function
#define RADIO_DIO1 30                //PB1 -- exposed but it must not be a good idea manipulate this pin for other function
#define RADIO_DIO2 31                //PB0 -- exposed but it must not be a good idea manipulate this pin for other function
#define RADIO_DIO3 32                //PC13 -- exposed but it must not be a good idea manipulate this pin for other function
#define RADIO_ANT_SWITCH_RX 33       //CRF1/PA1 -- exposed but it must not be a good idea manipulate this pin for other function
#define RADIO_ANT_SWITCH_TX_RFO 34   //CRF2/PC2 -- exposed but it must not be a good idea manipulate this pin for other function
#define RADIO_ANT_SWITCH_TX_BOOST 35 //CRF3/PC1 -- exposed but it must not be a good idea manipulate this pin for other function

// Dedicated pins connected to STSAFE
#define STSAFE_SCL 36 // PB10 - not exposed, connected internally to STSAFE by I2C2
#define STSAFE_SDA 37 // PB11 - not exposed, connected internally to STSAFE by I2C2

/*
Notes: 
 - DIO4 and DIO5 pins on Murate module are lines directly from SX1276
 - PB10 (SCL) and PB11 (SDA) are connected internally to STSAFE by I2C2
 - PC14 and PC15 are connected internally to a 32khz oscillator
*/
#endif /* CMWX1ZZABZ_091 */

// **** Other Pin Definitions
#define NOPIN 255
#define UNUSED(__X__) ((void)(__X__))

#ifdef __cplusplus
}
#endif

#endif
