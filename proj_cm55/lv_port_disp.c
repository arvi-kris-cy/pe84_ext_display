/*******************************************************************************
* File Name        : lv_port_disp.c
*
* Description      : This file provides implementation of low level display
*                    device driver for LVGL.
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
#include "lv_port_disp.h"
#include "lvgl_support.h"
#include "lv_draw_sw.h"
#include <stdbool.h>
#include <string.h>
#include "cyhal.h"
#include "cy_graphics.h"


/*******************************************************************************
* Macros
*******************************************************************************/
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual
screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual
screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

#define FRAME_COMPLT_TIME_MS                (22U)


/*******************************************************************************
* Global Variables
*******************************************************************************/
CY_SECTION(".cy_gpu_buf") lv_color_t disp_buf1[MY_DISP_HOR_RES *
                                               MY_DISP_VER_RES];
CY_SECTION(".cy_gpu_buf") lv_color_t disp_buf2[MY_DISP_HOR_RES *
                                               MY_DISP_VER_RES];

extern cy_stc_gfx_context_t gfx_context;


/*******************************************************************************
* Function Name: disp_flush
********************************************************************************
* Summary:
*  Flush the content of the internal buffer the specific area on the display.
*  You can use DMA or any hardware acceleration to do this operation in the
*  background but 'lv_disp_flush_ready()' has to be called when finished.
*
* Parameters:
*  *disp_drv: Pointer to the display driver structure to be registered by HAL.
*  *area: Pointer to the area of the screen (not used).
*  *color_p: Pointer to the frame buffer address.
*
* Return:
*  void
*
*******************************************************************************/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area,
        lv_color_t *color_p)
{
    CY_UNUSED_PARAMETER(area);

    Cy_GFXSS_Set_FrameBuffer((GFXSS_Type*) GFXSS, (uint32_t*) color_p,
            &gfx_context);

    fb_pending = true;

    while(fb_pending);

    /* Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(disp_drv);
}

/*******************************************************************************
* Function Name: lv_port_indev_init
********************************************************************************
* Summary:
*  Initialization function for display devices supported by LittelvGL.
*   LVGL requires a buffer where it internally draws the widgets.
*   Later this buffer will passed to your display driver's `flush_cb` to copy
*   its content to your display.
*   The buffer has to be greater than 1 display row
*
*   There are 3 buffering configurations:
*   1. Create ONE buffer:
*      LVGL will draw the display's content here and writes it to your display
*
*   2. Create TWO buffer:
*      LVGL will draw the display's content to a buffer and writes it your
*      display.
*      You should use DMA to write the buffer's content to the display.
*      It will enable LVGL to draw the next part of the screen to the other
*      buffer while the data is being sent form the first buffer.
*      It makes rendering and flushing parallel.
*
*   3. Double buffering
*      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
*      This way LVGL will always provide the whole rendered screen in `flush_cb`
*      and you only need to change the frame buffer's address.
*
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void lv_port_disp_init(void)
{
    static lv_disp_draw_buf_t buf;
    static lv_disp_drv_t disp_drv;

    memset(disp_buf1, 0, sizeof(disp_buf1));
    memset(disp_buf2, 0, sizeof(disp_buf2));

    lv_disp_draw_buf_init(&buf, disp_buf1, disp_buf2, (MY_DISP_HOR_RES *
            MY_DISP_VER_RES*2));

    lv_disp_drv_init(&disp_drv);

    disp_drv.draw_buf = &buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.full_refresh = 1;

#ifdef USE_GPU
    disp_drv.draw_ctx_init = cy_draw_ctx_init;
    disp_drv.draw_ctx_size = sizeof(lv_draw_sw_ctx_t);
#endif

    lv_disp_drv_register(&disp_drv);
}

/* [] END OF FILE */
