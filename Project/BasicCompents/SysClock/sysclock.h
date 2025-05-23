//////////////////////////////////////////////////////////////////////////
/// Copyright, (c) Wuhan Fine Visual Healthcare Inc., 2022
/// All rights reserved.
///
/// \file sysclock.h
///
/// \author kai.chen(mailto: chenkai@visualfine.com)
///
/// \version 1.0
///
/// \date 16/04/2025
/////////////////////////////////////////////////////////////////////////

#ifndef SYSCLOCK_H
#define SYSCLOCK_H

#include<stdint.h>
#include"common_esw_errno.h"

typedef uint32_t errorcode_t;

typedef enum
{
    USE_HSI = 0,
    USE_HSE = 1,
}PLL_CLK_SOURCE;

typedef struct
{
    uint32_t userSysClkFrequency; /*!<  SYSCLK clock frequency expressed in Hz */
    uint32_t userHclkFrequency;   /*!<  HCLK clock frequency expressed in Hz   */
    uint32_t userPclk1Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
    uint32_t userPclk2Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
    PLL_CLK_SOURCE pllSource;
}SYSTEM_CLK;



/**
 * @brief  Configures the system clock to run at 168 MHz using PLL.
 *         Tries to use HSE (external crystal) as the PLL source first.
 *         If HSE fails, falls back to HSI (internal RC oscillator).
 *
 * @param[in,out] sysclk Pointer to a SYSTEM_CLK structure to store configured clock frequencies.
 *                       Must not be NULL.
 *
 * @return ERRNO_SUCCESS      if configuration is successful using HSE.
 * @return ERRNO_TIMEOUT      if HSE startup failed and HSI is used instead.
 * @return ERRNO_NULL_POINTER if sysclk is NULL.
 *
 * @note   This function:
 *         - Configures Flash latency and enables caches.
 *         - Sets up HCLK, PCLK1, and PCLK2 dividers.
 *         - Configures and enables the PLL.
 *         - Switches the system clock to PLL output.
 *         - Updates the SystemCoreClock variable.
 *         - Fills in the SYSTEM_CLK structure with the configured frequencies.
 */
extern errorcode_t ConfigSysClkTo168MHZ(SYSTEM_CLK* sysclk);

extern SYSTEM_CLK g_systemClkInfo;


#endif //SYSTEMRCC_H

