/* ms5611.h --- 
 * 
 * @file ms5611.h
 * @brief MS5611 barometric pressure sensor driver interface
 * @author Zoltán Molnár
 * @date Wed Nov 19 15:40:09 2014 (+0100)
 * Version: 
 * Last-Updated: Thu Dec 11 10:44:16 2014 (+0100)
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

#ifndef __MS5611_H
#define __MS5611_H

/*******************************************************************************/
/* INCLUDES                                                                    */
/*******************************************************************************/
#include "ch.h"
#include "hal.h"

/*******************************************************************************/
/* DEFINED CONSTANTS                                                           */
/*******************************************************************************/
/**
 * SPI peripheral identifier
 * @defgroup MS5611 serial interface
 * @{
 */
#define BPS_SPI                  &SPID4 /**< SPI peripheral indentifier. */
#define BPS_SPI_MOSI_PORT         GPIOE /**< SPI MOSI pin port. */
#define BPS_SPI_MOSI_PAD              6 /**< SPI MOSI pin pad. */
#define BPS_SPI_MISO_PORT         GPIOE /**< SPI MISO pin port. */
#define BPS_SPI_MISO_PAD              5 /**< SPI MISO pin pad. */
#define BPS_SPI_SCK_PORT          GPIOE /**< SPI SCK pin port. */
#define BPS_SPI_SCK_PAD               2 /**< SPI SCK pin pad. */
#define BPS_SPI_NCS_PORT          GPIOE /**< SPI slave-select pin port. */
#define BPS_SPI_NCS_PAD               4 /**< SPI slave-select pin pad. */
/** @} */

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
/**
 * Initialize MS5611 serial interface
 */
void bpsInit (void);

/**
 * Send reset command and read calibration data from MS5611
 */
void bpsReset (void);

/**
 * Read raw pressure and temperature values from MS5611
 * @param[in] pP Pointer to the variable to store temperature compensated 
 *               raw pressure value
 * @param[in] pT Pointer to the variable to store raw temperature value
 */
void bpsMeasure (int32_t *pP, int32_t *pT);

#endif /* MS5611_H */

/******************************* END OF FILE ***********************************/

