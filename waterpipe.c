/*!
**************************************************************
* @file    waterpipe.c
* @brief   BME280 Driver
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
**************************************************************
*/

/*=========================================================*/
/*== INCLUDES =============================================*/
/*=========================================================*/

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

/*=========================================================*/
/*== PRIVATE INCLUDES =====================================*/
/*=========================================================*/
#include "bme280.h"
#include "waterpipe.h"


int main()
{
    stdio_init_all();
    sleep_ms(2000);

    /*=========================================================*/
    /*== I2C SETTINGS =========================================*/
    /*=========================================================*/

    i2c_init(i2c_default, 400 * 1000);
    debugMsg("==================\r\n");
    debugMsg("INIT I2C HARDWARE: ");

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning Programm requires a board with I2C pins
#error "Seems hardware/i2c.h" is missing
    DebugMsg("Default I2C pins were not defined\r\n");
#else

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    debugMsg("[X] I2C HARDWARE SUCCESSFULLY SET [X]\r\n");
    sleep_ms(1000);
#endif

    /*=========================================================*/
    /*== GPIO SETTINGS ========================================*/
    /*=========================================================*/
    gpio_init(LED);
    debugMsg("==================\r\n");
    debugMsg("INIT GPIO HARDWARE: ");

#ifndef PICO_DEFAULT_LED_PIN
#warning Programm requires a board with a regular LED
#else
    gpio_set_dir(LED, GPIO_OUT);
    if (gpio_is_dir_out(LED) != GPIO_OUT)
    {
        debugMsg("[X] CHECK YOUR DEFAULT LED CONFIGURATION OF THE BOARD [X]\r\n");
    }
    else
    {
        __NOP();
    }
#endif
    debugMsg("[X] GPIO HARDWARE SUCCESSFULLY SET [X]\r\n");
    sleep_ms(1000);
    debugMsg("==========================\r\n");
    debugMsg("OPERATION MODE STARTED: ");
    debugModMsg;
    debugMsg("==========================\r\n");
    sleep_ms(1000);



    /*=========================================================*/
    /*== BME280 SETTINGS ======================================*/
    /*=========================================================*/
    BME280ChipID();
    BME280_ReadComp();
    BME280_SoftReset();
    BME280_SetStandby(BME280_STBY_0_5);
    BME280_Set_OSRS_h(BME280_OSRS_H_x1);
    BME280_SetFilter(BME280_FILTER_16);
    BME280_Set_OSRS_t(BME280_OSRS_T_x2);
    BME280_Set_OSRS_p(BME280_OSRS_P_x16);
    BME280_SetMode(BME280_NORMAL_MODE);

    /*==  Reading the Register Values ==*/
    BME280_Read_CTRL_MEAS();
    BME280_ReadMode();
    BME280_ReadStandby();
    BME280_Read_OSRS_h();
  

    while (true)
    {
        debugTerm();
        BME280_RawData();
        
        int32_t temperature = BME280_CompTemp();
        debugVal("[X] Temperature:%.4f °C \r\n", temperature / 100.0f);

        toggleLed();
        BME280_MeasurementTime();

        /*== TEST FUNCTION == */
        while (BME280_ReadStatus() & BME280_STATUS_IM_UPDATE)
        {
            debugMsg("=================================== STORING\n");
            //return -1;
        };

        sleep_ms(1000);
    }
    return 0;
}

/*!
**************************************************************
 * @brief 
 *
 * @param[in]  : 
 *
 * @return Result of API execution status
 *
 * @retval = 0 -> Success.
 * @retval > 0 -> Warning.
 * @retval < 0 -> Fail.
 *
 * 
**************************************************************
 */
void toggleLed()
{
    if (gpio_get_out_level(LED) != true)
    {
        gpio_put(LED, true);
        debugMsg("LED ON\r\n");
    }
    else
    {
        gpio_put(LED, false);
        debugMsg("LED OFF\r\n");
    }
}
/*!
**************************************************************
 * @brief 
 *
 * @param[in]  : 
 *
 * @return Result of API execution status
 *
 * @retval = 0 -> Success.
 * @retval > 0 -> Warning.
 * @retval < 0 -> Fail.
 *
 * 
**************************************************************
 */
void debugTerm(void)
{
    debugMsg("\n\n======================================================================\r\n");
    debugMsg("======================== DEBUG TERMINAL===============================\r\n");
    debugMsg("======================================================================\r\n\n");
}
