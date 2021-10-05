

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



int presence(uint8_t ds18b20_gpio_pin)
{
	gpio_set_dir(ds18b20_gpio_pin, GPIO_OUT);
	gpio_put(ds18b20_gpio_pin, 1);
	sleep_ms(1);
	gpio_put(ds18b20_gpio_pin, 0);
	sleep_us(480);
	gpio_set_dir(ds18b20_gpio_pin, GPIO_IN);
	sleep_us(80);
	int b = gpio_get(ds18b20_gpio_pin);
	sleep_us(400);
	return b;
}

void writeBit(uint8_t ds18b20_gpio_pin, int b)
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

void writeByte(uint8_t ds18b20_gpio_pin, int byte)
{
	for (int i = 0; i < 8; i++)
	{
		if (byte & 1)
		{
			writeBit(ds18b20_gpio_pin, 1);
		}
		else
		{
			writeBit(ds18b20_gpio_pin, 0);
		}
		byte = byte >> 1;
	}
}
uint8_t readBit(uint8_t ds18b20_gpio_pin)
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
int32_t readByte(uint8_t ds18b20_gpio_pin)
{
	int byte = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		byte = byte | readBit(ds18b20_gpio_pin) << i;
	};
	return byte;
}
int32_t convert(uint8_t ds18b20_gpio_pin)
{
	uint16_t i=750;
	writeByte(ds18b20_gpio_pin, THERM_CMD_CONVERTTEMP);

	do
	{
		sleep_ms(1);
		if (readBit(ds18b20_gpio_pin))
		{
			return i;
		}
		else
		{
			__NOP();
		}
		
		
	} while (i--);
	debugVal("\n [X] Conversation time:%d", i);
	return i;
}

uint8_t crc8(uint8_t *data, uint8_t len)
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
	if (presence(DS18B20_PIN) == 1)
	{
		debugMsg("\n [X] NO DEVICE found ...");
		return -1000;
	}
	writeByte(DS18B20_PIN, THERM_CMD_SKIPROM);
	if (convert(DS18B20_PIN) == 750)
	{
		debugMsg("\n [X] Max. Conversion time reached...");
		return -3000;
	}

	debugVal("\n [X] Conversion time:%d", i);
	presence(DS18B20_PIN);
	writeByte(DS18B20_PIN, THERM_CMD_SKIPROM);
	writeByte(DS18B20_PIN, THERM_CMD_RSCRATCHPAD);
	int i;
	uint8_t data[9];
	for (i = 0; i < 9; i++)
	{
		data[i] = readByte(DS18B20_PIN);
	}
	uint8_t crc = crc8(data, 9);
	if (crc != 0)
		return -2000;
	int t1 = data[0];
	int t2 = data[1];
	int16_t temp1 = (t2 << 8 | t1);
	float temp22 = (float)temp1 / 16;
	return temp22;
}