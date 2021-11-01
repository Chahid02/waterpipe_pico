/*!
*****************************************************************
* @file    bme280.c
* @brief   BME280 Driver
* @author  Lukasz Piatek
* @version V1.0
* @date    2021-09-28
* @brief   BME280 Driver
* @copyright Copyright (c) Lukasz Piatek. All rights reserved.
*****************************************************************
*/

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

#include "hardware/i2c.h"

/*=========================================================*/
/*== PRIVATE INCLUDES =====================================*/
/*=========================================================*/

#include "bme280.h"
#include "waterpipe.h" /* Insert for Error Log Function! */

/*=========================================================*/
/*== FUNCTION DEFINITION ==================================*/
/*=========================================================*/

/*!
**************************************************************
 * @brief Attempt to read the chip-id number of BM*-280 device
 * 
 * @note asdasdas
 * 
 * @warning
 * 
 * @param[in]  deviceAddr Description
 * @param[out]
 *
 * @return Result of reading the ID-register for chip 
 * identification
 *
 * @retval = 0 -> Success
 * @retval > 0 -> Warning
 * @retval < 0 -> Fail
 *
**************************************************************
 */
int8_t BME280_CHIPID(void)
{
    debugMsg("====================  BME280 CHIP INIT PROGRESS STARTED  ============= \r\n");
    size_t lenChipAddr = sizeof(BME280_CHIP_ID_ADDR);
    size_t lenChipID = sizeof(BME280_CHIP_ID);
    uint8_t *ptrChipID = malloc(sizeof(*ptrChipID) * lenChipID);
    uint8_t *ptrChipAddr = malloc(sizeof(*ptrChipID) * lenChipAddr);

    /*== Check Memory ============================*/
    if (ptrChipID == NULL) /*== FAIL, NO MEMORY ==*/
    {
        LOG_ERROR("[X] Fail, No Memory Allocation [X] ErrorCode: -8 [X] ");
        debugVal("[X] Fail, No Memory Allocation [X] ErrorCode:%X [X] \r\n", BME280_E_MEMORY_ALLOC);
    }
    else if (ptrChipAddr == NULL)
    {
        LOG_ERROR("[X] Fail, No Memory Allocation [X] ErrorCode: -8 [X] ");
        debugVal("[X] Fail, No Memory Allocation [X] ErrorCode:%X [X] \r\n", BME280_E_MEMORY_ALLOC);
    }
    else
    {
        __NOP();
    }

    *ptrChipAddr = BME280_CHIP_ID_ADDR;

    if (i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrChipAddr, lenChipAddr, false) != lenChipAddr)
    {
        LOG_ERROR("[X] Device not found [X] ErrorCode: -2 [X] ");
        debugVal("[X] Device not found [X] ErrorCode:%X [X] \r\n", BME280_E_DEV_NOT_FOUND);
    }
    else
    {
        debugVal("[X] Writing to Address:0x%02X [X] \r\n", (*ptrChipAddr));
    }

    if (i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrChipID, lenChipID, false) == lenChipID)
    {
        debugVal("[X] Reading Register:0x%02X [X] \r\n", (*ptrChipAddr));
    }
    else
    {
        LOG_ERROR("[X] No Data Received [X] Reading I2C ChipID failed [X] ")
    }

    debugVal("[X] Chip ID:0x%02X [X] \r\n", (*ptrChipID));

    if (*ptrChipID == BME280_CHIP_ID)
    {
        debugMsg("[X] Chip is BME280 [X] \r\n");
        return 0;
    }
    else if (*ptrChipID == (BMP280_CHIP_ID_MP || BMP280_CHIP_ID_SP))
    {
        debugMsg("[X] Chip is BMP280 [X] Wrong Sensor [X] \r\n");
        LOG_ERROR("[X] WRONG SENSOR [X]\r\n ")
        debugMsg("[X] ALARM SET [X] \r\n");
        //PANIC FUNCTION !!!!
        return -1;
    }
    else
    {
        debugMsg("[X] No specific BM*-280 ID !!! CHIP IS CORRUPT [X] \r\n");
        LOG_ERROR("[X] CHIP IS CORRUPT [X] ")
        debugMsg("[X] ALARM SET [X] \r\n");
        //PANIC FUNCTION !!!!
        return -1;
    }
    free(ptrChipID);
    free(ptrChipAddr);
}

/*!
**************************************************************
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
**************************************************************
 */
int8_t BME280_SOFT_RESET(void)
{
    debugMsg("====================  BME280 SOFTRESET PROGRESS STARTED  ============= \r\n");
    size_t lenSoftRst = sizeof(BME280_SOFTRESET_ADDR) + sizeof(BME280_SOFTRESET_VALUE);
    uint8_t *ptrSoftRst = malloc(sizeof(*ptrSoftRst) * lenSoftRst);

    /*== MEMORY CHECK ============================*/
    if (ptrSoftRst == NULL) /*== FAIL, NO MEMORY ==*/
    {
        LOG_ERROR("[X] Fail, No Memory Allocation [X] ErrorCode: -8 [X] ");
        debugVal("[X] Fail, No Memory Allocation [X] ErrorCode:%X [X] \r\n", BME280_E_MEMORY_ALLOC);
    }
    else if (ptrSoftRst == NULL)
    {
        LOG_ERROR("[X] Fail, No Memory Allocation [X] ErrorCode: -8 [X] ");
        debugVal("[X] Fail, No Memory Allocation [X] ErrorCode:%X [X] \r\n", BME280_E_MEMORY_ALLOC);
    }
    else
    {
        __NOP();
    }

    *ptrSoftRst = BME280_SOFTRESET_ADDR;
    *(ptrSoftRst + 1) = BME280_SOFTRESET_VALUE;

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrSoftRst, lenSoftRst, false);

    if (i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrSoftRst, lenSoftRst, false) != lenSoftRst)
    {
        LOG_ERROR("[X] Softreset failed [X] ErrorCode: -2 [X] ");
        debugVal("[X] Softreset failed [X] ErrorCode:%X [X] \r\n", BME280_E_DEV_NOT_FOUND);
    }
    else
    {
        debug2Val("[X] Writing SoftReset:0x%02X with Value:0x%X [X] \r\n", ptrSoftRst[0], ptrSoftRst[1]);
    }

    free(ptrSoftRst);

    return 0;
}

/*!
**************************************************************
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
**************************************************************
 */
int8_t BME280_SET_MODE(uint8_t deviceMode)
{
    debugMsg("====================  BME280 MODE SETTING STARTED  ===================\r\n");
    uint8_t ptrWrite[] = {BME280_CTRL_MEAS_ADDR};
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    size_t lenWrite = sizeof(ptrWrite);
    size_t lenRead = 1;
    uint8_t ptrRead[1];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWrite, lenWrite, false);
    sleep_ms(10);
    debugVal("[X] Writing Mode Register:0x%02X [X] \r\n", (*ptrWrite));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading Mode Register:0x%02X [X] \r\n", (*ptrRead));

    status = *ptrRead & ~(BME280_MODE_MSK);
    status |= (deviceMode & BME280_MODE_MSK);

    measureMode = deviceMode;

    debugVal("[X] Setting Mode:0x%02X [X] \r\n", status);
    mode = status;

    uint8_t ptrWriteMode[] = {BME280_CTRL_MEAS_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);

    return 0;
}

/*!
**************************************************************
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
**************************************************************
 */
int8_t BME280_READ_MODE(void)
{
    debugMsg("====================  BME280 MODE STATUS STARTED  ================== \r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    uint8_t status = 0;
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading Mode Register:0x%02X [X] \r\n", (*ptrData));

    return status;
}
/*!
**************************************************************
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
**************************************************************
 */
int8_t BME280_READ_STATUS(void)
{
    debugMsg("====================  BME280 UPDATE STATUS STARTED  ==================\r\n");
    uint8_t ptrData[] = {BME280_REGISTER_STATUS};
    size_t lenWrite = sizeof(BME280_REGISTER_STATUS);
    size_t lenRead = sizeof(BME280_REGISTER_STATUS);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading Status Register:0x%02X [X] \r\n", (*ptrData));
    *ptrData &= ~(BME280_STATUS_MSK); /*!< clear unused or corrupt bits */
    switch (*ptrData)
    {
    case 0x09:
        debugMsg("[X] New Measuring & pre-measured Data-Storage in progress [X] \r\n");
        break;
    case 0x08:
        debugMsg("[X] Measuring in progress [X] \r\n");
        break;
    case 0x01:
        debugMsg("[X] Data-Storage in progress [X] \r\n");
        break;
    case 0x00:
        debugMsg("[X] Measuring-Data available [X] \r\n");
        break;

    default:
        debugVal("[X] Corrupt Register @ 0x%02X @[7:4]-[2:1] [X] \r\n", BME280_REGISTER_STATUS);
        *ptrData &= ~(BME280_STATUS_MSK); /*== clear unused or corrupt bits ==*/
        break;
    }

    return (uint8_t)*ptrData;
}

/*!
**************************************************************
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
**************************************************************
 */
int8_t BME280_READ_COMP(void)
{
    debugMsg("====================  BME280 COMP DATA READ STARTED  =================\r\n");
    /*== Table 16 : Compensation parameter storage, naming and data type ==*/

    ptrComp = &Comp;

    uint8_t buffer[BME280_TEMP_PRESS_CALIB_DATA_LEN + BME280_HUMIDITY_CALIB_DATA_LEN] = {0};
    uint8_t ptrData[] = {BME280_REGISTER_DIG_T1};

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, 1, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, buffer, 25, true);
    sleep_ms(10);

    ptrComp->dig_T1 = buffer[0] | (buffer[1] << 8);             /*!< 0x88 / 0x89 dig_T1 [7:0] / [15:8] uint8_t */
    ptrComp->dig_T2 = (int16_t)(buffer[2] | (buffer[3] << 8));  /*!< 0x8A / 0x8B dig_T2 [7:0] / [15:8] int16_t */
    ptrComp->dig_T3 = (int16_t)(buffer[4] | (buffer[5] << 8));  /*!< 0x8C / 0x8D dig_T3 [7:0] / [15:8] int16_t */
    ptrComp->dig_P1 = (uint16_t)(buffer[6] | (buffer[7] << 8)); /*!< 0x8E / 0x8F dig_P1 [7:0] / [15:8] uint16_t */
    ptrComp->dig_P2 = (int16_t)(buffer[8] | (buffer[9] << 8));  /*!< 0x90 / 0x91 dig_P2 [7:0] / [15:8] int16_t */

    ptrComp->dig_P3 = (int16_t)(buffer[10] | (buffer[11] << 8)); /*!< 0x92 / 0x93 dig_P3 [7:0] / [15:8] int16_t */
    ptrComp->dig_P4 = (int16_t)(buffer[12] | (buffer[13] << 8)); /*!< 0x94 / 0x95 dig_P4 [7:0] / [15:8] int16_t */
    ptrComp->dig_P5 = (int16_t)(buffer[14] | (buffer[15] << 8)); /*!< 0x96 / 0x97 dig_P5 [7:0] / [15:8] int16_t */
    ptrComp->dig_P6 = (int16_t)(buffer[16] | (buffer[17] << 8)); /*!< 0x98 / 0x99 dig_P6 [7:0] / [15:8] int16_t */
    ptrComp->dig_P7 = (int16_t)(buffer[18] | (buffer[19] << 8)); /*!< 0x9A / 0x9B dig_P7 [7:0] / [15:8] int16_t */
    ptrComp->dig_P8 = (int16_t)(buffer[20] | (buffer[21] << 8)); /*!< 0x9C / 0x9D dig_P8 [7:0] / [15:8] int16_t */
    ptrComp->dig_P9 = (int16_t)(buffer[22] | (buffer[23] << 8)); /*!< 0x9E / 0x9F dig_P9 [7:0] / [15:8] int16_t */

    /*!< This Same Register-> BME280_HUMIDITY_CALIB_DATA_LEN-1 */
    ptrComp->dig_H1 = buffer[24]; /*!< 0xA1 dig_H1 [7:0] uint8_t*/

    *ptrData = BME280_REGISTER_DIG_H2;
    uint8_t dataLen = BME280_HUMIDITY_CALIB_DATA_LEN - 1;

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, 1, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, &buffer[25], dataLen, true);
    sleep_ms(10);

    ptrComp->dig_H2 = (int16_t)buffer[25] | (buffer[26] << 8);                      /*!< 0xE1 / 0xE2 dig_H2 [7:0] / [15:8] int16_t */
    ptrComp->dig_H3 = buffer[27];                                                   /*!< 0xE3 dig_H3 [7:0] uint8_t */
    ptrComp->dig_H4 = (int16_t)(buffer[28] << 4 | (((buffer[29]) & ~(0x78))));      /*!< 0xE4 / 0xE5[3:0] dig_H4 [11:4] / [3:0] int16_t */
    ptrComp->dig_H5 = (int16_t)(((buffer[30] & ~(0xF0)) >> 4) | (buffer[31] << 4)); /*!< 0xE5[7:4] / 0xE6 dig_H5 [3:0] / [11:4] int16_t */
    ptrComp->dig_H6 = (int8_t)(buffer[32]);                                         /*!< 0xE7 dig_H6 int8_t */
    memset(buffer, '\0', sizeof(buffer));
    BME280_PRINT_COMP(ptrComp);

    return 0;
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_PRINT_COMP(struct CompData *ptrComp)
{
    debugVal("[X] dig_T1:%d [X] \r\n", ptrComp->dig_T1);
    debugVal("[X] dig_T2:%d [X] \r\n", ptrComp->dig_T2);
    debugVal("[X] dig_T3:%d [X] \r\n", ptrComp->dig_T3);
    debugVal("[X] dig_P1:%d [X] \r\n", ptrComp->dig_P1);
    debugVal("[X] dig_P2:%d [X] \r\n", ptrComp->dig_P2);
    debugVal("[X] dig_P3:%d [X] \r\n", ptrComp->dig_P3);
    debugVal("[X] dig_P4:%d [X] \r\n", ptrComp->dig_P4);
    debugVal("[X] dig_P5:%d [X] \r\n", ptrComp->dig_P5);
    debugVal("[X] dig_P6:%d [X] \r\n", ptrComp->dig_P6);
    debugVal("[X] dig_P7:%d [X] \r\n", ptrComp->dig_P7);
    debugVal("[X] dig_P8:%d [X] \r\n", ptrComp->dig_P8);
    debugVal("[X] dig_P9:%d [X] \r\n", ptrComp->dig_P9);
    debugVal("[X] dig_H1:%d [X] \r\n", ptrComp->dig_H1);
    debugVal("[X] dig_H2:%d [X] \r\n", ptrComp->dig_H2);
    debugVal("[X] dig_H3:%d [X] \r\n", ptrComp->dig_H3);
    debugVal("[X] dig_H4:%d [X] \r\n", ptrComp->dig_H4);
    debugVal("[X] dig_H5:%d [X] \r\n", ptrComp->dig_H5);
    debugVal("[X] dig_H6:%d [X] \r\n", ptrComp->dig_H6);
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_SET_STANDBY(uint8_t tsb)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("====================  BME280 STANDBY STATUS STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    debugVal("[X] Writing STANDBY Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading STANDBY Register:0x%02X [X] \r\n", (*ptrData));

    status = *ptrData & ~BME280_STBY_MSK;
    status |= tsb & BME280_STBY_MSK;
    mode = status;
    debugVal("[X] Setting STANDBY:0x%02X [X] \r\n", status);

    uint8_t ptrWriteMode[] = {BME280_CONFIG_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_READ_STANDBY(void)
{
    debugMsg("====================  BME280 STANDBY STATUS STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    uint8_t ptrRead[1];
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading STANDBY Register:0x%02X [X] \r\n", (*ptrRead));
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_SET_FILTER(uint8_t filter)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("====================  BME280 FILTER SETTING STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    debugVal("[X] Writing FILTER Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading FILTER Register:0x%02X [X] \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_FILTER_MSK;
    status |= filter & BME280_FILTER_MSK;
    mode = status;

    filtCoeff = filter;

    debugVal("[X] Setting Mode:0x%02X [X] \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CONFIG_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_READ_FILTER(void)
{
    debugMsg("====================  BME280 FILTER READING STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CONFIG_ADDR};
    size_t lenWrite = sizeof(BME280_CONFIG_ADDR);
    size_t lenRead = sizeof(BME280_CONFIG_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    sleep_ms(10);
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    sleep_ms(10);
    debugVal("[X] Reading FILTER Register:0x%02X [X] \r\n", (*ptrRead));
}
/*!
**************************************************************
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
**************************************************************
 */
void BBME280_SET_OSRS_T(uint8_t osrs_t)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("====================  BME280 OSRS_t SETTING STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("[X] Writing OSRS_t Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("[X] Reading OSRS_t Register:0x%02X [X] \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_OSRS_T_MSK;
    status |= osrs_t & BME280_OSRS_T_MSK;
    mode = status;
    ovsTime = osrs_t;

    debugVal("[X] Setting OSRS_t:0x%02X [X] \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CTRL_MEAS_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_SET_OSRS_P(uint8_t osrs_p)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;
    debugMsg("====================  BME280 OSRS_p SETTING STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("[X] Writing OSRS_p Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("[X] Reading OSRS_p Register:0x%02X [X] \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_OSRS_P_MSK;
    status |= osrs_p & BME280_OSRS_P_MSK;
    mode = status;
    ovsPressure = osrs_p;

    debugVal("[X] Setting OSRS_p:0x%02X [X] \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CTRL_MEAS_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}

/*!
**************************************************************
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
**************************************************************
 */
void BME280_SET_OSRS_H(uint8_t osrs_h)
{
    uint8_t status = 0x00;
    uint8_t mode = 0x00;

    debugMsg("====================  BME280 OSRS_h SETTING STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CTRL_HUM_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_HUM_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_HUM_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("[X] Writing OSRS_h Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("[X] Reading OSRS_h Register:0x%02X [X] \r\n", (*ptrRead));

    status = *ptrRead & ~BME280_OSRS_H_MSK;
    status |= osrs_h & BME280_OSRS_H_MSK;
    mode = status;
    ovsHumidity = osrs_h;

    debugVal("[X] Setting OSRS_h:0x%02X [X] \r\n", mode);
    uint8_t ptrWriteMode[] = {BME280_CTRL_HUM_ADDR, mode};
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);

    *ptrData = BME280_CTRL_MEAS_ADDR;
    lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    lenRead = sizeof(BME280_CTRL_MEAS_ADDR);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("[X] Writing CTRL_MEAS Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("[X] Reading CTRL_MEAS Register:0x%02X [X] \r\n", (*ptrRead));

    ptrWriteMode[0] = BME280_CTRL_MEAS_ADDR;
    ptrWriteMode[0] = *ptrRead;
    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrWriteMode, 2, false);
}
/*!
**************************************************************
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
**************************************************************
 */
void BME280_READ_OSRS_H(void)
{
    debugMsg("====================  BME280 OSRS_h READING STARTED  =================\r\n");
    uint8_t ptrData[] = {BME280_CTRL_HUM_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_HUM_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_HUM_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("[X] Writing OSRS_h Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("[X] Reading OSRS_h Register:0x%02X [X] \r\n", (*ptrRead));
}

/*!
**************************************************************
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
**************************************************************
 */
void BME280_READ_CTRL_MEAS(void)
{
    debugMsg("====================  BME280 CTRL_MEAS READING STARTED  ==============\r\n");
    uint8_t ptrData[] = {BME280_CTRL_MEAS_ADDR};
    size_t lenWrite = sizeof(BME280_CTRL_MEAS_ADDR);
    size_t lenRead = sizeof(BME280_CTRL_MEAS_ADDR);
    uint8_t ptrRead[lenRead];

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);
    debugVal("[X] Writing CTRL_MEAS Register:0x%02X [X] \r\n", (*ptrData));
    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrRead, lenRead, false);
    debugVal("[X] Reading CTRL_MEAS Register:0x%02X [X] \r\n", (*ptrRead));
}
/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */

void BME280_RAW_DATA(void)
{
    uint8_t buffer[8];

    debugMsg("====================  BME280 RAW DATA READING STARTED  ===============\r\n");
    uint8_t ptrData[] = {BME280_DATA_ADDR};
    size_t lenWrite = sizeof(BME280_DATA_ADDR);
    size_t lenRead = sizeof(buffer);

    i2c_write_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, ptrData, lenWrite, false);

    i2c_read_blocking(i2c_default, BME280_I2C_ADDR_PRIMARY, buffer, lenRead, true);

    press = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | (buffer[2] >> 4);
    temp = ((uint32_t)buffer[3] << 12) | ((uint32_t)buffer[4] << 4) | (buffer[5] >> 4);
    hum = (uint32_t)buffer[6] << 8 | buffer[7];

    debugVal("[X] Temperature:%X \r\n", temp);
    debugVal("[X] Pressure:%X  \r\n", press);
    debugVal("[X] Humidity:%X  \r\n", hum);
}

/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
int32_t BME280_COMP_TEMP(void)
{
    int32_t adc_T = temp;
    int32_t var1;
    int32_t var2;
    int32_t temperature;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    var1 = (((adc_T >> 3) - ((int32_t)Comp.dig_T1 << 1)) * ((int32_t)Comp.dig_T2)) >> 11;
    var2 = ((adc_T >> 4) - ((int32_t)Comp.dig_T1)); 
    var2 = (var2 *((adc_T >> 4) - ((int32_t)Comp.dig_T1)) >> 12);
    var2 = (var2 * ((int32_t)Comp.dig_T3)) >> 14;

    t_fine = var1 + var2;
    temperature = (t_fine * 5 + 128) >> 8;

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
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
uint32_t BME280_COMP_PRESSURE(void)
{
    int32_t adc_P = press;
    int32_t var1;
    int32_t var2;
    int32_t pressure;

    var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)Comp.dig_P6);
    var2 = var2 + ((var1 * ((int32_t)Comp.dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)Comp.dig_P4) << 16);
    var1 = ((Comp.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3);
    var1 = (var1 + ((((int32_t)Comp.dig_P2) * var1) >> 1)) >> 18;

    var1 = ((((32768 + var1)) * ((int32_t)Comp.dig_P1)) >> 15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    pressure = (((uint32_t)(((int32_t)1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (pressure < 0x80000000)
    {
        pressure = (pressure << 1) / ((uint32_t)var1);
    }
    else
    {
        pressure = (pressure / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)Comp.dig_P9) * ((int32_t)(((pressure >> 3) * (pressure >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(pressure >> 2)) * ((int32_t)Comp.dig_P8)) >> 13;
    pressure = (uint32_t)((int32_t)pressure + ((var1 + var2 + Comp.dig_P7) >> 4));
    return pressure;
}
/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
double BME280_COMP_HUM_DOUBLE(void)
{
    int32_t adc_H = hum;
    double var_H;
    var_H = (((double)t_fine) - 76800.0);
    var_H = (adc_H - (((double)Comp.dig_H4) * 64.0 + ((double)Comp.dig_H5) / 16384.0 * var_H));
    var_H = var_H * (((double)Comp.dig_H2) / 65536.0);
    var_H = var_H * (1.0 + ((double)Comp.dig_H6) / 67108864.0 * var_H * (1.0 + ((double)Comp.dig_H3) / 67108864.0 * var_H));
    var_H = var_H * (1.0 - ((double)Comp.dig_H1) * var_H / 524288.0);

    if (var_H > 100.0)
    {
        var_H = 100.0;
    }
    else if (var_H < 0.0)
    {
        var_H = 0.0;
    }

    return var_H;
}

uint32_t BME280_COMP_HUM_INT32(void)
{
    int32_t adc_H = hum;
    int32_t var_H;
 
    int32_t var_H3 = ((int32_t)Comp.dig_H3); 
    int32_t var_H4 = ((int32_t)Comp.dig_H4) << 20;
    int32_t var_H5 = ((int32_t)Comp.dig_H5);
    int32_t var_H6 = ((int32_t)Comp.dig_H6);

    var_H = (t_fine - ((int32_t)76800));
    var_H = (((((adc_H << 14) - var_H4 - (var_H5 * var_H)) + ((int32_t)16384)) >> 15) * \
            (((((((var_H * var_H6) >> 10) * (((var_H * var_H3) >> 11) + ((int32_t)32768))) >> 10) + \
            ((int32_t)2097152)) * ((int32_t)Comp.dig_H2) + 8192) >> 14));
    var_H = (var_H - (((((var_H >> 15) * (var_H >> 15)) >> 7) * ((int32_t)Comp.dig_H1)) >> 4));
    var_H = (var_H < 0 ? 0 : var_H);
    var_H = (var_H > 419430400 ? 419430400 : var_H);

    return (uint32_t)(var_H >> 12);
}

/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
void BME280_MEASUREMENT_TIME(void)
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
        odrMs = 1000 / (timeMax);
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
    debugMsg("====================  BME280 RESPONSE TIMING STARTED =================\r\n");
    debug2Val("[X] MeasurementRate: %f Hz\n[X] IIR-ResponseTime: %.2f ms\n", odrMs, rspTimeIIR);
    debug2Val("[X] Typ. MeasurementTime: %f ms\n[X] Max. MeasurementTime: %.2f ms\n", timeTyp, timeMax);
}
/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
void BME280_DATA_READ(int32_t temperature, uint32_t pressure, uint32_t humidity)
{
    debugMsg("====================  BME280 SENSOR DATA READING STARTED =============\r\n");
    temperature = BME280_COMP_TEMP();
    debugVal("[X] Temperature: %.2f °C \r\n", temperature / 100.0f);
    pressure = BME280_COMP_PRESSURE();
    debugVal("[X] Pressure: %.2f °hPa \r\n", pressure / 100.0f);
    /* 
    double humidity = BME280_COMP_HUM_DOUBLE();
    debugVal("[X] Humidity: %lf \r\n", humidity); 
    */
    humidity = BME280_COMP_HUM_INT32();
    debugVal("[X] Humidity: %.2f %% \r\n", humidity / 1024.0f);
}
/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
void BME280_INIT(void)
{
    /*=========================================================*/
    /*== BME280 SETTINGS ======================================*/
    /*=========================================================*/
    BME280_CHIPID();
    BME280_READ_COMP();
    BME280_SOFT_RESET();
    BME280_SET_STANDBY(BME280_STBY_0_5);
    BME280_SET_OSRS_H(BME280_OSRS_H_x1);
    BME280_SET_FILTER(BME280_FILTER_16);
    BBME280_SET_OSRS_T(BME280_OSRS_T_x2);
    BME280_SET_OSRS_P(BME280_OSRS_P_x16);
    BME280_SET_MODE(BME280_NORMAL_MODE);
}
/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
void BME280_READ_REGVALUE(void)
{
    BME280_READ_CTRL_MEAS();
    BME280_READ_MODE();
    BME280_READ_STANDBY();
    BME280_READ_OSRS_H();
}
/*!
**************************************************************
 * @brief 
 * 
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
**************************************************************
 */
void BME280_TEMP_READ(int32_t bmeTemp, uint32_t bmePress, uint32_t bmeHum)
{
    BME280_RAW_DATA();
    BME280_DATA_READ(bmeTemp, bmePress, bmeHum);
    BME280_MEASUREMENT_TIME();
}
