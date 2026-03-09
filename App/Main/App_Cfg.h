/**
 * @file App_Cfg.h
 * @brief Application Test Mode configuration.
 */

#ifndef APP_CFG_H
#define APP_CFG_H

/* Available Test Modes */
#define TEST_MODE_GTM 0  /* GTM TOM based task scheduler */
#define TEST_MODE_STM 1  /* STM based periodic interrupts */

/* Selection of current mode */
#define CURRENT_TEST_MODE TEST_MODE_STM

#endif /* APP_CFG_H */
