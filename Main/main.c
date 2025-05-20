//
// Created by 15185 on 25-4-14.
//
#include <SEGGER_RTT.h>
#include <common_esw_errno.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include "systemRcc.h"
#include "systick.h"
#include "export.h"
volatile uint32_t count = 0;
void userSystemInit(void);
void fault_test_by_div0(void);
int main(void)
{
    userSystemInit();
    /* for debug using led toogle*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    while (1)
    {
        GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
        sysDelayMs(100);
        count++;
        if (count == 100)
            fault_test_by_div0();
    }
}

void userSystemInit(void)
{

#ifdef DEBUG
    static const char *RccNullptr = "ERROR: Nullptr while use ConfigSysClkTo168MHZ\n";
    static const char *RccWarning = "WARNING: Config HSE Failed, System Use HSI Now!\n";
#endif
    /* System clock config !!!*/
    SEGGER_RTT_Init();
    errorcode_t ret = ConfigSysClkTo168MHZ(&g_systemClkInfo);
#ifdef DEBUG
    if (ERRNO_TIMEOUT == ret)
    {
        SEGGER_RTT_WriteString(0, RccWarning);
    }
    else if (ERRNO_NULL_POINTER == ret)
    {
        SEGGER_RTT_WriteString(0, RccNullptr);
    }
    else if (ret == ERRNO_SUCCESS)
    {
        SEGGER_RTT_WriteString(0, "USE HSE:8000000\n");
    }

#endif
    if (ret == ERRNO_SUCCESS)
    {
        SEGGER_RTT_WriteString(0, "INFO: System Clock Config 168MHz \n");
    }


    // nvic and systick init
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    sysInitTick(15UL);

#ifdef DEBUG
    uint8_t pllSource = RCC_GetSYSCLKSource();
    if (pllSource == 0x00)
        SEGGER_RTT_printf(0, "0x00: HSI used as system clock\n");
    else if (pllSource == 0x04)
        SEGGER_RTT_printf(0, "0x00: HSE used as system clock\n");
    else if (pllSource == 0x08)
        SEGGER_RTT_printf(0, "PLL used as system clock\n");
    else if (pllSource == 0x0C)
        SEGGER_RTT_printf(0, "PLL R used as system clock\n");
    else
        SEGGER_RTT_printf(0, "Do not know!");

    RCC_ClocksTypeDef frequency = {0};
    RCC_GetClocksFreq(&frequency);
    uint32_t freq = HSE_VALUE;
    SEGGER_RTT_printf(0, "HSE_VALUE = %d\n", freq);
    SEGGER_RTT_printf(0, "SYSCLK = %d\n", frequency.SYSCLK_Frequency);
    SEGGER_RTT_printf(0, "HCLK = %d\n", frequency.HCLK_Frequency);
    SEGGER_RTT_printf(0, "PCLK1 = %d\n", frequency.PCLK1_Frequency);
    SEGGER_RTT_printf(0, "PCLK2 = %d\n", frequency.PCLK2_Frequency);
#endif
}


void fault_test_by_div0(void)
{
    volatile int *SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */

    x = 10;
    y = 0;
    z = x / y;
    printf("z:%d\n", z);
}
