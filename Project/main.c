#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

int counttest;
void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile("nop");
    }
}
int main()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_1);
    counttest = 100;

    while (1)
    {
        counttest+=5;
        GPIO_ResetBits(GPIOC, GPIO_Pin_1);
        delay(100000);
        GPIO_SetBits(GPIOC, GPIO_Pin_1);
        delay(100000);
    }
}
