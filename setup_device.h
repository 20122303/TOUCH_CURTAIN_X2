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
 * Revision:         $Revision: 2.2 $
 * Last Changed:     $Date: 2016-06-21 15:45:00 (Tue, 21 Jun 2016) $
 *
 ******************************************************************************/
#ifndef _SETUP_DEVICE_H_
#define _SETUP_DEVICE_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "config_app.h"
#include <ZW_TransportEndpoint.h>
#include <agi.h>

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
#if NUMBER_OF_ENDPOINTS > 1
extern EP_FUNCTIONALITY_DATA g_EndPointFunctionality;
extern EP_NIF g_EndpointNIF[NUMBER_OF_ENDPOINTS];
#endif /* NUMBER_OF_ENDPOINTS > 1 */

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
);

/**
 * @func   SetAllEnpointsNIF   
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetAllEnpointsNIF();

#endif /* _SETUP_DEVICE_H_ */
