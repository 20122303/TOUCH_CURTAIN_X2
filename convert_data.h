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
 * Last Changed By:  Author: trungtq
 * Revision:         Revision: 2.2
 * Last Changed:     Date: 2016-07-09 16:20:00 (Sat, 09 Jun 2016)
 *                   1. Change [pByDefaultDevice] to [pByTypeDevice]
 *                   Date: 2016-06-23 14:10:00 (Wen, 23 Jun 2016)
 *
 ******************************************************************************/
#ifndef _CONVERT_DATA_H_
#define _CONVERT_DATA_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ZW_typedefs.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

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
 * @func   CorrectValue   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
CorrectValue(
    BYTE byValue,
    BYTE byTpDev
);

/**
 * @func   ConvertValueZwToMcu   
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
ConvertValueZwToMcu(
    BYTE byValue,
    BYTE byTpDev
);

/**
 * @func   ConvertValueMcuToZw    
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
ConvertValueMcuToZw(
    BYTE byValue,
    BYTE byTpDev
);

/**
 * @func   GetLengthCmd    
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
GetLengthCmd(
    BYTE byTypeDev
);


#endif /* _CONVERT_DATA_H_ */
