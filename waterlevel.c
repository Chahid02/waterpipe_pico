/*=========================================================*/
/*== INCLUDES =============================================*/
/*=========================================================*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
/*=========================================================*/
/*== PICO INCLUDES ========================================*/
/*=========================================================*/
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

/*=========================================================*/
/*== PRIVATE INCLUDES =====================================*/
/*=========================================================*/

#include "waterpipe.h" /*!< Insert for Error Log Function! */
#include "ds18b20.h"
#include "waterlevel.h"

/*=========================================================*/
/*== WATERLEVEL FUNCTIONS =================================*/
/*=========================================================*/

uint8_t WATERLEVEL_ADC_SET(void)
{
	adc_gpio_init(ADC_CHANNEL0);
	adc_init();
	adc_select_input(ADC_CHANNEL0);
	adc_fifo_setup(true, true, 1, false, true);
	adc_set_clkdiv(CLOCK_DIV);
	return adc_get_selected_input();
}


int8_t WATERLEVEL_DMA_SET(void)
{
	int8_t dma_channel = dma_claim_unused_channel(true);
	if (dma_channel == -1)
	{
		debugMsg("[X] DMA CANNOT BE SET !!! [X]");
	}
	else
	{
		switch (dma_channel)
		{
		case 0:
			debugMsg("[X] DMA CHANNEL 0 SET [X]\n");
			break;
		case 1:
			debugMsg("[X] DMA CHANNEL 1 SET [X]\n");
			break;
		case 2:
			debugMsg("[X] DMA CHANNEL 2 SET [X]\n");
			break;
		default:
			debugMsg("[X] WRONG RETURN !!! [X]\n");
			return -1;
			break;
		}

		dma_channel_config dma_config = dma_channel_get_default_config(dma_channel);

		channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_8);
		channel_config_set_write_increment(&dma_config, true);
		channel_config_set_read_increment(&dma_config, false);
		channel_config_set_dreq(&dma_config, DREQ_ADC);
	}
	
return 0;
}
