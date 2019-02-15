/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQ
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 2.1 $
 * Last Changed:     $Date: 2016-07-09 16:20:00 (Sat, 09 Jun 2016) $
 *                   1. Del: [pByTypeDevice]
 *                   2. Add: #include "setup_device.h"
 *                   $Date: 2016-07-09 16:20:00 (Sat, 09 Jun 2016) $
 *                   1. Change: [pByDefaultDevice] to [pByTypeDevice]
 *                   $Date: 2016-06-23 14:10:00 (Wen, 23 Jun 2016) $                   
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_uart_api.h>
#include "config_app.h"
#include "lumi_serial_api.h"
#include "convert_data.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   CorrectValue   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
CorrectValue(
    BYTE byValue,
    BYTE byTpDev
) {
    BYTE byRetValue = 0;

    switch (byTpDev) {
    case DEVICE_CONTACT:
        if (0 == byValue) {
            byRetValue = 0;
        } else if ((byValue < 0x64) || (byValue == 0xFF)) {
            byRetValue = 0xFF;
        }
        break;

    case DEVICE_DIMMER:
    case DEVICE_CURTAIN:
        if ((byValue > 0x63) && (byValue != 0xFF)) {
            byRetValue = 0x63;
        } else {
            byRetValue = byValue;
        }
        break;
    }
    return byRetValue;
}

/**
 * @func   ConvertValueZwToMcu      
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
ConvertValueZwToMcu(
    BYTE byValue,
    BYTE byTpDev
) {
    BYTE byRetValue = 0;
    switch (byTpDev) {
    case DEVICE_CONTACT:
        if (0 == byValue) {
            byRetValue = 0;
        } else if ((byValue < 0x64) || (byValue == 0xFF)) {
            byRetValue = 1;
        }
        break;
        
    case DEVICE_DIMMER:
    case DEVICE_CURTAIN:
        if (byValue >= 0x63) {
            byRetValue = 0xFF;
        } else {
            byRetValue = (byValue << 1) + (byValue >> 1) + (byValue >> 4);
        }
        break;
    }
    return byRetValue;
}

/**
 * @func   ConvertValueMcuToZw   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
ConvertValueMcuToZw(
    BYTE byValue,
    BYTE byTpDev
) {
    BYTE byRetValue = 0;
    switch (byTpDev) {
    case DEVICE_CONTACT:
        if (0 == byValue) {
            byRetValue = 0x00;
        } else if (1 == byValue) {
            byRetValue = 0xFF;
        }
        break;
        
    case DEVICE_DIMMER:
    case DEVICE_CURTAIN:
        if (byValue != 0) {
            byRetValue = (byValue >> 2) + (byValue >> 3) + (byValue >> 6) + 2;
        } else {
            byRetValue = 0;
        }

        if (byRetValue > 0x63) {
            byRetValue = 0x63;
        }
        break;
    }
    
    return byRetValue;
}


/**
 * @func   GetLengthCmd    
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
GetLengthCmd(
    BYTE byTypeDev
) {
    BYTE byLength = 0;
    switch (byTypeDev) {
    case DEVICE_CONTACT:
    case DEVICE_SOCKET:
    case DEVICE_DIMMER:
    case DEVICE_FAN:
        byLength = 3;
        break;

    case DEVICE_CURTAIN:
        byLength = 4;
        break;
    }
    return byLength;
}

