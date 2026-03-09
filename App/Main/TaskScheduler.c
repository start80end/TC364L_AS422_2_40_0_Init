/**
 * @file TaskScheduler.c
 * @brief Cooperative task scheduler using GTM TOM notification.
 */

#include "TaskScheduler.h"
#include "Gpt.h"
#include "LedToggleTask.h"

/* Internal variables */
static TASK_CONTROL TaskControl;
static TASK_CONFIG  TaskConfig;

/**
 * @brief GPT Notification callback called by TOM0_CH0 interrupt.
 * Triggered every 1ms (as defined in SYSTEM_TICK_TIMER_PERIOD).
 */
void IoHwAb_GptNotification0(void) {
    TaskControl.B.TickCount++;
    TaskControl.B.Enable = 1; /* Request task execution */
}

/* Dummy callbacks required by Tresos output generation */
void IoHwAb_GptNotification1(void) {}
void Stm_Isr(unsigned int ModuleNumber, unsigned int InterruptNode) {
    (void)ModuleNumber; (void)InterruptNode;
}

/**
 * @brief Scheduler Initialization
 */
void TaskScheduler_Initialization(unsigned int maxTask) {
    TaskControl.U = 0;
    TaskConfig.TaskMax = maxTask;
    TaskConfig.SystemTick = SYSTEM_TICK;

    /* Enable GPT notification (GTM TOM0_CH0) */
    Gpt_EnableNotification(GptConf_GptChannelConfiguration_GptChannelConfiguration_0);

    /* Start GPT timer */
    Gpt_StartTimer(GptConf_GptChannelConfiguration_GptChannelConfiguration_0,
                   SYSTEM_TICK_TIMER_PERIOD);
}

/**
 * @brief Logic to decide which task should run based on TickCount.
 */
static void TaskScheduler_TaskCalculation(void) {
    uint32 count = TaskControl.B.TickCount;

    /* Check tasks based on their frequency (Modulo check) */
    if ((count % TASK_1000ms) == 0) TaskControl.B.TaskRun = TASK_1000ms;
    else if ((count % TASK_500ms) == 0)  TaskControl.B.TaskRun = TASK_500ms;
    else if ((count % TASK_100ms) == 0)  TaskControl.B.TaskRun = TASK_100ms;
    else if ((count % TASK_50ms) == 0)   TaskControl.B.TaskRun = TASK_50ms;
    else if ((count % TASK_20ms) == 0)   TaskControl.B.TaskRun = TASK_20ms;
    else if ((count % TASK_10ms) == 0)   TaskControl.B.TaskRun = TASK_10ms;
    else if ((count % TASK_5ms) == 0)    TaskControl.B.TaskRun = TASK_5ms;
    else if ((count % TASK_1ms) == 0)    TaskControl.B.TaskRun = TASK_1ms;
}

/**
 * @brief Central Task Caller
 */
void TaskScheduler_ActivateTask(void) {
    /* Reset counter if it exceeds the maximum cycle defined */
    if (TaskControl.B.TickCount >= TaskConfig.TaskMax) {
        TaskControl.B.TickCount = 0;
    }

    /* Check if 1ms tick has occurred */
    if (TaskControl.B.Enable == 1) {
        TaskControl.B.Enable = 0; /* Clear trigger flag */

        /* Determine which task to run */
        TaskScheduler_TaskCalculation();

        /* Execute Task based on priority/time */
        switch (TaskControl.B.TaskRun) {
            case TASK_1000ms: Toggle_LED2(); break; /* 1.0s interval */
            case TASK_100ms:  Toggle_LED1(); break; /* 0.1s interval */
            default: break;
        }
        
        /* Clear TaskRun state for next cycle */
        TaskControl.B.TaskRun = 0;
    }
}
