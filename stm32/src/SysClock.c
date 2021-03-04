#include "SysClock.h"

//******************************************************************************************
// Switch the PLL source from MSI to HSI, and select the PLL as SYSCLK source.
//******************************************************************************************
void System_Clock_Init(void){
	
	uint32_t HSITrim;

	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
	// must be correctly programmed according to the frequency of the CPU clock
	// (HCLK) and the supply voltage of the device.		
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	// Enable the Internal High Speed oscillator (HSI
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	// Adjusts the Internal High Speed oscillator (HSI) calibration value
	// RC oscillator frequencies are factory calibrated by ST for 1 % accuracy at 25oC
	// After reset, the factory calibration value is loaded in HSICAL[7:0] of RCC_ICSCR	
	HSITrim = 16; // user-programmable trimming value that is added to HSICAL[7:0] in ICSCR.
	RCC->ICSCR &= ~RCC_ICSCR_HSITRIM;
	RCC->ICSCR |= HSITrim << 24;
	
	RCC->CR    &= ~RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY);
	
	// Select clock source to PLL
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // 00 = No clock, 01 = MSI, 10 = HSI, 11 = HSE
	
	// Make PLL as 80 MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 20/2 = 160 MHz
	// f(PLL_R) = f(VCO clock) / PLLR = 160MHz/2 = 80MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 20U << 8;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 1U << 4; // 000: PLLM = 1, 001: PLLM = 2, 010: PLLM = 3, 011: PLLM = 4, 100: PLLM = 5, 101: PLLM = 6, 110: PLLM = 7, 111: PLLM = 8

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;  // 00: PLLR = 2, 01: PLLR = 4, 10: PLLR = 6, 11: PLLR = 8	
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN; // Enable Main PLL PLLCLK output 

	RCC->CR   |= RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	// Select PLL selected as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL; // 00: MSI, 01:HSI, 10: HSE, 11: PLL
	
	// Wait until System Clock has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
	// The maximum frequency of the AHB, the APB1 and the APB2 domains is 80 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB prescaler = 1; SYSCLK not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB high-speed prescaler (APB1) = 1, HCLK not divided
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2) = 1, HCLK not divided
	
	// RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
	// RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	// RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP; 
	// RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ;	
	// RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN; // Enable Main PLL PLLSAI3CLK output enable
	// RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN; // Enable Main PLL PLL48M1CLK output enable
	
	RCC->CR &= ~RCC_CR_PLLSAI1ON;  // SAI1 PLL enable
	while ( (RCC->CR & RCC_CR_PLLSAI1ON) == RCC_CR_PLLSAI1ON );
	
	// Configure and enable PLLSAI1 clock to generate 11.294MHz 
	// 8 MHz * 24 / 17 = 11.294MHz
	// f(VCOSAI1 clock) = f(PLL clock input) *  (PLLSAI1N / PLLM)
	// PLLSAI1CLK: f(PLLSAI1_P) = f(VCOSAI1 clock) / PLLSAI1P
	// PLLUSB2CLK: f(PLLSAI1_Q) = f(VCOSAI1 clock) / PLLSAI1Q
	// PLLADC1CLK: f(PLLSAI1_R) = f(VCOSAI1 clock) / PLLSAI1R
	RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1N;
	RCC->PLLSAI1CFGR |= 24U<<8;
	
	// SAI1PLL division factor for PLLSAI1CLK
	// 0: PLLSAI1P = 7, 1: PLLSAI1P = 17
	RCC->PLLSAI1CFGR |= RCC_PLLSAI1CFGR_PLLSAI1P;
	RCC->PLLSAI1CFGR |= RCC_PLLSAI1CFGR_PLLSAI1PEN;
	
	// SAI1PLL division factor for PLL48M2CLK (48 MHz clock)
	// RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1Q;
	// RCC->PLLSAI1CFGR |= U<<21;
	// RCC->PLLSAI1CFGR |= RCC_PLLSAI1CFGR_PLLSAI1QEN;
	
	// PLLSAI1 division factor for PLLADC1CLK (ADC clock)
	// 00: PLLSAI1R = 2, 01: PLLSAI1R = 4, 10: PLLSAI1R = 6, 11: PLLSAI1R = 8
	// RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1R; 
	// RCC->PLLSAI1CFGR |= U<<25;
	// RCC->PLLSAI1CFGR |= RCC_PLLSAI1CFGR_PLLSAI1REN;
	
	RCC->CR |= RCC_CR_PLLSAI1ON;  // SAI1 PLL enable
	while ( (RCC->CR & RCC_CR_PLLSAI1ON) == 0);
	
	// SAI1 clock source selection
	// 00: PLLSAI1 "P" clock (PLLSAI1CLK) selected as SAI1 clock
	// 01: PLLSAI2 "P" clock (PLLSAI2CLK) selected as SAI1 clock
	// 10: PLL "P" clock (PLLSAI3CLK) selected as SAI1 clock
	// 11: External input SAI1_EXTCLK selected as SAI1 clock	
	RCC->CCIPR &= ~RCC_CCIPR_SAI1SEL;

	RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;
}
