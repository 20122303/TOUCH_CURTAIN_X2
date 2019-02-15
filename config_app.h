/**
 *
 * Copyright (c) 2001-2014
 * Sigma Designs, Inc.
 * All Rights Reserved
 *
 * @file config_app.h
 *
 * @brief This header file contains defines for application version
 *  in a generalized way.
 *
 *  Don't change the name of the file, and son't change the names of
 *  APP_VERSION and APP_REVISION, as they are handled automatically by
 *  the release procedure. The version information will be set automatically
 *  by the "make_release.bat"-script.
 *
 * Author: Erik Friis Harck
 *
 * Last Changed By: $Author: tro $
 * Revision: $Revision: 0.00 $
 * Last Changed: $Date: 2014/12/09 14:28:21 $
 *
 */
#ifndef _CONFIG_APP_H_
#define _CONFIG_APP_H_

#ifdef __C51__
#include "ZW_product_id_enum.h"
#include <commandClassManufacturerSpecific.h>
#include <agi.h>
#endif

/****************************************************************************
 *
 * Application version, which is generated during release of SDK.
 * The application developer can freely alter the version numbers
 * according to his needs.
 *
 ****************************************************************************/
#define APP_VERSION                     2
#define APP_REVISION                    40

/****************************************************************************
 *
 * Defines device generic and specific types
 *
 ****************************************************************************/
#define GENERIC_TYPE                    GENERIC_TYPE_WINDOW_COVERING
#define SPECIFIC_TYPE                   SPECIFIC_TYPE_SIMPLE_WINDOW_COVERING
/**
 * See ZW_basic_api.h for ApplicationNodeInformation field deviceOptionMask
 */
#define DEVICE_OPTIONS_MASK             APPLICATION_NODEINFO_LISTENING | APPLICATION_NODEINFO_OPTIONAL_FUNCTIONALITY

/****************************************************************************
 *
 * Defines used to initialize the Z-Wave Plus Info Command Class.
 *
 ****************************************************************************/
#define APP_ROLE_TYPE                   ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_ALWAYS_ON
#define APP_NODE_TYPE                   ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_NODE

#define APP_ICON_TYPE                   ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE
#define APP_USER_ICON_TYPE              ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE

/****************************************************************************
 *
 * Defines used to initialize the Manufacturer Specific Command Class.
 *
 ****************************************************************************/
#define APP_MANUFACTURER_ID             MFG_ID_HOME_CONTROLS

#define APP_PRODUCT_TYPE_ID             PRODUCT_TYPE_ID_ZWAVE_PLUS
#define APP_PRODUCT_ID                  PRODUCT_ID_Lumi_Curtain_Touch_x2

#define APP_FIRMWARE_ID                 APP_PRODUCT_ID | (APP_PRODUCT_TYPE_ID << 8)

#define APP_DEVICE_ID_TYPE              DEVICE_ID_TYPE_PSEUDO_RANDOM
#define APP_DEVICE_ID_FORMAT            DEVICE_ID_FORMAT_BIN

/****************************************************************************
 *
 * Defines used to initialize the Association Group Information (AGI)
 * Command Class.
 *
 ****************************************************************************/
#define NUMBER_OF_INDIVIDUAL_ENDPOINTS  2
#define NUMBER_OF_AGGREGATED_ENDPOINTS  0
#define NUMBER_OF_ENDPOINTS             NUMBER_OF_INDIVIDUAL_ENDPOINTS + NUMBER_OF_AGGREGATED_ENDPOINTS

#define MAX_ASSOCIATION_GROUPS                  1
#define MAX_ASSOCIATION_IN_GROUP                5

#define AGITABLE_LIFELINE_GROUP \
{COMMAND_CLASS_SWITCH_MULTILEVEL, SWITCH_MULTILEVEL_REPORT},\    
{COMMAND_CLASS_DEVICE_RESET_LOCALLY, DEVICE_RESET_LOCALLY_NOTIFICATION}

#define AGITABLE_LIFELINE_GROUP_EP1_2_N \
{COMMAND_CLASS_SWITCH_MULTILEVEL, SWITCH_MULTILEVEL_REPORT}

#define AGITABLE_ROOTDEVICE_GROUPS              NULL

#define MULTILEVEL_SWITCH_VERSION               1
#define DIMMING_DURATION_CONFIG_DEFAULT         4

#define ERROR_NVR_FLASH_SEC                     10

#define ENDPOINT_ICONS \
    {ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE, ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE},\
    {ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE, ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE}

/**
 * PowerDownTimeout determines the number of seconds before powerdown.
 */
#define KEY_SCAN_POWERDOWNTIMEOUT               3  /* 300 mSec */
#define MSEC_200_POWERDOWNTIMEOUT               2  /* 200 mSec */
#define SEC_2_POWERDOWNTIMEOUT                  20
#define SEC_10_POWERDOWNTIMEOUT                 100
#define SEC_SCHEMENONCE_POWERDOWNTIMEOUT        100 /* 10 sec */
#define LEARNMODE_POWERDOWNTIMEOUT              255

#endif /* _CONFIG_APP_H_ */

