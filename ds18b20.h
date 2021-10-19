/*!
**************************************************************
* @file    ds18b20
* @brief   DS18B20 driver Header file
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
**************************************************************
*/

#ifndef DS18B20_H_
#define DS18B20_H_

/*=========================================================*/
/*== TYPEDEF MACROS =======================================*/
/*=========================================================*/
typedef float float32_t;

#define DS18B20_PIN 16


/*=========================================================*/
/*== MEMORY MACROS ========================================*/
/*=========================================================*/

#define THERM_CMD_CONVERTTEMP   (uint8_t) 0x44
#define THERM_CMD_RSCRATCHPAD   (uint8_t) 0xBE
#define THERM_CMD_WSCRATCHPAD   (uint8_t) 0x4E
#define THERM_CMD_CPYSCRATCHPAD (uint8_t) 0x48
#define THERM_CMD_RECEEPROM     (uint8_t) 0xB8
#define THERM_CMD_RPWRSUPPLY    (uint8_t) 0xB4
#define THERM_CMD_SEARCHROM     (uint8_t) 0xF0
#define THERM_CMD_READROM       (uint8_t) 0x33
#define THERM_CMD_MATCHROM      (uint8_t) 0x55
#define THERM_CMD_SKIPROM       (uint8_t) 0xCC
#define THERM_CMD_ALARMSEARCH   (uint8_t) 0xEC
#define THERM_CMD_10BIT_RES     (uint8_t) 0x3F
#define THERM_CMD_9BIT_RES      (uint8_t) 0x1F
#define THERM_CMD_11BIT_RES     (uint8_t) 0x5F
#define THERM_CMD_12BIT_RES     (uint8_t) 0x7F

/*=========================================================*/
/*== PROTOTYPE DECLARATION ================================*/
/*=========================================================*/

uint8_t DS18B20_RESET(uint8_t ds18b20_gpio_pin);
void DS18B20_WRITE_BIT(uint8_t ds18b20_gpio_pin, uint8_t bitValue);
void DS18B20_WRITE_BYTE(uint8_t ds18b20_gpio_pin, uint8_t writeByte);
uint8_t DS18B20_READ_BIT(uint8_t ds18b20_gpio_pin);
uint8_t DS18B20_READ_BYTE(uint8_t ds18b20_gpio_pin);
uint16_t DS18B20_REQUEST_TEMP(uint8_t ds18b20_gpio_pin);
uint8_t DS18B20_CRC8_CHECK(uint8_t *data, uint8_t len);
float32_t DS18B20_TEMP_READ(uint8_t ds18b20_gpio_pin);
int16_t DS18B20_INIT(void);

#endif