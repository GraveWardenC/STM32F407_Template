#include "stm32f4xx_it.h"
#include"systick.h"


/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Hard fault interrupt.
 */

void HardFault_Handler(void)
{
    asm __volatile (
        "tst    LR, #4\n"           // Check EXC_RETURN in Link register bit 2.
        "ite    EQ\n"
        "mrseq  R0, MSP\n"            // Stacking was using MSP.
        "mrsne  R0, PSP\n"            // Stacking was using PSP.
        "b      HardFaultHandler"   // Stack pointer passed through R0.
    );

}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{

    while (1)
    {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{

}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    sysIncTick();
}



