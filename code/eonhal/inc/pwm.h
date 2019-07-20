/**
  ******************************************************************************
  * @file    pwmo.h 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   PWM Library
  ******************************************************************************
*/

#ifndef __PWM_H
#define __PWM_H

#include "pinmap_hal.h"
#include "stm32l0xx_ll_tim.h"

/** 
 ===============================================================================
              ##### Public Functions #####
 ===============================================================================
 */

void pwm_init(pin_t pin, uint32_t prescaler, uint32_t period);
void pwm_init1KHz(pin_t pin);
void pwm_init500Hz(pin_t pin);
void pwm_init4MHz(pin_t pin);
void pwm_write(pin_t pin, uint16_t val);

#endif
