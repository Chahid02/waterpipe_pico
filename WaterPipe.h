/*!
*****************************************************************
* Copyright (c) Lukasz Piatek. All rights reserved.
* @file    WaterPipe.h
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   Default main function.
*****************************************************************
*/

#ifndef WATERPIPE_H_
#define WATERPIPE_H_

/*-------------------------------------------------------------*/
/* PRIVATE MACROS ---------------------------------------------*/
/*-------------------------------------------------------------*/

/* SET LOG INFO */
#define LOG_ERROR(MSG)                                                                \
    {                                                                                 \
        printf("ErrorLog: %s [%s:%d]\n\n", MSG, __FUNCTION__, __LINE__); \
    }

/* DISABLE GoTo */
#pragma GCC poison goto

/* SET GPIO MACROS */
#define LED 25

/* SET USER __NOP() MACRO */
#define __NOP() __asm("NOP");

/* SET DEBUG MODE ON/OFF */
#define DEBUG 1 

/*-------------------------------------------------------------*/
/* DEBUG DEFINITION -------------------------------------------*/
/*-------------------------------------------------------------*/
#if DEBUG == 1
#define debugModMsg         printf("-- DEBUGGING MODE IS ON --\n")
#define debugMsg(x)         printf(x)
#define debugVal(x, y)      printf(x, y)
#define debug2Val(x, y, z)  printf(x, y, z)
#else
#define debugModMsg         printf("-- DEBUGGING MODE IS OFF--\n")
#define debugMsg(x) 
#define debugVal(x, y) 
#define debug2Val(x, y, z)
#endif

/*-------------------------------------------------------------*/
/* PROTOTYPE DECLARATION --------------------------------------*/
/*-------------------------------------------------------------*/
//void printBinary(uint8_t i, int8_t *strArray);
void printBinary(uint8_t i, char*strArray);//Just for the Compiler Warning

#endif