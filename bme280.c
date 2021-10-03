/*!
*****************************************************************
* Copyright (c) Lukasz Piatek. All rights reserved.
* @file    bme280.c
* @brief   BME280 Driver
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   BME280 Driver
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
*****************************************************************
*/

/*-------------------------------------------------------------*/
/* INCLUDES ---------------------------------------------------*/
/*-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bme280.h"
#include "hardware/i2c.h"
#include "waterpipe.h" /* Insert for Error Log Function! */

/*-------------------------------------------------------------*/
/* FUNCTION DEFINITION ----------------------------------------*/
/*-------------------------------------------------------------*/

/*!
 *************************************************************************
 * @brief Attempt to read the chip-id number of BM*-280 device
 * 
 * @note asdasdas
 * 
 * @warning
 * 
 * @param[in]  deviceAddr Description
 * @param[out]
 *
 * @return Result of reading the ID-register for chip identification
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 *************************************************************************
 */
int8_t BME280ChipID(void)
{
    debugMsg("------------- BME280 CHIP INIT PROGRESS STARTED -------------\r\n");
    size_t lenWrite = sizeof(BME280_CHIP_ID_ADDR);
    size_t lenRead = sizeof(BME280_CHIP_ID);
    //int8_t rslt;
    uint8_t ptrData[lenRead];
    uint8_t ptrAdrr[lenWrite];
    *ptrAdrr = BME280_CHIP_ID_ADDR;

    if (i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrAdrr, lenWrite, false) != lenWrite)
    {
        LOG_ERROR("-- Device not found -- ErrorCode: -2 --");
        debugVal("-- Device not found -- ErrorCode:%X --\r\n", BME280_E_DEV_NOT_FOUND);
    }
    else
    {
        debugVal("-- Writing to Address:0x%02X -- \r\n", (*ptrAdrr));
    }
    sleep_ms(10);

    if (i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false) == lenRead)
    {
        debugVal("-- Reading Register:0x%02X -- \r\n", (*ptrAdrr));
    }
    else
    {
        LOG_ERROR("-- No Data Received -- Reading I2C ChipID failed --")
    }

    sleep_ms(10);
    debugVal("-- Chip ID:0x%02X -- \r\n", (*ptrData));

    if (*ptrData == BME280_CHIP_ID)
    {
        debugMsg("-- Chip is BME280 --\r\n");
        return 0;
    }
    else if (*ptrData == (BMP280_CHIP_ID_MP || BMP280_CHIP_ID_SP))
    {
        debugMsg("-- Chip is BMP280 -- Wrong Sensor --\r\n");
        LOG_ERROR("-- WRONG SENSOR --")
        debugMsg("-- ALARM SET --\r\n");
        //PANIC FUNCTION !!!!
        return -1;
    }
    else
    {
        debugMsg("-- No specific BM*-280 ID !!! CHIP IS CORRUPT --\r\n");
        LOG_ERROR("-- CHIP IS CORRUPT --")
        debugMsg("-- ALARM SET --\r\n");
        //PANIC FUNCTION !!!!
        return -1;
    }
}
/*!
 *************************************************************************
 * @brief Test
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void funcReadRegister(uint8_t deviceAddr, uint8_t *regAddr, uint8_t *regData, size_t lenRead)
{
    uint8_t Data[lenRead];
    i2c_write_blocking(i2c_default, deviceAddr, regAddr, lenRead, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, deviceAddr, regData, lenRead, false);
    sleep_ms(10);

    for (uint8_t i = 0; i < lenRead; i++)
    {
        Data[i] = regData[i];
        printf("Register:%d Data:0x%X \r\n", i, Data[i]);
    }
}

/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
int8_t BME280_SoftReset(void)
{
    debugMsg("------------- BME280 SOFTRESET PROGRESS STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_SOFTRESET_ADDR, BME280_SOFTRESET_VALUE};
    size_t lenWrite = sizeof(ptrData);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debug2Val("-- Writing SoftReset:0x%02X with Value:0x%X-- \r\n", ptrData[0], ptrData[1]);
    return 0;
}

/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
int8_t BME280_SetMode(uint8_t deviceMode)
{
    debugMsg("------------- BME280 MODE SETTING STARTED -------------\r\n");
    uint8_t ptrWrite[] = {BME280_CTRL_MEAS_ADDR};
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    size_t lenWrite = sizeof(ptrWrite);
    size_t lenRead = 1;
    uint8_t ptrRead[1];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWrite, lenWrite, false);
    sleep_ms(10);
    debugVal("-- Writing Mode Register:0x%02X -- \r\n", (*ptrWrite));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading Mode Register:0x%02X -- \r\n", (*ptrRead));

    status = *ptrRead & ~(BME280_MODE_MSK);
    status |= (deviceMode & BME280_MODE_MSK);

    measureMode = deviceMode;

    debugVal("-- Setting Mode:0x%02X -- \r\n", status);
    mode = status;

    uint8_t ptrWriteMode[] = {BME280_CTRL_MEAS_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);

    return 0;
}

/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
int8_t BME280_ReadMode(void)
{
    debugMsg("------------- BME280 MODE STATUS STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    uint8_t status = 0;
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading Mode Register:0x%02X -- \r\n", (*ptrData));

    return status;
}
/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
int8_t BME280_ReadStatus(void)
{
    debugMsg("------------- BME280 UPDATE STATUS STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_REGISTER_STATUS};
    size_t lenWrite = sizeof(BME280_REGISTER_STATUS);
    size_t lenRead = sizeof(BME280_REGISTER_STATUS);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading Status Register:0x%02X -- \r\n", (*ptrData));
    //*ptrData &= ~(BME280_STATUS_MSK); //Clear unused or corrupt bits
    *ptrData &= ~BME280_STATUS_MSK;
    switch (*ptrData)
    {
    case 0x09:
        debugMsg("-- New Measuring & pre-measured Data-storage in progress --\r\n");
        break;
    case 0x08:
        debugMsg("-- Measuring in progress --\r\n");
        break;
    case 0x01:
        debugMsg("-- Data-Storage in progress --\r\n");
        break;
    case 0x00:
        debugMsg("-- Measuring-Data available --\r\n");
        break;

    default:
        debugVal("-- Corrupt Register @ 0x%02X @[7:4]-[2:1] --\r\n", BME280_REGISTER_STATUS);
        //*ptrData &= ~(BME280_STATUS_MSK); //Clear unused or corrupt bits
        *ptrData &= (0x06);
        break;
    }

    *ptrData &= ~0x09;
    //*ptrData &=BME280_STATUS_MSK; //True if measuring and saving NVM clear their bits

    return (uint8_t)*ptrData;
}

/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
int8_t BME280_ReadComp(void)
{
    debugMsg("------------- BME280 COMP DATA READ STARTED -------------\r\n");
    /* Table 16 : Compensation parameter storage, naming and data type */
 
    ptrComp = &Comp;

    uint8_t buffer[BME280_TEMP_PRESS_CALIB_DATA_LEN + BME280_HUMIDITY_CALIB_DATA_LEN] = {0};
    uint8_t ptrData[] = {BME280_REGISTER_DIG_T1};

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, 1, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, buffer, 24, true);
    sleep_ms(10);

    ptrComp->dig_T1 = buffer[0] | (buffer[1] << 8); //0x88 / 0x89 dig_T1 [7:0] / [15:8] uint8_t
    ptrComp->dig_T2 = buffer[2] | (buffer[3] << 8); //0x8A / 0x8B dig_T2 [7:0] / [15:8] int16_t
    ptrComp->dig_T3 = buffer[4] | (buffer[5] << 8); //0x8C / 0x8D dig_T3 [7:0] / [15:8] int16_t
    ptrComp->dig_P1 = buffer[6] | (buffer[7] << 8); //0x8E / 0x8F dig_P1 [7:0] / [15:8] uint16_t
    ptrComp->dig_P2 = buffer[8] | (buffer[9] << 8); //0x90 / 0x91 dig_P2 [7:0] / [15:8] int16_t

    ptrComp->dig_P3 = buffer[10] | (buffer[11] << 8); //0x92 / 0x93 dig_P3 [7:0] / [15:8] int16_t
    ptrComp->dig_P4 = buffer[12] | (buffer[13] << 8); //0x94 / 0x95 dig_P4 [7:0] / [15:8] int16_t
    ptrComp->dig_P5 = buffer[14] | (buffer[15] << 8); //0x96 / 0x97 dig_P5 [7:0] / [15:8] int16_t
    ptrComp->dig_P6 = buffer[16] | (buffer[17] << 8); //0x98 / 0x99 dig_P6 [7:0] / [15:8] int16_t
    ptrComp->dig_P7 = buffer[18] | (buffer[19] << 8); //0x9A / 0x9B dig_P7 [7:0] / [15:8] int16_t
    ptrComp->dig_P8 = buffer[20] | (buffer[21] << 8); //0x9C / 0x9D dig_P8 [7:0] / [15:8] int16_t
    ptrComp->dig_P9 = buffer[22] | (buffer[23] << 8); //0x9E / 0x9F dig_P9 [7:0] / [15:8] int16_t

    /*This Same Register-> BME280_HUMIDITY_CALIB_DATA_LEN-1 */
    ptrComp->dig_H1 = buffer[24];                               //0xA1 dig_H1 [7:0] uint8_t

    *ptrData = BME280_REGISTER_DIG_H2;
    uint8_t dataLen = BME280_HUMIDITY_CALIB_DATA_LEN - 1;

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, 1, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, &buffer[25], dataLen, true);
    sleep_ms(10);
    //memset(&buffer[25], 0, 8);
    ptrComp->dig_H2 = buffer[25] | (buffer[26] << 8);             //0xE1 / 0xE2 dig_H2 [7:0] / [15:8] int16_t
    ptrComp->dig_H3 = buffer[27];                                 //0xE3 dig_H3 [7:0] uint8_t
    ptrComp->dig_H4 = buffer[28] << 4 | ((buffer[29]) & ~(0x78)); //0xE4 / 0xE5[3:0] dig_H4 [11:4] / [3:0] int16_t
    ptrComp->dig_H5 = (buffer[30] >> 4) | (buffer[31] << 4);      //0xE5[7:4] / 0xE6 dig_H5 [3:0] / [11:4] int16_t
    ptrComp->dig_H6 = buffer[32];                                 //0xE7 dig_H6 int8_t
    memset(buffer, '\0', 33);
    printCompParam(ptrComp);

    return 0;
}
/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void printCompParam(struct CompData *ptrComp)
{
    debugVal("-- dig_T1:%d --\r\n", ptrComp->dig_T1);
    debugVal("-- dig_T2:%d --\r\n", ptrComp->dig_T2);
    debugVal("-- dig_T3:%d --\r\n", ptrComp->dig_T3);
    debugVal("-- dig_P1:%d --\r\n", ptrComp->dig_P1);
    debugVal("-- dig_P2:%d --\r\n", ptrComp->dig_P2);
    debugVal("-- dig_P3:%d --\r\n", ptrComp->dig_P3);
    debugVal("-- dig_P4:%d --\r\n", ptrComp->dig_P4);
    debugVal("-- dig_P5:%d --\r\n", ptrComp->dig_P5);
    debugVal("-- dig_P6:%d --\r\n", ptrComp->dig_P6);
    debugVal("-- dig_P7:%d --\r\n", ptrComp->dig_P7);
    debugVal("-- dig_P8:%d --\r\n", ptrComp->dig_P8);
    debugVal("-- dig_P9:%d --\r\n", ptrComp->dig_P9);
    debugVal("-- dig_H1:%d --\r\n", ptrComp->dig_H1);
    debugVal("-- dig_H2:%d --\r\n", ptrComp->dig_H2);
    debugVal("-- dig_H3:%d --\r\n", ptrComp->dig_H3);
    debugVal("-- dig_H4:%d --\r\n", ptrComp->dig_H4);
    debugVal("-- dig_H5:%d --\r\n", ptrComp->dig_H5);
    debugVal("-- dig_H6:%d --\r\n", ptrComp->dig_H6);
}
/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_SetStandby(uint8_t tsb)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("------------- BME280 STANDBY STATUS STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    debugVal("-- Writing STANDBY Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading STANDBY Register:0x%02X -- \r\n", (*ptrData));

    status = *ptrData & ~BME280_STBY_MSK;
    status |= tsb & BME280_STBY_MSK;
    mode = status;
    debugVal("-- Setting STANDBY:0x%02X -- \r\n", status);

    uint8_t ptrWriteMode[] = {BME280_CONFIG_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_ReadStandby(void)
{
    debugMsg("------------- BME280 STANDBY STATUS STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    uint8_t ptrRead[1];
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading STANDBY Register:0x%02X -- \r\n", (*ptrRead));
}
/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
/*
Oversampling settings pressure ×16, 
temperature ×2, humidity ×1 IIR filter settings filter coefficient 16
*/
void BME280_SetFilter(uint8_t filter)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("------------- BME280 FILTER SETTING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    debugVal("-- Writing FILTER Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading FILTER Register:0x%02X -- \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_FILTER_MSK;
    status |= filter & BME280_FILTER_MSK;
    mode = status;

    filtCoeff=filter;

    debugVal("-- Setting Mode:0x%02X -- \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CONFIG_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);

}
/*!
 *************************************************************************
 * @brief 
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_ReadFilter(void)
{
    debugMsg("------------- BME280 FILTER READING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("-- Reading FILTER Register:0x%02X -- \r\n", (*ptrRead));
}
/*!
 *************************************************************************
 * @brief 
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_Set_OSRS_t(uint8_t osrs_t)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("------------- BME280 OSRS_t SETTING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing OSRS_t Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading OSRS_t Register:0x%02X -- \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_OSRS_T_MSK;
    status |= osrs_t & BME280_OSRS_T_MSK;
    mode = status;
    ovsTime=osrs_t;

    debugVal("-- Setting OSRS_t:0x%02X -- \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CTRL_MEAS_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
 *************************************************************************
 * @brief 
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_Set_OSRS_p(uint8_t osrs_p)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("------------- BME280 OSRS_p SETTING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing OSRS_p Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading OSRS_p Register:0x%02X -- \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_OSRS_P_MSK;
    status |= osrs_p & BME280_OSRS_P_MSK;
    mode = status;
    ovsPressure=osrs_p;

    debugVal("-- Setting OSRS_p:0x%02X -- \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CTRL_MEAS_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}

/*!
 *************************************************************************
 * @brief This function has to be set as last param after writing ctrl_hum
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_Set_OSRS_h(uint8_t osrs_h)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("------------- BME280 OSRS_h SETTING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CTRL_HUM_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_HUM_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_HUM_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing OSRS_h Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading OSRS_h Register:0x%02X -- \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_OSRS_H_MSK;
    status |= osrs_h & BME280_OSRS_H_MSK;
    mode = status;
    ovsHumidity = osrs_h;

    debugVal("-- Setting OSRS_h:0x%02X -- \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CTRL_HUM_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);

    *ptrData = BME280_CTRL_MEAS_ADDR;
    lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing CTRL_MEAS Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading CTRL_MEAS Register:0x%02X -- \r\n", (*ptrRead));

    
    ptrWriteMode[0] = BME280_CTRL_MEAS_ADDR;
    ptrWriteMode[0] = *ptrRead;
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
 *************************************************************************
 * @brief 
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_Read_OSRS_h(void)
{
    debugMsg("------------- BME280 OSRS_h READING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CTRL_HUM_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_HUM_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_HUM_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing OSRS_h Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading OSRS_h Register:0x%02X -- \r\n", (*ptrRead));
}

/*!
 *************************************************************************
 * @brief 
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */
void BME280_Read_CTRL_MEAS(void)
{
    debugMsg("------------- BME280 CTRL_MEAS READING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing CTRL_MEAS Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading CTRL_MEAS Register:0x%02X -- \r\n", (*ptrRead));
}
/*!
 *************************************************************************
 * @brief 
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */

int32_t compensate_temperature()
{
    int32_t adc_T=temp;
    int32_t var1;
    int32_t var2;
    int32_t temperature;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    var1 = (((adc_T >> 3 ) - ((int32_t)Comp.dig_T1 << 1)) * ((int32_t)Comp.dig_T2 )) >> 11;
    var2 =  ((((adc_T >> 4) - ((int32_t)Comp.dig_T1)) * ((adc_T >> 4) -\
            ((int32_t)Comp.dig_T1))) >> 12 * ((int32_t)Comp.dig_T3)) >> 14;
    t_fine = var1 + var2;
    temperature = (t_fine * 5 + 128 ) >> 8;

    if (temperature < temperature_min)
    {
        temperature = temperature_min;
    }
    else if (temperature > temperature_max)
    {
        temperature = temperature_max;
    }

    return temperature;
}
/*!
 *************************************************************************
 * @brief 
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */

void BME_RawData(void)
{
    uint8_t buffer[8];
    


    debugMsg("------------- BME280 RAW DATA READING STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_DATA_ADDR};
    size_t lenWrite = sizeof(BME280_DATA_ADDR);
    size_t lenRead = sizeof(buffer);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);

    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, buffer, lenRead, true);

    press = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | (buffer[2] >> 4);
    temp= ((uint32_t)buffer[3] << 12) | ((uint32_t)buffer[4] << 4) | (buffer[5] >> 4);
    hum = (uint32_t)buffer[6] << 8 | buffer[7];

    debugVal("-- temp:%X --\r\n", temp);
    debugVal("-- hum:%X --\r\n", hum) ;
    debugVal("-- press:%X--\r\n", press);

}

/*!
 *************************************************************************
 * @brief 
 * the ctrl_meas register has to be set
 *
 *
 * @param[in]  deviceAddr Description
 * @param[out]   
 *
 * @return Result of 
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
 * 
 *************************************************************************
 */

void BME280_MeasurementTime()
{
    float32_t timeTyp;
    float32_t timeMax;
    uint8_t osTime;
    uint8_t osHum;
    uint8_t osPress;
    float32_t stdByMode;

    switch (ovsTime)
    {
    case BME280_OSRS_T_SKIP:
        osTime = 0;
        break;
    case BME280_OSRS_T_x1:
        osTime = 1;
        break;
    case BME280_OSRS_T_x2:
        osTime = 2;
        break;
    case BME280_OSRS_T_x4:
        osTime = 4;
        break;
    case BME280_OSRS_T_x8:
        osTime = 8;
        break;
    case BME280_OSRS_T_x16:
        osTime = 16;
        break;

    default:
        break;
    }

    switch (ovsHumidity)
    {
    case BME280_OSRS_H_SKIP:
        osHum = 0;
        break;
    case BME280_OSRS_H_x1:
        osHum = 1;
        break;
    case BME280_OSRS_H_x2:
        osHum = 2;
        break;
    case BME280_OSRS_H_x4:
        osHum = 4;
        break;
    case BME280_OSRS_H_x8:
        osHum = 8;
        break;
    case BME280_OSRS_H_x16:
        osHum = 16;
        break;

    default:
        break;
    }

    switch (ovsPressure)
    {
    case BME280_OSRS_P_SKIP:
        osPress = 0;
        break;
    case BME280_OSRS_P_x1:
        osPress = 1;
        break;
    case BME280_OSRS_P_x2:
        osPress = 2;
        break;
    case BME280_OSRS_P_x4:
        osPress = 4;
        break;
    case BME280_OSRS_P_x8:
        osPress = 8;
        break;
    case BME280_OSRS_P_x16:
        osPress = 16;
        break;

    default:
        break;
    }

    timeTyp = 1 + (2 * osTime) + (2 * osPress + 0.5) + (2 * osHum + 0.5);
    timeMax = 1.25 + (2.3 * osTime) + (2.3 * osPress + 0.5) + (2.3 * osHum + 0.575);

    switch (stdBy)
    {
    case BME280_STBY_0_5:
        stdByMode = 0.5;
        break;
    case BME280_STBY_62_5:
        stdByMode = 62.5;
        break;
    case BME280_STBY_125:
        stdByMode = 125;
        break;
    case BME280_STBY_250:
        stdByMode = 250;
        break;
    case BME280_STBY_500:
        stdByMode = 5;
        break;
    case BME280_STBY_1000:
        stdByMode = 1000;
        break;
    case BME280_STBY_10:
        stdByMode = 10;
        break;
    case BME280_STBY_20:
        stdByMode = 20;
        break;

    default:
        break;
    }

    float32_t odrMs;
    if (measureMode == BME280_NORMAL_MODE)
    {
        odrMs = 1000 / (timeMax + stdByMode);
    }
    else if (measureMode == BME280_FORCED_MODE)
    {
        odrMs = 1000 / (timeMax );

    }
    else
    {
        __NOP();
    }
    
    uint32_t stepRsp = 0;

    switch (filtCoeff)
    {
    case BME280_FILTER_OFF:
        stepRsp = 1;
        break;
    case BME280_FILTER_2:
        stepRsp = 2;
        break;
    case BME280_FILTER_4:
        stepRsp = 5;
        break;
    case BME280_FILTER_8:
        stepRsp = 11;
        break;
    case BME280_FILTER_16:
        stepRsp = 22;
        break;

    default:
        break;
    }
   
    float32_t rspTimeIIR;
    rspTimeIIR = 1000 * stepRsp / odrMs;

    debug2Val("-- MeasurementRate: %f Hz\n-- ResponseTime:%.2f ms\n", odrMs, rspTimeIIR);
    debug2Val("-- Typ. MeasurementTime: %f ms\n-- Max. MeasurementTime:%.2f ms\n", timeTyp, timeMax);
}
