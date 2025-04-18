//////////////////////////////////////////////////////////////////////////
/// Copyright, (c) Wuhan Fine Visual Healthcare Inc., 2022
/// All rights reserved.
///
/// \file systick.c
///
/// \author chenkai(mailto: chenkai@visualfine.com)
///
/// \version 1.0
///
/// \date 25-4-17
/////////////////////////////////////////////////////////////////////////

#include "systick.h"
#include <stm32f4xx.h>

#define MAX_DELAY 0xFFFFFFFFU
#define TICK_INT_PRIORITY 15U /*!< tick interrupt priority */

volatile uint32_t uwTick;/* Tick */

uint32_t uwTickPrio = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
SYS_TICK_FREQ uwTickFreq = SYS_TICK_FREQ_DEFAULT; /* 1KHz */

static inline uint32_t sysGetTick();

/**
 * @brief Initializes the system tick timer with a specified priority.
 *
 * This function is used to configure the system tick timer with a given priority. The tick
 * timer is essential for timekeeping operations in embedded systems. The function is declared
 * as weak, which means that it can be overridden by a function with the same name defined in
 * another module or file. If no overriding function is provided, the weak function will be
 * used as a default.
 *
 * @param TickPriority The priority level for the tick timer interrupt. A lower value indicates
 *                     a higher priority.
 *
 * @return bool Returns `true` if the tick timer was successfully initialized,
 *              `false` otherwise.
 *
 * @note This function is typically used in embedded applications where precise time control
 *       is necessary.
 *
 * @weak
 */
__attribute__((weak)) bool sysInitTick(uint32_t TickPriority)
{
    /* Configure the SysTick to have interrupt in 1ms time basis*/
    if (SysTick_Config(SystemCoreClock / (1000U / uwTickFreq)) > 0U)
    {
        return false;
    }

    /* Configure the SysTick IRQ priority */
    if (TickPriority < (1UL << __NVIC_PRIO_BITS))
    {
        uint32_t priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TickPriority, 0UL);
        NVIC_SetPriority(SysTick_IRQn, priority);
        uwTickPrio = TickPriority;
    }
    else
    {
        return false;
    }

    /* Return function status */
    return true;
}

/**
 * @brief Delays execution for a specified number of milliseconds.
 *
 * This function causes a delay by blocking execution for a specified number of milliseconds.
 * It is commonly used in embedded systems to introduce time delays for various tasks such as
 * waiting for hardware to respond or creating periodic time intervals. This function is typically
 * used when a simple blocking delay is required, and the system does not need to perform other
 * tasks during the delay.
 *
 * @param delay The delay duration in milliseconds.
 *
 * @return void This function does not return a value. It simply blocks execution for the
 *              specified duration.
 *
 * @note This function may rely on a system timer or a busy-wait loop to generate the delay.
 *       The accuracy and efficiency of the delay may vary depending on the implementation.
 */
void sysDelayMs(uint32_t delay)
{
    uint32_t tickstart = sysGetTick();
    uint32_t wait = delay;

    /* Add a freq to guarantee minimum wait */
    if (wait < MAX_DELAY)
    {
        wait += (uint32_t) (uwTickFreq);
    }

    while ((sysGetTick() - tickstart) < wait)
    {
    }
}

/**
 * @brief Retrieves the current system tick value.
 *
 * This function returns the current value of the system tick counter, which is typically
 * incremented by an interrupt handler (e.g., SysTick or a timer interrupt). The tick counter
 * is used for tracking time in the system and may be used for various time-based operations
 * such as delays, timeouts, or scheduling.
 *
 * @return uint32_t The current value of the system tick counter.
 *
 * @note This function is usually called when the system needs to check the current tick count
 *       for purposes such as calculating time intervals or triggering events based on elapsed time.
 */
static inline uint32_t sysGetTick() { return uwTick; }

/**
 * @brief Increments the system tick counter by the tick frequency.
 *
 * This function increments the system tick counter `uwTick` by the frequency value `uwTickFreq`,
 * which represents the number of ticks per unit of time. This is typically called within a timer
 * interrupt or a periodic function to simulate the passage of time. The function is declared
 * as weak, meaning it can be overridden by another definition of the same name in another module.
 *
 * @return void This function does not return a value.
 *
 * @note The weak attribute allows for a default implementation of the tick increment, but this
 *       function can be overridden if a different mechanism is used to increment the tick
 *       counter (e.g., if a custom timer or interrupt is implemented).
 *
 * @weak
 */
__attribute__((weak)) void sysIncTick(void) { uwTick += uwTickFreq; }


