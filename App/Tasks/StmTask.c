/**
 * @file StmTask.c
 * @brief STM0 Free-running timer with comparator match interrupts.
 *
 * This implementation provides periodic tasks based on the System Timer (STM).
 * - CMP0: 100ms interval (P33.4 Toggle)
 * - CMP1: 500ms interval (P33.5 Toggle)
 */

#include "StmTask.h"
#include "Dio.h"
#include "IFX_Os.h"
#include "IfxSrc_reg.h"
#include "McalLib.h"
#include "Mcu_17_TimerIp.h"
#include "Irq.h"

/* Clock Frequency = 100MHz (1 Tick = 10ns) */
#define STM_TICKS_PER_100MS  (10000000UL)
#define STM_TICKS_PER_500MS  (50000000UL)

/* STM Module Constants */
#define STM_INSTANCE_0 (0U)
#define STM_CMP0       (0U)
#define STM_CMP1       (1U)

/* Next tick holders for comparator reload */
static uint32 Stm0_Cmp0_NextTick;
static uint32 Stm0_Cmp1_NextTick;

/**
 * @brief Initializes STM0 comparators.
 */
void StmTask_Init(void) {
    uint32 currentTick;
    Mcu_17_Stm_TimerConfigType stm_config;

    /* Get current system tick from STM0 */
    currentTick = Mcal_DelayGetTick();

    /* Initialize CMP0 for 100ms */
    Stm0_Cmp0_NextTick = currentTick + STM_TICKS_PER_100MS;
    stm_config.StmTimerId = STM_INSTANCE_0;
    stm_config.CMPRegId = STM_CMP0;
    stm_config.CompareRegVal = Stm0_Cmp0_NextTick;
    stm_config.CmconRegVal = 31U; /* Compare lower 32-bits */
    Mcu_17_Stm_SetupComparator(&stm_config);

    /* Initialize CMP1 for 500ms */
    Stm0_Cmp1_NextTick = currentTick + STM_TICKS_PER_500MS;
    stm_config.CMPRegId = STM_CMP1;
    stm_config.CompareRegVal = Stm0_Cmp1_NextTick;
    Mcu_17_Stm_SetupComparator(&stm_config);

    /* Directly configure Service Request Control registers for STM0 */
    /* SRE: Service Request Enable (Bit 10) */
    /* SRPN: Service Request Priority Number (Bit 0-7) */
    SRC_STM0SR0.U = 0x0000040AUL; /* Enable SR0 with Priority 10 */
    SRC_STM0SR1.U = 0x0000040BUL; /* Enable SR1 with Priority 11 */
}

/**
 * @brief STM0 CMP0 ISR: 100ms Periodic Task
 */
IFX_INTERRUPT(Stm0_Sr0_Isr, 0, 10) {
    Mcu_17_Stm_TimerConfigType stm_config;

    /* Toggle LED1 */
    (void)Dio_FlipChannel(DioConf_DioChannel_LED1);

    /* Schedule next 100ms interrupt */
    Stm0_Cmp0_NextTick += STM_TICKS_PER_100MS;
    stm_config.StmTimerId = STM_INSTANCE_0;
    stm_config.CMPRegId = STM_CMP0;
    stm_config.CompareRegVal = Stm0_Cmp0_NextTick;
    stm_config.CmconRegVal = 31U;
    Mcu_17_Stm_SetupComparator(&stm_config);
}

/**
 * @brief STM0 CMP1 ISR: 500ms Periodic Task
 */
IFX_INTERRUPT(Stm0_Sr1_Isr, 0, 11) {
    Mcu_17_Stm_TimerConfigType stm_config;

    /* Toggle LED2 */
    (void)Dio_FlipChannel(DioConf_DioChannel_LED2);

    /* Schedule next 500ms interrupt */
    Stm0_Cmp1_NextTick += STM_TICKS_PER_500MS;
    stm_config.StmTimerId = STM_INSTANCE_0;
    stm_config.CMPRegId = STM_CMP1;
    stm_config.CompareRegVal = Stm0_Cmp1_NextTick;
    stm_config.CmconRegVal = 31U;
    Mcu_17_Stm_SetupComparator(&stm_config);
}
