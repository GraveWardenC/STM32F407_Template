#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


void initLED(void);
void toggleLED(void);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTaskLed(void *pvParameters);

QueueHandle_t xQueue;
SemaphoreHandle_t xSemaphore;
int main()
{
    __set_PRIMASK(1);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    initLED();
    xQueue = xQueueCreate(5, sizeof(uint32_t));
    xSemaphore = xSemaphoreCreateMutex();

    if (xQueue != NULL && xSemaphore != NULL) 
    {
        // xTaskCreate(vTask1, "Task1", 256, NULL, 2, NULL);
        // xTaskCreate(vTask2, "Task2", 256, NULL, 2, NULL);
        xTaskCreate(vTaskLed, "TaskLed", 256, NULL, 2, NULL);
    }

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
void toggleLED(void)
{
    GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
}
void vTask1(void *pvParameters)
{
    uint32_t count = 0;

    for(;;) 
    {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
         {
            xQueueSend(xQueue, &count, portMAX_DELAY);
            xSemaphoreGive(xSemaphore);
            count++;
        }
        toggleLED();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}

void vTask2(void *pvParameters)
{
    uint32_t recvVal;
    for(;;)
    {
        if (xQueueReceive(xQueue, &recvVal, portMAX_DELAY) == pdPASS) 
        {
            toggleLED();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void vTaskLed(void *pvParameters)
{
    for(;;)
    {
        toggleLED();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}