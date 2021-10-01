/*!
*****************************************************************
* Copyright (c) Lukasz Piatek. All rights reserved.
* @file    bme280.c
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   BME280 Driver
*****************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bme280.h"
#include "hardware/i2c.h"
#include "WaterPipe.h" /* Insert for Error Log Function! */


/*! @file bme280.c
 * @brief Sensor driver for BME280 sensor
 */

/*-------------------------------------------------------------*/
/* FUNCTION DEFINITION ----------------------------------------*/
/*-------------------------------------------------------------*/

/*!
 *************************************************************************
 * @brief Attempt to read the chip-id number of BM*-280 device
 *
 * @param[in]  void
 * @param[out] void  
 *
 * @return Result of reading the ID-register for chip identification
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail

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
    else if (*ptrData == ( BMP280_CHIP_ID_MP  || BMP280_CHIP_ID_SP))
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
    debugMsg("------------- BME280 SETTING MODE STARTED -------------\r\n");
    uint8_t ptrWrite[] = {BME280_CTRL_MEAS_ADDR};
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    size_t lenWrite = sizeof(ptrWrite);
    size_t lenRead = 1;
    uint8_t ptrRead[1];


    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWrite, lenWrite, false);
    debugVal("-- Writing Mode Register:0x%02X -- \r\n", (*ptrWrite));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading Mode Register:0x%02X -- \r\n", (*ptrRead));


    status = *ptrRead & ~(BME280_MODE_MSK);
    status |= (deviceMode & BME280_MODE_MSK); //set deviceMode for BME280_NORMAL_MODE
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
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
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
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    debugVal("-- Reading Status Register:0x%02X -- \r\n", (*ptrData));
    //*ptrData &= ~(BME280_STATUS_MSK); //Clear unused or corrupt bits
    *ptrData &= ~BME280_STATUS_MSK;
    switch (*ptrData)
    {
    case    0x09:
            debugMsg("-- New Measuring & pre-measured Data-storeage in progress --\r\n");
            return -1;
        break;
    case    0x08:
            debugMsg("-- Measuring in progress --\r\n");
        break;
    case    0x01:
            debugMsg("-- Data-Storeage in progress --\r\n");
        break;
    case    0x00:
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

int8_t BME280_ReadComp(void)
{
    debugMsg("------------- BME280 COMP DATA READ STARTED -------------\r\n");
    /* Table 16 : Compensation parameter storage, naming and data type */
    struct CompData Comp;
    struct CompData *ptrComp;
    ptrComp = &Comp;

    uint8_t buffer[BME280_TEMP_PRESS_CALIB_DATA_LEN + BME280_HUMIDITY_CALIB_DATA_LEN] = {0};
    uint8_t ptrData[] = {BME280_REGISTER_DIG_T1};
  

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, 1, false);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, buffer, 24, true);


    ptrComp->dig_T1 = buffer[0]     | (buffer[1] << 8);     //0x88 / 0x89 dig_T1 [7:0] / [15:8] uint8_t
    ptrComp->dig_T2 = buffer[2]     | (buffer[3] << 8);     //0x8A / 0x8B dig_T2 [7:0] / [15:8] int16_t
    ptrComp->dig_T3 = buffer[4]     | (buffer[5] << 8);     //0x8C / 0x8D dig_T3 [7:0] / [15:8] int16_t

    ptrComp->dig_P1 = buffer[6]     | (buffer[7] << 8);     //0x8E / 0x8F dig_P1 [7:0] / [15:8] uint16_t
    ptrComp->dig_P2 = buffer[8]     | (buffer[9] << 8);     //0x90 / 0x91 dig_P2 [7:0] / [15:8] int16_t
    ptrComp->dig_P3 = buffer[10]    | (buffer[11] << 8);    //0x92 / 0x93 dig_P3 [7:0] / [15:8] int16_t
    ptrComp->dig_P4 = buffer[12]    | (buffer[13] << 8);    //0x94 / 0x95 dig_P4 [7:0] / [15:8] int16_t
    ptrComp->dig_P5 = buffer[14]    | (buffer[15] << 8);    //0x96 / 0x97 dig_P5 [7:0] / [15:8] int16_t
    ptrComp->dig_P6 = buffer[16]    | (buffer[17] << 8);    //0x98 / 0x99 dig_P6 [7:0] / [15:8] int16_t
    ptrComp->dig_P7 = buffer[18]    | (buffer[19] << 8);    //0x9A / 0x9B dig_P7 [7:0] / [15:8] int16_t
    ptrComp->dig_P8 = buffer[20]    | (buffer[21] << 8);    //0x9C / 0x9D dig_P8 [7:0] / [15:8] int16_t
    ptrComp->dig_P9 = buffer[22]    | (buffer[23] << 8);    //0x9E / 0x9F dig_P9 [7:0] / [15:8] int16_t

    /*This Same Register-> BME280_HUMIDITY_CALIB_DATA_LEN-1 */
    ptrComp->dig_H1 = buffer[24]; //0xA1 dig_H1 [7:0] uint8_t

    *ptrData = BME280_REGISTER_DIG_H2;
    uint8_t dataLen = BME280_HUMIDITY_CALIB_DATA_LEN - 1;

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, 1, false);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, &buffer[25], dataLen, true);
    /* read_registers(0xE1, buffer, 8); */
    //memset(&buffer[25], 0, 8);
    ptrComp->dig_H2 = buffer[25] | (buffer[26] << 8);               //0xE1 / 0xE2 dig_H2 [7:0] / [15:8] int16_t
    ptrComp->dig_H3 = buffer[27];                                   //0xE3 dig_H3 [7:0] uint8_t
    ptrComp->dig_H4 = buffer[28] << 4 | ((buffer[29]) & ~(0x78));   //0xE4 / 0xE5[3:0] dig_H4 [11:4] / [3:0] int16_t
    ptrComp->dig_H5 = (buffer[30] >> 4) | (buffer[31] << 4);        //0xE5[7:4] / 0xE6 dig_H5 [3:0] / [11:4] int16_t
    ptrComp->dig_H6 = buffer[32];                                   //0xE7 dig_H6 int8_t
    memset(buffer,'\0', 33);
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
    debugVal("-- Writing STANDBY Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    debugVal("-- Reading STANDBY Register:0x%02X -- \r\n", (*ptrData));

    status = *ptrData & ~BME280_STBY_MSK;
    status |= tsb & BME280_STBY_MSK;
    mode = status;
    debugVal("-- Setting Mode:0x%02X -- \r\n", status);

    uint8_t ptrWriteMode[] = {BME280_CONFIG_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);

}
void BME280_ReadStandby()
{
    debugMsg("------------- BME280 STANDBY STATUS STARTED -------------\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    uint8_t ptrRead[1];
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("-- Writing STANDBY Register:0x%02X -- \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("-- Reading STANDBY Register:0x%02X -- \r\n", (*ptrRead));
}
