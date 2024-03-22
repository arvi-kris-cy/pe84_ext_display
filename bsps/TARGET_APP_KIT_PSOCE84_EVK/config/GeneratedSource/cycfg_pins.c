/*******************************************************************************
* File Name: cycfg_pins.c
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* Configurator Backend 3.10.0
* device-db 4.5.0.5210
* mtb-pdl-cat1 3.5.4.31266
*
********************************************************************************
* Copyright 2024 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t CYBSP_DISP_TP_RST_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DISP_TP_RST_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t CYBSP_DISP_TP_RST_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_DISP_TP_RST_PORT_NUM,
        .channel_num = CYBSP_DISP_TP_RST_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_WCO_OUT_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t CYBSP_WCO_OUT_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_WCO_OUT_PORT_NUM,
        .channel_num = CYBSP_WCO_OUT_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_WCO_IN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t CYBSP_WCO_IN_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_WCO_IN_PORT_NUM,
        .channel_num = CYBSP_WCO_IN_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t ioss_0_port_19_pin_0_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ioss_0_port_19_pin_0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t ioss_0_port_19_pin_0_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_19_pin_0_PORT_NUM,
        .channel_num = ioss_0_port_19_pin_0_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t ioss_0_port_19_pin_1_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ioss_0_port_19_pin_1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t ioss_0_port_19_pin_1_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = ioss_0_port_19_pin_1_PORT_NUM,
        .channel_num = ioss_0_port_19_pin_1_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t CYBSP_DISP_BACKLIGHT_PWM_config = 
{
    .outVal = 0,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DISP_BACKLIGHT_PWM_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t CYBSP_DISP_BACKLIGHT_PWM_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_DISP_BACKLIGHT_PWM_PORT_NUM,
        .channel_num = CYBSP_DISP_BACKLIGHT_PWM_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t CYBSP_DISP_RST_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_DISP_RST_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t CYBSP_DISP_RST_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_DISP_RST_PORT_NUM,
        .channel_num = CYBSP_DISP_RST_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
const cy_stc_gpio_pin_config_t CYBSP_EXT_CLK_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = CYBSP_EXT_CLK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
    .pullUpRes = CY_GPIO_PULLUP_RES_DISABLE,
    .nonSec = 1,
};
#if defined (CY_USING_HAL) || (CY_USING_HAL_LITE)
    const cyhal_resource_inst_t CYBSP_EXT_CLK_obj = 
    {
        .type = CYHAL_RSC_GPIO,
        .block_num = CYBSP_EXT_CLK_PORT_NUM,
        .channel_num = CYBSP_EXT_CLK_PIN,
    };
#endif //defined (CY_USING_HAL) || (CY_USING_HAL_LITE)


void init_cycfg_pins(void)
{
    Cy_GPIO_Pin_Init(CYBSP_DISP_TP_RST_PORT, CYBSP_DISP_TP_RST_PIN, &CYBSP_DISP_TP_RST_config);
    Cy_GPIO_Pin_Init(ioss_0_port_19_pin_0_PORT, ioss_0_port_19_pin_0_PIN, &ioss_0_port_19_pin_0_config);
    Cy_GPIO_Pin_Init(ioss_0_port_19_pin_1_PORT, ioss_0_port_19_pin_1_PIN, &ioss_0_port_19_pin_1_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_BACKLIGHT_PWM_PORT, CYBSP_DISP_BACKLIGHT_PWM_PIN, &CYBSP_DISP_BACKLIGHT_PWM_config);
    Cy_GPIO_Pin_Init(CYBSP_DISP_RST_PORT, CYBSP_DISP_RST_PIN, &CYBSP_DISP_RST_config);
}

void reserve_cycfg_pins(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_DISP_TP_RST_obj);
    cyhal_hwmgr_reserve(&CYBSP_WCO_OUT_obj);
    cyhal_hwmgr_reserve(&CYBSP_WCO_IN_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_19_pin_0_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_19_pin_1_obj);
    cyhal_hwmgr_reserve(&CYBSP_DISP_BACKLIGHT_PWM_obj);
    cyhal_hwmgr_reserve(&CYBSP_DISP_RST_obj);
    cyhal_hwmgr_reserve(&CYBSP_EXT_CLK_obj);
#endif //defined (CY_USING_HAL)
}
