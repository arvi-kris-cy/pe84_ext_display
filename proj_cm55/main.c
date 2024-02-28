/*******************************************************************************
* File Name        : main.c
*
* Description      : This source file contains the main routine for non-secure
*                    application running on CM55 CPU.
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
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "vg_lite.h"
#include "vg_lite_platform.h"
#include "lvgl.h"
#include "WF101JTYAHMNB0.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "demos/lv_demos.h"
#include "extdisplay.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GFX_TASK_NAME                       ("CM55 Gfx Task")
/* stack size in words */
#define GFX_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 8)

#define GFX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)

#define GPU_INT_PRIORITY                    (3U)
#define DC_INT_PRIORITY                     (3U)

#define APP_BUFFER_COUNT                    (2U)
/* 64 KB */
#define DEFAULT_GPU_CMD_BUFFER_SIZE         ((64U) * (1024U))
#define GPU_TESSELLATION_BUFFER_SIZE        ((DISPLAY_H) * 128U)


#define VGLITE_HEAP_SIZE                    (((DEFAULT_GPU_CMD_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)) + \
                                             ((GPU_TESSELLATION_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)))


#define GPU_MEM_BASE                        (0x0U)
#define TICK_VAL                            (1U)


/*******************************************************************************
* Global Variables
*******************************************************************************/
GFXSS_Type* base = (GFXSS_Type*) GFXSS;

cy_stc_gfx_context_t gfx_context;
volatile bool fb_pending        = false;

static uint32_t register_mem_base = (uint32_t) GFXSS_GFXSS_GPU_GCNANO;
static volatile bool s_lvgl_initialized = false;

/* Heap memory for VGLite to allocate memory for buffers, command, and
   tessellation buffers */
CY_SECTION(".cy_gpu_buf") uint8_t contiguous_mem[VGLITE_HEAP_SIZE];

volatile void *vglite_heap_base = &contiguous_mem;


/*******************************************************************************
* Function Name: dc_irq_handler
********************************************************************************
* Summary:
*  Display Controller interrupt handler which gets invoked when the DC finishes
*  utilizing the current frame buffer.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void dc_irq_handler(void)
{
    fb_pending = false;
    Cy_GFXSS_Clear_DC_Interrupt(base, &gfx_context);
}

/*******************************************************************************
* Function Name: gpu_irq_handler
********************************************************************************
* Summary:
*  GPU interrupt handler which gets invoked when the GPU finishes composing
*  a frame.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void gpu_irq_handler(void)
{
    Cy_GFXSS_Clear_GPU_Interrupt(base, &gfx_context);
    vg_lite_IRQHandler();
}

/*******************************************************************************
* Function Name: cm55_ns_gfx_task 
********************************************************************************
* Summary:
*   This is the FreeRTOS task callback function.
*   It initializes:  
*       - GFX subsystem.
*       - Configure the GPU interrupt.
*       - Initializes "WF101JTYAHMNB0 panel" and vglite driver.
*       - Allocates vglite memory.
*       - Configures LVGL, low level display and touch driver.
*       - Finally invokes the UI application.
*
* Parameters:
*  void *arg
*
* Return:
*  void
*
*******************************************************************************/
static void cm55_ns_gfx_task(void *arg)
{
    CY_UNUSED_PARAMETER(arg);

    cy_rslt_t result;
    cy_en_gfx_status_t status     = CY_GFX_SUCCESS;
    vg_lite_error_t vglite_status = VG_LITE_SUCCESS;

    /* Initializes the graphics subsystem according to the configuration */
    status = Cy_GFXSS_Init(base, &GFXSS_config, &gfx_context);

    if (CY_GFX_SUCCESS == status)
    {
        /* Prevent CPU to go to DeepSleep */
        cyhal_syspm_lock_deepsleep();

        result = cyhal_system_set_isr(GFXSS_DC_IRQ, GFXSS_DC_IRQ,
                DC_INT_PRIORITY, &dc_irq_handler);
        if (CY_RSLT_SUCCESS != result)
        {
            printf("Error in registering DC interrupt: 0x%lx\r\n",result);
            CY_ASSERT(0);
        }
        /* Enable DC interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_DC_IRQ);

        result = cyhal_system_set_isr(GFXSS_GPU_IRQ, GFXSS_GPU_IRQ,
                GPU_INT_PRIORITY, &gpu_irq_handler);
        if (CY_RSLT_SUCCESS != result)
        {
            printf("Error in registering GPU interrupt: 0x%lx\r\n",result);
            CY_ASSERT(0);
        }
        base->GFXSS_GPU.MXGPU.INTR_MASK =
                GFXSS_GPU_MXGPU_INTR_MASK_CORE_MASK_Msk;
        /* Enable GPU interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_GPU_IRQ);

        /* 10.1" display initialization */
        // wf101jtyahmnb0_init(GFXSS_GFXSS_MIPIDSI);

        // qcom,mdss-dsi-on-command = [
		// 	05 01 00 00 a0 00 02 11 00
		// 	05 01 00 00 a0 00 02 29 00];
		// qcom,mdss-dsi-off-command = [
		// 	05 01 00 00 78 00 02 28 00
		// 	05 01 00 00 78 00 02 10 00];

        viv_set_commit(1);

        /* Allocate memory for VGLite from the vglite_heap_base */
        vg_lite_init_mem(register_mem_base, GPU_MEM_BASE,
                         (volatile void *) vglite_heap_base,
                         (uint32_t)VGLITE_HEAP_SIZE);

        /* Initialize the memory and data structures needed for VGLite draw/blit
           functions */
        vglite_status = vg_lite_init((MY_DISP_HOR_RES),
                             (MY_DISP_VER_RES));

        result = display_init(); // display adapter init
        if (CY_RSLT_SUCCESS != result) {
            CY_ASSERT(0);
        }

        if (VG_LITE_SUCCESS == vglite_status)
        {
            /* Initialize LVGL library */
            lv_init();
            lv_port_disp_init();
            // lv_port_indev_init();
            s_lvgl_initialized = true;
            /* Run the Music demo */
            lv_demo_music();
        }
        else
        {
            printf("vg_lite_init failed, status: %d\r\n", vglite_status);
            /* Deallocate all the resources and free up all the memory */
            vg_lite_close();
            CY_ASSERT(0);
        }
    }
    else
    {
        printf("Graphics subsystem init failed, status: %d\r\n", status);
        CY_ASSERT(0);
    }

    for (;;)
    {
        static uint8_t loop_count;
        lv_task_handler();
        cyhal_system_delay_ms(LV_DISP_DEF_REFR_PERIOD);

        if (loop_count++ > 100)
        {
            printf("\r\n \r\n \r\n \r\n ");
            // printf("\x1b[2J\x1b[;H"); /* Clear screen */
            loop_count = 0;
            read_test_data();
        }
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  This is the main function for CM55 non-secure application.
*    1. It initializes the device and board peripherals.
*    2. It creates the FreeRTOS application task 'cm55_ns_gfx_task'.
*    3. It starts the RTOS task scheduler.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    BaseType_t task_return;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io with flow control to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                CY_RETARGET_IO_BAUDRATE);

    /* retarget-io init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Create the FreeRTOS Task */
    task_return = xTaskCreate(cm55_ns_gfx_task, GFX_TASK_NAME,
                              GFX_TASK_STACK_SIZE, NULL,
                              GFX_TASK_PRIORITY, NULL);

    if (pdPASS == task_return)
    {
        /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
        printf("\x1b[2J\x1b[;H");

        printf("****************** "
               "PSoC Edge MCU: Graphics LVGL Demo "
               "****************** \r\n\n");

        /* Start the RTOS Scheduler */
        vTaskStartScheduler();

        /* Should never get here! */
        /* Halt the CPU if scheduler exits */
        CY_ASSERT(0);
    }

    return 0;
}

/*******************************************************************************
* Function Name: vApplicationTickHook
********************************************************************************
* Summary:
*   This hook function is called in the system tick handler after any
*   OS work is completed.
*   This function calls `lv_tick_inc(x)` every `x` milliseconds.
*   It is required for the internal timing of LVGL.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void vApplicationTickHook(void)
{
    if (s_lvgl_initialized)
    {
        /* Call lv_tick_inc periodically for the internal timing of LVGL */
        lv_tick_inc(TICK_VAL);
    }
}

/* [] END OF FILE */
