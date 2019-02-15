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
 * Last Changed:     $Date: 2016-06-22 08:35:00 (Wen, 22 Jun 2016) $
 *
 ******************************************************************************/
#ifndef _PROCESS_MAN_H_
#define _PROCESS_MAN_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
#include "lumi_serial_api.h"
#include "lumi_cmd.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define SIZE_BUFF_IN_PACKAGE        5
#define ENDPOINT_INDEX              0
#define CMDCLASS_INDEX              1
#define COMMAND_INDEX               2
#define PAYLOAD_INDEX               3

typedef void (CODE *APP_STATE_FUNC)(BYTE);
typedef void (CODE *TIMEOUT_FUNC)(BYTE);

typedef struct _VALUE_DEVICE_ {
    BYTE        active;
    BYTE        length; 
    CMD_BUFFER  cmd;
} VALUE_DEVICE_T, *VALUE_DEVICE_P;

#define DEFINE_EVENT_REPORT                 0x40

typedef enum _EVENT_REPORT_ {
    EVENT_REPORT_STATE_ENDPOINT = DEFINE_EVENT_REPORT,
    EVENT_REPORT_INFO_DEVICE,
    EVENT_REPORT_DONE
} EVENT_REPORT;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
#if NUMBER_OF_ENDPOINTS > 1
extern VALUE_DEVICE_T g_value[NUMBER_OF_ENDPOINTS];
#else /* NUMBER_OF_ENDPOINTS > 1 */
extern VALUE_DEVICE_T g_value[1];
#endif /* NUMBER_OF_ENDPOINTS > 1 */

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   InitProcessManager   
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitProcessManager(
    APP_STATE_FUNC app_func,
    TIMEOUT_FUNC   timeout_func
);

/**
 * @func   ProcessManScan   
 * @brief  None
 * @param  None
 * @retval None
 */
void
ProcessManScan();

#endif /* _PROCESS_MAN_H_ */
