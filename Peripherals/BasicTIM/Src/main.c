/**
  ******************************************************************************
  * @file    main.c
  * @author  Simon Mekonen
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"

// Function declarations -------------------------------------------------------
void initGPIO();
void initTimer();

volatile uint32_t PRESCALER = 0x00ff;

// Main function ---------------------------------------------------------------
int main(void)
{
	initGPIO();
	initTimer();

	while(1);
}

void initGPIO(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER |= GPIO_MODER_MODE15_0;
	GPIOD->BSRR |= GPIO_BSRR_BR15;
}


void initTimer(){

	// SYSCLK --(PSC)--> AHB --(PSC)--> APB

	// SYSCLK: System Clock (HSI, LSI, HSE, LSE or PLL)
	// AHB: Advanced High-performance Bus (180MHz max)
	// APB: Advanced Peripheral Bus
	// APB1 (45MHz max), APB2 (90MHz) max

	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;	// Enable clock


	TIM7->CR1 = 1;			// Enable counter
	TIM7->DIER = 1;			// Enable interrupt

	TIM7->PSC = PRESCALER;	// Pre-scaler divide by value+1
	TIM7->ARR = 0xffff;		// Max count value (before auto-reset)

	NVIC_EnableIRQ(TIM7_IRQn);
}


void TIM7_IRQHandler(void){

	if(TIM7->SR == SET)
	{
		//Toggle LED15
		if(GPIOD->ODR & GPIO_ODR_ODR_15){
			GPIOD->BSRR |= GPIO_BSRR_BR_15;
		} else {
			GPIOD->BSRR |= GPIO_BSRR_BS15;
		}

		TIM7->SR = RESET;	// Remove interrupt flag
	}

}


