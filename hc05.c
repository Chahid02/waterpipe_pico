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

#include "waterpipe.h" /* Insert for Error Log Function! */
#include "hc05.h"

/* void HC05_Check(uart_inst_t *uart, uint8_t *sendCommand)
{
    uint8_t getCharRx;
    uart_puts(uart, sendCommand);
    while (getCharRx = uart_getc(UART_ID0))
    {
    debugVal("%c",getCharRx);
    }
} */


void HC05_Check(uart_inst_t *uart, uint8_t *sendCommand)
{

    uart_puts(uart, sendCommand);

    int8_t HC_05_NL;
    size_t maxSize = 100;
    uint8_t buffRx[maxSize];
    memset(buffRx,'\0',maxSize);
    uint8_t RxCount;
    uint8_t getCharRx;
    uint8_t *arrayRtr;
    uint8_t searchPoint='\n';
    uint8_t searchNL = '+';
    uint8_t HC_OK[] = "\nOK";
    uint8_t *arrayOK = NULL;
    uint8_t carriageReturn = 0;
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


void HC05_Set(uart_inst_t *uart, uint8_t *sendCommand)
{
uart_puts(uart,sendCommand);
    
}



uint8_t HC05_ProgSetup(void)
{

    debugMsg("====================  HC-05 SETUP PROCESS STARTED  ====================\n");
    gpio_init(HC05_PROG_GPIO);
    gpio_set_dir(HC05_PROG_GPIO, GPIO_OUT);

    uart_init(UART_ID0, BAUD_RATE_DEFAULT);
    gpio_set_function(UART0_TX, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID0, BAUD_RATE_DEFAULT);
    uart_set_hw_flow(UART_ID0, false, false);
    uart_set_format(UART_ID0, DATA_BITS, STOP_BITS, PARITY_BIT);
    uart_set_fifo_enabled(UART_ID0, false);
    gpio_put(HC05_PROG_GPIO, false);
    sleep_ms(1000);
    gpio_put(HC05_PROG_GPIO, true);
   debug2Val("[X] PROGRAMMING PIN %d ->KEY PUT: %d [X] \r\n", HC05_PROG_GPIO, gpio_get_out_level(HC05_PROG_GPIO));
   sleep_ms(1000);

    
    return 0;
}

uint8_t UART_IRQ;
uint8_t HC05_INIT(void)
{
    debugMsg("====================  HC-05 INITIALIZATION PROCESS STARTED  =========== \r\n");
    uart_init(UART_ID0, BAUD_RATE_DEFAULT);
    gpio_set_function(UART0_TX, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX, GPIO_FUNC_UART);
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
    uart_puts(UART_ID0, "Sensor Unit BT-Module is Ready!!! \r\n");
}

uint16_t getCharRxCnt = 0;
void HC05_UART_RX_IRQ(void)
{
    while (uart_is_readable(UART_ID0))
    {
        uint8_t getCharRx = uart_getc(UART_ID0);
        if (uart_is_writable(UART_ID0))
        {
            getCharRx++;
            uart_putc(UART_ID0, getCharRx);
        }
        else
        {
            __NOP();
        }
        getCharRxCnt++;
    }
}

void IRQ_SETUP(void)
{
    irq_set_exclusive_handler(UART_IRQ, HC05_UART_RX_IRQ);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID0, true, false);
}