

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
#include "hardware/i2c.h"

/*=========================================================*/
/*== PRIVATE INCLUDES =====================================*/
/*=========================================================*/


#include "waterpipe.h" /* Insert for Error Log Function! */
#include "ds18b20.h"

/*=========================================================*/
/*== DS18B20 FUNCTIONS ====================================*/
/*=========================================================*/

uint8_t DS18B20_Reset(uint8_t ds18b20_gpio_pin)
{
	gpio_set_dir(ds18b20_gpio_pin, GPIO_OUT);
	gpio_put(ds18b20_gpio_pin, 1);
	sleep_ms(1);
	gpio_put(ds18b20_gpio_pin, 0);
	sleep_us(480);
	gpio_set_dir(ds18b20_gpio_pin, GPIO_IN);
	sleep_us(100);
	uint8_t slaveResponse = gpio_get(ds18b20_gpio_pin);
	sleep_us(400);
	return slaveResponse;
}

void DS18B20_Write_Bit(uint8_t ds18b20_gpio_pin, uint8_t bitValue)
{
	uint16_t delay1, delay2;
	if (bitValue == 1)
	{
		delay1 = 10;
		delay2 = 45;
	}
	else
	{
		delay1 = 60;
		delay2 = 10;
	}
	gpio_set_dir(ds18b20_gpio_pin, GPIO_OUT);
	gpio_put(ds18b20_gpio_pin, 0);
	sleep_us(delay1);
	gpio_set_dir(ds18b20_gpio_pin, GPIO_IN);
	sleep_us(delay2);
}

void DS18B20_Write_Byte(uint8_t ds18b20_gpio_pin, uint8_t writeByte)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (writeByte & 0x01)
		{
			DS18B20_Write_Bit(ds18b20_gpio_pin, 1);
		}
		else
		{
			DS18B20_Write_Bit(ds18b20_gpio_pin, 0);
		}
		writeByte >>= 1;
	}
}
uint8_t DS18B20_Read_Bit(uint8_t ds18b20_gpio_pin)
{
	gpio_set_dir(ds18b20_gpio_pin, GPIO_OUT);
	gpio_put(ds18b20_gpio_pin, 0);
	sleep_us(8);
	gpio_set_dir(ds18b20_gpio_pin, GPIO_IN);
	sleep_us(2);
	uint8_t bitValue = gpio_get(ds18b20_gpio_pin);
	sleep_us(60);
	return bitValue;
}
uint8_t DS18B20_Read_Byte(uint8_t ds18b20_gpio_pin)
{
	uint8_t readByte = 0;
	for (size_t i = 0; i < 8; i++)
	{
		readByte = readByte | DS18B20_Read_Bit(ds18b20_gpio_pin) << i;
	};
	return readByte;
}
uint16_t DS18B20_Request_Temp(uint8_t ds18b20_gpio_pin)
{
	uint16_t requestTime = 0;
	DS18B20_Write_Byte(ds18b20_gpio_pin, THERM_CMD_CONVERTTEMP);
	while (!DS18B20_Read_Bit(ds18b20_gpio_pin))
	{
		sleep_ms(1);
		requestTime++;
		if (requestTime == 750)
		{
			break;
		} 
		
	}
	return requestTime;
}



float32_t DS18B20_tempRead(uint8_t ds18b20_gpio_pin)
{
	debugMsg("====================  DS18B20 SENSOR DATA READING STARTED  =========== \r\n");
	if (DS18B20_Reset(DS18B20_PIN) == 1)
	{
		debugMsg("\n[X] NO DEVICE found ...");
		return -1000;
	}
	DS18B20_Write_Byte(DS18B20_PIN, THERM_CMD_SKIPROM);
	uint16_t convTime;
	if ((convTime = DS18B20_Request_Temp(DS18B20_PIN)) == 750)
	{
		debugMsg("\n[X] Max. Conversion time reached...");
		debugVal("[X] Conversion time: %d ms\n", convTime);
		return -2000;
	}
	else
	{
		debugVal("[X] Conversion time: %d ms\n", convTime);
	}
	DS18B20_Reset(DS18B20_PIN);
	DS18B20_Write_Byte(DS18B20_PIN, THERM_CMD_SKIPROM);
	DS18B20_Write_Byte(DS18B20_PIN, THERM_CMD_RSCRATCHPAD);
	uint8_t memoryRead[9];
	for (size_t i = 0; i < 9; i++)
	{
		memoryRead[i] = DS18B20_Read_Byte(DS18B20_PIN);
	}
	uint8_t crc = DS18B20_Crc8_Check(memoryRead, 9);
	if (crc != 0)
	{
		return -3000;
	}
	int16_t tempLSB = memoryRead[0];
	int16_t tempMSB = memoryRead[1];
	int16_t temperature = ((tempMSB << 8 | tempLSB));// for 9 Bits
	float32_t temperatureFloat = (float32_t)(temperature / 16.0f);
	debugVal("[X] DS18B20 Temperature: %.2f °C\r\n", temperatureFloat);

	return temperature;
}

uint8_t DS18B20_Crc8_Check(uint8_t *data, uint8_t len)
{

	uint8_t temp;
	uint8_t databyte;
	uint8_t crc = 0;
	for (uint8_t i = 0; i < len; i++)
	{
		databyte = data[i];
		for (uint8_t j = 0; j < 8; j++)
		{
			temp = (crc ^ databyte) & 0x01;
			crc >>= 1;
			if (temp)
				crc ^= 0x8C;
			databyte >>= 1;
		}
	}
	return crc;
}