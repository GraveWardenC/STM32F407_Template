#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sysclock.h"

void initLED(void);
void vTaskLED(void *pvParameters);
int main()
{
    __set_PRIMASK(1);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    (void)ConfigSysClkTo168MHZ(&g_systemClkInfo);
    initLED();
    xTaskCreate(vTaskLED, "LED", 256, NULL, 18, NULL);
    vTaskStartScheduler();
    
    for(;;) 
    {
        //loop
    }
}


void initLED(void)
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

void vTaskLED(void *pvParameters)
{
    while (1)
    {
        GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
        vTaskDelay(pdMS_TO_TICKS(500));  // 延时500ms
    }
}