
#include "stm32f4xx.h"

void initRTC();

int main(void)
{
	SystemInit();
	initRTC();

	while(1){


	}
}



/* General initialization for RTC
 * Good resource: http://embedded-lab.com/blog/stm32s-internal-rtc/ */

void initRTC(){

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;		// Enable PWR clock
	PWR->CR |= PWR_CR_DBP;					// Disable backup domain write protection

	// For Alarm interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		// Enable SysConfig clock for EXTI
	EXTI->IMR |= EXTI_EMR_EM17;					// Enable EXTI17 (RTC alarm) with rising edge trigger
	EXTI->RTSR |= EXTI_RTSR_TR17;


	/// ENABLE OSCILATOR

	RCC->CSR |= RCC_CSR_LSION;				// Enable low-speed internal oscillator (32.687 kHz)

	while(!(RCC->CSR & RCC_CSR_LSIRDY)){}		// Wait for LSI ready (stable)


	/// ENABLE RTC

	RCC->BDCR |= RCC_BDCR_RTCSEL_1;			// Enable RTC with LSI oscillator
	RCC->BDCR |= RCC_BDCR_RTCEN;



	RTC->WPR = 0xCA;			// Disabling write protection
	RTC->WPR = 0x53;


	/// INITIALIZATION MODE

	RTC->ISR |= RTC_ISR_INIT;				// Enter initialization mode

	while(!(RTC->ISR & RTC_ISR_INITF)){}	// Wait for initialization mode flag (and alarm init)

	RTC->PRER = RTC->PRER;					// Default pre-scaler produces 1 Hz for 32.687 kHz (LSI)

	RTC->TR = 0x113700;						// Set initial time to 11:37:00

	RTC->ISR &= ~RTC_ISR_INIT;				// Exit initialization mode



	/// ALARM INTERRUPT SETUP

	RTC->CR &= ~RTC_CR_ALRAE;					// Disable Alarm A

	while(!(RTC->ISR & RTC_ISR_ALRAWF)){}		// Wait for Alarm A write flag

	RTC->ALRMAR |= 0xffffff05;  				// Don't care date/hour/minute. Alarm on xx:xx:05;

	RTC->CR |= RTC_CR_ALRAE | RTC_CR_ALRAIE;	// Enable Alarm A interrupt

	NVIC_EnableIRQ(RTC_Alarm_IRQn);


	RTC->WPR = 0xFF;			// Enabling write protection

}

void RTC_Alarm_IRQHandler(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |=GPIO_MODER_MODE15_0;
	GPIOD->BSRR |= GPIO_BSRR_BS15;
}
