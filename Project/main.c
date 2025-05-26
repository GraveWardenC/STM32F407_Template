#include "sysclock.h"
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_gpio.h"

void initTestLED(void);
void toggleTestLED(void);
void DWT_Init(void);
static inline void delayUs(uint32_t us);
static inline void delayMs(uint32_t us);
int main()
{
    // it can config the system clock to 168Mhz,USE HSE_Value = 8MHz
    (void)ConfigSysClkTo168MHZ(&g_systemClkInfo);
    if (!(DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk))
    {
        DWT_Init();
    }

    initTestLED();
    
    while (1)
    {
       toggleTestLED();
       delayMs(500);
    }
}

void initTestLED(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_1);
}

void toggleTestLED(void)
{
    GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
}

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 
    DWT->CYCCNT = 0;                                
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            
}

static inline void delayUs(uint32_t us)
{
    uint32_t cycles_per_us = SystemCoreClock / 1000000;
    uint32_t start = DWT->CYCCNT;
    uint32_t delay_cycles = us * cycles_per_us;
    while ((DWT->CYCCNT - start) < delay_cycles);
}
static inline void delayMs(uint32_t ms)
{
    while (ms--) delayUs(1000);
}