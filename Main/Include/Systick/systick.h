//////////////////////////////////////////////////////////////////////////
/// Copyright, (c) Wuhan Fine Visual Healthcare Inc., 2022
/// All rights reserved.
///
/// \file systick.h
///
/// \author chenkai(mailto: chenkai@visualfine.com)
///
/// \version 1.0
///
/// \date 25-4-17
/////////////////////////////////////////////////////////////////////////

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdbool.h>
#include <stdint.h>


typedef enum
{
    SYS_TICK_FREQ_10HZ = 100U,
    SYS_TICK_FREQ_100HZ = 10U,
    SYS_TICK_FREQ_1KHZ = 1U,
    SYS_TICK_FREQ_DEFAULT = SYS_TICK_FREQ_1KHZ
} SYS_TICK_FREQ;


extern __attribute__((weak)) void sysIncTick(void);/* Systick Irq*/
extern __attribute__((weak)) bool sysInitTick(uint32_t TickPriority);
extern void sysDelayMs(uint32_t delay);
#endif // SYSTICK_H
