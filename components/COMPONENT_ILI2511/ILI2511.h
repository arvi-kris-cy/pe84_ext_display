/*******************************************************************************
* File Name        : ILI2511.h
*
* Description      : This file provides constants, parameter values, and  
*                    API prototypes for I2C based ILI2511 CTP driver.
*
* Related Document : See README.md
*
********************************************************************************
* Copyright 2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/
#ifndef ILI2511_H
#define ILI2511_H

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_result.h"
#include <stdbool.h>


/*******************************************************************************
* Macros
*******************************************************************************/
#define CY_RSLT_MODULE_TOUCH_PANEL     (0x0253)

/*! @brief Error code definition. */
/* No touch happened */
#define CY_TOUCH_PANEL_NO_TOUCH                  \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_TOUCH_PANEL, 1)

extern volatile bool touch_event;
/*******************************************************************************
* Function Prototypes
*******************************************************************************/
cy_rslt_t ili2511_init(void);
cy_rslt_t ili2511_get_single_touch(int *touch_x, int *touch_y);
void ili2511_deinit(void);

#if defined(__cplusplus)
}
#endif

#endif /* ILI2511_H */

/* [] END OF FILE */
