/*!
*****************************************************************
* @file    hc05.c
* @brief   HC05 Driver
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   BME280 Driver
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
*****************************************************************
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

#include "waterpipe.h" /*!< Insert for Error Log Function! */
#include "hc05.h"

/* void HC05_CHECK(uart_inst_t *uart, uint8_t *sendCommand)
{
    uint8_t getCharRx;
    uart_puts(uart, sendCommand);
    while (getCharRx = uart_getc(UART_ID0))
    {
    debugVal("%c",getCharRx);
    }
} */




void HC05_SET(uart_inst_t *uart, uint8_t *sendCommand, uint8_t *ATCommand )
{
    debugVal("[X] SET %s [X]\r\n", ATCommand);

    while (uart_is_writable(uart) )
    {
        uart_puts(uart, sendCommand);
    }
    
    sleep_ms(1000);
    
}

void HC05_CHECK(uart_inst_t *uart, uint8_t *sendCommand, uint8_t *ATCommand )
{
    debugVal("[X] CHECK %s [X]\r\n", ATCommand);

    while (uart_is_writable(uart) )
    {
        uart_puts(uart, sendCommand);
    }
    
    sleep_ms(1000);
    
}

uint8_t HC05_PROG_SETUP(void)
{
    debugMsg("====================  HC-05 SETUP PROCESS STARTED  ====================\n");
    gpio_init(HC05_PROG_GPIO);
    gpio_set_dir(HC05_PROG_GPIO, GPIO_OUT);

    uart_init(UART_ID0, BAUD_RATE_DEFAULT);
    gpio_set_function(UART0_TX, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID0, BAUD_RATE_DEFAULT);
    uart_set_hw_flow(UART_ID0, false, false);
    uart_set_format(UART_ID0, DATA_BITS, STOP_BITS, PARITY_BIT);
    uart_set_fifo_enabled(UART_ID0, false);
    gpio_put(HC05_PROG_GPIO, false);
    sleep_ms(1000);
    gpio_put(HC05_PROG_GPIO, true);
      if (!gpio_get_out_level(HC05_PROG_GPIO))
    {
        debugVal("[X] PROGRAMMING GPIO_PIN %d ->KEY PUT: LOW [X] \r\n", HC05_PROG_GPIO);
        debugMsg("[X] PROGRAMMING FINISHED [X]\r\n");
    }
    else
    {
        debugVal("[X] PROGRAMMING GPIO_PIN %d ->KEY PUT: HIGH [X] \r\n", HC05_PROG_GPIO);
        debugMsg("[X] PROGRAMMING STARTED [X]\r\n");
    }
   sleep_ms(1000);

    
    return 0;
}


uint8_t HC05_PROG_FINISHED(void)
{
    debugMsg("====================  HC-05 FINISHED PROCESS STARTED  ================\n");
    //uart_deinit(UART_ID0);
    //uart_init(UART_ID0, 115200);
    gpio_set_function(UART0_TX, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID0, false, false);
    uart_set_format(UART_ID0, DATA_BITS, STOP_BITS, PARITY_BIT);
    uart_set_fifo_enabled(UART_ID0, false);
    gpio_put(HC05_PROG_GPIO, false);
    sleep_ms(1000);
    if (!gpio_get_out_level(HC05_PROG_GPIO))
    {
        debugVal("[X] PROGRAMMING GPIO_PIN %d ->KEY PUT: LOW [X] \r\n", HC05_PROG_GPIO);
        debugMsg("[X] PROGRAMMING FINISHED [X]\r\n");
    }
    else
    {
        debugVal("[X] PROGRAMMING GPIO_PIN %d ->KEY PUT: HIGH [X] \r\n", HC05_PROG_GPIO);
        debugMsg("[X] PROGRAMMING STARTED [X]\r\n");
    }
    sleep_ms(1000);

    
    return 0;
}


uint8_t UART_IRQ;
uint8_t HC05_INIT(void)
{
    debugMsg("====================  HC-05 INITIALIZATION PROCESS STARTED  =========== \r\n");
    uart_init(UART_ID0, BAUD_RATE_DEFAULT);
    gpio_set_function(UART0_TX, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID0, 115200);
    uart_set_hw_flow(UART_ID0, false, false);
    uart_set_format(UART_ID0, DATA_BITS, STOP_BITS, PARITY_BIT);
    uart_set_fifo_enabled(UART_ID0, false);

    if (UART_ID0 == uart0)
    {
        UART_IRQ = UART0_IRQ;
    }
    else
    {
        UART_IRQ = UART1_IRQ;
    }
    debugMsg("[X] BLUETOOTH MODULE IS READY [X] \r\n");
    monitorMsg("[X] BLUETOOTH MODULE IS READY [X] \r\n");
 
}

void HC05_RX_MSG_IRQ(void)
{

    //debugVal("[X] GET BLUETOOTH MSG: %s [X]\r\n", MSGData);

    /*!< Just for Testing */
    if (MSGData[0] != NULL)
    {
      monitorVal("[X] GET BLUETOOTH MSG: %s\r\n", MSGData);
    }
    else
    {
       monitorMsg("[X] NO BLUETOOTH MSG !!! [X]\r\n");    
    }
    


    if (HC_MSG_COUNT >= 127)
    {
        HC_MSG_COUNT = 0;
        //memset(MSGData, 0, sizeof(MSGData));
    }

}


uint8_t HC05_UART_RX_READ_IRQ(void)
{

    uint8_t getCharRx;

    while (uart_is_readable(UART_ID0))
    {    
 
        getCharRx = uart_getc(UART_ID0);
        //debugVal("%c", getCharRx);
        //monitorVal("%c", getCharRx);
        MSGData[HC_MSG_COUNT] = getCharRx;
        HC_MSG_COUNT++;
        //printf("%d",HC_MSG_COUNT);
        

        
    }    
    irq_clear(UART0_IRQ);
    return getCharRx;
}


void HC05_UART_RX_READ_MSG_IRQ(void)
{

}



void HC05_TX_WATERLEVEL(float32_t adc)
{
    uint8_t RecData[50];
    uint8_t WTData[6];
    /*!< ADC float to string */
    gcvt(adc, 2, WTData);
    debugMsg("====================  HC-05 WT SEND STARTED  ========================= \r\n");
    debugVal("[X] Waterlevel:%s [X]\r\n",WTData);

    monitorMsg("====================  HC-05 WT SEND STARTED  ========================= \r\n");
    monitorVal("[X] Waterlevel:%s [X]\r\n",WTData);

    strcpy(RecData,"E:");
    strncat(RecData,WTData,sizeof(WTData));
    strncat(RecData,"ÿ",sizeof("ÿ"));

    uart_puts(UART_ID0, RecData);
    //getCharRxCnt++;
} 
 
void HC05_TX_DS18B20(float32_t temperature)
{
    uint8_t RecData[100];
    uint8_t TempData[5];
    /*!< Temperature float to string */
    gcvt(temperature,5,TempData);
    strcpy(RecData,"D:");
    strncat(RecData,TempData,sizeof(TempData));
    strncat(RecData,"ÿ",sizeof("ÿ"));


    debugMsg("====================  HC-05 DSB SEND STARTED  ======================== \r\n");
    debugVal("[X] DS1820 Temperature %s [X]\r\n",TempData);
    monitorMsg("====================  HC-05 DSB SEND STARTED  ======================== \r\n");
    monitorVal("[X] DS1820 Temperature %s [X]\r\n",TempData);
    uart_puts(UART_ID0, RecData);
    //getCharRxCnt++;
} 

void HC05_TX_BME280(float32_t temperature, float32_t pressure, float32_t humidity)
{
    uint8_t RecData[100];
    uint8_t TempData[5];
    uint8_t HumData[5];
    uint8_t PressData[7];

    /*!< Temperature float to string */
    gcvt(temperature,5,TempData);
    strcpy(RecData,"A:");
    strncat(RecData,TempData,sizeof(TempData));
    strncat(RecData,"ÿ",sizeof("ÿ"));

    /*!< Pressure float to string */
    gcvt(pressure,7,PressData);
    strncat(RecData,"B:",sizeof("B:"));
    strncat(RecData,PressData,sizeof(PressData));
    strncat(RecData,"ÿ",sizeof("ÿ"));

    /*!< Humidity float to string */
    gcvt(humidity,5,HumData);
    strncat(RecData,"C:",sizeof("C:"));
    strncat(RecData,HumData,sizeof(HumData));
    strncat(RecData,"ÿ",sizeof("ÿ"));

    debugMsg("====================  HC-05 BME SEND STARTED  ======================== \r\n");
    debugVal("[X] Temperature:%s [X]\r\n",TempData);
    debugVal("[X] Pressure:%s [X]\r\n",PressData);
    debugVal("[X] Humidity:%s [X]\r\n",HumData);

    monitorMsg("====================  HC-05 BME SEND STARTED  ======================== \r\n");
    monitorVal("[X] Temperature:%s [X]\r\n",TempData);
    monitorVal("[X] Pressure:%s [X]\r\n",PressData);
    monitorVal("[X] Humidity:%s [X]\r\n",HumData);
    uart_puts(UART_ID0, RecData);
    //getCharRxCnt++;
} 


void IRQ_SETUP_EN(irq_handler_t handler)
{
    irq_set_exclusive_handler(UART0_IRQ, handler);
    irq_set_priority(UART0_IRQ,0x01);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID0, true, false);
}

void IRQ_SETUP_DIS(irq_handler_t handler)
{
    irq_set_exclusive_handler(UART0_IRQ, handler);
    irq_set_enabled(UART0_IRQ, false);
    uart_set_irq_enables(UART_ID0, true, false);
}






/* 
void HC05_CHECK(uart_inst_t *uart, uint8_t *sendCommand)
{
    debugMsg("[X] CHECK [X]\r\n");
    uart_puts(uart, sendCommand);

    size_t maxSize = 32;
    uint8_t buffRx[maxSize];
    memset(buffRx,'\0',maxSize);
    uint8_t RxCount;
    uint8_t getCharRx;
    uint8_t HC_OK[] = "\nOK";
    uint8_t *arrayOK = NULL;
    while ((arrayOK = strstr(buffRx, HC_OK)) == NULL)
    {
    getCharRx = uart_getc(UART_ID0); 
    //printf("%d-Nr: %s\r\n",RxCount, buffRx);
    buffRx[RxCount] = getCharRx;
    //buffRx[RxCount+1]='\0'; 
    //printf("%d-Nr: %s\r\n",RxCount, buffRx);
    RxCount++;
    }
    printf("%s\r\n",buffRx);
    memset(buffRx,'\0',maxSize);
    sleep_ms(1000);

}  
*/