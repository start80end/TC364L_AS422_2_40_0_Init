/**
 * @file StmTask.h
 * @brief STM0 Timer based periodic tasks header.
 * 
 * Configures STM0 comparators CMP0 and CMP1 for periodic interrupt generation.
 */

#ifndef STM_TASK_H
#define STM_TASK_H

#include "Std_Types.h"

/**
 * @brief Initialize STM0 comparators for periodic interrupt intervals.
 */
void StmTask_Init(void);

#endif /* STM_TASK_H */
