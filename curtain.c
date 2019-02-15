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
 * Last Changed By:  Author: trungtq
 * Revision:         Revision: 2.1
 * Last Changed:     Date: 2016-05-31 09:00:00 (Tue, 31 May 2016)
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "config_app.h"

#include <slave_learn.h>
#include <ZW_slave_api.h>
#ifdef ZW_SLAVE_32
#include <ZW_slave_32_api.h>
#else /* ZW_SLAVE_32 */
#include <ZW_slave_routing_api.h>
#endif  /* ZW_SLAVE_32 */

#include <ZW_classcmd.h>
#include <ZW_mem_api.h>
#include <ZW_TransportLayer.h>

#include <eeprom.h>
#include <ZW_uart_api.h>

#include <misc.h>

#ifdef BOOTLOADER_ENABLED
#include <ota_util.h>
#include <CommandClassFirmwareUpdate.h>
#endif /* BOOTLOADER_ENABLED */

#ifdef ZW_ISD51_DEBUG
#include "ISD51.h"
#endif /* ZW_ISD51_DEBUG */

#include <ev_man.h>
#include <io_zdp03a.h>
#include <association_plus.h>
#include <agi.h>
#include <CommandClassAssociation.h>
#include <CommandClassAssociationGroupInfo.h>
#include <CommandClassVersion.h>
#include <CommandClassZWavePlusInfo.h>
#include <CommandClassPowerLevel.h>
#include <CommandClassDeviceResetLocally.h>
#include <manufacturer_specific_device_id.h>
#include <CommandClassBasic.h>
#include <CommandClassBinarySwitch.h>

#if MULTILEVEL_SWITCH_VERSION == 1
#include <CommandClassMultiLevelSwitch_v1.h>
#elif MULTILEVEL_SWITCH_VERSION == 2
#include <CommandClassMultiLevelSwitch_v2.h>
#elif MULTILEVEL_SWITCH_VERSION == 3
#include <CommandClassMultiLevelSwitch_v3.h>
#endif

#include <CommandClassWindowCovering.h>
#include <ZW_TransportEndpoint.h>

#ifdef MULTI_CHANNEL_TRANSPORT
#include <CommandClassMultiChan.h>
#include <CommandClassMultiChanAssociation.h>
#endif /*MULTI_CHANNEL_TRANSPORT*/

#include "curtain.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef MAIN_DB
#define MAIN_DEBUG(x)       (x)
#else /* MAIN_DB */
#define MAIN_DEBUG(x)
#endif /* MAIN_DB */

#ifdef ZW_DEBUG_CURTAIN
#define ZW_DEBUG_CURTAIN_SEND_BYTE(data)        ZW_DEBUG_SEND_BYTE(data)
#define ZW_DEBUG_CURTAIN_SEND_STR(STR)          ZW_DEBUG_SEND_STR(STR)
#define ZW_DEBUG_CURTAIN_SEND_NUM(data)         ZW_DEBUG_SEND_NUM(data)
#define ZW_DEBUG_CURTAIN_SEND_WORD_NUM(data)    ZW_DEBUG_SEND_WORD_NUM(data)
#define ZW_DEBUG_CURTAIN_SEND_NL()              ZW_DEBUG_SEND_NL()
#else /* ZW_DEBUG_CURTAIN */
#define ZW_DEBUG_CURTAIN_SEND_BYTE(data)
#define ZW_DEBUG_CURTAIN_SEND_STR(STR)
#define ZW_DEBUG_CURTAIN_SEND_NUM(data)
#define ZW_DEBUG_CURTAIN_SEND_WORD_NUM(data)
#define ZW_DEBUG_CURTAIN_SEND_NL()
#endif /* ZW_DEBUG_CURTAIN */

typedef enum _EVENT_APP_ {
    EVENT_EMPTY = DEFINE_EVENT_APP_NBR,
    EVENT_APP_INIT,
    EVENT_APP_OTA_START,
    EVENT_APP_LEARN_MODE_FINISH,
    EVENT_APP_WAKEUP_NVR_ERROR_FINSIH,
    EVENT_APP_REFRESH_MMI,
    EVENT_REPORT_ENDPOINT,
    EVENT_REPORT_LIFELINE
} EVENT_APP;

typedef enum _STATE_APP_ {
    STATE_APP_STARTUP,
    STATE_APP_IDLE,
    STATE_APP_LEARN_MODE,
    STATE_APP_LOCAL_RESET,
    STATE_APP_CURTAIN_CALIB,
    STATE_APP_OTA,
    STATE_APP_NVR_ERROR
} STATE_APP;

#define TIME_CHECK_RESET            5

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/* CHANGE - Change the list of command classes to fit your product */

/**
 * Unsecure node information list.
 * Be sure Command classes are not duplicated in both lists.
 * CHANGE THIS - Add all supported non-secure command classes here.
 * Remember to add COMMAND_CLASS_MULTI_CHANNEL_V3 if device has end-points!
 **/
static code BYTE cmdClassListNonSecureNotIncluded[] = {
    COMMAND_CLASS_ZWAVEPLUS_INFO,
    COMMAND_CLASS_VERSION,
    COMMAND_CLASS_ASSOCIATION,
    COMMAND_CLASS_ASSOCIATION_GRP_INFO,
    COMMAND_CLASS_MANUFACTURER_SPECIFIC,
    COMMAND_CLASS_DEVICE_RESET_LOCALLY,
    COMMAND_CLASS_POWERLEVEL,
    COMMAND_CLASS_SWITCH_MULTILEVEL,
    COMMAND_CLASS_WINDOW_COVERING

    #ifdef MULTI_CHANNEL_TRANSPORT
    ,COMMAND_CLASS_MULTI_CHANNEL_V4
    ,COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2
    #endif /* MULTI_CHANNEL_TRANSPORT */

    #ifdef SECURITY
    ,COMMAND_CLASS_SECURITY
    #endif /* SECURITY */
    
    #ifdef BOOTLOADER_ENABLED
    ,COMMAND_CLASS_FIRMWARE_UPDATE_MD_V2
    #endif /* BOOTLOADER_ENABLED */
};

/**
 * Unsecure node information list Secure included.
 * Be sure Command classes are not duplicated in both lists.
 * CHANGE THIS - Add all supported non-secure command classes here
 **/
static code BYTE cmdClassListNonSecureIncludedSecure[] = {
    #ifdef SECURITY
    COMMAND_CLASS_ZWAVEPLUS_INFO,
    COMMAND_CLASS_SECURITY
    #else /* SECURITY */
    NULL
    #endif /* SECURITY */
};

/**
 * Secure node inforamtion list.
 * Be sure Command classes are not duplicated in both lists.
 * CHANGE THIS - Add all supported secure command classes here
 * Remember to add COMMAND_CLASS_MULTI_CHANNEL_V3 if device has end-points!
 **/
static BYTE cmdClassListSecure[] = {
    #ifdef SECURITY
    COMMAND_CLASS_VERSION,
    COMMAND_CLASS_ASSOCIATION,
    COMMAND_CLASS_ASSOCIATION_GRP_INFO,
    COMMAND_CLASS_MANUFACTURER_SPECIFIC,
    COMMAND_CLASS_DEVICE_RESET_LOCALLY,
    COMMAND_CLASS_POWERLEVEL,
    COMMAND_CLASS_SWITCH_MULTILEVEL,
    COMMAND_CLASS_WINDOW_COVERING
    
    #ifdef MULTI_CHANNEL_TRANSPORT
    ,COMMAND_CLASS_MULTI_CHANNEL_V4
    ,COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2
    #endif /* MULTI_CHANNEL_TRANSPORT */

    #ifdef BOOTLOADER_ENABLED
    ,COMMAND_CLASS_FIRMWARE_UPDATE_MD_V2
    #endif /* BOOTLOADER_ENABLED */
    #else /* SECURITY */
    NULL
    #endif /* SECURITY */
};

/**
 * Structure includes application node information list's and device type.
 */
APP_NODE_INFORMATION m_AppNIF = {
    cmdClassListNonSecureNotIncluded, sizeof(cmdClassListNonSecureNotIncluded),
    cmdClassListNonSecureIncludedSecure, sizeof(cmdClassListNonSecureIncludedSecure),
    cmdClassListSecure, sizeof(cmdClassListSecure),
    DEVICE_OPTIONS_MASK, GENERIC_TYPE, SPECIFIC_TYPE
};

const char GroupName[] = "Lifeline";
static CMD_CLASS_GRP  agiTableLifeLine[] = { AGITABLE_LIFELINE_GROUP };

/*Handle only one event!*/
static EVENT_APP eventQueue = EVENT_EMPTY;
static BYTE g_byLastEvent = EVENT_EMPTY;
static BYTE g_byCountCheckReset = 0;
static BOOL ValidNVR = FALSE;

static BOOL g_boGetFirstTime = TRUE;
static STATE_APP currentState = STATE_APP_IDLE;

BYTE wakeupReason;
BYTE myNodeID = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void ZCB_DeviceResetLocallyDone(BYTE status);
STATE_APP AppState();
void AppStateManager(EVENT_APP ev);
void ChangeState(STATE_APP st);
BOOL AddEvent(EVENT_APP ev);
void EventSchedularInit();
void EventSchedular(void);
void ZCB_ReportNVM(void);

#ifdef BOOTLOADER_ENABLED
void ZCB_OTAFinish(OTA_STATUS otaStatus);
BOOL ZCB_OTAStart();
#endif /* BOOTLOADER_ENABLED */

void ZCB_ErrorNvrFlashFinish();
void CheckResetState();

static void Report_StateCompleted(BYTE byStatus);
static void KeyStatus(CMD_BUFFER* pCmd, BYTE byLength);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   ApplicationRfNotify
 * @brief  Notify the application when the radio switch state
 *         Called from the Z-Wave PROTOCOL When radio switch from Rx to Tx or from Tx to Rx
 *         or when the modulator PA (Power Amplifier) turn on/off
 *
 * @param  BYTE rfState
 * @retval None
 */
void
ApplicationRfNotify(
BYTE rfState /* IN state of the RF, the available values is as follow:
                ZW_RF_TX_MODE: The RF switch from the Rx to Tx mode, the modualtor is started and PA is on
                ZW_RF_PA_ON: The RF in the Tx mode, the modualtor PA is turned on
                ZW_RF_PA_OFF: the Rf in the Tx mode, the modulator PA is turned off
                ZW_RF_RX_MODE: The RF switch from Tx to Rx mode, the demodulator is started.*/
) {
    UNUSED(rfState);
}

/**
 * @func   ApplicationInitHW
 * @brief  Initialization of non Z-Wave module hardware
 * @param  None
 * @retval Returning FALSE from this function will force the API into
 *         production test mode.
 */
BYTE
ApplicationInitHW(
    BYTE bWakeupReason /* IN  Nothing */
) {
    /* CHANGE THIS - Set updown your external hardware here */
    ValidNVR = InitZDP03A();
    wakeupReason = bWakeupReason;
    Transport_OnApplicationInitHW(bWakeupReason);

    return (TRUE);
}

/**
 * @func   ApplicationInitSW
 * @brief  Initialization of the Application Software variables and states
 * @param  None
 * @retval BYTE
 */
BYTE
ApplicationInitSW(void) { /* IN   Nothing */
    /* Init state machine*/
    currentState = STATE_APP_STARTUP;
    InitSerialMan();

    EventSchedularInit();

    #ifdef WATCHDOG_ENABLED
    ZW_WatchDogEnable();
    #endif /* WATCHDOG_ENABLED */

    /* Initialize the NVM if needed */
    if (MemoryGetByte((WORD) & EEOFFSET_MAGIC_far) == APPL_MAGIC_VALUE) {
        /* Initialize PowerLevel functionality */
        loadStatusPowerLevel(NULL, NULL);
    } else {
        /* Initialize transport layer NVM */
        Transport_SetDefault();
        /* Reset protocol */
        ZW_SetDefault();        
        /* Initialize PowerLevel functionality.*/
        loadInitStatusPowerLevel(NULL, NULL);
        /* CHANGE THIS - Add initialization of application NVM here */
        ZW_MEM_PUT_BYTE((WORD)&EEOFFSET_MAGIC_far, APPL_MAGIC_VALUE);
    }

    /* Initialize association module */
    AssociationInit(FALSE);

    /* Setup AGI group lists*/
    AGI_Init();
    AGI_LifeLineGroupSetup(
                    agiTableLifeLine, 
                    (sizeof(agiTableLifeLine) / sizeof(CMD_CLASS_GRP)), 
                    GroupName EP_SUPPORT(COMMA ENDPOINT_ROOT));
    
    /* Get this sensors identification on the network */
    MemoryGetID(NULL, &myNodeID);

    /* Initialize manufactory specific module */
    ManufacturerSpecificDeviceIDInit();

    InitProcessManager(AppStateManager, NULL);

    #ifdef BOOTLOADER_ENABLED
    /* Initialize OTA module */
    OtaInit(ZWAVE_PLUS_TX_OPTIONS, ZCB_OTAStart, ZCB_OTAFinish);
    #endif /* BOOTLOADER_ENABLED */

    Transport_OnApplicationInitSW(&m_AppNIF, NULL);
    InitReportManager(KeyStatus);
    StartGetStateEP();
    
    ZW_TIMER_START(ZCB_ReportNVM, TIMER_ONE_SECOND, TIMER_ONE_TIME + 1);

    if (FALSE == ValidNVR) {
        wakeupReason = EVENT_WAKEUP_NVR_ERROR;
        ZW_DEBUG_CURTAIN_SEND_STR("NVR_ERROR ");
    }

    /* Init state machine*/
    AppStateManager((EVENT_WAKEUP)wakeupReason);

    return TRUE;
}

/**
 * @func   ApplicationTestPoll
 * @brief  This function is called when the slave enters test mode.
 *         Code will not exit until it is reset
 * @param  None
 * @retval None
 */
void
ApplicationTestPoll(void) {
}

/**
 * @func   ApplicationPoll
 * @brief  Application poll function for the slave application
 * @param  None
 * @retval None
 */
void
ApplicationPoll(void) {
    #ifdef WATCHDOG_ENABLED
    ZW_WatchDogKick();
    #endif /* WATCHDOG_ENABLED */
    ProcessManScan();
    EventSchedular();
}

/**
 * @func   Transport_ApplicationCommandHandlerEx
 * @brief  Handling of a received application commands and requests
 * @param  RECEIVE_OPTIONS_TYPE_EX_P    rxOpt
 *         ZW_APPLICATION_TX_BUFFER_P   pCmd
 *         BYTE                         cmdLength
 * @retval None
 */
void
Transport_ApplicationCommandHandlerEx(
    RECEIVE_OPTIONS_TYPE_EX *rxOpt, /* IN receive options of type RECEIVE_OPTIONS_TYPE_EX  */
    ZW_APPLICATION_TX_BUFFER *pCmd, /* IN Payload from the received frame, the union */
                                    /*    should be used to access the fields */
    BYTE cmdLength                  /* IN Number of command bytes including the command */
) {
    /* Call command class handlers */
    switch (pCmd->ZW_Common.cmdClass) {
    case COMMAND_CLASS_VERSION:
        handleCommandClassVersion(rxOpt, pCmd, cmdLength);
        break;

    #ifdef BOOTLOADER_ENABLED
    case COMMAND_CLASS_FIRMWARE_UPDATE_MD_V2:
        handleCommandClassFWUpdate(rxOpt, pCmd, cmdLength);
        break; 
    #endif /* BOOTLOADER_ENABLED */

    case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
        handleCommandClassAssociationGroupInfo(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_ASSOCIATION:
		handleCommandClassAssociation(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_POWERLEVEL:
        handleCommandClassPowerLevel(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_MANUFACTURER_SPECIFIC:
        handleCommandClassManufacturerSpecific(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_ZWAVEPLUS_INFO:
        handleCommandClassZWavePlusInfo(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_BASIC:
        handleCommandClassBasic(rxOpt, pCmd, cmdLength);
        break;

    case COMMAND_CLASS_SWITCH_MULTILEVEL:
        #if   MULTILEVEL_SWITCH_VERSION == 1
        handleCommandClassMultiLevelSwitch(rxOpt, pCmd, cmdLength);
        #elif   MULTILEVEL_SWITCH_VERSION == 2
        handleCommandClassMultiLevelSwitchV2(rxOpt, pCmd, cmdLength);
        #elif   MULTILEVEL_SWITCH_VERSION == 3
        handleCommandClassMultiLevelSwitchV3(rxOpt, pCmd, cmdLength);
        #endif
        break;

    case COMMAND_CLASS_WINDOW_COVERING:
        handleCommandClassWindowConvering(rxOpt, pCmd, cmdLength);
        break;

    #ifdef MULTI_CHANNEL_TRANSPORT
    case COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2:
        handleCommandClassMultiChannelAssociation(rxOpt, pCmd, cmdLength);
        break;
    #endif /*MULTI_CHANNEL_TRANSPORT*/

    }
}

/**
 * @func   handleCommandClassVersionAppl
 * @brief  Application specific Command Class Version handler
 * @param  None
 * @retval BYTE
 */
BYTE
handleCommandClassVersionAppl(
    BYTE cmdClass
) {
    BYTE commandClassVersion = UNKNOWN_VERSION;

    switch (cmdClass) {
    case COMMAND_CLASS_VERSION:
        commandClassVersion = CommandClassVersionVersionGet();
        break;

    #ifdef BOOTLOADER_ENABLED
    case COMMAND_CLASS_FIRMWARE_UPDATE_MD:
        commandClassVersion = CommandClassFirmwareUpdateMdVersionGet();
        break;
    #endif /* BOOTLOADER_ENABLED */

    case COMMAND_CLASS_POWERLEVEL:
        commandClassVersion = CommandClassPowerLevelVersionGet();
        break;

    case COMMAND_CLASS_MANUFACTURER_SPECIFIC:
        commandClassVersion = CommandClassManufacturerVersionGet();
        break;

    case COMMAND_CLASS_ASSOCIATION:
        commandClassVersion = CommandClassAssociactionVersionGet();
        break;

    case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
        commandClassVersion = CommandClassAssociationGroupInfoVersionGet();
        break;

    case COMMAND_CLASS_DEVICE_RESET_LOCALLY:
        commandClassVersion = CommandClassDeviceResetLocallyVersionGet();
        break;

    case COMMAND_CLASS_ZWAVEPLUS_INFO:
        commandClassVersion = CommandClassZWavePlusVersion();
        break;

    case COMMAND_CLASS_BASIC:
        commandClassVersion =  CommandClassBasicVersionGet();
        break;

    case COMMAND_CLASS_WINDOW_COVERING:
        commandClassVersion = CommandClassWindowCoveringVersionGet();
        break;

   case COMMAND_CLASS_SWITCH_MULTILEVEL:
        #if   MULTILEVEL_SWITCH_VERSION == 1
        commandClassVersion = CommandClassMultiLevelSwitchVersionGet();
        #elif  MULTILEVEL_SWITCH_VERSION == 2
        commandClassVersion = CommandClassMultiLevelSwitchV2VersionGet();
        #elif  MULTILEVEL_SWITCH_VERSION == 3
		commandClassVersion = CommandClassMultiLevelSwitchV3VersionGet();
        #endif
        break;

    #ifdef MULTI_CHANNEL_TRANSPORT
    case COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2:
        commandClassVersion = CmdClassMultiChannelAssociationVersion();
        break;

    case COMMAND_CLASS_MULTI_CHANNEL_V3:
        commandClassVersion = CmdClassMultiChannelGet();
        break;
    #endif /* MULTI_CHANNEL_TRANSPORT */

    #ifdef SECURITY
    case COMMAND_CLASS_SECURITY:
        commandClassVersion = CommandClassSecurityVersionGet();
        break;
    #endif /* SECURITY */

    default:
        commandClassVersion = UNKNOWN_VERSION;
    }
    return commandClassVersion;
}

/**
 * @func   ApplicationSlaveUpdate
 * @brief  Inform a slave application that a node information is received.
 *         Called from the slave command handler when a node information frame
 *         is received and the Z-Wave protocol is not in a state where it is needed.
 * @param  BYTE bStatus
 *         BYTE bNodeID
 *         BYTE_P pCmd
 *         BYTE bLen
 * @retval None
 */
void
ApplicationSlaveUpdate(
    BYTE bStatus,       /*IN  Status event */
    BYTE bNodeID,       /*IN  Node id of the node that send node info */
    BYTE* pCmd,         /*IN  Pointer to Application Node information */
    BYTE bLen           /*IN  Node info length */
) {
    UNUSED(bStatus);
    UNUSED(bNodeID);
    UNUSED(pCmd);
    UNUSED(bLen);
}

/**
 * @func   handleNbrFirmwareVersions
 * @brief  Read number of firmwares in application.
 * @param  None
 * @retval BYTE
 */
BYTE
handleNbrFirmwareVersions(void) {
    return 1; /* CHANGE THIS - firmware 0 version */
}

/**
 * @func   handleGetFirmwareVersion
 * @brief  Read application firmware version informations
 * @param  BYTE bFirmwareNumber
 *         VG_VERSION_REPORT_V2_VG_P pVariantgroup
 * @retval None
 */
void
handleGetFirmwareVersion(
    BYTE bFirmwareNumber,
    VG_VERSION_REPORT_V2_VG* pVariantgroup
) {
    /* firmware 0 version and sub version */
    if (bFirmwareNumber == 0) {
        pVariantgroup->firmwareVersion = APP_VERSION;
        pVariantgroup->firmwareSubVersion = APP_REVISION;
    } else {
        /* Just set it to 0 if firmware n is not present */
        pVariantgroup->firmwareVersion = 0;
        pVariantgroup->firmwareSubVersion = 0;
    }
}

/**
 * @func   LearnCompleted
 * @brief  Callback which is called on learnmode completed
 *         Application specific handling of LearnModeCompleted - called from
 *         slave_learn.c
 * @param  None
 * @retval None
 */
void
LearnCompleted(
    BYTE bNodeID        /* IN The nodeID assigned */
) {
    /* If bNodeID= 0xff.. learn mode failed */
    if (bNodeID != NODE_BROADCAST) {
        /*Success*/
        myNodeID = bNodeID;
        if (myNodeID == 0) {
            /* Clear association */
            AssociationInit(TRUE);
        }
    }
    AppStateManager(EVENT_APP_LEARN_MODE_FINISH);
    Transport_OnLearnCompleted(bNodeID);
}

/**
 * @func   GetMyNodeID
 * @brief  Get the device node ID
 * @param  None
 * @retval BYTE
 */
BYTE
GetMyNodeID(void) {
    return myNodeID;
}

/**
 * @func   AppState
 * @brief  Return application statemachine state
 * @param  None
 * @retval None
 */
STATE_APP
AppState(
) {
    return currentState;
}

/**
 * @func   AppStateManager
 * @brief  None
 * @param  EVENT_APP ev
 * @retval None
 */
void
AppStateManager(
    EVENT_APP ev
) {
    ZW_DEBUG_CURTAIN_SEND_STR("AppStateManager ev ");
    ZW_DEBUG_CURTAIN_SEND_NUM(ev);
    ZW_DEBUG_CURTAIN_SEND_STR(" stt ");
    ZW_DEBUG_CURTAIN_SEND_NUM(currentState);
    ZW_DEBUG_CURTAIN_SEND_NL();

    switch (currentState) {
    case STATE_APP_STARTUP:
        if (EVENT_WAKEUP_NVR_ERROR == ev) {
            ChangeState(STATE_APP_NVR_ERROR);
        } else {
            ChangeState(STATE_APP_IDLE);
            AddEvent(EVENT_APP_REFRESH_MMI);
        }
        break;

    case STATE_APP_IDLE:
        if (EVENT_BUTTON_ADDNODE == ev) {
            if (myNodeID != 0) {
                StartLearnModeNow(LEARN_MODE_EXCLUSION);
            } else {
                StartLearnModeNow(LEARN_MODE_INCLUSION);
            }
            ChangeState(STATE_APP_LEARN_MODE);
        }

        if (EVENT_BUTTON_RESET == ev) {
            SerialReport(EVENT_BUTTON_REPORT_RESET);
            handleCommandClassDeviceResetLocally(ZCB_DeviceResetLocallyDone);
            ChangeState(STATE_APP_LOCAL_RESET);
        }

        if (EVENT_BUTTON_REPORT_ADDNODE == ev) {
            SerialReport(EVENT_BUTTON_REPORT_ADDNODE);
        } else if (EVENT_BUTTON_REPORT_TOUCH_HOLD == ev) {
            //SerialReport(EVENT_BUTTON_REPORT_TOUCH_HOLD);
        } 

        if (EVENT_REPORT_STATE_ENDPOINT == ev) {
            AddEvent(EVENT_REPORT_ENDPOINT);
        }

        if (EVENT_REPORT_ENDPOINT == ev) {
            if (!ReportState()) {
                AddEvent(EVENT_EMPTY);
            }
        }
        
        break;

    case STATE_APP_LEARN_MODE:
        if (EVENT_BUTTON_ADDNODE == ev) {
            StartLearnModeNow(LEARN_MODE_DISABLE);
            ChangeState(STATE_APP_IDLE);
        }

        if (EVENT_APP_LEARN_MODE_FINISH == ev) {
            ChangeState(STATE_APP_IDLE);
        }

        if (EVENT_BUTTON_REPORT_ADDNODE == ev) {
            SerialReport(EVENT_BUTTON_REPORT_ADDNODE);
        } else if (EVENT_BUTTON_REPORT_TOUCH_HOLD == ev) {
            //SerialReport(EVENT_BUTTON_REPORT_TOUCH_HOLD);
        }
        break;

    case STATE_APP_LOCAL_RESET:
        //device reboot in this state by ZCB_CommandClassDeviceResetLocally
        break;

    case STATE_APP_OTA:
        /* OTA state... do nothing until firmware update is finish */
        break;

    case STATE_APP_NVR_ERROR:
        if (EVENT_APP_WAKEUP_NVR_ERROR_FINSIH == ev) {
            ChangeState(STATE_APP_IDLE);
            AddEvent(EVENT_APP_REFRESH_MMI);
        }
        break;
    }
}

/**
 * @func   ChangeState
 * @brief  Change state
 * @param  STATE_APP st
 * @retval None
 */
void
ChangeState(
    STATE_APP st
) {
    currentState = st;
}

/**
 * @func   ZCB_DeviceResetLocallyDone
 * @brief  callback function perform reset device
 * @param  None
 * @retval None
 */
PCB(ZCB_DeviceResetLocallyDone)(
    BYTE status
) {
    UNUSED(status);
    /* CHANGE THIS - clean your own application data from NVM*/
    Transport_SetDefault();
    AssociationInit(TRUE);
    ZW_SetDefault();
    ZW_WatchDogEnable(); /* reset asic */
    for (;;) {}
}

#ifdef BOOTLOADER_ENABLED
/**
 * @func   ZCB_OTAFinish
 * @brief  OTA is finish
 * @param  None
 * @retval None
 */
code const void (code * ZCB_OTAFinish_p)(OTA_STATUS otaStatus) = &ZCB_OTAFinish;
void
ZCB_OTAFinish(
    OTA_STATUS otaStatus /* IN: Status on OTA */
) {
    UNUSED(otaStatus);
    /* Just reboot node to cleanup and start on new FW. */
    ZW_WatchDogEnable(); /* reset asic */
    while (1);
}

/**
 * @func   OTA_Start
 * @brief  Ota_Util calls this function when firmware update is ready to start.
 *         Return FALSE if OTA should be rejected else TRUE
 *
 * @param  None
 * @retval BOOL
 */
code const BOOL (code * ZCB_OTAStart_p)(void) = &ZCB_OTAStart;
BOOL   /* Return FALSE if OTA should be rejected else TRUE */
ZCB_OTAStart() {
    BOOL status = FALSE;
    if (STATE_APP_IDLE == AppState()) {
        AppStateManager(EVENT_APP_OTA_START);
        status = TRUE;
    }
    return status;
}
#endif /* BOOTLOADER_ENABLED */

/**
 * @func   AddEvent
 * @brief  Add event to queue. If return FALSE is queue full
 * @param  EVENT_APP ev
 * @retval BOOL
 */
BOOL
AddEvent(
    EVENT_APP ev
) {
    if (EVENT_EMPTY == eventQueue) {
        eventQueue = ev;
        return TRUE;
    } 
    return FALSE;
}

/**
 * @func   EventSchedularInit
 * @brief  Init queue
 * @param  None
 * @retval None
 */
void
EventSchedularInit(
) {
    eventQueue = EVENT_EMPTY;
}

/**
 * @func   EventSchedular
 * @brief  Event handler... can only handle one event
 * @param  None
 * @retval None
 */
void
EventSchedular(void) {
    if (EVENT_EMPTY != eventQueue) {
        BYTE tempEventQ = eventQueue;
        eventQueue = EVENT_EMPTY;
        AppStateManager(tempEventQ);
    }
}

/**
 * @func   ZCB_ErrorNvrFlashFinish
 * @brief  Callback function when error led is finish flashing
 * @param  None
 * @retval None
 */
PCB(ZCB_ErrorNvrFlashFinish) (void) {
    ZW_DEBUG_CURTAIN_SEND_STR("ErrorNvrFlashFinish ");
    ZW_DEBUG_CURTAIN_SEND_NL();
    AppStateManager(EVENT_APP_WAKEUP_NVR_ERROR_FINSIH);
}

/**
 * @func   ReportNVM
 * @brief  Check NVM Content
 * @param  None
 * @retval None
 */
PCB(ZCB_ReportNVM)(void) {
    if (MemoryGetByte((WORD) &EEOFFSET_MAGIC_far) == APPL_MAGIC_VALUE) {
        SerialReport(EVENT_BUTTON_REPORT_HAS_MAGIC);
    } else {
        SerialReport(EVENT_BUTTON_REPORT_NON_MAGIC);
    }
}

/******************************************************************************/
/*                                     BASIC                                  */
/******************************************************************************/
/**
 * @func   handleBasicSetCommand 
 * @brief  Handling of a Basic Set Command
 * @param  BYTE val 
 * @retval None
 */
void
handleBasicSetCommand(
    BYTE value
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    SetCurtainLevel(value, endpoint);
}

/**
 * @func   getAppBasicReport
 * @brief  return the On / Off state 
 * @param  None
 * @retval None
 */
BYTE
getAppBasicReport(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(endpoint);
    return 0;
}

/******************************************************************************/
/*                            MULTILEVEL SWITCH                               */
/******************************************************************************/
#if MULTILEVEL_SWITCH_VERSION >= 1
/**
 * @func   CommandClassMultiLevelSwitchSet
 * @brief  None
 * @param  None
 * @retval None
 */
void 
CommandClassMultiLevelSwitchSet(
    BYTE byLevel
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(byLevel);
    MAIN_DEBUG(ZW_UART_tx_send_str(" MultiLevelSet "));
    MAIN_DEBUG(ZW_UART_tx_send_nl());
    SetCurtainLevel(byLevel, endpoint);
}

/**
 * @func   CommandClassMultiLevelSwitchGet
 * @brief  Read the multilevel switch to HW level
 * @param  endpoint is the destination endpoint
 * @retval The current device HW level.
 */
BYTE 
CommandClassMultiLevelSwitchGet(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return GetCurtainLevel(endpoint);
}

/**
 * @func   HandleMultiLevelSwitchStartLevelChange
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleMultiLevelSwitchStartLevelChange(
    BYTE byStartLevel,
    BOOL boIgnoreStartLevel,
    BOOL boDimUp
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(byStartLevel);
    if (boIgnoreStartLevel) {
        if (boDimUp) {
            SetCurtainLevel(0xFF, endpoint);
        } else {
            SetCurtainLevel(0, endpoint);
        }
    } else {
    }
}

/**
 * @func   HandleMultiLevelStopLevelChange
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleMultiLevelStopLevelChange(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    StopCurtain(endpoint);
}
#endif /* MULTILEVEL_SWITCH_VERSION >= 1 */

#if MULTILEVEL_SWITCH_VERSION >= 2
/**
 * @func   CommandClassMultiLevelSwitchV2Set
 * @brief  None
 * @param  None
 * @retval None
 */
void
CommandClassMultiLevelSwitchV2Set(
    BYTE byLevel,
    BYTE byDuration
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(byLevel);
    UNUSED(byDuration);
    SetCurtainLevel(byLevel, endpoint);
}

/**
 * @func   CommandClassMultiLevelSwitchV2DefaultDuration
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
CommandClassMultiLevelSwitchV2DefaultDuration(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return DIMMING_DURATION_CONFIG_DEFAULT;
}

/**
 * @func   HandleMultiLevelSwitchV2StartLevelChange
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleMultiLevelSwitchV2StartLevelChange(
    BOOL boDimUp,
    BOOL boIgnoreStartLevel,
    BYTE byStartLevel,
    BYTE byDimmingDuration
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(boDimUp);
    UNUSED(boIgnoreStartLevel);
    UNUSED(byStartLevel);
    UNUSED(byDimmingDuration);
}
#endif /* MULTILEVEL_SWITCH_VERSION >= 2 */

#if MULTILEVEL_SWITCH_VERSION >= 3
/**
 * @func   CommandClassMultiLevelSwitchV3DefaultDuration
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE 
CommandClassMultiLevelSwitchV3DefaultDuration(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return DIMMING_DURATION_CONFIG_DEFAULT;
}

/**
 * @func   HandleMultiLevelSwitchV3StartLevelChange
 * @brief  None
 * @param  None
 * @retval None
 */
extern void 
HandleMultiLevelSwitchV3StartLevelChange(
    BYTE byDimUp,
    BOOL boIgnoreStartLevel,
    BYTE byInc,
    BYTE byStartLevel,
    BYTE byDimmingDuration,
    BYTE byStepSize
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(byDimUp);
    UNUSED(boIgnoreStartLevel);
    UNUSED(byInc);
    UNUSED(byStartLevel);
    UNUSED(byDimmingDuration);
    UNUSED(byStepSize);
}

/**
 * @func   CommandClassMultiLevelSwitchV3PrimaryTypeGet
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
CommandClassMultiLevelSwitchV3PrimaryTypeGet(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return PRIMARY_SWITCH_RESERVED;
}

/**
 * @func   CommandClassMultiLevelSwitchV3SecondaryTypeGet
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
CommandClassMultiLevelSwitchV3SecondaryTypeGet(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return SECONDARY_SWITCH_RESERVED;
}
#endif /* MULTILEVEL_SWITCH_VERSION >= 3 */

#if MULTILEVEL_SWITCH_VERSION >= 4
/**
 * @func   CommandClassMultiLevelSwitchV4Get
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE 
CommandClassMultiLevelSwitchV4Get(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return GetCurtainLevel(endpoint);
}

/**
 * @func   CommandClassMultiLevelSwitchGetTargetValue
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
CommandClassMultiLevelSwitchV4GetTargetValue(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return 0;
}

/**
 * @func   CommandClassMultiLevelSwitchV4GetDuration
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE 
CommandClassMultiLevelSwitchV4GetDuration(
    EP_SUPPORT(BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    return 0;
}

#endif /* MULTILEVEL_SWITCH_VERSION >= 4 */

/******************************************************************************/
/*                              WINDOW COVERING                               */
/******************************************************************************/
/**
 * @func   HandleWindowCoveringStartLevelChange
 * @brief  None
 * @param  None
 * @retval None
 */
void 
HandleWindowCoveringStartLevelChange(
    BOOL boDimUp,
    WINDOW_COVERING_PARAMS_T paramId,
    BYTE byDuration
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(boDimUp);
    UNUSED(paramId);
    UNUSED(byDuration);
}

/**
 * @func   HandleWindowCoveringStopLevelChange
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleWindowCoveringStopLevelChange(
    WINDOW_COVERING_PARAMS_T paramId
    EP_SUPPORT(COMMA BYTE endpoint)
) {
    NO_EP_SUPPORT(BYTE endpoint = 0;)
    UNUSED(paramId);
}

/******************************************************************************/
/*                              WINDOW COVERING                               */
/******************************************************************************/

/**
 * @func   HandleCurtainLevel
 * @brief  None
 * @param  None
 * @retval None
 */
void
HandleCurtainLevel(
    BYTE byLevel,
    BYTE byEndpoint
) {
    BYTE byDestNode = AssociationGetLifeLineNodeID();
    UNUSED(byEndpoint);

    if ((byDestNode == 0xFF) || (GetMyNodeID() == 0)) { return; }

    if (!g_boGetFirstTime) {
        #if   MULTILEVEL_SWITCH_VERSION == 1
        CmdClassMultiLevelSwitchV1ReportSendUnsolicited(
                                ZWAVE_PLUS_TX_OPTIONS,
                                byDestNode,
                                byLevel, 
                                Report_StateCompleted);
        #endif /* MULTILEVEL_SWITCH_VERSION == 1 */ 
        
        #if MULTILEVEL_SWITCH_VERSION == 2
        CmdClassMultiLevelSwitchV2ReportSendUnsolicited(
                                ZWAVE_PLUS_TX_OPTIONS,
                                byDestNode,
                                byLevel, 
                                Report_StateCompleted);
        #endif /* MULTILEVEL_SWITCH_VERSION == 2 */
            
        #if MULTILEVEL_SWITCH_VERSION == 3
        CmdClassMultiLevelSwitchV3ReportSendUnsolicited(
                                ZWAVE_PLUS_TX_OPTIONS,
                                byDestNode,
                                byLevel, 
                                Report_StateCompleted);
        #endif /* MULTILEVEL_SWITCH_VERSION == 3 */
    }
    g_boGetFirstTime = FALSE;
}

/**
 * @func   KeyStatus
 * @brief  None
 * @param  None
 * @retval None
 */
PCB(KeyStatus) (
    CMD_BUFFER* pCmd,
    BYTE        byLength
) {
    switch (pCmd->lumi_common.type) {
    case DEVICE_CURTAIN:
        HandleCurtain(pCmd, byLength);
        break;
    }
}

/**
 * @func   Report_StateCompleted
 * @brief  None
 * @param  None
 * @retval None
 */
PCB(Report_StateCompleted)(
    BYTE byStatus
) {
    UNUSED(byStatus);
}

