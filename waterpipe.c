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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/*=========================================================*/
/*== PICO INCLUDES ========================================*/
/*=========================================================*/

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

/*=========================================================*/
/*== PRIVATE INCLUDES =====================================*/
/*=========================================================*/

#include "bme280.h"
#include "waterpipe.h"
#include "ds18b20.h"
#include "waterlevel.h"
#include "hc05.h"

int main()
{
    stdio_init_all();
    sleep_ms(2000);
    debugMsg("======================================================================\r\n");
    debugMsg("OPERATION MODE STARTED: ");
    debugModMsg;
    debugMsg("=======================\r\n");
    sleep_ms(1000);

    /*=========================================================*/
    /*== I2C SETTINGS =========================================*/
    /*=========================================================*/

    i2c_init(i2c_default, 400 * 1000);

    debugMsg("INIT I2C HARDWARE: ");

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || \
    !defined(PICO_DEFAULT_I2C_SCL_PIN)
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
    debugMsg("==================\r\n");
#endif

    /*=========================================================*/
    /*== GPIO SETTINGS ========================================*/
    /*=========================================================*/
    gpio_init(LED);
    gpio_init(TEMPERATURE_OK);
    gpio_init(PRESSURE_OK);
    gpio_init(HUMIDITY_OK);
    gpio_init(WATER_TEMP_OK);
    gpio_init(PRESSURE_FSR_OK);
    gpio_init(WATER_LEVEL_OK);
    gpio_init(DS18B20_PIN);

    debugMsg("INIT GPIO HARDWARE: ");
    //debugMsg("\n==================\r\n");

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
    gpio_set_dir(TEMPERATURE_OK, GPIO_OUT);
    gpio_set_dir(PRESSURE_OK, GPIO_OUT);
    gpio_set_dir(HUMIDITY_OK, GPIO_OUT);
    gpio_set_dir(WATER_TEMP_OK, GPIO_OUT);
    gpio_set_dir(PRESSURE_FSR_OK, GPIO_OUT);
    gpio_set_dir(WATER_LEVEL_OK, GPIO_OUT);

    debugMsg("[X] GPIO HARDWARE SUCCESSFULLY SET [X]\r\n");
    sleep_ms(1000);

    debugMsg("==========================\r\n");
    debugMsg("OPERATION MODE STARTED: ");
    debugModMsg;
    sleep_ms(1000);
    debugMsg("==========================\r\n");

    debugMsg("INIT ADC CONFIGURATION: ");
    /*!< Init ADC for waterlevel sensor */
    if (WATERLEVEL_AdcSet() >= 3)
    {
        debugMsg("[X] ADC HARDWARE FAILED TO BE SET [X]\r\n");
    }
    else
    {
        debugMsg("[X] ADC HARDWARE SUCCESSFULLY SET [X]\r\n");
    }

    sleep_ms(1000);
    debugMsg("======================\r\n");

    debugMsg("INIT DMA CONFIGURATION: ");
    /*!< Init DMA for waterlevel sensor */
    WATERLEVEL_DmaSet();
    sleep_ms(1000);
    debugMsg("======================\r\n");


    /*!< Init BME280 Sensor */
    BME280_Init();

    /*!< Reading the register values */
    BME280_Read_RegValue();

    /*!< Init DS18B20 Sensor */
    DS18B20_Init();

    /*!< SETUP HC-05 Module */
    HC05_ProgSetup();


    irq_set_exclusive_handler(UART0_IRQ, HC05_UART_RX_READ_IRQ);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID0, true, false);


    HC05_Set(UART_ID0,HC05_SET_NAME);

    HC05_Set(UART_ID0,HC05_SET_PWD);

    HC05_Check(UART_ID0,HC05_CHECK_NAME);
/*     
    HC05_Check(UART_ID0,HC05_CHECK_ADDR);
  
    HC05_Check(UART_ID0,HC05_CHECK_VERSION);

    HC05_Check(UART_ID0,HC05_CHECK_UART);
   
    HC05_Check(UART_ID0,HC05_CHECK_PWD);
  
    HC05_Check(UART_ID0,HC05_CHECK_ROLE); */
  
    HC05_ProgFinished();

    /*!< User Code starts here */
    while (true)
    {

        debugTerm();
        while (BME280_ReadStatus() & BME280_STATUS_IM_UPDATE)
        {
            /*!< Waiting for updated values */
        };
        int32_t bmeTemp;
        uint32_t bmePress;
        uint32_t bmeHum;
        BME280_Temp_Reading(bmeTemp, bmePress, bmeHum);
         
        toggleLed();

        WATERLEVEL_Run();

        DS18B20_tempRead(DS18B20_PIN);

        //sleep_ms(500);
    }
    /*!< User Code ends here */
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
        gpio_put(TEMPERATURE_OK, true);
        gpio_put(PRESSURE_OK, true);
        gpio_put(HUMIDITY_OK, true);
        gpio_put(WATER_TEMP_OK, true);
        gpio_put(PRESSURE_FSR_OK, true);
        gpio_put(WATER_LEVEL_OK, true);
        gpio_put(LED, true);
        debugMsg("[X] LED ON\r\n");
    }
    else
    {
        gpio_put(TEMPERATURE_OK, false);
        gpio_put(PRESSURE_OK, false);
        gpio_put(HUMIDITY_OK, false);
        gpio_put(WATER_TEMP_OK, false);
        gpio_put(PRESSURE_FSR_OK, false);
        gpio_put(WATER_LEVEL_OK, false);
        gpio_put(LED, false);
        debugMsg("[X] LED OFF\r\n");
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
