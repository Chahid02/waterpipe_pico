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

#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xBE
#define THERM_CMD_WSCRATCHPAD 0x4E
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xB8
#define THERM_CMD_RPWRSUPPLY 0xB4
#define THERM_CMD_SEARCHROM 0xF0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xCC
#define THERM_CMD_ALARMSEARCH 0xEC

/*=========================================================*/
/*== PROTOTYPE DECLARATION ================================*/
/*=========================================================*/

int presence(uint8_t ds18b20_gpio_pin);
void writeBit(uint8_t ds18b20_gpio_pin, int b);
void writeByte(uint8_t ds18b20_gpio_pin, int byte);
uint8_t readBit(uint8_t ds18b20_gpio_pin);
int32_t readByte(uint8_t ds18b20_gpio_pin);
int32_t convert(uint8_t ds18b20_gpio_pin);
uint8_t crc8(uint8_t *data, uint8_t len);
float32_t DS18B20_tempRead(uint8_t ds18b20_gpio_pin);

#endif