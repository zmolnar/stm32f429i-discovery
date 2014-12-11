/* thrDisplay.h --- 
 * 
 * @file thrDisplay.h
 * @brief Display handler thread header file
 * @author Zoltán Molnár
 * @date Mon Nov 17 16:21:04 2014 (+0100)
 * Version: 
 * Last-Updated: Thu Dec 11 10:38:53 2014 (+0100)
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

#ifndef __THRDISPLAY_H
#define __THRDISPLAY_H

/*******************************************************************************/
/* INCLUDES                                                                    */
/*******************************************************************************/

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
/* DECLARATIONS OF GLOBAL VARIABLES                                           */
/*******************************************************************************/

/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
/*
 * Display handler thread
 */
msg_t ThrDisplay( void *arg);

#endif /* THRDISPLAY_H */

/******************************* END OF FILE ***********************************/

