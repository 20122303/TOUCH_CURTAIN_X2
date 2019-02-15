#include "lumi_serial_api.h"
#include "convert_data.h"
#include "serial.h"
#include "serial_man.h"
#include "config_app.h"
#include "curtain_device.h"

#define STATE_STOP                          0x01
#define STATE_RUN                           0x02 
#define TX_BUFFER_SIZE                      64
#define LENGTH_CMD_CURTAIN_SET              6

static BYTE g_byValue[1] = { 0 };

static BYTE g_byTxBuffer[TX_BUFFER_SIZE] = { 0 };    

/**
 * @func   HandleCurtain
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleCurtain(
    CMD_BUFFER* pCmd, 
    BYTE byLength
) {
    BYTE byValue = 0;
    UNUSED(byLength);
    switch (pCmd->lumi_curtain.state) {
    case STATE_RUN:
        byValue = ConvertValueMcuToZw(pCmd->lumi_curtain.level, DEVICE_CURTAIN);
        if (pCmd->lumi_common.no == 1) { // Now only support Endpoint 0
            g_byValue[0] = byValue;
            HandleCurtainLevel(byValue, 0);
        }
        break;
    }
}

/**
 * @func   SetCurtainLevel
 * @brief  None
 * @param  None
 * @retval None
 */
void
SetCurtainLevel(
    BYTE byLevel,
    BYTE byEndpoint
) {
    if (byEndpoint == 0) { // Now only support Endpoint 0
        g_byTxBuffer[0] = 1; // Device No = 1
        g_byTxBuffer[1] = DEVICE_CURTAIN;
        g_byTxBuffer[2] = STATE_RUN;
        g_byTxBuffer[3] = ConvertValueZwToMcu(byLevel, DEVICE_CURTAIN);
        SendTxData(CMD_SET_CTRL, g_byTxBuffer, LENGTH_CMD_CURTAIN_SET);
    }
}

/**
 * @func   GetCurtainLevel
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
GetCurtainLevel(
    BYTE byEndpoint
) {
    if (byEndpoint == 0) { // Now only support Endpoint 0
        return g_byValue[byEndpoint];
    } else {
        return 0;
    }
}

/**
 * @func   StopCurtain
 * @brief  None
 * @param  None
 * @retval None
 */
void
StopCurtain(
    BYTE byEndpoint
) {
    if (byEndpoint == 0) { // Now only support Endpoint 0
        g_byTxBuffer[0] = 1;
        g_byTxBuffer[1] = DEVICE_CURTAIN;
        g_byTxBuffer[2] = STATE_STOP;
        g_byTxBuffer[3] = 0; // Don't care
        SendTxData(CMD_SET_CTRL, g_byTxBuffer, LENGTH_CMD_CURTAIN_SET);
    }
}