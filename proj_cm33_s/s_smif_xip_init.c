/*******************************************************************************
* File Name: s_smif_xip_init.c
* version 1.0
* Description:
* this file provide function to configure SMIF and Pinmux required for XIP
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
#include "s_smif_xip_init.h"

static cy_stc_smif_context_t SMIFContext;
static void Cy_SMIFCfgPins(void);
static cy_en_smif_status_t IsMemoryReady(cy_stc_smif_mem_config_t const *memConfig);

static const cy_stc_gpio_pin_config_t ioss_0_port_1_pin_0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = ioss_0_port_1_pin_0_HSIOM,
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

static const cy_stc_gpio_pin_config_t ioss_0_port_1_pin_1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = ioss_0_port_1_pin_1_HSIOM,
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

static const cy_stc_gpio_pin_config_t ioss_0_port_1_pin_2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = ioss_0_port_1_pin_2_HSIOM,
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

static const cy_stc_gpio_pin_config_t ioss_0_port_1_pin_3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = ioss_0_port_1_pin_3_HSIOM,
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

static const cy_stc_gpio_pin_config_t ioss_0_port_2_pin_0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP_IN_OFF,
    .hsiom = ioss_0_port_2_pin_0_HSIOM,
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
    .nonSec = 0,
};

static const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP_IN_OFF,
    .hsiom = ioss_0_port_5_pin_0_HSIOM,
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
    .nonSec = 0,
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_readCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_writeEnCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_writeDisCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_eraseCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_chipEraseCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_programCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_readStsRegQeCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_readStsRegWipCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_writeStsRegQeCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x00U,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 0U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /* The Data rate of data */
    .dataRate = CY_SMIF_SDR,
    /* This specifies the presence of the dummy field */
    .dummyCyclesPresence = CY_SMIF_NOT_PRESENT,
    /* This specifies the presence of the mode field */
    .modePresence = CY_SMIF_NOT_PRESENT,
    /* The high byte of a 16-bit mode. This value is 0x0 when there is no higher byte mode present */
    .modeH = 0x00,
    /* The Data rate of mode */
    .modeRate = CY_SMIF_SDR,
    /* The Data rate of address */
    .addrRate = CY_SMIF_SDR,
    /* This specifies the width of the command field */
    .cmdPresence = CY_SMIF_PRESENT_1BYTE,
    /* The high byte of a 16-bit command. This value is 0x0 when there is no higher byte command present */
    .commandH = 0x00,
    /* The Data rate of command */
    .cmdRate = CY_SMIF_SDR
#endif /* CY_IP_MXSMIF_VERSION */
};

static cy_stc_smif_mem_cmd_t SFDP_SlaveSlot_2_readSfdpCmd =
{
    /* The 8-bit command. 1 x I/O read command. */
    .command = 0x5AU,
    /* The width of the command transfer. */
    .cmdWidth = CY_SMIF_WIDTH_SINGLE,
    /* The width of the address transfer. */
    .addrWidth = CY_SMIF_WIDTH_SINGLE,
    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present. */
    .mode = 0xFFFFFFFFU,
    /* The width of the mode command transfer. */
    .modeWidth = CY_SMIF_WIDTH_SINGLE,
    /* The number of dummy cycles. A zero value suggests no dummy cycles. */
    .dummyCycles = 8U,
    /* The width of the data transfer. */
    .dataWidth = CY_SMIF_WIDTH_SINGLE
};

#if (CY_SMIF_DRV_VERSION_MAJOR > 1) || (CY_SMIF_DRV_VERSION_MINOR >= 50)
static cy_stc_smif_hybrid_region_info_t SFDP_SlaveSlot_2_regionInfoStorage[16];

static cy_stc_smif_hybrid_region_info_t *SFDP_SlaveSlot_2_regionInfo[16] = {
    &SFDP_SlaveSlot_2_regionInfoStorage[0],
    &SFDP_SlaveSlot_2_regionInfoStorage[1],
    &SFDP_SlaveSlot_2_regionInfoStorage[2],
    &SFDP_SlaveSlot_2_regionInfoStorage[3],
    &SFDP_SlaveSlot_2_regionInfoStorage[4],
    &SFDP_SlaveSlot_2_regionInfoStorage[5],
    &SFDP_SlaveSlot_2_regionInfoStorage[6],
    &SFDP_SlaveSlot_2_regionInfoStorage[7],
    &SFDP_SlaveSlot_2_regionInfoStorage[8],
    &SFDP_SlaveSlot_2_regionInfoStorage[9],
    &SFDP_SlaveSlot_2_regionInfoStorage[10],
    &SFDP_SlaveSlot_2_regionInfoStorage[11],
    &SFDP_SlaveSlot_2_regionInfoStorage[12],
    &SFDP_SlaveSlot_2_regionInfoStorage[13],
    &SFDP_SlaveSlot_2_regionInfoStorage[14],
    &SFDP_SlaveSlot_2_regionInfoStorage[15]
};
#endif

static cy_stc_smif_mem_device_cfg_t deviceCfg_SFDP_SlaveSlot_2 =
{
    /* Specifies the number of address bytes used by the memory slave device. */
    .numOfAddrBytes = 0x03U,
    /* The size of the memory. */
    .memSize = 0x0000100U,
    /* Specifies the Read command. */
    .readCmd = &SFDP_SlaveSlot_2_readCmd,
    /* Specifies the Write Enable command. */
    .writeEnCmd = &SFDP_SlaveSlot_2_writeEnCmd,
    /* Specifies the Write Disable command. */
    .writeDisCmd = &SFDP_SlaveSlot_2_writeDisCmd,
    /* Specifies the Erase command. */
    .eraseCmd = &SFDP_SlaveSlot_2_eraseCmd,
    /* Specifies the sector size of each erase. */
    .eraseSize = 0x0001000U,
    /* Specifies the Chip Erase command. */
    .chipEraseCmd = &SFDP_SlaveSlot_2_chipEraseCmd,
    /* Specifies the Program command. */
    .programCmd = &SFDP_SlaveSlot_2_programCmd,
    /* Specifies the page size for programming. */
    .programSize = 0x0000100U,
    /* Specifies the command to read the QE-containing status register. */
    .readStsRegQeCmd = &SFDP_SlaveSlot_2_readStsRegQeCmd,
    /* Specifies the command to read the WIP-containing status register. */
    .readStsRegWipCmd = &SFDP_SlaveSlot_2_readStsRegWipCmd,
    /* Specifies the read SFDP command */
    .readSfdpCmd = &SFDP_SlaveSlot_2_readSfdpCmd,
    /* Specifies the command to write into the QE-containing status register. */
    .writeStsRegQeCmd = &SFDP_SlaveSlot_2_writeStsRegQeCmd,
    /* The mask for the status register. */
    .stsRegBusyMask = 0x00U,
    /* The mask for the status register. */
    .stsRegQuadEnableMask = 0x00U,
    /* The max time for the erase type-1 cycle-time in ms. */
    .eraseTime = 1U,
    /* The max time for the chip-erase cycle-time in ms. */
    .chipEraseTime = 16U,
    /* The max time for the page-program cycle-time in us. */
    .programTime = 8U,
#if (CY_SMIF_DRV_VERSION_MAJOR > 1) || (CY_SMIF_DRV_VERSION_MINOR >= 50)
    /* Points to NULL or to structure with info about sectors for hybrid memory. */
    .hybridRegionCount = 0U,
    .hybridRegionInfo = SFDP_SlaveSlot_2_regionInfo
#endif
};

static const cy_stc_smif_mem_config_t SFDP_SlaveSlot_2 =
{
    /* Determines the slot number where the memory device is placed. */
    .slaveSelect = CY_SMIF_SLAVE_SELECT_1,
    /* Flags. */
#if (CY_IP_MXSMIF_VERSION >= 2)
    .flags = CY_SMIF_FLAG_SMIF_REV_3 | CY_SMIF_FLAG_MEMORY_MAPPED | CY_SMIF_FLAG_WR_EN | CY_SMIF_FLAG_DETECT_SFDP | CY_SMIF_FLAG_MERGE_ENABLE,
#else
    .flags = CY_SMIF_FLAG_MEMORY_MAPPED | CY_SMIF_FLAG_WR_EN | CY_SMIF_FLAG_DETECT_SFDP,
#endif /* CY_IP_MXSMIF_VERSION */
    /* The data-line selection options for a slave device. */
    .dataSelect = CY_SMIF_DATA_SEL0,
    /* The base address the memory slave is mapped to in the memory map.
    Valid when the memory-mapped mode is enabled. */
    .baseAddress = 0x08000000U,
    /* The size allocated in the PSoC memory map, for the memory slave device.
    The size is allocated from the base address. Valid when the memory mapped mode is enabled. */
    .memMappedSize = 0x04000000U,
    /* If this memory device is one of the devices in the dual quad SPI configuration.
    Valid when the memory mapped mode is enabled. */
    .dualQuadSlots = 0,
    /* The configuration of the device. */
    .deviceCfg = &deviceCfg_SFDP_SlaveSlot_2,
#if (CY_IP_MXSMIF_VERSION >= 2)
    /** Continous transfer merge timeout.
     * After this period the memory device is deselected. A later transfer, even from a
     * continuous address, starts with the overhead phases (command, address, mode, dummy cycles).
     * This configuration parameter is available for CAT1B devices. */
    .mergeTimeout = CY_SMIF_MERGE_TIMEOUT_256_CYCLES
#endif /* CY_IP_MXSMIF_VERSION */
};

static const cy_stc_smif_mem_config_t* const smifMemConfigs[CY_SMIF_DEVICE_NUM] = {
   &SFDP_SlaveSlot_2
};

static const cy_stc_smif_block_config_t smifBlockConfig =
{
    /* The number of SMIF memories defined. */
    .memCount = CY_SMIF_DEVICE_NUM,
    /* The pointer to the array of memory config structures of size memCount. */
    .memConfig = (cy_stc_smif_mem_config_t**)smifMemConfigs,
    /* The version of the SMIF driver. */
    .majorVersion = CY_SMIF_DRV_VERSION_MAJOR,
    /* The version of the SMIF driver. */
    .minorVersion = CY_SMIF_DRV_VERSION_MINOR
};

static const cy_stc_smif_config_t SMIF_0_CORE_0_config =
{
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = SMIF_0_CORE_0_DESELECT_DELAY,
    .rxClockSel = (uint32_t)CY_SMIF_SEL_INVERTED_FEEDBACK_CLK,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};

/*******************************************************************************
* Function Name: Cy_SMIFCfgPins
****************************************************************************//**
* \cond
*  configure SMIF pins
* \endcond
*******************************************************************************/
static void Cy_SMIFCfgPins(void)
{
     /* SMIF Data signal */
    (void)Cy_GPIO_Pin_Init(ioss_0_port_1_pin_0_PORT, ioss_0_port_1_pin_0_PIN, &ioss_0_port_1_pin_0_config);
    /* SMIF Data signal */
    (void)Cy_GPIO_Pin_Init(ioss_0_port_1_pin_1_PORT, ioss_0_port_1_pin_1_PIN, &ioss_0_port_1_pin_1_config);
    /* SMIF Data signal */
    (void)Cy_GPIO_Pin_Init(ioss_0_port_1_pin_2_PORT, ioss_0_port_1_pin_2_PIN, &ioss_0_port_1_pin_2_config);
    /* SMIF Data signal */
    (void)Cy_GPIO_Pin_Init(ioss_0_port_1_pin_3_PORT, ioss_0_port_1_pin_3_PIN, &ioss_0_port_1_pin_3_config);
    /* SMIF Clock signal */
    (void)Cy_GPIO_Pin_Init(ioss_0_port_2_pin_0_PORT, ioss_0_port_2_pin_0_PIN, &ioss_0_port_2_pin_0_config);
    /* SMIF Slave Select 0 signal*/
    (void)Cy_GPIO_Pin_Init(ioss_0_port_5_pin_0_PORT, ioss_0_port_5_pin_0_PIN, &ioss_0_port_5_pin_0_config);
}

/*******************************************************************************
* Function Name: IsMemoryReady
****************************************************************************//**
*
* Polls the memory device to check whether it is ready to accept new commands or
* not until either it is ready or the retries have exceeded the limit.
*
* \param memConfig
* memory device configuration
*
* \return Status of the operation.
* CY_SMIF_SUCCESS          - Memory is ready to accept new commands.
* CY_SMIF_EXCEED_TIMEOUT - Memory is busy.
*
*******************************************************************************/
static cy_en_smif_status_t IsMemoryReady(cy_stc_smif_mem_config_t const *memConfig)
{
    uint32_t retries = 0;
    bool isBusy;

    do
    {
        isBusy = Cy_SMIF_Memslot_IsBusy(SMIF_HW, (cy_stc_smif_mem_config_t* )memConfig, &SMIFContext);
        Cy_SysLib_Delay(5);
        retries++;
    }while(isBusy && (retries < MEMORY_BUSY_CHECK_RETRIES));

    return (isBusy ? CY_SMIF_EXCEED_TIMEOUT : CY_SMIF_SUCCESS);
}

/*******************************************************************************
* Function Name: IsQuadEnabled
****************************************************************************//**
*
* Checks whether QE (Quad Enable) bit is set or not in the configuration
* register of the memory.
*
* \param memConfig
* Memory device configuration
*
* \param isQuadEnabled
* This parameter is updated to indicate whether Quad mode is enabled (true) or
* not (false). The value is valid only when the function returns
* CY_SMIF_SUCCESS.
*
* \return Status of the operation. See cy_en_smif_status_t.
*
*******************************************************************************/
static cy_en_smif_status_t IsQuadEnabled(cy_stc_smif_mem_config_t const *memConfig, bool *isQuadEnabled)
{
    cy_en_smif_status_t status;
    uint8_t readStatus = 0;
    uint8_t statusCmd = (uint8_t)memConfig->deviceCfg->readStsRegQeCmd->command;
    uint8_t maskQE = (uint8_t) memConfig->deviceCfg->stsRegQuadEnableMask;

    status = Cy_SMIF_Memslot_CmdReadSts(SMIF_HW, memConfig, &readStatus, statusCmd, &SMIFContext);

    *isQuadEnabled = false;
    if(CY_SMIF_SUCCESS == status)
    {
        /* Check whether Quad mode is already enabled or not */
        *isQuadEnabled = (maskQE == (readStatus & maskQE));
    }

    return status;
}

/*******************************************************************************
* Function Name: EnableQuadMode
****************************************************************************//**
*
* This function sets the QE (QUAD Enable) bit in the external memory
* configuration register to enable Quad SPI mode.
*
* \param memConfig
* Memory device configuration
*
* \return Status of the operation. See cy_en_smif_status_t.
*
*******************************************************************************/
static cy_en_smif_status_t EnableQuadMode(cy_stc_smif_mem_config_t const *memConfig)
{
    cy_en_smif_status_t status;

    /* Send Write Enable to external memory */
    status = Cy_SMIF_Memslot_CmdWriteEnable(SMIF_HW, smifMemConfigs[0], &SMIFContext);

    if(CY_SMIF_SUCCESS == status)
    {
        status = Cy_SMIF_Memslot_QuadEnable(SMIF_HW, (cy_stc_smif_mem_config_t* )memConfig, &SMIFContext);

        if(CY_SMIF_SUCCESS == status)
        {
            /* Poll memory for the completion of operation */
            status = IsMemoryReady(memConfig);
        }
    }

    return status;
}

/*******************************************************************************
* Function Name: Cy_SMIFXIPInit
****************************************************************************//**
* \cond
* Initializes SMIF in XIP mode
* \endcond
*******************************************************************************/
void Cy_SMIFXIPInit(void)
{
  cy_en_smif_status_t SMIF_Status = CY_SMIF_BAD_PARAM;
  bool QE_status = false;

  Cy_SMIFCfgPins();
  SMIF_Status = Cy_SMIF_Init(SMIF0_CORE0, &SMIF_0_CORE_0_config, TIMEOUT_1_MS, &SMIFContext);

  if(SMIF_Status == CY_SMIF_SUCCESS)
  {
    Cy_SMIF_SetDataSelect(SMIF_HW, smifMemConfigs[0]->slaveSelect, smifMemConfigs[0]->dataSelect);
    Cy_SMIF_Enable(SMIF0_CORE0, &SMIFContext);
    SMIF_Status = Cy_SMIF_Memslot_Init(SMIF_HW, (cy_stc_smif_block_config_t *) &smifBlockConfig, &SMIFContext);
    if ((smifMemConfigs[0]->deviceCfg->readStsRegQeCmd->command == 0U) || /* 0 - if configurator generated code */
        (smifMemConfigs[0]->deviceCfg->readStsRegQeCmd->command == CY_SMIF_NO_COMMAND_OR_MODE)) /* 0xFF's if SFDP enumerated */
    {
      SMIF_Status = Cy_SMIF_MemInitSfdpMode(SMIF_HW,
          smifMemConfigs[0],
          CY_SMIF_WIDTH_QUAD,
          CY_SMIF_SFDP_QER_1,
          &SMIFContext);
    }
    if (SMIF_Status == CY_SMIF_SUCCESS)
    {
      /* Check if Quad mode is already enabled */
      SMIF_Status = IsQuadEnabled(smifMemConfigs[0], &QE_status);
      /* If not enabled, enable quad mode */
      if ((SMIF_Status == CY_SMIF_SUCCESS) && (!QE_status))
      {
        /* Enable Quad mode */
        SMIF_Status = EnableQuadMode(smifMemConfigs[0]);
      }
    }
    if(SMIF_Status == CY_SMIF_SUCCESS)
    {
      Cy_SMIF_SetMode(SMIF_HW, CY_SMIF_MEMORY);
    }

  }
}
