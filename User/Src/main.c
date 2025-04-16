//
// Created by 15185 on 25-4-14.
//
#include <stdio.h>
#include "stm32f4xx.h"
#include<core_cm4.h>
#include <SEGGER_RTT.h>
#include <cm_backtrace.h>
#include<stm32f4xx_rcc.h>
void SetSysClockTo168(void);
#define SYSTEM_CLOCK_FREQ  168000000  // 示例：STM32F4主频168MHz

// 微秒级延时（粗略）
void delay_us(uint32_t us) {
    uint32_t ticks = us * (SYSTEM_CLOCK_FREQ / 1000000) / 3;  // 经验值调整
    while(ticks--) {
        __NOP();  // 空操作，防止编译器优化
    }
}

// 毫秒级延时
void delay_ms(uint32_t ms) {
    while(ms--) {
        delay_us(1000);  // 调用微秒级延时
    }
}
volatile uint32_t count =0;
volatile uint8_t state=0;
volatile uint8_t pa8 = 0;

#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V0.1.0"
void fault_test_by_div0(void);
void fault_test_by_unalign(void);

int main(void)
{
    SetSysClockTo168();

    RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    SEGGER_RTT_Init();
    SEGGER_RTT_printf(0,"RTT Init!\n");
    cm_backtrace_init("TEMPLATE", HARDWARE_VERSION, SOFTWARE_VERSION);
    delay_ms(500);
    while (1)
    {
        SEGGER_RTT_printf(0,"count: %d\n",count);
        GPIO_WriteBit(GPIOC,GPIO_Pin_1,1);
        delay_ms(10);
        GPIO_WriteBit(GPIOC,GPIO_Pin_1,0);
        delay_ms(10);
        count++;
        // printf("helloworld");
        // if (count == 40)
        //     fault_test_by_div0();
    }

}

void SetSysClockTo168(void)
{
    // 1. 启用外部 HSE 振荡器
    RCC->CR |= RCC_CR_HSEON;             // 启动 HSE
    while ((RCC->CR & RCC_CR_HSERDY) == 0); // 等待 HSE 准备好

    // 2. 配置 PLL (使用 HSE 作为 PLL 的输入)
    // HSE = 8 MHz, 目标主频 = 168 MHz (PLL 输入为 8 MHz, 目标输出 168 MHz, 需要倍频 21)
    RCC->CFGR |= RCC_PLLCFGR_PLLSRC_HSE;    // 设置 PLL 输入为 HSE
    RCC->CFGR &= ~RCC_CFGR_SWS_PLL;      // 清除 PLL 倍频因子
    RCC->CFGR |= RCC_PLLCFGR_PLLM_4;     // 设置 PLL 倍频因子为 21
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_6;   // 设置 PLL N 因子
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_3;   // 设置 PLL N 因子为 336 (此时 PLL 输出为 168 MHz)
    // 3. 启动 PLL
    RCC->CR |= RCC_CR_PLLON;             // 启动 PLL
    while ((RCC->CR & RCC_CR_PLLRDY) == 0); // 等待 PLL 锁定

    // 4. 设置系统时钟源为 PLL 输出
    RCC->CFGR |= RCC_CFGR_SW_PLL;        // 选择 PLL 为系统时钟源
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // 等待 PLL 成为系统时钟源

    // 5. 配置 AHB 和 APB 时钟分频器
    // AHB 时钟 (SYSCLK) 使用 168 MHz, 不需要分频
    RCC->CFGR &= ~RCC_CFGR_HPRE;         // AHB 不分频，时钟频率为 168 MHz
    // APB1 时钟最大为 42 MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;    // 设置 APB1 时钟为 42 MHz (168 MHz / 4)
    // APB2 时钟最大为 84 MHz
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;    // 设置 APB2 时钟为 84 MHz (168 MHz / 2)
}


void fault_test_by_unalign(void) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    volatile int * p;
    volatile int value;

    *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */

    p = (int *) 0x00;
    value = *p;
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

    p = (int *) 0x04;
    value = *p;
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

    p = (int *) 0x03;
    value = *p;
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);
}

void fault_test_by_div0(void) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */

    x = 10;
    y = 0;
    z = x / y;
    printf("z:%d\n", z);
}
