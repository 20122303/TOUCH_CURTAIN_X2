/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * Description: Include file for application
 *
 * Author: TrungTQ
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 2.1 $
 * Last Changed:     $Date: 2016-06-21 15:25:00 (Tue, 21 Jun 2016) $
 *
 ******************************************************************************/
#ifndef _SERIAL_MAN_H_
#define _SERIAL_MAN_H_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
#include <ev_man.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define DEFINE_EVENT_BUTTON_NBR             0x30

typedef enum _EVENT_BUTTON_ {
    EVENT_BUTTON_RELEASE = DEFINE_EVENT_BUTTON_NBR,
    EVENT_BUTTON_HOLD,
    EVENT_BUTTON_RESET,
    EVENT_BUTTON_ADDNODE,
    EVENT_BUTTON_REPORT_RESET,
    EVENT_BUTTON_REPORT_TOUCH_HOLD,
    EVENT_BUTTON_REPORT_ADDNODE,
    EVENT_BUTTON_REPORT_HAS_MAGIC,
    EVENT_BUTTON_REPORT_NON_MAGIC,
    EVENT_BUTTON_MAX
} EVENT_BUTTON;

typedef void (CODE *report_func_callback)(void);

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
 * @func   InitSerialMan   
 * @brief  Init BAUDRATE, ...
 * @param  None
 * @retval None
 */
void
InitSerialMan();

/**
 * @func   SerialSetValue
 * @brief  Send Serial Set
 * @param  None
 * @retval None
 */
void
SerialSetValue(
    BYTE byValue,       /* IN: value */
    BYTE byEndpoint     /* IN: endpoint */
);

/**
 * @func   Serial_GetValueEP   
 * @brief  Send Serial Get
 * @param  None
 * @retval None
 */
BYTE
SerialGetValue(
    BYTE byEndpoint
);

/**
 * @func   SerialReport
 * @brief  None
 * @param  None
 * @retval None
 */
void
SerialReport(
    EVENT_BUTTON eventButton
);

#endif /* _SERIAL_MAN_H_ */
