/**
 ******************************************************************************
 * @file    System.h
 * @version V1.0.1
 * @date    2019
 * @brief  	STM32 System
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_H
#define __SYSTEM_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32l0xx.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_exti.h"
#include "unix_time.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Useful macro definitions *********************/
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

	/* Typedef Void Function **********************/
	typedef void (*voidFuncPtr)(void);

/* Redefinitions ******************************/
#define delay LL_mDelay
#define delay_ms LL_mDelay
#define system_softReset NVIC_SystemReset

	/* Millis *************************************/
	uint32_t millis(void);

	/* Clock Functions ***************************/
	void CLOCK_HSI_32MHZ(void);
	void CLOCK_HSI_16MHZ(void);
	void CLOCK_HSI_8MHZ(void);
	void CLOCK_HSI_6MHZ(void);
	void CLOCK_HSI_4MHZ(void);
	void CLOCK_MSI_2MHZ(void);
	void clock_init(void (*clockFunc)(void)); // Function defined in "system_lowpower_l0.c"

/* System RTC Functions **********************/
//Definitions
#define MIN_TO_SEC(__x__) ((__x__)*60)
#define HOUR_TO_SEC(__x__) ((__x__)*60 * 60)
#define RTC_MONDAY LL_RTC_WEEKDAY_MONDAY
#define RTC_TUESDAY LL_RTC_WEEKDAY_TUESDAY
#define RTC_WEDNESDAY LL_RTC_WEEKDAY_WEDNESDAY
#define RTC_THURSDAY LL_RTC_WEEKDAY_THURSDAY
#define RTC_FRIDAY LL_RTC_WEEKDAY_FRIDAY
#define RTC_SATURDAY LL_RTC_WEEKDAY_SATURDAY
#define RTC_SUNDAY LL_RTC_WEEKDAY_SUNDAY
#define RTC_ALARM_TYPE_DATE 0
#define RTC_ALARM_TYPE_WEEKDAY 1
// RTC Tamper definitions
#define RTC_TAMPER_RISING ((uint32_t)0x00000000U)
#define RTC_TAMPER_FALLING ((uint32_t)0x00000002U)
#define RTC_TAMPER_ERASEBKP_EN ((uint32_t)0x00000000U)
#define RTC_TAMPER_ERASEBKP_DIS ((uint32_t)0x00020000U)
#define RTC_TAMPER_AUTOCLRFLG_EN ((uint32_t)0x00040000U)
#define RTC_TAMPER_AUTOCLRFLG_DIS ((uint32_t)0x00000000U)
#define RTC_TAMPER_SAMPLINGFREQ_DIV32768 ((uint32_t)0x00000000U) //  frequency at which each of the RTC_TAMPx inputs are sampled
#define RTC_TAMPER_SAMPLINGFREQ_DIV16384 RTC_TAMPCR_TAMPFREQ_0
#define RTC_TAMPER_SAMPLINGFREQ_DIV8192 RTC_TAMPCR_TAMPFREQ_1
#define RTC_TAMPER_SAMPLINGFREQ_DIV4096 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_0 | RTC_TAMPCR_TAMPFREQ_1))
#define RTC_TAMPER_SAMPLINGFREQ_DIV2048 RTC_TAMPCR_TAMPFREQ_2
#define RTC_TAMPER_SAMPLINGFREQ_DIV1024 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_0 | RTC_TAMPCR_TAMPFREQ_2))
#define RTC_TAMPER_SAMPLINGFREQ_DIV512 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_1 | RTC_TAMPCR_TAMPFREQ_2))
#define RTC_TAMPER_SAMPLINGFREQ_DIV256 ((uint32_t)(RTC_TAMPCR_TAMPFREQ_0 | RTC_TAMPCR_TAMPFREQ_1 | RTC_TAMPCR_TAMPFREQ_2))
	typedef struct
	{
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
	} RTCTime_t;
	typedef struct
	{
		uint8_t weekday;
		uint8_t day;
		uint8_t month;
		uint8_t year;
	} RTCDate_t;
	typedef struct
	{
		uint32_t Mode;
		uint32_t EraseBKP;
		uint32_t AutoClearFlag;
		uint32_t SamplingFreq;
	} RTCTamper_t;
	// Functions defined in "system_rtc_l0.c"
	void rtc_initLsi(bool resetBKP);
	void rtc_initLse(bool resetBKP);
	void rtc_setTime(uint8_t hour, uint8_t min, uint8_t sec);
	void rtc_setDate(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year);
	void rtc_setUnix(uint32_t unix);
	void rtc_getTime(RTCTime_t *rtc_time);
	void rtc_getDate(RTCDate_t *rtc_date);
	uint32_t rtc_getUnix(void);
	// RTC Interrupts
	void rtc_setWKUPMillis(uint16_t milliseconds); //put 0 to disable
	void rtc_setWKUPSeconds(uint16_t seconds);		 //put 0 to disable
	void rtc_setAlarmAAfter(uint32_t seconds);		 //put 0 to disable
	void rtc_setAlarmBAfter(uint32_t seconds);		 //put 0 to disable
	// Just one tamper per application
	void rtc_setTamper1IT(RTCTamper_t *tamper);
	void rtc_setTamper2IT(RTCTamper_t *tamper);
	void rtc_setTamper3IT(RTCTamper_t *tamper);
	void rtc_disableTamper1IT(void);
	void rtc_disableTamper2IT(void);
	void rtc_disableTamper3IT(void);
	void rtc_setAlarmA(uint8_t RTC_ALARM_TYPE_X, uint8_t day_weekday, uint8_t hours, uint8_t minutes, uint8_t seconds);
	void rtc_setAlarmB(uint8_t RTC_ALARM_TYPE_X, uint8_t day_weekday, uint8_t hours, uint8_t minutes, uint8_t seconds);
// Handler INT Functions
#define IRQ_RTC_WKUP() void __Handler_RTC_WKUP(void)
#define IRQ_RTC_ALARMA() void __Handler_RTC_ALARMA(void)
#define IRQ_RTC_ALARMB() void __Handler_RTC_ALARMB(void)
#define IRQ_RTC_TAMPER1() void __Handler_RTC_TAMPER1(void)
#define IRQ_RTC_TAMPER2() void __Handler_RTC_TAMPER2(void)
#define IRQ_RTC_TAMPER3() void __Handler_RTC_TAMPER3(void)
	IRQ_RTC_WKUP();
	IRQ_RTC_ALARMA();
	IRQ_RTC_ALARMB();
	IRQ_RTC_TAMPER1();
	IRQ_RTC_TAMPER2();
	IRQ_RTC_TAMPER3();

/* System Backup (BKP) Registers Functions **********************/
// Definitions
#define BKP_DR0 LL_RTC_BKP_DR0
#define BKP_DR1 LL_RTC_BKP_DR1
#define BKP_DR2 LL_RTC_BKP_DR2
#define BKP_DR3 LL_RTC_BKP_DR3
#define BKP_DR4 LL_RTC_BKP_DR4
	// Functions defined in "system_rtc_l0.c"
	// This functions required System_RTC_initLSI first
	void system_BKPWrite(uint32_t BKP_DRx, uint32_t data);
	uint32_t system_BKPRead(uint32_t BKP_DRx);

/* System Low Power Modes Functions **********************/
// Definitions
#define WAKEUP_PIN_PA0 LL_PWR_WAKEUP_PIN1
#define WAKEUP_PIN_PC13 LL_PWR_WAKEUP_PIN2
#define WAKEUP_PIN_PA2 LL_PWR_WAKEUP_PIN3
	// Functions defined in "system_lowpower_l0.c"
	void system_turnOffAllGPIOs(void);
	// This functions required rtc_initLsi first
	void system_sleepSeconds(uint32_t seconds);
	void system_sleepMillis(uint32_t milliseconds);
	void system_sleepLPSeconds(uint32_t seconds);
	void system_sleepLPMillis(uint32_t milliseconds);
	void system_stopSeconds(uint32_t seconds);
	void system_stopMillis(uint32_t milliseconds);
	void system_stopUntilInterrupt(void); // This function doesn't required System_RTC_initLSI
	void system_standby(void);
	void system_standbySeconds(uint32_t seconds);
	void system_standbyUntilWakeUpPin(uint32_t WAKEUP_PIN_x); // This function doesn't required System_RTC_initLSI

	/* System EEPROM Functions *********************************/
	// OJO: PADDING = 4 bytes
	void eeprom_unlock(void);
	void eeprom_lock(void);
	uint8_t eeprom_writeByte(uint32_t address, uint8_t data);
	uint8_t eeprom_writeHalfWord(uint32_t address, uint16_t data);
	uint8_t eeprom_writeWord(uint32_t address, uint32_t data);
	uint8_t eeprom_writeFloat(uint32_t address, float *data);
	uint8_t eeprom_readByte(uint32_t address);
	uint16_t eeprom_readHalfWord(uint32_t address);
	uint32_t eeprom_readWord(uint32_t address);
	void eeprom_readFloat(uint32_t address, float *rdata);

#ifdef __cplusplus
}
#endif

#endif
