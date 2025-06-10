// #include "usart.h"
// #include "misc.h"
// #include "stm32f4xx_gpio.h"
// #include "stm32f4xx_rcc.h"
// #include "stm32f4xx_usart.h"


// #define RX_BUFFER_SIZE 128
// #define TX_BUFFER_SIZE 128
// #define FRAME_TIMEOUT_MS 20 // 超过20ms判定帧结束
// extern volatile uint32_t uwTick;
// volatile uint32_t lastRxTick = 0;
// volatile uint8_t frameReady = 0;

// volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
// volatile uint8_t txBuffer[TX_BUFFER_SIZE];

// volatile uint16_t rxWriteIndex = 0;
// volatile uint16_t rxReadIndex = 0;

// volatile uint16_t txWriteIndex = 0;
// volatile uint16_t txReadIndex = 0;

// volatile uint8_t txBusy = 0;

// /**
//  * @brief  Configures the USART Peripheral.
//  * @param  None
//  * @retval None
//  */
// void USART_Config(uint32_t baudrate)
// {
//     USART_InitTypeDef USART_InitStructure;
//     NVIC_InitTypeDef NVIC_InitStructure;
//     GPIO_InitTypeDef GPIO_InitStructure;

//     /* Enable GPIO clock */
//     RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);

//     /* Enable USART clock */
//     USARTx_CLK_INIT(USARTx_CLK, ENABLE);

//     /* Connect USART pins to AF7 */
//     GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
//     GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

//     /* Configure USART Tx and Rx as alternate function push-pull */
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//     GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
//     GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

//     GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
//     GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

//     /* Enable the USART OverSampling by 8 */
//     USART_OverSampling8Cmd(USARTx, ENABLE);

//     USART_InitStructure.USART_BaudRate = baudrate;
//     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//     USART_InitStructure.USART_StopBits = USART_StopBits_1;
//     USART_InitStructure.USART_Parity = USART_Parity_No;
//     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//     USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//     USART_Init(USARTx, &USART_InitStructure);

//     /* NVIC configuration */
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

//     /* Enable the USARTx Interrupt */
//     NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);

//     /* Enable USART */
//     USART_Cmd(USARTx, ENABLE);
//     USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
// }


// void USART_SendBytes(uint8_t *data, uint16_t len)
// {
//     for (uint16_t i = 0; i < len; i++)
//     {
//         uint16_t nextIndex = (txWriteIndex + 1) % TX_BUFFER_SIZE;
//         while (nextIndex == txReadIndex);

//         txBuffer[txWriteIndex] = data[i];
//         txWriteIndex = nextIndex;
//     }

//     if (!txBusy)
//     {
//         txBusy = 1;
//         USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
//     }
// }
// uint16_t USART_ReadBytes(uint8_t *data, uint16_t maxlen)
// {
//     if (!frameReady)
//         return 0;

//     uint16_t count = 0;

//     while (rxReadIndex != rxWriteIndex && count < maxlen)
//     {
//         data[count++] = rxBuffer[rxReadIndex];
//         rxReadIndex = (rxReadIndex + 1) % RX_BUFFER_SIZE;
//     }

//     frameReady = 0; // 清除帧标志
//     return count;
// }

// void User_USART1_IRQHandler()
// {
//     // 接收中断
//     if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
//     {
//         uint8_t ch = USART_ReceiveData(USARTx) & 0xFF;
//         uint16_t nextIndex = (rxWriteIndex + 1) % RX_BUFFER_SIZE;

//         if (nextIndex != rxReadIndex)
//         {
//             rxBuffer[rxWriteIndex] = ch;
//             rxWriteIndex = nextIndex;
//             lastRxTick = uwTick;
//         }

//         USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
//     }

//     // 发送中断
//     if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
//     {
//         if (txReadIndex != txWriteIndex)
//         {
//             USART_SendData(USARTx, txBuffer[txReadIndex]);
//             txReadIndex = (txReadIndex + 1) % TX_BUFFER_SIZE;
//         }
//         else
//         {
//             USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
//             txBusy = 0;
//         }

//         USART_ClearITPendingBit(USARTx, USART_IT_TXE);
//     }
// }
