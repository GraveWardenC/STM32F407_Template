//////////////////////////////////////////////////////////////////////////
/// Copyright, (c) Wuhan Fine Visual Healthcare Inc., 2022
/// All rights reserved.
///
/// \file systemRcc.c
///
/// \author kai.chen(mailto: chenkai@visualfine.com)
///
/// \version 1.0
///
/// \date 16/04/2025
/////////////////////////////////////////////////////////////////////////
#include "sysclock.h"
#include <stm32f4xx_flash.h>
#include <stm32f4xx_rcc.h>
#include "common_esw_errno.h"

SYSTEM_CLK g_systemClkInfo = {0};

errorcode_t ConfigSysClkTo168MHZ(SYSTEM_CLK *sysclk)
{
    if ((void *) (0) == sysclk)
        return ERRNO_NULL_POINTER;

/* make sure HSE and HSI no error*/
#ifdef HSE_VALUE
#undef HSE_VALUE
#define HSE_VALUE ((uint32_t) 8000000)
#endif

#ifdef HSI_VALUE
#undef HSI_VALUE
#define HSI_VALUE ((uint32_t) 16000000)
#endif

    PLL_CLK_SOURCE clockSource = USE_HSE;

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_OFF);
    RCC_HSICmd(ENABLE);
    RCC_HSEConfig(RCC_HSE_ON);

    ErrorStatus status = RCC_WaitForHSEStartUp();
    if (status != SUCCESS)
    {
        clockSource = USE_HSI;
    }
    // 2. set Flash delay
    FLASH_SetLatency(FLASH_Latency_5);
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);
    // 3. bus clk
    RCC_HCLKConfig(RCC_SYSCLK_Div1); // HCLK = SYSCLK / 1
    RCC_PCLK1Config(RCC_HCLK_Div4); // PCLK1 = HCLK / 4 (<=42MHz)
    RCC_PCLK2Config(RCC_HCLK_Div2); // PCLK2 = HCLK / 2 (<=84MHz)
    // 4. pll
    RCC_PLLCmd(DISABLE);

    if (clockSource == USE_HSE)
    {
        RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7); // PLLCLK = 8/8*336/2 = 168MHz
    }
    else
    {
        RCC_PLLConfig(RCC_PLLSource_HSI, 16, 336, 2, 7); // PLLCLK = 16/16*336/2 = 168MHz
    }

    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        ; // if error will enter HardFault
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08)
        ;

    SystemCoreClockUpdate();

    sysclk->userSysClkFrequency = SystemCoreClock; // = 168 MHz
    sysclk->userHclkFrequency = SystemCoreClock; // = 168 MHz
    sysclk->userPclk1Frequency = SystemCoreClock / 4; // = 42 MHz
    sysclk->userPclk2Frequency = SystemCoreClock / 2; // = 84 MHz
    sysclk->pllSource = clockSource;

    if (USE_HSE == clockSource)
        return ERRNO_SUCCESS;
    else
        return ERRNO_TIMEOUT; // just use hse timeout
}
