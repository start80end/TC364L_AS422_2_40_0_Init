/**
 * @file Cpu0_Main.c
 * @brief Main entry point for Core 0.
 *
 * This file handles System Initialization, Clock distribution, Port setup,
 * and starts the selected timer test mode (GTM or STM).
 */

#include "Ifx_Os.h"
#include "Ifx_Ssw_Infra.h"
#include "Ifx_Ssw.h"

#include "App_Cfg.h"
#include "Dio.h"
#include "Gpt.h"
#include "IfxSrc_reg.h"
#include "Irq.h"
#include "LedToggleTask.h"
#include "Mcu.h"
#include "Port.h"
#include "TaskScheduler.h"

#if (CURRENT_TEST_MODE == TEST_MODE_STM)
#include "StmTask.h"
#endif

#include "Mcu_PBcfg.h"
#include "Port_PBcfg.h"

/**
 * @brief Main function for Core 0
 */
void core0_main(void) {
    unsigned short cpuWdtPassword;
    unsigned short safetyWdtPassword;

    /* Enable interrupts */
    ENABLE();

    /* Disable Watchdogs for debugging/demo purposes */
    cpuWdtPassword = Ifx_Ssw_getCpuWatchdogPassword(&MODULE_SCU.WDTCPU[0]);
    safetyWdtPassword = Ifx_Ssw_getSafetyWatchdogPassword();
    Ifx_Ssw_disableCpuWatchdog(&MODULE_SCU.WDTCPU[0], cpuWdtPassword);
    Ifx_Ssw_disableSafetyWatchdog(safetyWdtPassword);

    /* Phase 1: MCU Initialization (Clock & Reset) */
    Mcu_Init(&Mcu_Config);
    if (Mcu_InitClock(0) == E_OK) {
        while (Mcu_GetPllStatus() != MCU_PLL_LOCKED) {
            /* Wait for PLL Stability */
        }
        Mcu_DistributePllClock();
    }

    /* Phase 2: Port/GPIO Initialization */
    Port_Init(&Port_Config);

    /* Phase 3: Start Selected Test Mode */
#if (CURRENT_TEST_MODE == TEST_MODE_GTM)
    /* GTM TOM Header Based Task Scheduler Initialization */
    Gpt_Init(&Gpt_Config);
    IrqGtm_Init();
    
    /* Enable Service Request for TOM0_CH0 used by scheduler */
    SRC_GTMTOM00.B.SRE = 1; 

    /* Initialize scheduler with a max cycle of 1000ms */
    TaskScheduler_Initialization(TASK_1000ms);

    while (1) {
        /* Continuously check and trigger tasks in scheduler loop */
        TaskScheduler_ActivateTask();
    }

#elif (CURRENT_TEST_MODE == TEST_MODE_STM)
    /* System Timer (STM) Based Periodic Interrupts */
    StmTask_Init();

    while (1) {
        /* Idle loop: logic is handled within STM ISRs */
        __nop();
    }
#endif
}
