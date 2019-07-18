/**
  ******************************************************************************
  * @file    tim.c 
  * @authors Pablo Fuentes, Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   Timer Functions
  ******************************************************************************
*/

#include "tim.h"
#include "stm32l0xx_ll_rcc.h"

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

uint8_t tim_clkEnableAndGetIRQn(TIM_TypeDef *TIMx)
{

#if defined(TIM2)
	if (TIMx == TIM2)
	{
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
		return TIM2_IRQn;
	}
#endif

#if defined(TIM21)
	if (TIMx == TIM21)
	{
		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM21EN);
		return TIM21_IRQn;
	}
#endif

#if defined(TIM22)
	if (TIMx == TIM22)
	{
		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM22EN);
		return TIM22_IRQn;
	}
#endif

#if defined(TIM6)
	else if (TIMx == TIM6)
	{
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);
		return TIM6_IRQn;
	}
#endif

	return 0;
}

uint32_t tim_getSrcClk(TIM_TypeDef *TIMx)
{
	uint8_t tim_clock_factor = 1;
	LL_RCC_ClocksTypeDef clocks;

	LL_RCC_GetSystemClocksFreq(&clocks);

#if defined(TIM6) && defined(TIM2)
	if ((TIMx == TIM2) || (TIMx == TIM6))
	{
		if (LL_RCC_GetAPB1Prescaler() != LL_RCC_APB1_DIV_1)
			tim_clock_factor = 2;
		return (uint32_t)tim_clock_factor * (clocks.PCLK1_Frequency);
	}
#endif
#if defined(TIM2)
	if (TIMx == TIM2)
	{
		if (LL_RCC_GetAPB1Prescaler() != LL_RCC_APB1_DIV_1)
			tim_clock_factor = 2;
		return (uint32_t)tim_clock_factor * (clocks.PCLK1_Frequency);
	}
#endif

#if defined(TIM21) && defined(TIM22)
	if ((TIMx == TIM21) || (TIMx == TIM22))
	{
		if (LL_RCC_GetAPB2Prescaler() != LL_RCC_APB2_DIV_1)
			tim_clock_factor = 2;
		return (uint32_t)tim_clock_factor * (clocks.PCLK2_Frequency);
	}
#endif
	return 0;
}

uint32_t tim_getMinPrescalerAndMaxPeriod(timebase_t *parameter, TIM_TypeDef *TIMx, uint32_t desired_frecuency)
{

	uint32_t timer_source_freq;
	uint32_t tick_frecuency = 0;
	uint32_t max_period = 0x0000FFFF;
	uint32_t max_prescaler = 0x0000FFFF;

	timer_source_freq = tim_getSrcClk(TIMx);

	max_period = max_period + 1;

	parameter->prescaler = 0;
	do
	{

		parameter->prescaler++;
		tick_frecuency = (timer_source_freq) / (parameter->prescaler);
		parameter->period = ((tick_frecuency) / (desired_frecuency));

	} while ((parameter->period > (max_period)) && (parameter->prescaler <= (max_prescaler + 1)));
	if (parameter->period == 0)
		parameter->period = 1;
	return timer_source_freq;
}

/** 
 ===============================================================================
              ##### Functions #####
 ===============================================================================
 */

void tim_interrupt(TIM_TypeDef *TIMx, uint32_t prescaler, uint32_t period)
{
	uint8_t tim_irqn;
	LL_TIM_InitTypeDef TIM_InitStruct;

	tim_irqn = tim_clkEnableAndGetIRQn(TIMx);

	TIM_InitStruct.Prescaler = 0;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 0;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIMx, &TIM_InitStruct);

	LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_EnableCounter(TIMx);

	LL_TIM_EnableIT_UPDATE(TIMx);

	NVIC_SetPriority((IRQn_Type)tim_irqn, 0);
	NVIC_EnableIRQ((IRQn_Type)tim_irqn);
}

void tim_interruptMs(TIM_TypeDef *TIMx, uint32_t ms)
{
	LL_TIM_InitTypeDef TIM_InitStruct;
	uint32_t timer_source_freq;
	uint8_t tim_irqn;
	tim_irqn = tim_clkEnableAndGetIRQn(TIMx);

	timer_source_freq = tim_getSrcClk(TIMx);

	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = (timer_source_freq / 1000) - 1;
	TIM_InitStruct.Prescaler = ms - 1;
	LL_TIM_Init(TIMx, &TIM_InitStruct);

	LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_EnableCounter(TIMx);

	LL_TIM_EnableIT_UPDATE(TIMx);

	NVIC_SetPriority((IRQn_Type)tim_irqn, 0);
	NVIC_EnableIRQ((IRQn_Type)tim_irqn);
}
