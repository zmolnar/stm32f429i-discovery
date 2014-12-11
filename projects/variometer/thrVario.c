/* thrVario.c --- 
 * 
 * @file thrVario.c
 * @brief Variometer module
 * @author Zoltán Molnár
 * @date Thu Nov 20 10:55:30 2014 (+0100)
 * Version: 
 * Last-Updated: Thu Dec 11 10:45:13 2014 (+0100)
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
#include "ch.h"
#include "chprintf.h"
#include "gfx.h"
#include "ms5611.h"
#include <math.h>
#include <stdint.h>

/*******************************************************************************/
/* DEFINED CONSTANTS                                                           */
/*******************************************************************************/
extern SerialUSBDriver SDU1;

/*******************************************************************************/
/* MACRO DEFINITIONS                                                           */
/*******************************************************************************/

/*******************************************************************************/
/* TYPE DEFINITIONS                                                            */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITIONS OF GLOBAL CONSTANTS AND VARIABLES                               */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                               */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
/* See documentation in header file */
msg_t ThrVario( void *arg)
{
    (void)arg;
    chRegSetThreadName("Variometer");   

    /* Initialize and reset MS5611 */
    bpsInit ();
    bpsReset ();
    
    while (1) {
        int32_t p_raw = 0;
        int32_t t_raw = 0;
        
        bpsMeasure (&p_raw, &t_raw);
        
        chprintf((BaseSequentialStream *)&SDU1,"%d %d\n\r", chTimeNow(), p_raw);
    }
}


/******************************* END OF FILE ***********************************/

