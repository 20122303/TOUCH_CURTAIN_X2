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
 * Revision:         Revision: 2.2
 * Last Changed:     Date: 2016-06-21 15:45:00 (Tue, 21 Jun 2016)
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "config_app.h"
#include <ZW_classcmd.h>
#include <ZW_mem_api.h>
#include <ZW_string.h>
#include "lumi_serial_api.h"
#include "setup_device.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#if NUMBER_OF_ENDPOINTS > 1
EP_NIF g_EndpointNIF[NUMBER_OF_ENDPOINTS];

EP_FUNCTIONALITY_DATA 
g_EndPointFunctionality = {
    NUMBER_OF_INDIVIDUAL_ENDPOINTS,       /**< nbrIndividualEndpoints 7 bit*/
    RES_ZERO,                             /**< resIndZeorBit 1 bit*/
    NUMBER_OF_AGGREGATED_ENDPOINTS,       /**< nbrAggregatedEndpoints 7 bit*/
    RES_ZERO,                             /**< resAggZeorBit 1 bit*/
    RES_ZERO,                             /**< resZero 6 bit*/
    ENDPOINT_IDENTICAL_DEVICE_CLASS_YES,  /**< identical 1 bit*/
    ENDPOINT_DYNAMIC_NO                   /**< dynamic 1 bit*/
};
#endif /* NUMBER_OF_ENDPOINTS > 1 */

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static code BYTE g_EPSwitchNIF[] = {
    COMMAND_CLASS_ZWAVEPLUS_INFO,
    COMMAND_CLASS_SWITCH_MULTILEVEL,
    COMMAND_CLASS_ASSOCIATION,
    COMMAND_CLASS_ASSOCIATION_GRP_INFO
};

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
 * @func   SetEndpointNIF   
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetEndpointNIF(
    BYTE byEndpoint
) {
    #if NUMBER_OF_ENDPOINTS > 1
    g_EndpointNIF[byEndpoint - 1].genericDeviceClass  =  GENERIC_TYPE_WINDOW_COVERING;
    g_EndpointNIF[byEndpoint - 1].specificDeviceClass =  SPECIFIC_TYPE_SIMPLE_WINDOW_COVERING;
    g_EndpointNIF[byEndpoint - 1].sizeNIF = sizeof(g_EPSwitchNIF);
    g_EndpointNIF[byEndpoint - 1].pNIF = g_EPSwitchNIF;
    #else /* NUMBER_OF_ENDPOINTS > 1 */
    UNUSED(byEndpoint);
    #endif /* NUMBER_OF_ENDPOINTS > 1 */
}

/**
 * @func   SetAllEnpointsNIF   
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetAllEnpointsNIF() {
    #if NUMBER_OF_ENDPOINTS > 1
    BYTE i = 0;
    for (i = 0; i < NUMBER_OF_ENDPOINTS; i++) {
        SetEndpointNIF(i + 1);
    }
    #endif /* NUMBER_OF_ENDPOINTS > 1 */
}
