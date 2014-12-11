/* main.c --- 
 * 
 * @file main.c
 * @brief Variometer main function
 * @author Zoltán Molnár
 * @date Mon Nov 17 16:21:40 2014 (+0100)
 * Version: 
 * Last-Updated: Thu Dec 11 10:48:13 2014 (+0100)
 *           By: Zoltán Molnár
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/*******************************************************************************/
/* INCLUDES                                                                    */
/*******************************************************************************/
#include "gfx.h"
#include "shell.h"
#include "test.h"
#include "thrDisplay.h"
#include "thrVario.h"
#include "usbcfg.h"
#include "chprintf.h"

/*******************************************************************************/
/* DEFINED CONSTANTS                                                           */
/*******************************************************************************/
#define SHELL_WA_SIZE   THD_WA_SIZE(2048)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

/*******************************************************************************/
/* MACRO DEFINITIONS                                                           */
/*******************************************************************************/

/*******************************************************************************/
/* TYPE DEFINITIONS                                                            */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITIONS OF GLOBAL CONSTANTS AND VARIABLES                               */
/*******************************************************************************/
/* Virtual serial port over USB.*/
SerialUSBDriver SDU1;

/* Thread working areas */
static WORKING_AREA(waThrBlinker, 128);
static WORKING_AREA(waThrDisp, 2048);
static WORKING_AREA(waThrVario, 2048);

/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                               */
/*******************************************************************************/
/*
 * Red LED blinker thread, times are in milliseconds.
 */
static msg_t ThrBlinker(void *arg) {

    (void)arg;
    chRegSetThreadName("Red blinker");
    while (TRUE) {
        palClearPad(GPIOG, GPIOG_LED4_RED);
        chThdSleepMilliseconds(500);
        palSetPad(GPIOG, GPIOG_LED4_RED);
        chThdSleepMilliseconds(500);
    }

    return 0;
}

/* 
 * Shell command
 */
static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
    size_t n, size;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
    chprintf(chp, "heap fragments   : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

/* 
 * Shell command
 */
static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
    static const char *states[] = {THD_STATE_NAMES};
    Thread *tp;
    
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "    addr    stack prio refs     state time\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
                 (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                 (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                 states[tp->p_state], (uint32_t)tp->p_time);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
}

/* 
 * Shell command
 */
static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
    Thread *tp;
    
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: test\r\n");
        return;
    }
    tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriority(),
                             TestThread, chp);
    if (tp == NULL) {
        chprintf(chp, "out of memory\r\n");
        return;
    }
    chThdWait(tp);
}

/* 
 * Shell command configuration
 */
static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"test", cmd_test},
    {NULL, NULL}
};

/* 
 * Shell configuration
 */
static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *)&SDU1,
    commands
};

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
/*
 * Program entry point
 */
int main(void) {
    Thread *shelltp = NULL;

    // Initialize the display
    gfxInit();

    /*
     * Initializes a serial-over-USB CDC driver.
     */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    /*
     * Creating the threads.
     */
    chThdCreateStatic(waThrBlinker, sizeof(waThrBlinker),
                      NORMALPRIO + 11, ThrBlinker, NULL);
    chThdCreateStatic(waThrDisp, sizeof(waThrDisp), 
                      NORMALPRIO + 10, ThrDisplay, NULL);
    chThdCreateStatic(waThrVario, sizeof(waThrVario), 
                      NORMALPRIO + 10, ThrVario, NULL);

    while(1) {
        if (!shelltp) {
            if (SDU1.config->usbp->state == USB_ACTIVE) {
                /* Spawns a new shell.*/
                shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
            }
        }
        else {
            /* If the previous shell exited.*/
            if (chThdTerminated(shelltp)) {
                /* Recovers memory of the previous shell.*/
                chThdRelease(shelltp);
                shelltp = NULL;
            }
        }
        chThdSleepMilliseconds(1000);
    }
  
    return 0;
}


/******************************* END OF FILE ***********************************/

