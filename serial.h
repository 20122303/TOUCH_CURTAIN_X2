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
 * Last Changed:     $Date: 2016-02-23 02:16:21 (Tue, 23 Feb 2016) $
 *
 ******************************************************************************/
#ifndef _SERIAL_H_
#define _SERIAL_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define RX_BUFFER_SIZE                      64

#define FRAME_LENGTH_MIN                    2
#define FRAME_LENGTH_MAX                    RX_BUFFER_SIZE

#define RX_TIMEOUT                          10

#define ZW_INF_BAURATE                      192

#define lengthRxBuf                         (*byRxBuffer)
#define cmdRxBuf                            (*(byRxBuffer + 1))
#define dataRxBuf                           (byRxBuffer + 2)

typedef struct _TX_FRAME_ {
    BYTE length;
    BYTE command;
    BYTE_P buff;
} TX_FRAME;

typedef enum _UART_STATE_ {
    stateIdle,
    stateFrameReceived,
    stateFrameSent,
    stateFrameErr,
    stateRxTimeout,
    stateTxTimeout
} UART_STATE;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern BYTE byRxBuffer[RX_BUFFER_SIZE];

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   InitSerial   
 * @brief  None
 * @param  WORD wBaudRate
 * @retval None
 */
void
InitSerial(
    WORD wBaudRate
);

/**
 * @func   RxBufPoll   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
RxBufPoll(void);

/**
 * @func   SendTxFrame   
 * @brief  None
 * @param  TX_FRAME frame
 * @retval None
 */
void
SendTxFrame(
    TX_FRAME frame
);

/**
 * @func   SendTxData   
 * @brief  None
 * @param  BYTE     byLength
 *         BYTE     byCommand
 *         BYTE_P   pByBuffer
 * @retval None
 */
void
SendTxData(
    BYTE byCommand,
    BYTE_P pByBuffer,
    BYTE byLength
);

#endif /* _SERIAL_H_ */
