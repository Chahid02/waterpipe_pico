/*!
**************************************************************
* Copyright (c) Lukasz Piatek. All rights reserved.
* @file    waterpipe.h
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   Default main function.
**************************************************************
*/

#ifndef WATERPIPE_H_
#define WATERPIPE_H_

/*=========================================================*/
/*== PRIVATE MACROS =======================================*/
/*=========================================================*/

/*!< SET LOG INFO */
#define LOG_ERROR(MSG)                                                   \
    {                                                                    \
        printf("ErrorLog: %s [%s:%d]\n\n", MSG, __FUNCTION__, __LINE__); \
    }

/*!<  DISABLE GoTo */
#pragma GCC poison goto

/*=========================================================*/
/*== GPIO MACROS ==========================================*/
/*=========================================================*/

/*!< SET GPIO MACROS */
#define LED             25
#define TEMPERATURE_OK  9
#define PRESSURE_OK     10
#define HUMIDITY_OK     11
#define WATER_TEMP_OK   12
#define WATER_LEVEL_OK  14
#define PRESSURE_FSR_OK 13


/*!< SET USER __NOP() MACRO */
#define __NOP() __asm("NOP");

/*!< SET DEBUG MODE ON/OFF */
#define DEBUG 1

/*!< SET MONITOR MODE ON/OFF */
#define MONITOR 1
/*=========================================================*/
/*== DEBUG DEFINITION =====================================*/
/*=========================================================*/
#if DEBUG == 0
#define debugModMsg printf("[X] DEBUGGING MODE IS ON [X]\r\n")
#define debugMsg(x) printf(x)
#define debugVal(x, y) printf(x, y)
#define debug2Val(x, y, z) printf(x, y, z)
#else
#define debugModMsg printf("[X] DEBUGGING MODE IS OFF[X]\r\n")
#define debugMsg(x)
#define debugVal(x, y)
#define debug2Val(x, y, z)
#endif

/*=========================================================*/
/*== MONITORING DEFINITION ================================*/
/*=========================================================*/
#if MONITOR == 1
#define monitorModMsg printf("[X] MONITORING MODE IS ON [X]\r\n")
#define monitorMsg(x) printf(x)
#define monitorVal(x, y) printf(x, y)
#define monitor2Val(x, y, z) printf(x, y, z)
#else
#define monitorModMsg printf("[X] MONITORING MODE IS OFF[X]\r\n")
#define monitorMsg(x)
#define monitorVal(x, y)
#define monitor2Val(x, y, z)
#endif




/*=========================================================*/
/*== PROTOTYPE DECLARATION ================================*/
/*=========================================================*/

void debugTerm(void);
void toggleLed(void);
void toggleBuzz(void);
void core1_interrupt_handler(void);
#endif