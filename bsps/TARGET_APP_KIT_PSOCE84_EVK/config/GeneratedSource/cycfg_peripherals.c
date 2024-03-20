/*******************************************************************************
* File Name: cycfg_peripherals.c
*
* Description:
* Peripheral Groups configuration
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

#include "cycfg_peripherals.h"

cy_stc_gfx_layer_config_t GFXSS_graphics_layer = 
{
    .layer_type = GFX_LAYER_GRAPHICS,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 800,
    .height = 600,
    .zorder = 0,
    .layer_enable = true,
};
cy_stc_gfx_layer_config_t GFXSS_overlay0_layer = 
{
    .layer_type = GFX_LAYER_OVERLAY0,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 1024,
    .height = 600,
    .zorder = 0,
    .layer_enable = false,
};
cy_stc_gfx_layer_config_t GFXSS_overlay1_layer = 
{
    .layer_type = GFX_LAYER_OVERLAY1,
    .buffer_address = (gctADDRESS *)CY_SOCMEM_RAM_BASE,
    .input_format_type = vivRGB565,
    .tiling_type = vivLINEAR,
    .pos_x = 0,
    .pos_y = 0,
    .width = 1024,
    .height = 600,
    .zorder = 0,
    .layer_enable = false,
};
cy_stc_gfx_dc_config_t GFXSS_dc_config = 
{
    .gfx_layer_config = &GFXSS_graphics_layer,
    .ovl0_layer_config = &GFXSS_overlay0_layer,
    .ovl1_layer_config = &GFXSS_overlay1_layer,
    .display_type = GFX_DISP_TYPE_DSI_DPI,
    .display_format = vivD24,
    .display_size = vivDISPLAY_CUSTOMIZED,
};
cy_stc_mipidsi_display_params_t GFXSS_mipidsi_display_params = 
{
    .pixel_clock = 40091,
    .hdisplay = 800,
    .hsync_width = 128,
    .hfp = 48,
    .hbp = 88,
    .vdisplay = 600,
    .vsync_width = 4,
    .vfp = 1,
    .vbp = 23,
    .polarity_flags = 0,
};
cy_stc_mipidsi_config_t GFXSS_mipi_dsi_config = 
{
    .virtual_ch = 0,
    .num_of_lanes = 2,
    .per_lane_mbps = 481,
    .dpi_fmt = CY_MIPIDSI_FMT_RGB888,
    .dsi_mode = DSI_VIDEO_MODE,
    .max_phy_clk = 2500000000,
    .mode_flags = VID_MODE_TYPE_NON_BURST_SYNC_PULSES,
    .display_params = &GFXSS_mipidsi_display_params,
};
cy_stc_gfx_config_t GFXSS_config = 
{
    .dc_cfg = &GFXSS_dc_config,
    .mipi_dsi_cfg = &GFXSS_mipi_dsi_config,
    .display_update_type = GFX_DOUBLE_BUFFER,
};


void init_cycfg_peripherals(void)
{
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_GPU_PERI_NR, CY_MMIO_GFXSS_GPU_GROUP_NR, CY_MMIO_GFXSS_GPU_SLAVE_NR, CY_MMIO_GFXSS_GPU_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_DC_PERI_NR, CY_MMIO_GFXSS_DC_GROUP_NR, CY_MMIO_GFXSS_DC_SLAVE_NR, CY_MMIO_GFXSS_DC_CLK_HF_NR);
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_GFXSS_MIPIDSI_PERI_NR, CY_MMIO_GFXSS_MIPIDSI_GROUP_NR, CY_MMIO_GFXSS_MIPIDSI_SLAVE_NR, CY_MMIO_GFXSS_MIPIDSI_CLK_HF_NR);
    Cy_SysClk_PeriGroupSetDivider((0 << 8) | 2, 1U);
    Cy_SysClk_PeriGroupSetDivider((1 << 8) | 1, 1U);
    Cy_SysClk_PeriGroupSetDivider((1 << 8) | 3, 3U);
}
