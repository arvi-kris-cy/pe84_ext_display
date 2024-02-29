/*******************************************************************************
* File Name        : WF101JTYAHMNB0.h
*
* Description      : This file provides constants, parameter values, and  
*                    API prototypes for WF101JTYAHMNB0 (10.1" LCD) DSI driver.
*
* Related Document : See README.md
*
********************************************************************************
* Copyright 2023-2024, Cypress Semiconductor Corporation (an Infineon company) or
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
#ifndef WF101JTYAHMNB0_H
#define WF101JTYAHMNB0_H

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_graphics.h"
#include "cy_pdl.h"


/*******************************************************************************
* Macros
*******************************************************************************/
/* Tessellation window = DISPLAY_W * DISPLAY_H */
#define DISPLAY_W                 (640)
#define DISPLAY_H                 (480)


#define PIN_LOW_DELAY_MS          (1U)
#define PIN_HIGH_DELAY_MS         (5U)

/* Panel registers */
#define PANEL_CTRL_REG            (0xB2)


#define EN_2LANE_BIT_POS          (4UL)                   /**< Enable/disable 2 lane MIPI interface bit position */
#define EN_2LANE_MASK             (1 << EN_2LANE_BIT_POS) /**< Enable/disable 2 lane MIPI interface bit mask */


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
cy_en_gfx_status_t wf101jtyahmnb0_init(GFXSS_MIPIDSI_Type* mipi_dsi_base);


#if defined(__cplusplus)
}
#endif

#endif /* WF101JTYAHMNB0_H */

/* [] END OF FILE */
