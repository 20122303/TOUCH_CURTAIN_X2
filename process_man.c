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
 * Last Changed By:  QuyetDH
 * Revision:         Revision: 2.2
 * Last Changed:     Date: 2016-07-11 09:40:00 (Mon, 11 Jul 2016)
 *                   1. Change: [pByTypeDevice] to [g_pByTypeDevice]
 *                   2. Add: #include "setup_device.h"
 *                   Date: 2016-06-22 08:35:00 (Wen, 22 Jun 2016)
 *					 ......
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "config_app.h"
#include <ZW_uart_api.h>
#include <Zw_timer_api.h>
#include <misc.h>
#include <ZW_tx_mutex.h>

#include "lumi_serial_api.h"
#include "convert_data.h"
#include "serial.h"
#include "serial_man.h"
#include "process_man.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef PROCESS_DEBUG
#define PROCESS(x)                  (x)
#else /* PROCESS_DEBUG */
#define PROCESS(x)
#endif /* PROCESS_DEBUG */

#define TIMEOUT_AFTER_HOLD10SEC     5 // 5 second
#define TIMER_REPORT_ENDPOINT       2 // 20 ms

typedef struct _PROCESS_FUNC_ {
    APP_STATE_FUNC appfunc;
    TIMEOUT_FUNC   timeout;
} PROCESS_FUNC_T, *PROCESS_FUNC_P;

typedef enum _STATE_SERIAL_MAN_ {
    stateFree,
    stateFrameParse,
    stateTxSerial
} STATE_SERIAL_MAN;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static PROCESS_FUNC_T g_procFunc;
static STATE_SERIAL_MAN serialManState = stateFree;
static BYTE g_byCountHold = 0;
static BYTE g_bCurrState, g_bLastState;
static BYTE g_bTimeoutTimerHandle = 0xFF;
static BYTE g_bReportTimerHandle = 0xFF;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
#if NUMBER_OF_ENDPOINTS > 1
VALUE_DEVICE_T g_value[NUMBER_OF_ENDPOINTS] = { 0 };
#else /* NUMBER_OF_ENDPOINTS > 1 */
VALUE_DEVICE_T g_value[1] = { 0 };
#endif /* NUMBER_OF_ENDPOINTS > 1 */

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void ProcButton();
static void ProcDevice();
static void ProcEndpoint();
static void PrepareReportState();
static void StopTimerForReport();
static void StartTimerForReport();
static void CheckAfferHold();

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
    TIMEOUT_FUNC timeout_func
) {
    g_procFunc.appfunc = app_func;
    g_procFunc.timeout = timeout_func;    
}

/**
 * @func   ProcessManScan   
 * @brief  None
 * @param  None
 * @retval None
 */
void
ProcessManScan(
) {
    switch(serialManState) {
    case stateFree:
        if (stateFrameReceived == RxBufPoll()) {
            serialManState = stateFrameParse;
        }
        break;
        
    case stateFrameParse:
        switch(cmdRxBuf) {
        case CMD_REPORT_INFO_DEVICE:
            ProcDevice();
            break;
            
        case CMD_REPORT_STATE_ENDPOINT:
            ProcEndpoint();
            break;
            
        case CMD_REPORT_STATE_BUTTON:
            ProcButton();
            break;
        }
        serialManState = stateFree;
        break;
        
    case stateTxSerial:
        break;
        
    default:
        serialManState = stateFree;
        break;
    }
}

/**
 * @func   ProcButton     
 * @brief  None
 * @param  None
 * @retval None
 */
void 
ProcButton(
) {
    BYTE byButtonId = dataRxBuf[0];
    BYTE byButtonState = dataRxBuf[1];

    g_bCurrState = byButtonState;
	
    switch (byButtonState) {
    case BUTTON_RELEASE:
        if (g_procFunc.appfunc != NULL) {
            if (BUTTON_HOLD_15SEC == g_bLastState) {
                g_procFunc.appfunc(EVENT_BUTTON_ADDNODE);
            } 
        }
        break;

    case BUTTON_HOLD_3SEC:
        break;
        
    case BUTTON_HOLD_5SEC:
        break;
        
    case BUTTON_HOLD_10SEC:
        break;

	case BUTTON_HOLD_15SEC:
        if (g_procFunc.appfunc != NULL) {
            if (g_bTimeoutTimerHandle == 0xFF) {
                g_bTimeoutTimerHandle = ZW_TIMER_START(
                                    CheckAfferHold,
                                    TIMER_ONE_SECOND,
                                    TIMER_FOREVER);
            }
			
            g_procFunc.appfunc(EVENT_BUTTON_REPORT_ADDNODE);
        }
        break;		

	case BUTTON_PRESSC_2TIME:
		break;

	case BUTTON_PRESSC_3TIME:
        break;
		
	case BUTTON_PRESSC_10TIME:
		break;
		
	case BUTTON_PRESSC_5TIME:
		if (byButtonId == 0) 
		{
			g_procFunc.appfunc(EVENT_BUTTON_RESET);
		}
		break;
		
	}
		
	g_bLastState = g_bCurrState;
}

/**
 * @func   ProcEndpoint   
 * @brief  None
 * @param  None
 * @retval None
 */
void 
ProcEndpoint(
) {
    BYTE byEndNo = 0;
    BYTE byIndex = 0;
    BYTE byTpDev = 0;
    BYTE byLength = 0;

    do {
        byEndNo = dataRxBuf[byIndex];
        byTpDev = dataRxBuf[byIndex + 1];
        
        #if NUMBER_OF_ENDPOINTS > 1
        if (byEndNo <= NUMBER_OF_ENDPOINTS) {
        #else /* NUMBER_OF_ENDPOINTS > 1 */
        {
        #endif /* NUMBER_OF_ENDPOINTS > 1 */
            byLength = GetLengthCmd(byTpDev);
            if (!g_value[byEndNo - 1].active) {
                g_value[byEndNo - 1].active = TRUE;
                g_value[byEndNo - 1].length = byLength;
                ZW_memcpy((BYTE*)&g_value[byEndNo - 1].cmd, 
                          &dataRxBuf[byIndex], byLength);
            }
        }
        byIndex += byLength;
    } while (byIndex < lengthRxBuf - 2);

    StopTimerForReport();
    StartTimerForReport();
}

/**
 * @func   ProcDevice  
 * @brief  None
 * @param  None
 * @retval None
 */
void 
ProcDevice(
) {
    // Do something
}

/**
 * @func   CheckAfferHold10Secs   
 * @brief  Check Holding Touch.
 * @param  None
 * @retval None
 */
PCB(CheckAfferHold) (void) {
    g_byCountHold++;
    if (g_byCountHold >= TIMEOUT_AFTER_HOLD10SEC) {
        if (g_bTimeoutTimerHandle != 0xFF) {
            ZW_TIMER_CANCEL(g_bTimeoutTimerHandle);
            g_bTimeoutTimerHandle = 0xFF;
        }

        if (g_bCurrState == BUTTON_HOLD_15SEC) {
            g_bCurrState = BUTTON_RELEASE;
            g_bLastState = BUTTON_RELEASE;
        }
        g_byCountHold = 0;
    }
}

/**
 * @func   StartTimerForReport   
 * @brief  Start Timer for reporting
 * @param  None
 * @retval None
 */
void 
StartTimerForReport(
) {
    if (g_bReportTimerHandle == 0xFF) {
        g_bReportTimerHandle = 
        ZW_TIMER_START(PrepareReportState, 
                       TIMER_REPORT_ENDPOINT, 
                       TIMER_FOREVER);
    }
}

/**
 * @func   StopTimerForReport   
 * @brief  Start Timer for reporting
 * @param  None
 * @retval None
 */
void 
StopTimerForReport(
) {
    if (g_bReportTimerHandle != 0xFF) {
        ZW_TIMER_CANCEL(g_bReportTimerHandle);
        g_bReportTimerHandle = 0xFF;
    }
}

/**
 * @func   PrepareReportState   
 * @brief  Report State of Endpoint
 * @param  None
 * @retval None
 */
PCB(PrepareReportState) (void) {
    if ((g_procFunc.appfunc != NULL) && (FALSE == ActiveJobs())) {
		StopTimerForReport();
        g_procFunc.appfunc(EVENT_REPORT_STATE_ENDPOINT);
    }
}


