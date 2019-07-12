/**
  ******************************************************************************
  * @file    system_lowpower.c 
  * @authors Pablo Fuentes and Joseph Peñafiel
	* @version V1.0.1
  * @date    2019
  * @brief   System Low Power Functions
  ******************************************************************************
*/

/** 
 ===============================================================================
              ##### Dependencies #####
 ===============================================================================
 */

#include "System.h"

/** 
 ===============================================================================
              ##### Private Functions #####
 ===============================================================================
 */

static void SystemClock_Decrease(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
	{
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);

	LL_RCC_MSI_Enable();

	// Wait till MSI is ready
	while (LL_RCC_MSI_IsReady() != 1)
	{
	}

	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_5);

	LL_RCC_MSI_SetCalibTrimming(0);

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

	// Wait till System clock is ready
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
	{
	}

	LL_SetSystemCoreClock(2097000);
	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0);
}

/** 
 ===============================================================================
              ##### Public Function System Clock #####
 ===============================================================================
 */
static void nothing(void) {}

static voidFuncPtr cur_clock = nothing;

void systemInit(void (*clockFunc)(void))
{
	clockFunc();
	cur_clock = clockFunc;
}

/** 
 ===============================================================================
              ##### Funciones p�blicas #####
 ===============================================================================
 */

void system_sleepSeconds(uint32_t seconds)
{
	__disable_irq();
	LL_SYSTICK_DisableIT();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setAlarmBAfter(seconds);
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_MAIN);
	LL_LPM_EnableSleep();
	__WFI();
	__enable_irq();
	LL_SYSTICK_EnableIT();
}

void system_sleepMillis(uint32_t milliseconds)
{
	__disable_irq();
	LL_SYSTICK_DisableIT();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setWKUPMillis(milliseconds);
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_MAIN);
	LL_LPM_EnableSleep();
	__WFI();
	__enable_irq();
	LL_SYSTICK_EnableIT();
	rtc_setWKUPMillis(0); //disable rtc interrupt
}

void system_sleepLPSeconds(uint32_t seconds)
{
	LL_FLASH_DisablePrefetch();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_FLASH_EnableSleepPowerDown();
	LL_RCC_DeInit();
	SystemClock_Decrease();
	__disable_irq();
	LL_SYSTICK_DisableIT();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setAlarmBAfter(seconds);
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
	LL_LPM_EnableSleep();
	__WFI();
	// Exitting low power modes
	LL_PWR_DisableLowPowerRunMode();
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_MAIN);
	while (LL_PWR_IsActiveFlag_REGLPF() == SET)
		;
	LL_RCC_DeInit();
	cur_clock();
	__enable_irq();
	LL_SYSTICK_EnableIT();
}

void system_sleepLPMillis(uint32_t milliseconds)
{
	LL_FLASH_DisablePrefetch();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_FLASH_EnableSleepPowerDown();
	LL_RCC_DeInit();
	SystemClock_Decrease();
	__disable_irq();
	LL_SYSTICK_DisableIT();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setWKUPMillis(milliseconds);
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
	LL_LPM_EnableSleep();
	__WFI();
	// Exitting low power modes
	LL_PWR_DisableLowPowerRunMode();
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_MAIN);
	while (LL_PWR_IsActiveFlag_REGLPF() == SET)
		;
	LL_RCC_DeInit();
	cur_clock();
	__enable_irq();
	LL_SYSTICK_EnableIT();
	rtc_setWKUPMillis(0);
}

void system_stopSeconds(uint32_t seconds)
{
	__disable_irq();
	LL_SYSTICK_DisableIT();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setAlarmBAfter(seconds);
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
	LL_LPM_EnableDeepSleep();
	__WFI();
	LL_LPM_EnableSleep();
	cur_clock();
	__enable_irq();
}

void system_stopMillis(uint32_t milliseconds)
{
	__disable_irq();
	LL_SYSTICK_DisableIT();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setWKUPMillis(milliseconds);
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
	LL_LPM_EnableDeepSleep();
	__WFI();
	LL_LPM_EnableSleep();
	cur_clock();
	__enable_irq();
	rtc_setWKUPMillis(0);
}

void system_stopUntilInterrupt(void)
{
	LL_SYSTICK_DisableIT();
	CLEAR_BIT(PWR->CR, PWR_CR_PDDS); // Clear PDDS bits
	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
	LL_LPM_EnableDeepSleep();
	__WFI();
	LL_LPM_EnableSleep();
	cur_clock();
}

void system_standby(void)
{
	if (LL_PWR_IsActiveFlag_SB() != RESET)
	{
		LL_PWR_ClearFlag_SB();
	}
	LL_SYSTICK_DisableIT();
	CLEAR_BIT(PWR->CSR, PWR_CSR_VREFINTRDYF); // disable vref
	CLEAR_BIT(ADC->CCR, ADC_CCR_TSEN);				// disable temp sensor
	CLEAR_BIT(ADC->CCR, ADC_CCR_VREFEN);			// disable vref
	CLEAR_BIT(PWR->CR, PWR_CR_PVDE);					//disable PVD
	LL_PWR_ClearFlag_WU();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableUltraLowPower();
	LL_PWR_EnableFastWakeUp();
	LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
	LL_LPM_EnableDeepSleep();
#if defined(__CC_ARM)
	__force_stores();
#endif
	__WFI();
}

void system_standbySeconds(uint32_t seconds)
{
	if (LL_PWR_IsActiveFlag_SB() != RESET)
	{
		LL_PWR_ClearFlag_SB();
	}
	__disable_irq();
	LL_SYSTICK_DisableIT();
	CLEAR_BIT(PWR->CSR, PWR_CSR_VREFINTRDYF); // disable vref
	CLEAR_BIT(ADC->CCR, ADC_CCR_TSEN);				// disable temp sensor
	CLEAR_BIT(ADC->CCR, ADC_CCR_VREFEN);			// disable vref
	CLEAR_BIT(PWR->CR, PWR_CR_PVDE);					//disable PVD
	LL_PWR_ClearFlag_WU();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableUltraLowPower();
	LL_PWR_EnableFastWakeUp();
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_setAlarmBAfter(seconds);
	LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
	LL_LPM_EnableDeepSleep();
#if defined(__CC_ARM)
	__force_stores();
#endif
	__WFI();
}

void system_standbyUntilWakeUpPin(uint32_t WAKEUP_PIN_x)
{
	if (LL_PWR_IsActiveFlag_SB() != RESET)
	{
		LL_PWR_ClearFlag_SB();
	}
	__disable_irq();
	LL_SYSTICK_DisableIT();
	CLEAR_BIT(PWR->CSR, PWR_CSR_VREFINTRDYF); // disable vref
	CLEAR_BIT(ADC->CCR, ADC_CCR_TSEN);				// disable temp sensor
	CLEAR_BIT(ADC->CCR, ADC_CCR_VREFEN);			// disable vref
	CLEAR_BIT(PWR->CR, PWR_CR_PVDE);					//disable PVD
	LL_PWR_DisableWakeUpPin(WAKEUP_PIN_x);
	LL_PWR_ClearFlag_WU();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableUltraLowPower();
	LL_PWR_EnableFastWakeUp();
	LL_PWR_EnableWakeUpPin(WAKEUP_PIN_x);
	LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
	LL_LPM_EnableDeepSleep();
#if defined(__CC_ARM)
	__force_stores();
#endif
	__WFI();
}

void system_turnOffAllGPIOs(void)
{
#ifdef GPIOA
	GPIOA->MODER = 0xFFFFFFFF;
	GPIOA->OTYPER = 0x00000000;
	GPIOA->OSPEEDR = 0x00000000;
	GPIOA->PUPDR = 0x00000000;
	GPIOA->ODR = 0x00000000;
	GPIOA->BSRR = 0x00000000;
	GPIOA->BRR = 0x00000000;
#endif
#ifdef GPIOB
	GPIOB->MODER = 0xFFFFFFFF;
	GPIOB->OTYPER = 0x00000000;
	GPIOB->OSPEEDR = 0x00000000;
	GPIOB->PUPDR = 0x00000000;
	GPIOB->ODR = 0x00000000;
	GPIOB->BSRR = 0x00000000;
	GPIOB->BRR = 0x00000000;
#endif
#ifdef GPIOC
	GPIOC->MODER = 0xFFFFFFFF;
	GPIOC->OTYPER = 0x00000000;
	GPIOC->OSPEEDR = 0x00000000;
	GPIOC->PUPDR = 0x00000000;
	GPIOC->ODR = 0x00000000;
	GPIOC->BSRR = 0x00000000;
	GPIOC->BRR = 0x00000000;
#endif
}