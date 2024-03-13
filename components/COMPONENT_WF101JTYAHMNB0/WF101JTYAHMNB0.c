/*******************************************************************************
* File Name        : WF101JTYAHMNB0.c
*
* Description      : This source file provides implementation for 
*                    WF101JTYAHMNB0 (10.1" LCD) DSI driver.
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

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "WF101JTYAHMNB0.h"
#include "cycfg_pins.h"
#include "cyhal.h"


/*******************************************************************************
* Macros
*******************************************************************************/
#define PACKET_LENGTH                       (2U)
/* Display controller command commit mask */
#define DISPLAY_MASK                        (0x00000001U)


/*******************************************************************************
* Global Variables
*******************************************************************************/
static const uint8_t lcm_init_setting[][2] = {
    /* 2 data lanes */
    {PANEL_CTRL_REG, EN_2LANE_MASK},
};


/*******************************************************************************
* Function Name: wf101jtyahmnb0_init
********************************************************************************
* Summary:
*  This function performs the necessary initialization of WF101JTYAHMNB0 
*  (10.1") LCD.
*
* Parameters:
*  mipi_dsi_base
*
* Return:
*  cy_en_gfx_status_t
*
*******************************************************************************/
cy_en_gfx_status_t wf101jtyahmnb0_init(GFXSS_MIPIDSI_Type* mipi_dsi_base)
{
    uint32_t i                = 0;
    cy_en_gfx_status_t status = CY_GFX_SUCCESS;


    /* Perform reset. */
    cyhal_gpio_toggle(CYBSP_DISP_RST); /* toggle the CYBSP_DISP_RST pin to low */
    cyhal_system_delay_ms(PIN_LOW_DELAY_MS);
    cyhal_gpio_toggle(CYBSP_DISP_RST); /* toggle the CYBSP_DISP_RST pin to high */
    cyhal_system_delay_ms(PIN_HIGH_DELAY_MS);

    /* Set the LCM init settings */
    for (i = 0; i < CY_ARRAY_SIZE(lcm_init_setting); i++)
    {
        status = Cy_MIPIDSI_WritePacket(mipi_dsi_base, lcm_init_setting[i],
                                        PACKET_LENGTH);

        if (CY_GFX_SUCCESS != status)
        {
            return status;
        }
    }

    viv_set_commit(DISPLAY_MASK);

    return CY_GFX_SUCCESS;
}

/* [] END OF FILE */
