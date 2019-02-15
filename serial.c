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
 * Last Changed:     $Date: 2016-02-25 15:40:00 (5 Jeb 2016) $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_basis_api.h>
#include <ZW_uart_api.h>
#include <ZW_conbufio.h>
#include <ZW_SerialAPI.h>
#include "lumi_serial_api.h"
#include "serial.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define SOF_HI                              0x4C                 
#define SOF_LO                              0x4D

typedef enum _RX_STATE_ {
    stateSOF1,
    stateSOF2,
    stateLen,
    stateCmd,
    stateData,
    stateCheckSum
} RX_STATE;

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
static BYTE byRxBufState;
static BYTE byIndexRxBuf;
static BYTE byCheckSumRxBuf;
static WORD wTimeoutRx;
static BOOL bRxActive = FALSE;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
BYTE byRxBuffer[RX_BUFFER_SIZE] = { 0 };

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void TimerReset(WORD_P pwTimerStartTick);
static WORD TimerGetElapsedTime(WORD wTimerStartTick);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   TimerReset   
 * @brief  Reset startTick to current timer tick
 * @param  WORD_P pWTimerStartTick
 * @retval None
 */
void
TimerReset(
    WORD_P pWTimerStartTick
) {
    *pWTimerStartTick = getTickTime();
}

/**
 * @func   TimerGetElapsedTime   
 * @brief  Get elapsed time  
 * @param  WORD wTimerStartTick
 * @retval WORD Elapsed Time in 10 ms ticks
 */

WORD
TimerGetElapsedTime(
    WORD wTimerStartTick
) {
    return (getTickTime() - wTimerStartTick);
}

/**
 * @func   InitSerial   
 * @brief  Initialize UART
 * @param  WORD wBaudRate
 * @retval None
 */
void
InitSerial(
    WORD wBaudRate
) {
    ZW_InitSerialIf(wBaudRate);
    ZW_FinishSerialIf();

    byRxBufState = stateSOF1;
}

/**
 * @func   RxBufPoll   
 * @brief  None  
 * @param  None  
 * @retval None
 */
BYTE
RxBufPoll(
) {
    BYTE byRetVal = stateIdle;
    BYTE byRxData;
    
    if (0 != ZW_SerialCheck()) {
        do {
            byRxData = ZW_SerialGetByte();
            switch (byRxBufState) {
            case stateSOF1:
                if (SOF_HI == byRxData) {
                    byRxBufState++;
                    bRxActive = TRUE;
                    TimerReset(&wTimeoutRx);
                }
                break;
                
            case stateSOF2:
                if (SOF_LO == byRxData) {
                    byRxBufState++;
                    byIndexRxBuf = 0;
                    byCheckSumRxBuf = 0;
                }
                break;
                
            case stateLen:
                if ((byRxData < FRAME_LENGTH_MIN) || (byRxData > FRAME_LENGTH_MAX)) {
                    byRxBufState = stateSOF1;
                    bRxActive = FALSE;
                    break;
                }

            case stateCmd:
                byRxBufState++;

            case stateData:
                if (byIndexRxBuf < RX_BUFFER_SIZE){
                    byRxBuffer[byIndexRxBuf] = byRxData;
                    if (byIndexRxBuf > 0) {
                        byCheckSumRxBuf ^= byRxData;
                    }
                    if (++byIndexRxBuf >= lengthRxBuf) {
                        byRxBufState++;
                    }
                } else {
                    byRxBufState++;
                }
                TimerReset(&wTimeoutRx);
                break;
                
            case stateCheckSum:
                if (byRxData == byCheckSumRxBuf) {
                    byRetVal = stateFrameReceived;
                }
                byRetVal = stateFrameReceived;
                
            default:
                byRxBufState = stateSOF1;
                break;
            }
        } while ((ZW_SerialCheck() != 0) && (byRetVal == stateIdle));
    }
    
    if (byRetVal == stateIdle) {
        if (bRxActive && (TimerGetElapsedTime(wTimeoutRx) >= RX_TIMEOUT)) {
            byRetVal = stateRxTimeout;
            byRxBufState = stateSOF1;
        }
    }
    
    return byRetVal;
}

/**
 * @func   SendTxFrame   
 * @brief  None
 * @param  TX_FRAME frame
 * @retval None
 */
void
SendTxFrame(
    TX_FRAME frame
) {
    BYTE byLen, byCmd, byChecksum;
    BYTE_P pBuff;
    BYTE i;
    
    if (frame.buff != NULL) {
        byLen = frame.length;
        byCmd = frame.command;
        pBuff = frame.buff;
    }
    byChecksum = 0x00;
    ZW_SerialPutByte(SOF_HI);
    ZW_SerialPutByte(SOF_LO);
    ZW_SerialPutByte(byLen);
    ZW_SerialPutByte(byCmd);
    
    byChecksum ^= byCmd;
    
    for (i = 0; (pBuff != NULL) & (i < byLen - 2); i++) {
        ZW_SerialPutByte(pBuff[i]);
        byChecksum ^= pBuff[i];
    }
    
    ZW_SerialPutByte(byChecksum);
    ZW_SerialFlush();
}

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
) {
    BYTE byLen, byCmd, byChecksum;
    BYTE_P pByBuff;
    BYTE i;
    
    if (pByBuffer != NULL) {
        byLen = byLength;
        byCmd = byCommand;
        pByBuff = pByBuffer;
    }
    byChecksum = 0x00;
    ZW_SerialPutByte(SOF_HI);
    ZW_SerialPutByte(SOF_LO);
    ZW_SerialPutByte(byLen);
    ZW_SerialPutByte(byCmd);
    
    byChecksum ^= byCmd;
    
    for (i = 0; i < byLen - 2; i++) {
        ZW_SerialPutByte(pByBuff[i]);
        byChecksum ^= pByBuff[i];
    }
    ZW_SerialPutByte(byChecksum);
    ZW_SerialFlush();
}
