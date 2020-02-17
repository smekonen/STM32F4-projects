#include "stm32f4xx.h"

/// FUNCTION DECLARATIONS

void initGPIO();
void initTIMER();
void initDAC();
void initADC();
void initRTC();


/// MAIN FUNCTION

int main(void)
{
	SystemInit();
	initGPIO();
	initTIMER();
	initADC();
	initRTC();

	ADC1->CR2 |= ADC_CR2_SWSTART;	// start first conversion

	while(1){



		if(ADC1->SR & ADC_SR_EOC){		// poll end of conversion flag

			TIM7->PSC = ADC1->DR;
			ADC1->SR &= ~ADC_SR_EOC;	//remove flag (acknowledge)

		}


		if(RTC->ISR & RTC_ISR_RSF){		//poll synchronization flag
			//do something
		}

	}
}




/// FUNCTION DEFINITIONS

void initGPIO(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	// Enable clock

	GPIOA->MODER |= GPIO_MODER_MODE4_0		// Set PA3 and PA1 to analog mode, PA4 to IO mode
				 |  GPIO_MODER_MODE3
				 |	GPIO_MODER_MODE1;
}

void initDAC(){

	RCC->APB1ENR |= RCC_APB1ENR_DACEN;

	DAC->CR |= DAC_CR_TEN1					//TIM7 trigger event
			|  DAC_CR_TSEL1_1				//Triangle wave 0-5v amplitude
			|  DAC_CR_WAVE1_1
			|  0b1001 << DAC_CR_MAMP1_Pos;


	DAC->CR |= DAC_CR_EN1;		//Enable

	DAC->DHR12R1 = 0x000;		// Initial 12-bit data
}


/*
 * ADC guide: http://embedded-lab.com/blog/stm32-adc-2/2/
 * ADC application notes
 * */

void initADC(){

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	ADC1->CR1 |= ADC_CR1_RES_1; 		// 8-bit resolution


	ADC1->SQR3 |= ADC_SQR3_SQ1_0;		// Set first conversion in sequence to ADC channel 0 (PA0)


	ADC1->SMPR2 |= ADC_SMPR2_SMP0; 		// Set sample frequency of CH0 to 480 cycles (111)

	ADC1->CR2 |= ADC_CR2_ADON 			// Turn on ADC, continuous mode
			  |  ADC_CR2_CONT;

}


void initTIMER(){

	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

	TIM7->PSC = 0x0000;
	TIM7->ARR = 0x00ff;

	TIM7->CR2 = TIM_CR2_MMS_1;	// Trigger on every update

	TIM7->DIER = TIM_DIER_UIE;	// Enable update interrupt

	NVIC_EnableIRQ(TIM7_IRQn);

	TIM7->CR1 = TIM_CR1_CEN;
}

void TIM7_IRQHandler(){

	if(TIM7->SR == SET){

		if((GPIOA->ODR & GPIO_ODR_ODR_4) == GPIO_ODR_ODR_4){
			GPIOA->BSRR |= GPIO_BSRR_BR_4;
		} else {
			GPIOA->BSRR |= GPIO_BSRR_BS_4;
		}

		TIM7->SR = RESET;
	}
}

