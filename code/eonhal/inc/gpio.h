/**
  ******************************************************************************
  * @file    gpio.h 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   Header de GPIO Library
  ******************************************************************************
*/

#ifndef __GPIO_H
#define __GPIO_H

/** 
 ===============================================================================
              ##### DEPENDENCIAS #####
 ===============================================================================
 */

#include "stm32l0xx.h"
#include "stm32l0xx_ll_gpio.h"
#include "pinmap_hal.h"

/** 
 ===============================================================================
              ##### Definitions #####
 ===============================================================================
 */

// **** States
#define HIGH 1
#define LOW 0

// **** Modes
#define INPUT ((uint8_t)0x00)     /*!< Input Floating Mode*/
#define OUTPUT_PP ((uint8_t)0x01) /*!< Output Push Pull Mode*/
#define OUTPUT_OD ((uint8_t)0x11) /*!< Output Open Drain Mode*/
#define AF_PP ((uint8_t)0x02)     /*!< Alternate Function Push Pull Mode*/
#define AF_OD ((uint8_t)0x12)     /*!< Alternate Function Open Drain Mode*/
#define ANALOG ((uint8_t)0x03)    /*!< Analog Mode*/

// **** Speed
#define SPEED_LOW ((uint8_t)0x00)       /*!< range up to 0.4 MHz */
#define SPEED_MEDIUM ((uint8_t)0x01)    /*!< range 0.4 MHz to 2 MHz */
#define SPEED_HIGH ((uint8_t)0x02)      /*!< range   2 MHz to 10 MHz */
#define SPEED_VERY_HIGH ((uint8_t)0x03) /*!< range  10 MHz to 35 MHz */

// **** Pull
#define NOPULL ((uint8_t)0x00)   /*!< No Pull-up or Pull-down activation  */
#define PULLUP ((uint8_t)0x01)   /*!< Pull-up activation                  */
#define PULLDOWN ((uint8_t)0x02) /*!< Pull-down activation                */

/** 
 ===============================================================================
              ##### Public Functions #####
 ===============================================================================
 */

void gpio_mode(pin_t pin, mode_t mode, pull_t pull, speed_t speed);
void gpio_modeAF(pin_t pin, mode_t AF_PP_OD, pull_t pull, uint8_t AF_x);
void gpio_setAFSpeed(speed_t speed);
void gpio_write(pin_t pin, uint8_t state);
void gpio_toggle(pin_t pin);
uint8_t gpio_read(pin_t pin);

#include "pinmap_impl.h"
__STATIC_INLINE void gpio_set(pin_t pin)
{
  STM32_Pin_Info *pin_map = HAL_Pin_Map();
  LL_GPIO_SetOutputPin(pin_map[pin].GPIOx, pin_map[pin].pin);
}
__STATIC_INLINE void gpio_reset(pin_t pin)
{
  STM32_Pin_Info *pin_map = HAL_Pin_Map();
  LL_GPIO_ResetOutputPin(pin_map[pin].GPIOx, pin_map[pin].pin);
}

#endif
