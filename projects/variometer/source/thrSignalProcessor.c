/* thrSignalProcessor.c --- 
 * 
 * @file thrSignalProcessing.c
 * @brief Signal processing module.
 * @author Molnár Zoltán
 * @date Sun Nov 22 13:16:41 2015 (+0100)
 * Version: 1.0.0
 * Last-Updated: Sun Nov 22 17:58:48 2015 (+0100)
 *           By: Molnár Zoltán
 * 
 */

/*******************************************************************************/
/* INCLUDES                                                                    */
/*******************************************************************************/
#include "thrSignalProcessor.h"
#include "thrPressureReader.h"

#include "chprintf.h"
#include "gfx.h"
#include <math.h>
#include <stdint.h>

/*******************************************************************************/
/* DEFINED CONSTANTS                                                           */
/*******************************************************************************/
#define ALPHA                                                                 0.2
#define BETA                                                                0.005
#define BUFLENGTH                                                             100

/*******************************************************************************/
/* MACRO DEFINITIONS                                                           */
/*******************************************************************************/


/*******************************************************************************/
/* TYPE DEFINITIONS                                                            */
/*******************************************************************************/
extern SerialUSBDriver SDU1;

/*******************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                                */
/*******************************************************************************/


/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                               */
/*******************************************************************************/
static float ab_filter (float a, float b, 
                        float *xk_1, float *vk_1, 
                        int32_t x, float dt)
{
        float xk = *xk_1 + (*vk_1 * dt);
        float vk = *vk_1;

        float rk = x - xk;

        xk += a * rk;
        vk += b * rk / dt;

        *xk_1 = xk;
        *vk_1 = vk;

        return xk;
}

static float calc_slope (float *buf, size_t blength, 
                         size_t start, size_t dlength, float dt)
{
        float x_avg = 0;
        float y_avg = 0;
        size_t i;
        for ( i = 0; i < dlength; i++) {
                size_t j;
                if (start + i < blength)
                        j = start + i;
                else
                        j = i - blength + start;
                
                x_avg += i * dt;
                y_avg += buf[j];
        }

        x_avg /= dlength;
        y_avg /= dlength;

        float num = 0;
        float den = 0;
        for (i = 0; i < dlength; i++) {
                size_t j;
                if (start + i < blength)
                        j = start + i;
                else
                        j = i - blength + start;
                num += (i*dt - x_avg) * (buf[j] - y_avg);
                den += (i*dt - x_avg) * (i*dt - x_avg);
        }

        return num/den;
}

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
msg_t ThrSignalProcessor (void *arg) {
        (void)arg;
        static float xi_1;
        static float vi_1;
        static float ti_1;
        float hbuf[BUFLENGTH] = {0};
        size_t dlength = 0;
        size_t i = 0;

        Thread *sender = chMsgWait ();
        msg_t msg = chMsgGet (sender);
        chMsgRelease (sender, 0);        

        struct PressureData_s *pdata = (struct PressureData_s *) msg;
        xi_1 = (float)pdata->p_raw;
        vi_1 = 0;
        ti_1 = (float)pdata->t;

        while (1) {
                sender = chMsgWait ();
                msg = chMsgGet (sender);
                chMsgRelease (sender, 0);

                pdata = (struct PressureData_s *) msg;
                float dt = ((float)pdata->t - ti_1) / 1000;
                float p = ab_filter (ALPHA, BETA, &xi_1, &vi_1, pdata->p_raw, dt);
                float h = 44330 * (1 - pow ((p / 101325.0), 0.1902));

                hbuf[i] = h;
                if (i < (BUFLENGTH - 1))
                        i++;
                else
                        i = 0;
                
                if (dlength < BUFLENGTH) dlength++;                       

                float v = calc_slope (hbuf, BUFLENGTH, i, dlength, dt);

                ti_1 = (float)pdata->t;

                chprintf ((BaseSequentialStream *)&SDU1,"%d %d %04d\n", 
                          pdata->t, (int32_t)(100*h), (int32_t)(100*v));
        }
}


/******************************* END OF FILE ***********************************/

