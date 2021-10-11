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

uint8_t WATERLEVEL_AdcSet(void)
{
    adc_gpio_init(26);
    adc_init();
    adc_select_input(0);
    //adc_set_round_robin(0x01);
    adc_fifo_setup(true, true, 0, false, false);
    adc_set_clkdiv(CLOCK_DIV);
    return adc_get_selected_input();
}

dma_channel_config dma_config;
int8_t dma_channel;
int8_t WATERLEVEL_DmaSet(void)
{
    dma_channel = dma_claim_unused_channel(true);
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

        dma_config = dma_channel_get_default_config(dma_channel);

        channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_16);
        channel_config_set_read_increment(&dma_config, false);
        channel_config_set_write_increment(&dma_config, true);
        channel_config_set_dreq(&dma_config, DREQ_ADC);
    }

    return 0;
}

void WATERLEVEL_Run(void)
{
    uint16_t adcWaterLevel[ADC_SAMPLES];
    const float32_t conversion_factor = 3.3f / (1 << 12);
    float32_t waterLevelVoltage;
    float32_t buff;

    debugMsg("====================  WATERLEVEL SENSOR DATA READING STARTED  ======== \r\n");

    adc_run(true);
    dma_channel_configure(dma_channel, &dma_config, adcWaterLevel, &adc_hw->fifo, ADC_SAMPLES, true);

    dma_channel_wait_for_finish_blocking(dma_channel);
    adc_run(false);
    //adc_fifo_drain();
    for (size_t i = 0; i < ADC_SAMPLES - 1; i++)
    {
        buff += (adcWaterLevel[i] * conversion_factor);
    }
    adc_fifo_drain();

    waterLevelVoltage = (float32_t)(buff / ADC_SAMPLES);
    debugVal("[X] Voltage: %f V (DMA READ) [x]\n", waterLevelVoltage);

    uint16_t result = adc_read();
    debug2Val("[X] Compare Voltage: %f V (DIRECT ADC READ) [X]\n", result, result * conversion_factor);
}
