#ifndef CURTAIN_DEVICE_H
#define CURTAIN_DEVICE_H

#include <ZW_typedefs.h>
#include "lumi_cmd.h"

/**
 * @func   HandleCurtainLevel
 * @brief  None
 * @param  None
 * @retval None
 */
extern void
HandleCurtainLevel(
    BYTE byLevel,
    BYTE byEndpoint
);

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
);

/**
 * @func   GetCurtainLevel
 * @brief  None
 * @param  None
 * @retval None
 */
BYTE
GetCurtainLevel(
    BYTE byEndpoint
);

/**
 * @func   StopCurtain
 * @brief  None
 * @param  None
 * @retval None
 */
void
StopCurtain(
    BYTE byEndpoint
);


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
);

#endif /* CURTAIN_DEVICE_H */
