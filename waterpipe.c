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
#include "pico/multicore.h"
#include <pico/time.h>
/*=========================================================*/
/*== PRIVATE INCLUDES =====================================*/
/*=========================================================*/

#include "bme280.h"
#include "waterpipe.h"
#include "ds18b20.h"
#include "waterlevel.h"
#include "hc05.h"
#include "test.c"

float32_t hcTemp;
float32_t hcPress;
float32_t hcHum;

float32_t tempCompr ;

uint32_t count = 0;



void core1_entry() 
{

    multicore_fifo_clear_irq();
    //irq_add_shared_handler(SIO_IRQ_PROC1, core1_interrupt_handler,0x01); /* May be used for multiple IRQ functions */
    irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_interrupt_handler);
    irq_set_enabled(SIO_IRQ_PROC1, true);

    while (1)
    {   

        /* Just for testing purpose */
        //tight_loop_contents();
        //tempCompr = DS18B20_TEMP_READ(DS18B20_PIN);

  
    }
} 




 

int main()
{
    stdio_init_all();
    multicore_launch_core1(core1_entry);

    sleep_ms(2000);
    debugMsg("======================================================================\r\n");
    debugMsg("OPERATION MODE STARTED: ");
    debugModMsg;
    monitorModMsg;
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

    /* Init GPIOs on HIGH-LEVEL */
                                        /* GPIO DIRECTION CONTROL missing yet gpio_get_out_level(LED) != true) */
    gpio_put(TEMPERATURE_OK, true);
    gpio_put(PRESSURE_OK, true);
    gpio_put(HUMIDITY_OK, true);
    gpio_put(WATER_TEMP_OK, true);
    gpio_put(PRESSURE_FSR_OK, true);
    gpio_put(WATER_LEVEL_OK, true);
    gpio_put(LED, true);


    debugMsg("[X] GPIO HARDWARE SUCCESSFULLY SET [X]\r\n");
    sleep_ms(1000);

    debugMsg("==========================\r\n");
    debugMsg("OPERATION MODE STARTED: ");
    sleep_ms(1000);
    debugMsg("==========================\r\n");

    debugMsg("INIT ADC CONFIGURATION: ");
    /* Init ADC for waterlevel sensor */
    if (WATERLEVEL_SET_ADC() >= 3)
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
    /* Init DMA for waterlevel sensor */
    WATERLEVEL_SET_DMA();
    sleep_ms(1000);
    debugMsg("======================\r\n");


    debugMsg("[X] AES INIT SET [X]\r\n");
    int exit;

#if defined(AES256)
    printf("\nTesting AES256\n\n");
#elif defined(AES192)
    printf("\nTesting AES192\n\n");
#elif defined(AES128)
    printf("\nTesting AES128\n\n");
#else
    printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
    return 0;
#endif

    exit = test_encrypt_cbc() + test_decrypt_cbc() +
	test_encrypt_ctr() + test_decrypt_ctr() +
	test_decrypt_ecb() + test_encrypt_ecb();
    test_encrypt_ecb_verbose();
    debugVal("Return %d",exit);

    sleep_ms(1000);

    /* Enable IRQ for TX-Received Messages */
    /* Init BME280 Sensor */
    BME280_INIT();

    /* Reading the register values */
    BME280_READ_REGVALUE();

    /* Init DS18B20 Sensor */
    DS18B20_INIT();

    /* SETUP HC-05 Module */
    HC05_PROG_SETUP();


    IRQ_SETUP_EN(HC05_UART_RX_READ_IRQ); /* Enable IRQ for TX-Received Messages */
    
    HC05_SET(UART_ID0,"AT+NAMEWATERPIPE\r\n","NAME");

    /* 
    Has to be commment because of the use of HC-06 instead.
    There is no programming inside the code !!!
    */


    //HC05_SET(UART_ID0,HC05_SET_NAME,"NAME");
    //HC05_SET(UART_ID0,HC05_SET_PWD,"PASSWORD");
    //HC05_SET(UART_ID0,HC05_SET_ROLE_MS,"MASTER ROLE");
    //HC05_SET(UART_ID0,HC05_SET_ROLE_SL,"SLAVE ROLE");

    //HC05_CHECK(UART_ID0,HC05_CHECK_NAME,"NAME");  
    //HC05_CHECK(UART_ID0,HC05_CHECK_ADDR,"ADRESS");
    //HC05_CHECK(UART_ID0,HC05_CHECK_VERSION,"VERSION");
    //HC05_CHECK(UART_ID0,HC05_CHECK_UART,"BAUDRATE"); 
    //HC05_CHECK(UART_ID0,HC05_CHECK_PWD,"PASSWORD"); 
    //HC05_CHECK(UART_ID0,HC05_CHECK_ROLE,"ROLE"); 
  
    HC05_PROG_FINISHED();
    //IRQ_SETUP_DIS(HC05_UART_RX_READ_IRQ);

    //HC05_SET(UART_ID0,HC05_SET_RESET,"RESET");    /* Isnt nesserarly so far because HC-05 overtake new values */
    //debugMsg("[X] RESETING HC-05 BLUETOOTH MODULE [X]\r\n");

    size_t hcCount = 1;

    //IRQ_SETUP_EN(HC05_UART_RX_READ_IRQ);
    /* User Code starts here */
    while (true)
    {
        debugTerm();

 
            uint8_t freezeFlag = 1;
            while (BME280_READ_STATUS() & BME280_STATUS_IM_UPDATE & freezeFlag)
            {
            /* Waiting for updated values */
            for (size_t i = 0; i <= 27000000; i++)
            {
                if(i == (27000000-1))
                {
                   freezeFlag = 0;
                   printf("[X] ERROR BME280 NO CONNECTION [X]\r\n");
                   printf("[X] ANALYZING ... [X]\r\n");
                   sleep_ms(500);

                   BME280_INIT();
                   BME280_READ_REGVALUE();

                   printf("[X] REINIT BME280 [X]\r\n");
                   printf("[X] WAITING ... [X]\r\n");
                   sleep_ms(500);

                }
                else
                {
                    freezeFlag = 1;
                }
            }
            
            }; 
        




        int32_t bmeTemp;
        uint32_t bmePress;
        uint32_t bmeHum;

        /* Reading out register values*/
        BME280_TEMP_READ(bmeTemp, bmePress, bmeHum);

        /* Storing BME280 values */
        bmeTemp = BME280_COMP_TEMP();
        bmePress = BME280_COMP_PRESSURE();
        bmeHum = BME280_COMP_HUM_INT32();

        
        hcTemp = bmeTemp / 100.0f; 
        hcPress = bmePress / 100.0f; 
        hcHum = bmeHum / 1024.0f;

        uint8_t sendBuffer[50]; 
/*      snprintf(sendBuffer,100,"========== %d Value ========== \r\n",hcCount);
        uart_puts(UART_ID0, sendBuffer);
        hcCount++; 
        if (hcCount == 60)
        {
            hcCount = 1;
        }    
        */    
        

        //HC05_TX_BME280(hcTemp, hcPress, hcHum);

        float32_t waterlevelAdc;
        waterlevelAdc = WATERLEVEL_RUN();

        //HC05_TX_WATERLEVEL(waterlevelAdc);
  
        if(multicore_fifo_wready())
        {
            uint32_t dataCore0 = 200;
            debugMsg("======================== CORE0 FIFO ==================================\r\n");
            debugVal("[X] CORE 0 SENDS %d [X]\r\n",dataCore0);
            multicore_fifo_push_blocking(dataCore0);
        }
     
        

        if(multicore_fifo_rvalid())
        {
            uint32_t dataCore1 = multicore_fifo_pop_blocking();
            debugMsg("======================== CORE1 FIFO ==================================\r\n");
            debugVal("[X] CORE 1 SENDS %d [X]\r\n",dataCore1);  
        }
        //HC05_TX_DS18B20(tempCompr);
       
     
        //toggleLed();
        debugMsg("======================== WARNING LEVEL ===============================\r\n");
        monitorMsg("======================== WARNING LEVEL ===============================\r\n");

        debugVal("[X] TEMPERATURE TOLERANZ: %f [X]\r\n",30.0f - hcTemp);
        monitorVal("[X] TEMPERATURE TOLERANZ: %f [X]\r\n",30.0f - hcTemp);
        if (hcTemp >= 30.0f)
        {
            gpio_put(TEMPERATURE_OK, false);
        }

        debugVal("[X] PRESSURE TOLERANZ: %f [X]\r\n",1100.0f - hcPress);
        monitorVal("[X] PRESSURE TOLERANZ: %f [X]\r\n",1100.0f - hcPress);
        if (hcPress >= 1100.0f)
        {
            gpio_put(PRESSURE_OK, false);
        }

        debugVal("[X] HUMIDITY TOLERANZ: %f [X]\r\n",30.0f - hcHum);
        monitorVal("[X] HUMIDITY TOLERANZ: %f [X]\r\n",30.0f - hcHum);
        if (hcHum >= 30.0f)
        {
            gpio_put(HUMIDITY_OK, false);
        }

        debugVal("[X] WATERELEVEL TOLERANZ: %f cm [X]\r\n",3.5f - waterlevelAdc);
        monitorVal("[X] WATERELEVEL TOLERANZ: %f cm [X]\r\n",3.5f - waterlevelAdc);
        if (waterlevelAdc  >= 3.5f)
        {
            gpio_put(WATER_LEVEL_OK, false);
        }

        debugVal("[X] WATER TEMP TOLERANZ: %f [X]\r\n",25.0f - tempCompr );
        monitorVal("[X] WATER TEMP TOLERANZ: %f [X]\r\n",25.0f - tempCompr );
        if (tempCompr >= 25.0f)
        {
            gpio_put(WATER_TEMP_OK, false);
        }


        monitorMsg("======================== BT RECEIVED MSG =============================\r\n");

        HC05_TX_BME280(hcTemp, hcPress, hcHum);
        HC05_TX_WATERLEVEL(waterlevelAdc);
        HC05_TX_DS18B20(tempCompr);


        HC05_RX_MSG_IRQ();


        
        sleep_ms(500); /* For monitoring purpose */
        //clrscr();

    }
    /* User Code ends here */
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
        /*
        gpio_put(TEMPERATURE_OK, true);
        gpio_put(PRESSURE_OK, true);
        gpio_put(HUMIDITY_OK, true);
        gpio_put(WATER_TEMP_OK, true);
        gpio_put(PRESSURE_FSR_OK, true);
        gpio_put(WATER_LEVEL_OK, true);
         */
        gpio_put(LED, true);
        /* NO PRINTF on the SECOND CORE !!! */
        //debugMsg("[X] LED ON\r\n");
    }
    else
    {
        /*
        gpio_put(TEMPERATURE_OK, false);
        gpio_put(PRESSURE_OK, false);
        gpio_put(HUMIDITY_OK, false);
        gpio_put(WATER_TEMP_OK, false);
        gpio_put(PRESSURE_FSR_OK, false);
        gpio_put(WATER_LEVEL_OK, false);
        */
        gpio_put(LED, false);
        /* NO PRINTF on the SECOND CORE !!! */
        //debugMsg("[X] LED OFF\r\n");
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
void toggleBuzz()
{
    if (gpio_get_out_level(LED) != true)
    {
        gpio_put(PRESSURE_FSR_OK, true);
        /* NO PRINTF on the SECOND CORE !!! */
        //debugMsg("[X] LED ON\r\n");
    }
    else
    {
        gpio_put(PRESSURE_FSR_OK, false);
        /* NO PRINTF on the SECOND CORE !!! */
        //debugMsg("[X] LED OFF\r\n");
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

    monitorMsg("\n\n======================================================================\r\n");
    monitorMsg("======================== DEBUG TERMINAL===============================\r\n");
    monitorMsg("======================================================================\r\n\n");

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
 void core1_interrupt_handler(void)
{

    while (multicore_fifo_rvalid())
    {
        uint32_t blueDAta = multicore_fifo_pop_blocking();      
        blueDAta += blueDAta ; 
        multicore_fifo_push_blocking(blueDAta);
        tempCompr = DS18B20_TEMP_READ(DS18B20_PIN);
    }
    multicore_fifo_clear_irq();// Clear IRQ
} 

