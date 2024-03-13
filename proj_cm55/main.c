/******************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for the graphics rendering with 
 *              CPU vs GPU path.
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
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

#include <stdio.h>
#include "cyhal.h"
#include "cybsp.h"
#include "cy_graphics.h"
#include "cy_retarget_io.h"

#ifdef GPU_DISABLED_APP

/* 10.1 inch TFT display header */
#include "WF101JTYAHMNB0.h"

#include "infineon_logo_1024_600.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define IMAGE_RENDER_WAIT_MS            (500U)

/*******************************************************************************
 * Variable(s)
 ******************************************************************************/
static GFXSS_Type* base = (GFXSS_Type*) GFXSS;
cy_stc_gfx_context_t gfx_context;

#else

#include "gpu_enabled_app.h"

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    cy_rslt_t result;

#ifdef GPU_DISABLED_APP
    cy_en_gfx_status_t status = CY_GFX_SUCCESS;
    GFXSS_MIPIDSI_Type *mipi_dsi_base = (GFXSS_MIPIDSI_Type *) GFXSS_GFXSS_MIPIDSI;
#endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* If board init fails, stop the program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io with flow control to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
            CY_RETARGET_IO_BAUDRATE);

    /* Retarget-io init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

#ifdef GPU_DISABLED_APP

    printf("***********************************************\n"
            "PSoC Edge MCU: Graphics rendering using CPU   \n"
            "***********************************************\n");

    /* Initializes the graphics system according to the configuration */
    status = Cy_GFXSS_Init(base, &GFXSS_config, &gfx_context);

    if( status == CY_GFX_SUCCESS)
    {
        wf101jtyahmnb0_init(mipi_dsi_base);

        Cy_GFXSS_Set_FrameBuffer(base, (uint32_t*)img_ptr, &gfx_context);

        while (!(GFXSS_GFXSS_DC_DCNANO->GCREGDISPLAYINTR &
                GFXSS_DC_DCNANO_GCREGDISPLAYINTR_GCREGDISPLAYINTR_DISP0_Msk)){

            cyhal_system_delay_ms(IMAGE_RENDER_WAIT_MS);

        }
        while(true);
    }

#else

    BaseType_t task_return;

    /* Graphics Task creation */
    task_return = xTaskCreate(cm55_ns_gfx_task, GFX_TASK_NAME,
            GFX_TASK_STACK_SIZE, NULL,
            GFX_TASK_PRIORITY, NULL);

    if (pdPASS == task_return)
    {
        printf("*********************************************\n "
                "PSoC Edge MCU: Graphics rendering using GPU \n"
                "*********************************************\n");

        /* Start the FreeRTOS  Scheduler */
        vTaskStartScheduler();
    }
    else
    {
        printf("\n cm55_ns_gfx_task task creation failed");
        CY_ASSERT(0);
    }
#endif
}
