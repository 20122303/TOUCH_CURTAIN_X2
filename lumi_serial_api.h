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
 * Last Changed:     $Date: 2016-06-23 11:35:00 (Thur, 23 Jun 2016) $
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define DEVICE_NULL                         0x00
#define DEVICE_CONTACT                      0x01
#define DEVICE_SOCKET                       0x02
#define DEVICE_CURTAIN                      0x03
#define DEVICE_DIMMER                       0x04
#define DEVICE_FAN                          0x05
#define DEVICE_IR_CONTROL                   0x06
#define DEVICE_DETECT_MOTION                0x07
#define DEVICE_LIGHT_SENSOR                 0x08
#define DEVICE_TEMP_SENSOR                  0x09
#define DEVICE_HUMIDITY_SENSOR              0x0A

#define UNKNOW_LENGTH                       0
#define LENGTH_ONE_BYTE                     1
#define LENGTH_TWO_BYTE                     2
#define MAX_LENGTH_OF_VALUE_BUFFER          2


#define BUTTON_1                            1       /* Inclusion/Exclusion */
#define BUTTON_2                            2       /* Reset */
#define BUTTON_3                            3
#define BUTTON_4                            4
#define BUTTON_5                            5
#define BUTTON_6                            6
#define BUTTON_7                            7
#define BUTTON_8                            8
#define BUTTON_9                            9
#define BUTTON_10                           10

#define CMD_GET_INFO_DEVICE                 0x81
#define CMD_GET_STATE_ENDPOINT              0x82
#define CMD_SET_CTRL                        0x83
#define CMD_SET_LED_STATUS                  0x84    

#define CMD_REPORT_INFO_DEVICE              0x01
#define CMD_REPORT_STATE_ENDPOINT           0x02
#define CMD_REPORT_STATE_BUTTON             0x03

#define LENGTH_CMD_GET_INFO_DEVICE          2
#define LENGTH_CMD_GET_STATE_ENDPOINT       4
#define LENGTH_SINGLE_CMD_SET_CTRL          5
#define LENGTH_CMD_SET_LED_STATUS           7

#define REFRESH_LED                         0x00
#define BLUE_ON_RED_OFF                     0x01
#define RED_ON_BLUE_OFF                     0x02
#define RED_OFF_BLUE_OFF                    0x03
#define RED_ON_BLUE_ON                      0x04
#define BLUE_BLINK                          0x05
#define RED_BLINK                           0x06
#define ALL_BLINK                           0x07

#define STATUS_REFRESH_LED                  0x00
#define STATUS_RED_ON_BLUE_OFF              0x01
#define STATUS_BLUE_ON_RED_OFF              0x02
#define STATUS_BLUE_ON_RED_ON               0x03
#define STATUS_BLUE_OFF_RED_OFF             0x04

#define BUTTON_RELEASE                      0x00
#define BUTTON_HOLD_3SEC                    0x01
#define BUTTON_HOLD_5SEC                    0x02
#define BUTTON_HOLD_10SEC                   0x03
#define BUTTON_PRESSC_2TIME                 0x04
#define BUTTON_PRESSC_3TIME                 0x05
#define BUTTON_PRESSC_4TIME                 0x06
#define BUTTON_PRESSC_5TIME                 0x07
#define BUTTON_PRESSC_10TIME                0x08
#define BUTTON_HOLD_15SEC                   0x09

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


