

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



int DS18B20_Reset(uint8_t ds18b20_gpio_pin)
{
	gpio_set_dir(ds18b20_gpio_pin, GPIO_OUT);
	gpio_put(ds18b20_gpio_pin, 1);
	sleep_ms(1);
	gpio_put(ds18b20_gpio_pin, 0);
	sleep_us(480);
	gpio_set_dir(ds18b20_gpio_pin, GPIO_IN);
	sleep_us(100);
	int b = gpio_get(ds18b20_gpio_pin);
	sleep_us(400);
	return b;
}

void DS18B20_Write_Bit(uint8_t ds18b20_gpio_pin, int b)
{
	uint16_t delay1, delay2;
	if (b == 1)
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

void DS18B20_Write_Byte(uint8_t ds18b20_gpio_pin, int byte)
{
	for (int i = 0; i < 8; i++)
	{
		if (byte & 1)
		{
			DS18B20_Write_Bit(ds18b20_gpio_pin, 1);
		}
		else
		{
			DS18B20_Write_Bit(ds18b20_gpio_pin, 0);
		}
		byte = byte >> 1;
	}
}
uint8_t DS18B20_Read_Bit(uint8_t ds18b20_gpio_pin)
{
	gpio_set_dir(ds18b20_gpio_pin, GPIO_OUT);
	gpio_put(ds18b20_gpio_pin, 0);
	sleep_us(8);
	gpio_set_dir(ds18b20_gpio_pin, GPIO_IN);
	sleep_us(2);
	uint8_t b = gpio_get(ds18b20_gpio_pin);
	sleep_us(60);
	return b;
}
int32_t DS18B20_Read_Byte(uint8_t ds18b20_gpio_pin)
{
	int byte = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		byte = byte | DS18B20_Read_Bit(ds18b20_gpio_pin) << i;
	};
	return byte;
}
uint32_t DS18B20_Request_Temp(uint8_t ds18b20_gpio_pin)
{
	uint32_t i = 0;
	DS18B20_Write_Byte(ds18b20_gpio_pin, THERM_CMD_CONVERTTEMP);
	while (!DS18B20_Read_Bit(ds18b20_gpio_pin))
	{
		sleep_ms(1);
		i++;
	}
	return i;
}

uint8_t DS18B20_Crc8_Check(uint8_t *data, uint8_t len)
{
	
	uint8_t temp_dsb;
	uint8_t databyte;
	uint8_t crc = 0;
	for (uint8_t i = 0; i < len; i++)
	{
		databyte = data[i];
		for (uint8_t j = 0; j < 8; j++)
		{
			temp_dsb = (crc ^ databyte) & 0x01;
			crc >>= 1;
			if (temp_dsb)
				crc ^= 0x8C;
			databyte >>= 1;
		}
	}
	return crc;
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
		return -3000;
	}
	else
	{
		debugVal("[X] Conversion time: %d ms\n", convTime);
	}
	

	DS18B20_Reset(DS18B20_PIN);
	DS18B20_Write_Byte(DS18B20_PIN, THERM_CMD_SKIPROM);
	DS18B20_Write_Byte(DS18B20_PIN, THERM_CMD_RSCRATCHPAD);
	int i;
	uint8_t data[9];
	for (i = 0; i < 9; i++)
	{
		data[i] = DS18B20_Read_Byte(DS18B20_PIN);
	}
	uint8_t crc = DS18B20_Crc8_Check(data, 9);
	if (crc != 0)
		return -2000;
	int t1 = data[0];
	int t2 = data[1];
	int16_t temp1 = ((t2 << 8 | t1));// for 9 Bits
	float temp22 = (float)temp1 / 16;
	debugVal("[X] DS18B20 Temperature: %f °C\r\n", temp22);
	return temp22;
}