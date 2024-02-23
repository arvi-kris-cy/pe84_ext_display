/*******************************************************************************
* File Name: s_smif_xip_init.h
* version 1.0
* Description: smif xip init header file
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
********************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#ifndef S_SMIF_XIP_INIT_H
#define S_SMIF_XIP_INIT_H

#include "cy_gpio.h"
#include "cy_smif.h"
#include "cy_smif_memslot.h"

#define SMIF_0_CORE_0_DESELECT_DELAY 7
#define ioss_0_port_1_pin_0_PORT (GPIO_PRT_Type*)(void *)SMIF0_CORE0_SMIF_GPIO_SMIF_PRT0
#define ioss_0_port_1_pin_0_PIN 0U
#ifndef ioss_0_port_1_pin_0_HSIOM
    #define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_ACT_15
#endif

#define ioss_0_port_1_pin_1_PORT (GPIO_PRT_Type*)(void *)SMIF0_CORE0_SMIF_GPIO_SMIF_PRT0
#define ioss_0_port_1_pin_1_PIN 1U
#ifndef ioss_0_port_1_pin_1_HSIOM
    #define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_ACT_15
#endif

#define ioss_0_port_1_pin_2_PORT (GPIO_PRT_Type*)(void *)SMIF0_CORE0_SMIF_GPIO_SMIF_PRT0
#define ioss_0_port_1_pin_2_PIN 2U
#ifndef ioss_0_port_1_pin_2_HSIOM
    #define ioss_0_port_1_pin_2_HSIOM HSIOM_SEL_ACT_15
#endif

#define ioss_0_port_1_pin_3_PORT (GPIO_PRT_Type*)(void *)SMIF0_CORE0_SMIF_GPIO_SMIF_PRT0
#define ioss_0_port_1_pin_3_PIN 3U
#ifndef ioss_0_port_1_pin_3_HSIOM
    #define ioss_0_port_1_pin_3_HSIOM HSIOM_SEL_ACT_15
#endif


#define ioss_0_port_2_pin_0_PORT GPIO_PRT2
#define ioss_0_port_2_pin_0_PIN 0U
#define ioss_0_port_2_pin_0_HSIOM P2_0_SMIF_SMIF0_SPIHB_SELECT1

#define ioss_0_port_3_pin_1_PORT GPIO_PRT3
#define ioss_0_port_3_pin_1_PIN 1U
#ifndef ioss_0_port_3_pin_1_HSIOM
    #define ioss_0_port_3_pin_1_HSIOM HSIOM_SEL_GPIO
#endif

#define ioss_0_port_5_pin_0_PORT GPIO_PRT5
#define ioss_0_port_5_pin_0_PIN 0U
#ifndef ioss_0_port_5_pin_0_HSIOM
    #define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_ACT_0
#endif

#define TIMEOUT_1_MS                    (1000UL)  /* 1 ms timeout for all blocking functions */
#define MEMORY_BUSY_CHECK_RETRIES (750UL)
/* Supported QSPI Driver version */
#define CY_SMIF_DRV_VERSION_REQUIRED       (100)

#if !defined(CY_SMIF_DRV_VERSION)
    #define CY_SMIF_DRV_VERSION            (100)
#endif
#define CY_SMIF_DEVICE_NUM 1

void Cy_SMIFXIPInit(void);

#endif
