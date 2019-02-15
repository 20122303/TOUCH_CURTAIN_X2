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
 * Last Changed By:  Author: TrungTQ (trungkstn@gmail.com)
 * Revision:         Revision: 2.2
 * Last Changed:     Date: 2016-07-10 14:40:00 (Sun, 10 Jul 2016)
 *                   Change [Report_BinarySwitch] to [Report_RootState]
 *                   Date: 2016-06-21 15:45:00 (Tue, 21 Jun 2016)
 *
 ******************************************************************************/

 /******************************************************************************/
 /*                              INCLUDE FILES                                 */
 /******************************************************************************/
#include "config_app.h"
#include <misc.h>
#include <association_plus.h>
#include <ZW_timer_api.h>
#include <ZW_mem_api.h>
#include <ZW_uart_api.h>

#include "lumi_cmd.h"
#include "process_man.h"
#include "serial_man.h"
#include "report_man.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef REPORT_DEBUG
#define REPORT(x)                       (x)
#else /* REPORT_DEBUG */
#define REPORT(x)
#endif /* REPORT_DEBUG */

typedef void (CODE *CALLBACK_FUNC)(CMD_BUFFER*, BYTE);

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static BYTE g_bInitStateTimerHandle = 0xFF;
static CALLBACK_FUNC g_callbackFunc = NULL;
static BYTE g_bCountInitState = 0;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
#define TIMER_GET_STATE_EP              1

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void GetInitState();

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   InitReportManager
 * @brief  None
 * @param  None
 * @retval None
 */
void
InitReportManager(
    VOID_CALLBACKFUNC(callback_func)(CMD_BUFFER*, BYTE)
) {
    g_callbackFunc = callback_func;
}

/**
 * @func   StartGetStateEP
 * @brief  None
 * @param  None
 * @retval None
 */
void
StartGetStateEP(
) {
    if (g_bInitStateTimerHandle == 0xFF) {
        g_bInitStateTimerHandle =
        ZW_TIMER_START(GetInitState, TIMER_ONE_SECOND, TIMER_FOREVER);
    }
}

/**
 * @func   GetInitState
 * @brief  None
 * @param  None
 * @retval None
 */
PCB(GetInitState)(void) {
    g_bCountInitState++;
    if (g_bCountInitState >= TIMER_GET_STATE_EP) { // 3 seconds
        g_bCountInitState = 0;
        if (g_bInitStateTimerHandle != 0xFF) {
            ZW_TIMER_CANCEL(g_bInitStateTimerHandle);
            g_bInitStateTimerHandle = 0xFF;
        }
        SerialGetValue(0);
    }
}

/**
 * @func   ReportState
 * @brief  None
 * @param  None
 * @retval None
 */
BOOL 
ReportState(void) {
    BYTE i = 0;
    #if NUMBER_OF_ENDPOINTS > 1
    for (i = 0; i < NUMBER_OF_ENDPOINTS; i++)
    #endif /* NUMBER_OF_ENDPOINTS > 1 */
    {
        if (g_value[i].active) {
            if (g_callbackFunc != NULL) { 
                g_callbackFunc((CMD_BUFFER_P)&g_value[i].cmd, g_value[i].length);
            }
            g_value[i].active = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}
