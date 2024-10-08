/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#undef F4
#define G0

#ifdef F4
#define RCC_BASE_ADDR 0x40023800 // Datasheet, Table 10 STM32F4xx register boundary addresses
#define RCC_CFGR_REG_OFFSET 0x08UL // 6.3.3 (RCC clock configuration register, RCC_CFGR)
#define RCC_AHB1_ENR_OFFSET 0x30UL //

#else // G0 defines

#define RCC_BASE_ADDR 0x40021000UL //
#define GPIOA_BASE_ADDR 0x50000000UL // 5.4.12, GPIOA base address, GPIOAEN
#define RCC_CFGR_REG_OFFSET 0x08UL // defined in 5.4.3
#define RCC_APB_ENR_OFFSET 0x40UL // 5.4.15, APB peripheral clock enable register 2, RCC_APBENR2
#define RCC_AHB_ENR_OFFSET 0x34UL // 5.4.12,  0x38UL (AHB peripheral clock enable register)
#define RCC_AHB_REG_ADDR (RCC_BASE_ADDR + RCC_AHB_ENR_OFFSET)

#endif

#define RCC_CFGR_REG_ADDR (RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)

#include <stdint.h>
#include <stdio.h>


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t *pRccCfgrReg = (uint32_t *) RCC_CFGR_REG_ADDR;

	#ifdef F4
	*pRccCfgrReg &= ~(0x3 << 21); // clear 21 and 22 bit positions
	*pRccCfgrReg |= (1 << 24); // Configure MCO1 prescaler
	*pRccCfgrReg |= (1 << 25); // Configure MCO1 prescaler
	#else
	*pRccCfgrReg &= ~(0x3 << 26); //clear 26 and 27 bit positions
	*pRccCfgrReg |= (0x3 << 24); // mark as one 24, 25 bit positions
	*pRccCfgrReg |= (1 << 28); // Configure MCO prescaler
	*pRccCfgrReg |= (1 << 29); // Configure MCO prescaler (clk/4)
	#endif



	/// Part 2. Configure PA8 to AF0 mode to behave as MCO1 signal ///
	// In order to implement this, we need to make some changes on GPIO peripherals PA8

	// Step 2a: Enable the peripheral clock for GPIOA peripheral
	#ifdef F4
	uint32_t pRCCAhb1Enr = (uint32_t *) (RCC_BASE_ADDR + 0x30);
	*pRCCAhb1Enr |= (1 << 0); // Enable GPIOA peripheral clock, 7.3.10 on RM of F4
	#else
	uint32_t *pRCCAhbEnr = (uintre_t *) (RCC_AHB_REG_ADDR); // Table 5.4.13 (AHB peripheral clock enable register)
	*pRCCAhbEnr |= (1 << 0);
	#endif

	// Step 2b: Configure the mode of GPIOA pin 8 as alternate function mode (Datasheet, Table 13)
	#ifdef F4 // Table 8.4.1
	*uint32_t *pGPIOAModeReg = (uint32_t *) (GPIOA_BASE_ADDR + 0x00);
	*pGPIOAModeReg &= ~( 0X3 << 16); // clear, set zeros in these bits of the register
	*pGPIOAmodeReg |= (0x2 << 16);

	#else // Table 6.4.1
	uint32_t *pGPIOAModeReg = (uint32_t *) (GPIOA_BASE_ADDR + 0x00);
	*pGPIOAModeReg &= ~( 0x3 << 16); // clear, Table 6.4.12
	*pGPIOAModeReg |= (Ox2 << 16); // this command sets actually the pin
	#endif

	// Step 2c: Cofigure the alternation function register to set the mode 0 for PA8
	// Alternate function high/low register configuration for F4 and G0
	#ifdef F4 // F4, Tables 8.4.9 and 8.4.10  (0x20 and 0x24)
	uint32_t *pGPIOAAltFunHighReg = (uint32_t *) (GPIOA_BASE_ADDR + 0x24);
	*pGPIOAAltFunHighReg &= ~(0xf << 0);
	#else // G0, Tables 6.4.9 and 6.4.10 (0x20, 0x24)
	uint32_t *pGPIOAAltFunHighReg = (uint32_t *)(GPIOA_BASE_ADDR + 0x24);
	*pGPIOAAltFunHighReg &= ~( 0xf << 0); // AFSEL8, set 0000 (AF0)
	#endif

    /* Loop forever */
	for(;;);
}
