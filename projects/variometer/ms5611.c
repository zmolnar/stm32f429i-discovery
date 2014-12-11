/* ms5611.c --- 
 * 
 * @file ms5611.c
 * @brief MS5611 barometric pressure sensor driver interface
 * @author Zoltán Molnár
 * @date Wed Nov 19 15:39:36 2014 (+0100)
 * Version: 
 * Last-Updated: Thu Dec 11 10:44:37 2014 (+0100)
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
#include "ms5611.h"

/*******************************************************************************/
/* DEFINED CONSTANTS                                                           */
/*******************************************************************************/

/*******************************************************************************/
/* MACRO DEFINITIONS                                                           */
/*******************************************************************************/

/*******************************************************************************/
/* TYPE DEFINITIONS                                                            */
/*******************************************************************************/
/**
 * Enumeration to select which parameter to read from MS5611
 */
typedef enum bps_data {
    BPS_PRESSURE,
    BPS_TEMP
} bps_data_t;

/**
 * MS5611 command set
 * @defgroup MS5611 pressure sensor command set
 * @{
 */
typedef enum bps_command {
    MS5611_CMD_RESET              = 0x1e, /**< Reset command. */
    MS5611_CMD_CONV_D1_OSR_256    = 0x40, /**< Convert P OSR=256. */
    MS5611_CMD_CONV_D1_OSR_512    = 0x42, /**< Convert P OSR=512. */
    MS5611_CMD_CONV_D1_OSR_1024   = 0x44, /**< Convert P OSR=1024. */
    MS5611_CMD_CONV_D1_OSR_2048   = 0x46, /**< Convert P OSR=2048. */
    MS5611_CMD_CONV_D1_OSR_4096   = 0x48, /**< Convert P OSR=4096. */
    MS5611_CMD_CONV_D2_OSR_256    = 0x50, /**< Convert T OSR=256. */
    MS5611_CMD_CONV_D2_OSR_512    = 0x52, /**< Convert T OSR=512. */
    MS5611_CMD_CONV_D2_OSR_1024   = 0x54, /**< Convert T OSR=1024. */
    MS5611_CMD_CONV_D2_OSR_2048   = 0x56, /**< Convert T OSR=2048. */
    MS5611_CMD_CONV_D2_OSR_4096   = 0x58, /**< Convert T OSR=4096. */
    MS5611_CMD_ADC_READ           = 0x00, /**< Read conversation result. */
    MS5611_CMD_PROM_READ_BASE     = 0xa0  /**< Read PROM register. */
} bps_command_t;
/** @} */

/**
 * MS5611 PROM internal registers containing calibration data
 */
typedef enum bps_prom_register {
    MS5611_PROM_FACT = 0x00,
    MS5611_PROM_C1   = 0x01,
    MS5611_PROM_C2   = 0x02,
    MS5611_PROM_C3   = 0x03,
    MS5611_PROM_C4   = 0x04,
    MS5611_PROM_C5   = 0x05,
    MS5611_PROM_C6   = 0x06,
    MS5611_PROM_CRC  = 0x07
} bps_prom_register;

/**
 * Macro to calculate command byte for reading specific PROM register
 */
#define MS5611_CMD_PROM_READ(reg) (MS5611_CMD_PROM_READ_BASE | ((reg)<<1))

/*******************************************************************************/
/* DEFINITIONS OF GLOBAL CONSTANTS AND VARIABLES                               */
/*******************************************************************************/
/**
 * SPI driver configuration
 * @brief MS5611 SPI configuration structure
 */
const SPIConfig bps_spi_cfg = {
    NULL,
    BPS_SPI_NCS_PORT,
    BPS_SPI_NCS_PAD,
    (((0x5 << 3) & SPI_CR1_BR)    |
     SPI_CR1_MSTR                 | 
     SPI_CR1_CPOL                 |  
     SPI_CR1_CPHA)
};
    
/**
 * Factory set calibration constants
 * @defgroup Calibration constants
 * @{
 */
static uint16_t C1 = 0;  /**< Pressure sensitivity | SENS T1 */
static uint16_t C2 = 0;  /**< Pressure offset | OFF T1 */
static uint16_t C3 = 0;  /**< Temperature coefficient of pressure sensitivity | TCS */
static uint16_t C4 = 0;  /**< Temperature coefficient of pressure offset | TCO */
static uint16_t C5 = 0;  /**< Reference temperature | T REF */
static uint16_t C6 = 0;  /**< Temperature coefficient of the temperature | TEMPSENS */
/** @} */

/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                               */
/*******************************************************************************/
/**
 * Send reset sequence to the MS5611
 */
static void _bps_reset (void)
{
    uint8_t cmd = MS5611_CMD_RESET;

    spiSelect (BPS_SPI);
    spiSend(BPS_SPI, 1, (void *)&cmd);
    chThdSleepMilliseconds(5);
    spiUnselect( BPS_SPI);    
}

/**
 * Read PROM register from MS5611.
 *
 * @param[in] Reg Identifier of the register to read
 * @retval Register value
 */
static uint32_t _bps_read_reg (const bps_prom_register Reg)
{
    uint8_t cmd = MS5611_CMD_PROM_READ(Reg);
    uint8_t tmp[2];

    spiSelect (BPS_SPI);
    spiSend (BPS_SPI, 1, &cmd);
    spiReceive (BPS_SPI, 2, tmp); 
    spiUnselect (BPS_SPI);
    
    /* Swap bytes */
    return (((uint16_t)tmp[0]) << 8) + (uint16_t)tmp[1];
}

/**
 * Read measurement data from MS5611
 * @brief Start conversation and read result from the sensor.
 * @param[in] data Identifier of the desired parameter to read
 *            BPS_PRESSURE : read uncompensated pressure
 *            BPS_TEMP     : read uncompensated temperature
 * @return The result of the conversation
 */
static uint32_t _bps_convert (bps_data_t data)
{
    uint8_t cmd = 0;
    uint8_t tmp[3];
    
    switch (data) {
    case BPS_PRESSURE: {
        cmd = MS5611_CMD_CONV_D1_OSR_4096;
        break;
    }
    case BPS_TEMP: {
        cmd = MS5611_CMD_CONV_D2_OSR_4096;
        break;
    }
    default: 
        return 0;
    }

    /* Start conversation */
    spiSelect (BPS_SPI);
    spiSend (BPS_SPI, 1, (void *)&cmd);
    spiUnselect (BPS_SPI);
    chThdSleepMilliseconds(10);

    /* Read result */
    cmd = MS5611_CMD_ADC_READ;
    spiSelect (BPS_SPI);
    spiSend (BPS_SPI, 1, &cmd);
    spiReceive (BPS_SPI, 3, tmp);
    spiUnselect (BPS_SPI);

    /* Swap byte order */
    return (((uint32_t)tmp[0]) << 16) + (((uint32_t)tmp[1]) << 8) + (uint32_t)tmp[2];
}

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
void bpsInit (void)
{
    /* Configure NCS */
    palSetPad (BPS_SPI_NCS_PORT, BPS_SPI_NCS_PAD);
    palSetPadMode (BPS_SPI_NCS_PORT, BPS_SPI_NCS_PAD,
                   PAL_MODE_OUTPUT_PUSHPULL |
                   PAL_STM32_OSPEED_LOWEST);

    /* Configure SCK */
    palSetPadMode (BPS_SPI_SCK_PORT, BPS_SPI_SCK_PAD,
                   PAL_MODE_ALTERNATE(5)    |
                   PAL_STM32_OTYPE_PUSHPULL |
                   PAL_STM32_OSPEED_MID2);

    /* Configure MISO */
    palSetPadMode (BPS_SPI_MISO_PORT, BPS_SPI_MISO_PAD,
                   PAL_MODE_ALTERNATE(5));

    /* Configure MOSI */
    palSetPadMode (BPS_SPI_MOSI_PORT, BPS_SPI_MOSI_PAD,
                   PAL_MODE_ALTERNATE(5)    |
                   PAL_STM32_OTYPE_PUSHPULL |
                   PAL_STM32_OSPEED_MID2);

    spiObjectInit(BPS_SPI);
    spiStart (BPS_SPI, &bps_spi_cfg);
}

void bpsReset (void)
{
    _bps_reset ();
    C1 = _bps_read_reg (MS5611_PROM_C1);
    C2 = _bps_read_reg (MS5611_PROM_C2);
    C3 = _bps_read_reg (MS5611_PROM_C3);
    C4 = _bps_read_reg (MS5611_PROM_C4);
    C5 = _bps_read_reg (MS5611_PROM_C5);
    C6 = _bps_read_reg (MS5611_PROM_C6);
}

void bpsMeasure (int32_t *pP, int32_t *pT)
{
    uint32_t D1 = _bps_convert (BPS_PRESSURE);
    uint32_t D2 = _bps_convert (BPS_TEMP);
    int64_t dT = (int64_t)D2 - ((uint64_t)C5 << 8);
    int64_t TEMP = 2000 + ((dT * (int64_t)C6) >> 23);
    int64_t OFF = ((uint64_t)C2 << 16) + (((int64_t)C4 * dT) >> 7);
    int64_t SENS = ((int64_t)C1 << 15) + ((dT * (int64_t)(C3) >> 8));
    int64_t T2 = 0;;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    /* Second order temperature compensation. */
    if (TEMP < 20) {
        T2 = ((dT * dT) >> 31);
        OFF2 = (5 * (TEMP - 2000) * (TEMP - 2000)) >> 1;
        SENS2 = (5 * (TEMP - 2000) * (TEMP - 2000)) >> 2;

        /* Very low temperature. */
        if( TEMP < (-15)) {
            OFF2 = OFF2 + (7 * (TEMP + 1500) * (TEMP + 1500));
            SENS2 = SENS2 + ((11 * (TEMP + 1500) * (TEMP + 1500)) >> 2);
        }
    }
    else {
        T2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }
    
    TEMP -= T2;
    OFF -= OFF2;
    SENS -= SENS2;

    /* Calculate temperature and temperature compensated pressure */
    *pT = (int32_t)TEMP;
    *pP = (int32_t)(((((int64_t)D1 * SENS) >> 21) - OFF) >> 15);
}
/******************************* END OF FILE ***********************************/

