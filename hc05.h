/*!
**************************************************************
* @file    hc05
* @brief   hc05 driver Header file
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
**************************************************************
*/

#ifndef HC05_H_
#define HC05_H_



/*=========================================================*/
/*== TYPEDEF MACROS =======================================*/
/*=========================================================*/
typedef float float32_t;


#define HC05_PROG_GPIO           2


#define UART_ID0            uart0
#define UART_ID1            uart1
#define UART0_TX            (uint8_t) 0
#define UART0_RX            (uint8_t) 1
#define DATA_BITS           (uint8_t) 8
#define STOP_BITS           (uint8_t) 1
#define PARITY_BIT          (uint8_t) UART_PARITY_NONE
#define BAUD_RATE_DEFAULT   (int16_t) 9600

#define HC05_CHECK_NAME         "AT+NAME?\r\n"
#define HC05_SET_NAME           "AT+NAME=WATERPIPE\r\n"
#define HC05_CHECK_ADDR         "AT+ADDR?\r\n"
#define HC05_CHECK_VERSION      "AT+VERSION?\r\n"
#define HC05_CHECK_UART         "AT+UART?\r\n"
#define HC05_SET_UART           "AT+UARTBAUD=8\r\n"
#define HC05_CHECK_ROLE         "AT+ROLE?\r\n"
#define HC05_SET_ROLE_MS        "AT+ROLE=1\r\n"
#define HC05_SET_ROLE_SL        "AT+ROLE=0\r\n"
#define HC05_CHECK_PWD          "AT+PSWD?\r\n"
#define HC05_SET_PWD            "AT+PSWD=123456\r\n"
#define HC05_SET_RESET          "AT+RESET\r\n"

int32_t HC_MSG_COUNT;
uint8_t MSGData[1024];

uint8_t HC05_PROG_SETUP(void);
void HC05_CHECK(uart_inst_t *uart, uint8_t *sendCommand, uint8_t *ATCommand);
void HC05_SET(uart_inst_t *uart, uint8_t *sendCommand, uint8_t *ATCommand);
uint8_t HC05_PROG_FINISHED(void);
void HC05_TX_DS18B20(float32_t temperature);
void HC05_TX_BME280(float32_t temperature, float32_t pressure, float32_t humidity);
void HC05_TX_WATERLEVEL(float32_t adc);
uint8_t HC05_UART_RX_READ_IRQ(void);
void HC05_UART_RX_READ_MSG_IRQ(void);
void IRQ_SETUP_EN(irq_handler_t handler);
void IRQ_SETUP_DIS(irq_handler_t handler);
void HC05_READ_CHECK(uart_inst_t *uart, uint8_t *sendCommand);
#endif
