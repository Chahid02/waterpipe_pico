/*!
**************************************************************
* @file    waterlevel.h
* @brief   waterlevel driver Header file
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
**************************************************************
*/

#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

/*=========================================================*/
/*== TYPEDEF MACROS =======================================*/
/*=========================================================*/

typedef float float32_t;

/*=========================================================*/
/*== ADC MACROS ===========================================*/
/*=========================================================*/

/*!
**************************************************************
* @brief Sample Rate => 1 sample = 96 cycles = 2us
* 0/96  = 500 000 Hz    -> 500 kS/s 
* 960   = 50 000 Hz     -> 50 kS/s
* 9600  = 5 000 Hz      -> 5 kS/s
* @note For DMA-Interaction 0.5 MHz is the max. memory
* 		access-time
**************************************************************
*/
#define CLOCK_DIV 9600

#define ADC_CHANNEL0 	(uint8_t) (0x1A)
#define ADC_CHANNEL1 	(uint8_t) (0x1B)
#define ADC_CHANNEL2 	(uint8_t) (0x1C)
#define ADC_SAMPLES		1000

/*=========================================================*/
/*== DMA MACROS ===========================================*/
/*=========================================================*/



/*=========================================================*/
/*== PROTOTYPE DECLARATION ================================*/
/*=========================================================*/
uint8_t WATERLEVEL_AdcSet(void);
int8_t WATERLEVEL_DmaSet(void);
float32_t WATERLEVEL_Run(void);

#endif