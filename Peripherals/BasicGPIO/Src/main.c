/**
  ******************************************************************************
  * @file    main.c
  * @author  Simon Mekonen
  * @version V1.0
  * @brief	 Brief of GPIO registers
  ******************************************************************************
*/

//Will include the main CMSIS header "stm32f407xx.h"
//That will include system_stm32xx.h and core_cm4.h, cmsis_compiler.h, etc.
#include "stm32f4xx.h"

int main(void)
{

	/*GPIO Port D initialization example ----------------------------------------*/

	SystemInit();

	//Peripheral clock enable (GPIO Port D)
	//AHB: Advanced high-performance bus (derived from SYSCLK),
	//APB: Advanced peripheral bus (derived from AHB)

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	//Alternate function registers
	GPIOD->AFR[0];
	GPIOD->AFR[1];

	//Mode register - 00=I, 01=I/O, 10=AFR, 11=Analog
	GPIOD->MODER |=GPIO_MODER_MODE15_0
				 | GPIO_MODER_MODE14_0
				 | GPIO_MODER_MODE13_0
				 | GPIO_MODER_MODE12_0;

	//Bit set/reset register (write only)
	GPIOD->BSRR |=GPIO_BSRR_BS_15
				| GPIO_BSRR_BS_14
				| GPIO_BSRR_BS_13
				| GPIO_BSRR_BS_12;

	//Input/outupt data register (read only)
	GPIOD->IDR;
	GPIOD->ODR;

	//Lock register - lock value of port bit
	GPIOD->LCKR;


	//Output speed (32) - 00=Low, 01=Medium, 10=High, 11=VeryHigh
	GPIOD->OSPEEDR |=GPIO_OSPEEDER_OSPEEDR15_1
				   | GPIO_OSPEEDER_OSPEEDR14_1
				   | GPIO_OSPEEDER_OSPEEDR13_1
				   | GPIO_OSPEEDER_OSPEEDR12_1;

	//Output type (16) 0=push/pull, 1=open/drain
	GPIOD->OTYPER;

	//Pull-up/Pull-down register - 00=No, 01=Up, 10=Down
	GPIOD->PUPDR = GPIO_PUPDR_PUPDR15_0;


	while(1){

		if((GPIOD->ODR & GPIO_ODR_ODR_15) == GPIO_ODR_ODR_15){
			GPIOD->BSRR |= GPIO_BSRR_BR_15
						| GPIO_BSRR_BR_14
						| GPIO_BSRR_BR_13
						| GPIO_BSRR_BR_12;
		} else {
			GPIOD->BSRR |= GPIO_BSRR_BS_15
						| GPIO_BSRR_BS_14
						| GPIO_BSRR_BS_13
						| GPIO_BSRR_BS_12;
		}

		for(int i = 0; i < 1500000; i++){
			asm("nop");
		}
	}
}
