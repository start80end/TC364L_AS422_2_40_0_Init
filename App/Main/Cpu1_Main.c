/**
 * @file Cpu1_Main.c
 * @brief Entry point for Core 1 (Slave core).
 *
 * Current implementation keeps Core 1 in an idle loop as Core 0 manages
 * all timer tasks.
 */

#include "Ifx_Os.h"
#include "Ifx_Ssw_Infra.h"
#include "Ifx_Ssw.h"

/**
 * @brief Main function for Core 1
 */
void core1_main(void) {
    /* Initialize interrupts for Core 1 */
    ENABLE();

    while (1) {
        /* Placeholder for Core 1 specific tasks */
        __nop();
    }
}
