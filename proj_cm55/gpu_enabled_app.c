/*******************************************************************************
 * File Name        : gpu_enabled_app.c
 *
 * Description      : The file contains functions that perform graphics display
 * related tasks like initializing graphics stack and vglite, drawing the image
 * via its vector data paths.
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
#include "extdisplay.h"
#include "gpu_enabled_app.h"
#include "infineon_logo_paths.h"

/*******************************************************************************
 * Macros
*******************************************************************************/
#define IMG_POSTN_X                            (150)
#define IMG_POSTN_Y                            (-100)


/*******************************************************************************
 * Global Variables
 *******************************************************************************/
static GFXSS_Type* base = (GFXSS_Type*) GFXSS;
static cy_stc_gfx_context_t gfx_context;

static vg_lite_buffer_t buffer0;
static vg_lite_buffer_t buffer1;

static vg_lite_buffer_t *render_target;
static vg_lite_matrix_t matrix;
static uint32_t register_mem_base = (uint32_t)GFXSS_GFXSS_GPU_GCNANO;
static uint32_t gpu_mem_base      = 0x0;

/* Heap memory for VGLite to allocate memory for buffers, command, and tessellation buffers */
CY_SECTION(".cy_gpu_buf") CY_ALIGN(64) uint8_t contiguous_mem[VGLITE_HEAP_SIZE];

static volatile void *vglite_heap_base = &contiguous_mem;
static int current_buffer              = RESET_VAL;

/*******************************************************************************
 * Function Name: cleanup
 ********************************************************************************
 * Summary:
 *  This function clears path member values and frees the GPU memory. Ultimately
 *  deallocates all resources and free up all the memory initialized by the
 *  vg_lite_init function.
 *
 * Parameters:
 *  none
 *
 * Return:
 *  none
 *
 *******************************************************************************/
void cleanup(void)
{
    uint8_t i;
    for (i = 0; i < PATH_COUNT; i++)
    {
        /* Clear and reset path member values */
        vg_lite_clear_path(&path[i]);
    }

    /* Deallocate all the resource and free up all the memory */
    vg_lite_close();
}


/*******************************************************************************
 * Function Name: draw
 ********************************************************************************
 * Summary:
 *  This function draws the image using GPU and refreshes the frame buffer.
 *
 * Parameters:
 *  none
 *
 * Return:
 *  none
 *
 *******************************************************************************/
void draw(void)
{
    uint8_t count;
    vg_lite_error_t error = VG_LITE_SUCCESS;

    /* Draw the path using the matrix. */
    error = vg_lite_clear(render_target, NULL, WHITE_COLOR);
    if (error)
    {
        printf("Clear failed: vg_lite_clear() returned error %d\r\n", error);
        cleanup();
        CY_ASSERT(0);
    }

    for (count = 0; count < PATH_COUNT; count++)
    {
        error = vg_lite_draw(render_target, &path[count], VG_LITE_FILL_EVEN_ODD,
                &matrix, VG_LITE_BLEND_NONE, color_data[count]);

        if (error)
        {
            printf("vg_lite_draw() returned error %d\r\n", error);
            cleanup();
            CY_ASSERT(0);
        }
    }

    vg_lite_finish();

    /* Sets Video/Graphics layer buffer address and transfer the frame buffer to DC */
    Cy_GFXSS_Set_FrameBuffer(base, (uint32_t*)render_target->address,
            &gfx_context);

    /* Wait for completion of frame transfer */
    while (!(GFXSS_GFXSS_DC_DCNANO->GCREGDISPLAYINTR &
                    GFXSS_DC_DCNANO_GCREGDISPLAYINTR_GCREGDISPLAYINTR_DISP0_Msk));

    /* Swap buffers */
    render_target = (current_buffer) ? &buffer0 : &buffer1;
    current_buffer ^= 1;
}

/*******************************************************************************
 * Function Name: cm55_ns_gfx_task
 ********************************************************************************
 * Summary:
 *   It initializes:
 *       - "WF101JTYAHMNB0 panel" and vglite driver.
 *       - Allocate vglite buffers.
 *       - Load an identity matrix into a matrix variable.
 *       - Translate the matrix to a new location.
 *       - Scales the matrix in both horizontal and vertical directions.
 *       - Call redraw function to draw the image using GPU and render it in
 *         framebuffer.
 *
 * Parameters:
 *  void *arg: not used
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void cm55_ns_gfx_task(void *arg)
{

    CY_UNUSED_PARAMETER(arg);

    cy_en_gfx_status_t status;
    vg_lite_error_t error = VG_LITE_SUCCESS;


    GFXSS_MIPIDSI_Type* mipi_dsi_base = (GFXSS_MIPIDSI_Type *) GFXSS_GFXSS_MIPIDSI;

    /* Initializes the graphics subsystem according to the configuration */
    status = Cy_GFXSS_Init(base, &GFXSS_config, &gfx_context);

    if (CY_GFX_SUCCESS == status)
    {
        /* 10.1" display initialization */
        // wf101jtyahmnb0_init(mipi_dsi_base);

        viv_set_commit(DISPLAY_MASK);

        vg_lite_init_mem(register_mem_base, gpu_mem_base, vglite_heap_base,
                (uint32_t) VGLITE_HEAP_SIZE);

        /* Initialize the draw */
        error = vg_lite_init(DISPLAY_W, DISPLAY_H);
        if (VG_LITE_SUCCESS == error)
        {
            /* Allocate the off-screen buffer0 */
            buffer0.width  = DISPLAY_W;
            buffer0.height = DISPLAY_H;
            buffer0.format = VG_LITE_BGR565; //VG_LITE_RGB888;

            error = vg_lite_allocate(&buffer0);
            if (error)
            {
                printf("Buffer0 allocation failed in vglite space: vg_lite_allocate() returned error %d\r\n", error);
                cleanup();
                CY_ASSERT(0);
            }
            else
            {
                render_target = &buffer0;
                current_buffer = 0;
            }

            /* Allocate the off-screen buffer1 */
            buffer1.width  = DISPLAY_W;
            buffer1.height = DISPLAY_H;
            buffer1.format = VG_LITE_BGR565; //VG_LITE_RGB888;

            error = vg_lite_allocate(&buffer1);
            if (error)
            {
                printf("Buffer1 allocation failed in vglite space: vg_lite_allocate() returned error %d\r\n", error);
                cleanup();
                CY_ASSERT(0);
            }

            /* Clear the buffer with White */
            error = vg_lite_clear(render_target, NULL, WHITE_COLOR);
            if (error)
            {
                printf("Clear failed: vg_lite_clear() returned error %d\r\n", error);
                cleanup();
                CY_ASSERT(0);
            }

            /* Loads an identity matrix */
            vg_lite_identity(&matrix);

            /* Translate the matrix to adjust image to the center of display */
            vg_lite_translate(IMG_POSTN_X, IMG_POSTN_Y, &matrix);

            /* Set the default scale of the matrix */
            vg_lite_scale(DEF_X_SCALE, DEF_Y_SCALE, &matrix);
        }
        else
        {
            printf("vg_lite engine init failed: vg_lite_init() returned error %d\r\n", error);
            cleanup();
            CY_ASSERT(0);
        }
    }
    else
    {
        printf("Graphics subsystem initialization failed: Cy_GFXSS_Init() returned error %d\r\n", status);
        CY_ASSERT(0);
    }

    /* draw the image */
    draw();

    ext_display_adapter_init();
    printf("\r\n#LT8912B start# ("__DATE__ " : " __TIME__ ")");
	ext_display_adapter_reset();
	ext_display_adapter_configure();


    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(GFX_TASK_DELAY_MS));
    }

}

/* [] END OF FILE */
