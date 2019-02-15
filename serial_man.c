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
 * Last Changed By:  Author: TrungTQ
 * Revision:         Revision: 2.3
 * Last Changed:     Date: 2016-06-21 15:25:00 (Tue, 21 Jun 2016)
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "config_app.h"
#include <ZW_basis_api.h>
#include <ZW_timer_api.h>
#include <ZW_uart_api.h>
#include <ZW_conbufio.h>
#include <ZW_SerialAPI.h>

#include "macro.h"
#include "lumi_serial_api.h"
#include "convert_data.h"
#include "serial.h"
#include "serial_man.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define NUMBER_BLINK                        4
#define TX_BUFFER_SIZE                      64
#define STATUS_DEFAULT                      STATUS_REFRESH_LED

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE g_bTxBuffer[TX_BUFFER_SIZE] = { 0 };

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
/**
 * @func   InitSerialMan   
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitSerialMan(
) {
    InitSerial(ZW_INF_BAURATE);
}

/**
 * @func   SerialGetValue   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
SerialGetValue(
    BYTE byEndpoint
) {
    BYTE byRetValue = 0;
    WORD wEndpointmask = 0;

    if (byEndpoint > 0) {
        wEndpointmask = BV(byEndpoint - 1);
    } else if (byEndpoint == 0) {
        #if NUMBER_OF_ENDPOINTS > 1
        wEndpointmask = BV(NUMBER_OF_ENDPOINTS) - 1;
        #else /* NUMBER_OF_ENDPOINTS > 1 */ 
        wEndpointmask = 1;
        #endif /* NUMBER_OF_ENDPOINTS > 1 */
    }
    
    g_bTxBuffer[0] = HI_UINT16(wEndpointmask);
    g_bTxBuffer[1] = LO_UINT16(wEndpointmask);
    SendTxData(CMD_GET_STATE_ENDPOINT, g_bTxBuffer, LENGTH_CMD_GET_STATE_ENDPOINT);

    return byRetValue;
}

/**
 * @func   SerialReport
 * @brief  None
 * @param  None
 * @retval None
 */
void
SerialReport(
    EVENT_BUTTON eventButton
) {
    g_bTxBuffer[0] = 0xFF;
    g_bTxBuffer[1] = 0xFF;

    if (EVENT_BUTTON_REPORT_ADDNODE == eventButton) {
        g_bTxBuffer[2] = BLUE_BLINK;
    } else if (EVENT_BUTTON_REPORT_RESET == eventButton) {
        g_bTxBuffer[2] = RED_BLINK;
    } else if (EVENT_BUTTON_REPORT_HAS_MAGIC == eventButton) {
        g_bTxBuffer[2] = BLUE_BLINK;
    } else if (EVENT_BUTTON_REPORT_NON_MAGIC == eventButton) {
        g_bTxBuffer[2] = RED_BLINK;
    }

    g_bTxBuffer[3] = NUMBER_BLINK;
    g_bTxBuffer[4] = STATUS_DEFAULT;

    SendTxData(CMD_SET_LED_STATUS, g_bTxBuffer, LENGTH_CMD_SET_LED_STATUS);
}
