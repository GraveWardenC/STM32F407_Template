// //////////////////////////////////////////////////////////////////////////
// /// Copyright, (c) Wuhan Fine Visual Healthcare Inc., 2022
// /// All rights reserved.
// ///
// /// \file usart.h
// ///
// /// \author chenkai(mailto: chenkai@visualfine.com)
// ///
// /// \version 1.0
// ///
// /// \date 25-4-17
// /////////////////////////////////////////////////////////////////////////

// #ifndef USART_H
// #define USART_H

// #ifdef __cplusplus
// extern "C" {
// #endif
// #include "stm32f4xx.h"

// #define USARTx                           USART1
// #define USARTx_CLK                       RCC_APB2Periph_USART1
// #define USARTx_CLK_INIT                  RCC_APB2PeriphClockCmd
// #define USARTx_IRQn                      USART1_IRQn
// #define USARTx_IRQHandler                USART1_IRQHandler

// #define USARTx_TX_PIN                    GPIO_Pin_9                
// #define USARTx_TX_GPIO_PORT              GPIOA                       
// #define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
// #define USARTx_TX_SOURCE                 GPIO_PinSource9
// #define USARTx_TX_AF                     GPIO_AF_USART1

// #define USARTx_RX_PIN                    GPIO_Pin_10                
// #define USARTx_RX_GPIO_PORT              GPIOA                    
// #define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOA
// #define USARTx_RX_SOURCE                 GPIO_PinSource10
// #define USARTx_RX_AF                     GPIO_AF_USART1



// extern void USART_Config(uint32_t baudrate);
// extern void User_USART1_IRQHandler();
// extern void USART_SendBytes(uint8_t *data, uint16_t len);
// extern uint16_t USART_ReadBytes(uint8_t *data, uint16_t maxlen);

// #ifdef __cplusplus
// }
// #endif
// #endif
