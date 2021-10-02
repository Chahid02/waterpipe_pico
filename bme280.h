/*!
*****************************************************************
* Copyright (c) Lukasz Piatek. All rights reserved.
* @file    bme280.h
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   BME280 Driver
*****************************************************************
*/

#ifndef BME280_H_
#define BME280_H_


/*-------------------------------------------------------------*/
/* I2C ADRESS */
/*-------------------------------------------------------------*/
#define BME280_I2C_ADDR_PRIMARY     (uint8_t) 0x76
#define BME280_I2C_ADDR_SECONDARY   (uint8_t) 0x77
/*-------------------------------------------------------------*/
/* CHIP IDENTIFIER */
/*-------------------------------------------------------------*/
#define BME280_CHIP_ID          (uint8_t) 0x60
#define BMP280_CHIP_ID_SP       (uint8_t) 0x56
#define BMP280_CHIP_ID_MP       (uint8_t) 0x58

/*-------------------------------------------------------------*/
/* REGISTER ADRESS */
/*-------------------------------------------------------------*/
#define BME280_CHIP_ID_ADDR                     (uint8_t) 0xD0
#define BME280_RESET_ADDR                       (uint8_t) 0xE0
#define BME280_TEMP_PRESS_CALIB_DATA_ADDR       (uint8_t) 0x88
#define BME280_HUMIDITY_CALIB_DATA_ADDR         (uint8_t) 0xE1
#define BME280_CTRL_HUM_ADDR                    (uint8_t) 0xF2
#define BME280_CTRL_MEAS_ADDR                   (uint8_t) 0xF4
#define BME280_CONFIG_ADDR                      (uint8_t) 0xF5
#define BME280_DATA_ADDR                        (uint8_t) 0xF7

/*-------------------------------------------------------------*/
/* RESET REGISTER/MACROS */
/*-------------------------------------------------------------*/
#define BME280_SOFTRESET_ADDR       (uint8_t) 0xE0 //BME280 SOFT RESET REGISTER
#define BME280_SOFTRESET_VALUE      (uint8_t) 0xB6 //BME280 SOFT RESET VALUE
#define BME280_SOFTRESET_MSK        (uint8_t) 0x09
#
#define BME280_REGISTER_STATUS      (uint8_t) 0xF3 //BME280 STATUS REGISTER
#define BME280_STATUS_MEASURING     (uint8_t) 0x08 //Running conversion
#define BME280_STATUS_IM_UPDATE     (uint8_t) 0x01 //NVM data copying
#define BME280_STATUS_MSK           (uint8_t) 0xF6 //Corrupt BitMask

/*-------------------------------------------------------------*/
/* STANDBY MACROS*/
/*-------------------------------------------------------------*/
#define BME280_STBY_MSK     (uint8_t) 0xE0
#define BME280_STBY_0_5     (uint8_t) 0x00
#define BME280_STBY_62_5    (uint8_t) 0x20
#define BME280_STBY_125     (uint8_t) 0x40
#define BME280_STBY_250     (uint8_t) 0x60
#define BME280_STBY_500     (uint8_t) 0x80
#define BME280_STBY_1000    (uint8_t) 0xA0
#define BME280_STBY_10      (uint8_t) 0xC0
#define BME280_STBY_20      (uint8_t) 0xE0

/*-------------------------------------------------------------*/
/* DATA SIZE */
/*-------------------------------------------------------------*/
#define BME280_TEMP_PRESS_CALIB_DATA_LEN    (uint8_t) 24
#define BME280_HUMIDITY_CALIB_DATA_LEN      (uint8_t) 9
#define BME280_P_T_H_DATA_LEN               (uint8_t) 8

/*-------------------------------------------------------------*/
/* SENSOR MODES */
/*-------------------------------------------------------------*/
#define BME280_MODE_MSK     (uint8_t) 0x03
#define BME280_SLEEP_MODE   (uint8_t) 0x00
#define BME280_FORCED_MODE  (uint8_t) 0x01
#define BME280_NORMAL_MODE  (uint8_t) 0x03

/*-------------------------------------------------------------*/
/* DELAY CALCULATION MACROS */
/*-------------------------------------------------------------*/
#define BME280_MEAS_OFFSET          (uint16_t) 1250
#define BME280_MEAS_DUR             (uint16_t) 2300
#define BME280_PRES_HUM_MEAS_OFFSET (uint16_t) 575
#define BME280_MEAS_SCALING_FACTOR  (uint16_t) 1000

/*-------------------------------------------------------------*/
/* COMPENSATION PARAMETER */
/*-------------------------------------------------------------*/
typedef struct CompData
{
    uint16_t 	dig_T1:16;
    int16_t 	dig_T2:16;
    int16_t 	dig_T3:16;
    uint16_t 	dig_P1:16;
    int16_t 	dig_P2:16;
    int16_t 	dig_P3:16;
    int16_t 	dig_P4:16;
    int16_t 	dig_P5:16;
    int16_t 	dig_P6:16;
    int16_t 	dig_P7:16;
    int16_t 	dig_P8:16;
    int16_t 	dig_P9:16;
    uint8_t	 	dig_H1:8;
    int16_t 	dig_H2:16;
    uint8_t 	dig_H3:8;
    int16_t 	dig_H4:12;
    int16_t 	dig_H5:12;
    int8_t 		dig_H6:8;
}BME280_Comp;

/*-------------------------------------------------------------*/
/* COMPENSATION REGISTER */
/*-------------------------------------------------------------*/
#define BME280_REGISTER_DIG_T1 (uint8_t) 0x88
#define BME280_REGISTER_DIG_T2 (uint8_t) 0x8A
#define BME280_REGISTER_DIG_T3 (uint8_t) 0x8C
#define BME280_REGISTER_DIG_P1 (uint8_t) 0x8E
#define BME280_REGISTER_DIG_P2 (uint8_t) 0x90
#define BME280_REGISTER_DIG_P3 (uint8_t) 0x92
#define BME280_REGISTER_DIG_P4 (uint8_t) 0x94
#define BME280_REGISTER_DIG_P5 (uint8_t) 0x96
#define BME280_REGISTER_DIG_P6 (uint8_t) 0x98
#define BME280_REGISTER_DIG_P7 (uint8_t) 0x9A
#define BME280_REGISTER_DIG_P8 (uint8_t) 0x9C
#define BME280_REGISTER_DIG_P9 (uint8_t) 0x9E
#define BME280_REGISTER_DIG_H1 (uint8_t) 0xA1
#define BME280_REGISTER_DIG_H2 (uint8_t) 0xE1
#define BME280_REGISTER_DIG_H3 (uint8_t) 0xE3
#define BME280_REGISTER_DIG_H4 (uint8_t) 0xE4
#define BME280_REGISTER_DIG_H5 (uint8_t) 0xE5
#define BME280_REGISTER_DIG_H6 (uint8_t) 0xE7
/*-------------------------------------------------------------*/
/* FILTER MACROS */
/*-------------------------------------------------------------*/
#define BME280_FILTER_MSK   (uint8_t) 0x1C
#define BME280_FILTER_OFF   (uint8_t) 0x00
#define BME280_FILTER_2     (uint8_t) 0x04
#define BME280_FILTER_4     (uint8_t) 0x08
#define BME280_FILTER_8     (uint8_t) 0x0C
#define BME280_FILTER_16    (uint8_t) 0x10

/*-------------------------------------------------------------*/
/* OVERSAMPLING MACROS */
/*-------------------------------------------------------------*/
#define BME280_OSRS_T_MSK   (uint8_t) 0xE0
#define BME280_OSRS_T_SKIP  (uint8_t)0x00
#define BME280_OSRS_T_x1    (uint8_t)0x20
#define BME280_OSRS_T_x2    (uint8_t)0x40
#define BME280_OSRS_T_x4    (uint8_t)0x60
#define BME280_OSRS_T_x8    (uint8_t)0x80
#define BME280_OSRS_T_x16   (uint8_t)0xA0
#define BME280_OSRS_P_MSK   (uint8_t)0x1C
#define BME280_OSRS_P_SKIP  (uint8_t)0x00
#define BME280_OSRS_P_x1    (uint8_t)0x04
#define BME280_OSRS_P_x2    (uint8_t)0x08
#define BME280_OSRS_P_x4    (uint8_t)0x0C
#define BME280_OSRS_P_x8    (uint8_t)0x10
#define BME280_OSRS_P_x16   (uint8_t)0x14
#define BME280_OSRS_H_MSK   (uint8_t)0x07
#define BME280_OSRS_H_SKIP  (uint8_t)0x00
#define BME280_OSRS_H_x1    (uint8_t)0x01
#define BME280_OSRS_H_x2    (uint8_t)0x02
#define BME280_OSRS_H_x4    (uint8_t)0x03
#define BME280_OSRS_H_x8    (uint8_t)0x04
#define BME280_OSRS_H_x16   (uint8_t)0x05

/*-------------------------------------------------------------*/
/* ERROR CODES */
/*-------------------------------------------------------------*/
#define BME280_E_NULL_PTR           (int8_t) -1
#define BME280_E_DEV_NOT_FOUND      (int8_t) -2
#define BME280_E_INVALID_LEN        (int8_t) -3
#define BME280_E_COMM_FAIL          (int8_t) -4
#define BME280_E_SLEEP_MODE_FAIL    (int8_t) -5
#define BME280_E_NVM_COPY_FAILED    (int8_t) -6
#define BME280_E_INVALID_ID         (int8_t) -7

/*-------------------------------------------------------------*/
/* PROTOTYPE DECLARATION --------------------------------------*/
/*-------------------------------------------------------------*/
int8_t BME280ChipID(void);
int8_t BME280_ReadStatus(void);
int8_t BME280_SoftReset(void);
int8_t BME280_SetMode(uint8_t deviceMode);
int8_t BME280_ReadMode(void);
int8_t BME280_ReadComp(void);
void printCompParam(struct CompData *ptrComp);
void BME280_SetStandby(uint8_t tsb);
void BME280_ReadStandby(void);
void BME280_SetFilter(uint8_t filter);
void BME280_ReadFilter(void);
void BME280_Set_OSRS_h(uint8_t osrs_h);
void BME280_Read_OSRS_h(void);
void BME280_Set_OSRS_t(uint8_t osrs_t);
void BME280_Set_OSRS_p(uint8_t osrs_p);

void BME280_Read_CTRL_MEAS(void);

void funcReadRegister(uint8_t deviceAddr, uint8_t *regAddr, uint8_t *regData, size_t lenRead);

#endif