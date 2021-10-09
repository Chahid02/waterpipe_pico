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


#define UART_ID0            uart0
#define UART_ID1            uart1
#define UART0_TX            (uint8_t) 0
#define UART1_RX            (uint8_t) 1
#define DATA_BITS           (uint8_t) 8
#define STOP_BITS           (uint8_t) 1
#define PARITY_BIT          (uint8_t) UART_PARITY_NONE
#define BAUD_RATE_DEFAULT   (int16_t) 9600

#define HC05_CHECK_CONNECTION   (uint8_t) "AT?\r\n"
#define HC05_CHECK_NAME         (uint8_t) "AT+NAME?\r\n"
#define HC05_SET_NAME           (uint8_t) "AT+NAME=WATERPIPE_HC\r\n"
#define HC05_CHECK_ADDR         (uint8_t) "AT+ADDR?\r\n"
#define HC05_CHECK_VERSION      (uint8_t) "AT+VERSION?\r\n"
#define HC05_CHECK_UART         (uint8_t) "AT+UART?"
#define HC05_SET_UART           (uint8_t) "AT+UARTBAUD=8\r\n"
#define HC05_CHECK_ROLE         (uint8_t) "AT+ROLE?\r\n"
#define HC05_CHECK_PWD          (uint8_t) "AT+PSWD?\r\n"
#define HC05_SET_PWD            (uint8_t) "AT+PIN=waterpipe\r\n"




#endif
