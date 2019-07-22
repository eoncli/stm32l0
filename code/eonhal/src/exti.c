/**
  ******************************************************************************
  * @file    exti.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   External Interrupts Functions
  ******************************************************************************
*/

#include "exti.h"
#include "gpio.h"
#include "stm32l0xx_ll_exti.h"
#include "pinmap_impl.h"

#if !defined(GPIO_GET_INDEX)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA)) ? 0U : ((__GPIOx__) == (GPIOB)) ? 1U : ((__GPIOx__) == (GPIOC)) ? 2U : ((__GPIOx__) == (GPIOH)) ? 5U : 6U)
#endif

#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

/** 
 ===============================================================================
              ##### Helper array #####
 ===============================================================================
 */

static const uint8_t GPIO_IRQn[] = {
		EXTI0_1_IRQn,	//0
		EXTI0_1_IRQn,	//1
		EXTI2_3_IRQn,	//2
		EXTI2_3_IRQn,	//3
		EXTI4_15_IRQn, //4
};

/** 
 ===============================================================================
              ##### Private functions #####
 ===============================================================================
 */

static uint8_t GPIO_EXTIConfig(pin_t pin, uint8_t exti_mode, pull_t pull)
{
	uint32_t position = 0x00;
	uint32_t iocurrent = 0x00;
	uint32_t temp = 0x00;
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

#ifdef GPIOA
	if (pin_map[pin].GPIOx == GPIOA)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
	}
#endif
#ifdef GPIOB
	if (pin_map[pin].GPIOx == GPIOB)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN);
	}
#endif
#ifdef GPIOC
	if (pin_map[pin].GPIOx == GPIOC)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOCEN);
	}
#endif
#ifdef GPIOD
	if (pin_map[pin].GPIOx == GPIOD)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIODEN);
	}
#endif
#ifdef GPIOH
	if (pin_map[pin].GPIOx == GPIOH)
	{
		SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOHEN);
	}
#endif

	LL_GPIO_SetPinMode(pin_map[pin].GPIOx, pin_map[pin].pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(pin_map[pin].GPIOx, pin_map[pin].pin, pull);

	// SYSCFG Config
	while ((pin_map[pin].pin) >> position)
	{
		iocurrent = (pin_map[pin].pin) & (1 << position);
		if (iocurrent)
		{
			break;
		}
		position++;
	}

	temp = SYSCFG->EXTICR[position >> 2];
	temp &= ~(((uint32_t)0x0F) << (4 * (position & 0x03)));
	temp |= ((uint32_t)(GPIO_GET_INDEX(pin_map[pin].GPIOx)) << (4 * (position & 0x03)));
	SYSCFG->EXTICR[position >> 2] = temp;

	LL_EXTI_DisableEvent_0_31(pin_map[pin].pin);
	LL_EXTI_EnableIT_0_31(pin_map[pin].pin);

	LL_EXTI_DisableFallingTrig_0_31(pin_map[pin].pin);
	LL_EXTI_DisableRisingTrig_0_31(pin_map[pin].pin);

	if (exti_mode == RISING || exti_mode == CHANGE)
	{
		LL_EXTI_EnableRisingTrig_0_31(pin_map[pin].pin);
	}
	else if (exti_mode == FALLING || exti_mode == CHANGE)
	{
		LL_EXTI_EnableFallingTrig_0_31(pin_map[pin].pin);
	}

	if (position > 4)
		position = 4;
	return position;
}

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

void exti_attach(pin_t pin, pull_t pull, uint8_t exti_mode)
{
	uint8_t gpio_irqn = 0;

	gpio_irqn = GPIO_EXTIConfig(pin, exti_mode, pull);

	NVIC_SetPriority((IRQn_Type)GPIO_IRQn[gpio_irqn], 0);
	NVIC_EnableIRQ((IRQn_Type)GPIO_IRQn[gpio_irqn]);
}

void exti_attachPriority(pin_t pin, pull_t pull, uint8_t priority, uint8_t exti_mode)
{
	uint8_t gpio_irqn = 0;

	gpio_irqn = GPIO_EXTIConfig(pin, exti_mode, pull);

	NVIC_SetPriority((IRQn_Type)GPIO_IRQn[gpio_irqn], priority);
	NVIC_EnableIRQ((IRQn_Type)GPIO_IRQn[gpio_irqn]);
}

void exti_detach(uint16_t pin)
{
	uint32_t position = 0x00;
	uint32_t iocurrent = 0x00;
	uint32_t tmp = 0x00;
	STM32_Pin_Info *pin_map = HAL_Pin_Map();

	/* Configure the port pins */
	while ((pin_map[pin].pin) >> position)
	{
		/* Get the IO position */
		iocurrent = (pin_map[pin].pin) & (1 << position);

		if (iocurrent)
		{
			/*------------------------- GPIO Mode Configuration --------------------*/
			/* Configure IO Direction in Input Floting Mode */
			pin_map[pin].GPIOx->MODER &= ~(GPIO_MODER_MODE0 << (position * 2));

			/* Configure the default Alternate Function in current IO */
			pin_map[pin].GPIOx->AFR[position >> 3] &= ~((uint32_t)0xF << ((uint32_t)(position & (uint32_t)0x07) * 4));

			/* Configure the default value for IO Speed */
			pin_map[pin].GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEED0 << (position * 2));

			/* Configure the default value IO Output Type */
			pin_map[pin].GPIOx->OTYPER &= ~(GPIO_OTYPER_OT_0 << position);

			/* Deactivate the Pull-up oand Pull-down resistor for the current IO */
			pin_map[pin].GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (position * 2));

			/*------------------------- EXTI Mode Configuration --------------------*/
			/* Clear the External Interrupt or Event for the current IO */

			tmp = SYSCFG->EXTICR[position >> 2];
			tmp &= (((uint32_t)0x0F) << (4 * (position & 0x03)));
			if (tmp == (GPIO_GET_INDEX(pin_map[pin].GPIOx) << (4 * (position & 0x03))))
			{
				tmp = ((uint32_t)0x0F) << (4 * (position & 0x03));
				SYSCFG->EXTICR[position >> 2] &= ~tmp;

				/* Clear EXTI line configuration */
				EXTI->IMR &= ~((uint32_t)iocurrent);
				EXTI->EMR &= ~((uint32_t)iocurrent);

				/* Clear Rising Falling edge configuration */
				EXTI->RTSR &= ~((uint32_t)iocurrent);
				EXTI->FTSR &= ~((uint32_t)iocurrent);
			}
		}
		position++;
	}
}

void exti_softTrigger(pin_t pin)
{
	STM32_Pin_Info *pin_map = HAL_Pin_Map();
	EXTI->SWIER |= (pin_map[pin].pin);
}

/** 
 ===============================================================================
   ##### Weak functions #####
 ===============================================================================
 */

#if defined(__CC_ARM)
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
__weak void __EXTI0(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
__weak void __EXTI1(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
__weak void __EXTI2(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
__weak void __EXTI3(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
__weak void __EXTI4(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
__weak void __EXTI5(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
__weak void __EXTI6(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
__weak void __EXTI7(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
__weak void __EXTI8(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
__weak void __EXTI9(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
__weak void __EXTI10(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
__weak void __EXTI11(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
__weak void __EXTI12(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
__weak void __EXTI13(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
__weak void __EXTI14(void)
{
	/* Weak Function: No Modificar */
}
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
__weak void __EXTI15(void)
{
	/* Weak Function: No Modificar */
}
#endif
#elif defined(__GNUC__)
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
void __EXTI0(void) __attribute__((weak));
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
void __EXTI1(void) __attribute__((weak));
#endif
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
void __EXTI2(void) __attribute__((weak));
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
void __EXTI3(void) __attribute__((weak));
#endif
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
void __EXTI4(void) __attribute__((weak));
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
void __EXTI5(void) __attribute__((weak));
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
void __EXTI6(void) __attribute__((weak));
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
void __EXTI7(void) __attribute__((weak));
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
void __EXTI8(void) __attribute__((weak));
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
void __EXTI9(void) __attribute__((weak));
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
void __EXTI10(void) __attribute__((weak));
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
void __EXTI11(void) __attribute__((weak));
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
void __EXTI12(void) __attribute__((weak));
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
void __EXTI13(void) __attribute__((weak));
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
void __EXTI14(void) __attribute__((weak));
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
void __EXTI15(void) __attribute__((weak));
#endif
#endif

/** 
 ===============================================================================
              ##### Interruptions #####
 ===============================================================================
 */

#if defined(USE_EXTI0) || defined(USE_EXTI1) || defined(USE_ALL_EXTI)
void EXTI0_1_IRQHandler(void)
{
#if defined(USE_EXTI0) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_0) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_0); // Clear the EXTI line 0 pending bit
		__EXTI0();
	}
#endif
#if defined(USE_EXTI1) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_1) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_1); // Clear the EXTI line 1 pending bit
		__EXTI1();
	}
#endif
}
#endif

#if defined(USE_EXTI2) || defined(USE_EXTI3) || defined(USE_ALL_EXTI)
void EXTI2_3_IRQHandler(void)
{
#if defined(USE_EXTI2) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_2) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_2); // Clear the EXTI line 2 pending bit
		__EXTI2();
	}
#endif
#if defined(USE_EXTI3) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_3) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_3); // Clear the EXTI line 3 pending bit
		__EXTI3();
	}
#endif
}
#endif

#if defined(USE_EXTI4) || defined(USE_EXTI5) || defined(USE_EXTI6) || defined(USE_EXTI7) || defined(USE_EXTI8) || defined(USE_EXTI9) || defined(USE_EXTI10) || defined(USE_EXTI11) || defined(USE_EXTI12) || defined(USE_EXTI13) || defined(USE_EXTI14) || defined(USE_EXTI15) || defined(USE_ALL_EXTI)
void EXTI4_15_IRQHandler(void)
{
#if defined(USE_EXTI4) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_4) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_4); // Clear the EXTI line 4 pending bit
		__EXTI4();
	}
#endif
#if defined(USE_EXTI5) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_5) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_5); // Clear the EXTI line 5 pending bit
		__EXTI5();
	}
#endif
#if defined(USE_EXTI6) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_6) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_6); // Clear the EXTI line 6 pending bit
		__EXTI6();
	}
#endif
#if defined(USE_EXTI7) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_7) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_7); // Clear the EXTI line 7 pending bit
		__EXTI7();
	}
#endif
#if defined(USE_EXTI8) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_8) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_8); // Clear the EXTI line 8 pending bit
		__EXTI8();
	}
#endif
#if defined(USE_EXTI9) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_9) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_9); // Clear the EXTI line 9 pending bit
		__EXTI9();
	}
#endif
#if defined(USE_EXTI10) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_10) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_10); // Clear the EXTI line 10 pending bit
		__EXTI10();
	}
#endif
#if defined(USE_EXTI11) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_11) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_11); // Clear the EXTI line 11 pending bit
		__EXTI11();
	}
#endif
#if defined(USE_EXTI12) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_12) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_12); // Clear the EXTI line 12 pending bit
		__EXTI12();
	}
#endif
#if defined(USE_EXTI13) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_13) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_13); // Clear the EXTI line 13 pending bit
		__EXTI13();
	}
#endif
#if defined(USE_EXTI14) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_14) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_14); // Clear the EXTI line 14 pending bit
		__EXTI14();
	}
#endif
#if defined(USE_EXTI15) || defined(USE_ALL_EXTI)
	if (__HAL_GPIO_EXTI_GET_IT(LL_GPIO_PIN_15) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(LL_GPIO_PIN_15); // Clear the EXTI line 15 pending bit
		__EXTI15();
	}
#endif
}
#endif
