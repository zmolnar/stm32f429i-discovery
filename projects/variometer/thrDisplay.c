/* thrDisplay.c --- 
 * 
 * @file thrDisplay.c
 * @brief Display handler thread
 * @author Zoltán Molnár
 * @date Mon Nov 17 16:14:01 2014 (+0100)
 * Version: 
 * Last-Updated: Thu Dec 11 10:44:53 2014 (+0100)
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

/*******************************************************************************/
/* DEFINED CONSTANTS                                                           */
/*******************************************************************************/

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
void create_widgets(void) {
    // TODO: implement widgets
}

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
msg_t ThrDisplay( void *arg) {
    create_widgets();
    
    while(TRUE) {
        chThdSleepMilliseconds(1000);
    }
    
    return 0;
}
  
/******************************* END OF FILE ***********************************/

