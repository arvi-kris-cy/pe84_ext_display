/*******************************************************************************
* File Name        : main.c
*
* Description      : This source file contains the main routine for non-secure
*                    application running on CM55 CPU.
*
* Related Document : See README.md
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
*******************************************************************************/
#include "cy_pdl.h"
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
#include "ipc_communication.h"
#include "extdisplay.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GFX_TASK_NAME                       ("CM55 Gfx Task")
/* stack size in words */
#define GFX_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 8)

#define GFX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)

#if 1
#define IPC_TASK_NAME                       ("CM55 IPC Task")
/* stack size in words */
#define IPC_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 8)

#define IPC_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)
#endif

#define GFX_TASK_DELAY_MS                   (5U)
#define GPU_INT_PRIORITY                    (2U)

#define COLOR_DEPTH                         (16U)
#define BITS_PER_PIXEL                      (8U)
#define APP_BUFFER_COUNT                    (2U)
/* 64 KB */
#define DEFAULT_GPU_CMD_BUFFER_SIZE         ((64U) * (1024U))
#define GPU_TESSELLATION_BUFFER_SIZE        ((DISPLAY_H) * 128U)
#define FRAME_BUFFER_SIZE                   ((DISPLAY_W) * (DISPLAY_H) * \
                                             ((COLOR_DEPTH) / (BITS_PER_PIXEL)))

#define VGLITE_HEAP_SIZE                    (((DEFAULT_GPU_CMD_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)) + \
                                             ((GPU_TESSELLATION_BUFFER_SIZE) * \
                                              (APP_BUFFER_COUNT)))

#define MEM_RESIZE_VAL                      (2U)

/* Display controller command commit mask */
#define DISPLAY_MASK                        (0x00000001U)

#define GPU_MEM_BASE                        (0x0U)
#define TICK_VAL                            (1U)

#define IMAGE_BACKGROUND BG3
#define IMAGE_ROCKET rocket1

#define TEST_IMU_CONTROL
lv_obj_t * rocket;


/*******************************************************************************
* Global Variables
*******************************************************************************/
GFXSS_Type* base = (GFXSS_Type*) GFXSS;

cy_stc_gfx_context_t gfx_context;

static uint32_t register_mem_base = (uint32_t) GFXSS_GFXSS_GPU_GCNANO;
static volatile bool s_lvgl_initialized = false;

/* Heap memory for VGLite to allocate memory for buffers, command, and
   tessellation buffers */
CY_SECTION(".cy_gpu_buf") uint8_t contiguous_mem[VGLITE_HEAP_SIZE];

volatile void *vglite_heap_base = &contiguous_mem;
#if 1
/* IPC communcation variables*/
cyhal_ipc_t task1_queue1;
cyhal_ipc_t task1_queue2;
uint32_t task1_cmd;
uint32_t task1_msg;
static void ipc_task(void *arg);
#endif

static uint16_t r_x =100;
static uint16_t r_y =500;
#ifdef TEST_IMU_CONTROL
typedef enum
{
    LEFT=3,
    RIGHT=4,
    UP=1,
    DOWN=2,
    NO_DIRECTION=5,
} edirection_t;
volatile edirection_t ipc_direction;
#endif
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
    base->GFXSS_GPU.MXGPU.INTR = GFXSS_GPU_MXGPU_INTR_CORE_Msk;
    vg_lite_IRQHandler();
}

/**
 * Image styling and offset
 */

void lv_demo_img_bg(void)
{
    LV_IMG_DECLARE(IMAGE_BACKGROUND);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);
    lv_style_set_img_recolor_opa(&style, LV_OPA_TRANSP);
    lv_style_set_img_recolor(&style, lv_color_black());

    lv_obj_t * img = lv_img_create(lv_scr_act());
    lv_obj_add_style(img, &style, 0);
    lv_img_set_src(img, &IMAGE_BACKGROUND);
    lv_obj_set_size(img, 1024, 768);
    lv_obj_center(img);

    LV_IMG_DECLARE(IMAGE_ROCKET);
    rocket = lv_img_create(lv_scr_act());
    lv_obj_add_style(rocket, &style, 0);
    lv_img_set_src(rocket, &IMAGE_ROCKET);
    lv_obj_set_size(rocket, 89, 142);
    lv_obj_center(rocket);

    lv_obj_move_to(rocket, 100, 300);
#ifndef TEST_IMU_CONTROL 
    lv_example_event_1();
    lv_example_event_2();
    lv_example_event_3();
#endif
}

static void event_cb_1(lv_event_t * e)
{
    if(r_x >50 )
    {
         r_x -= 5;
    }
    lv_obj_move_to(rocket, r_x, r_y);
}

static void event_cb_2(lv_event_t * e)
{
    if(r_x <1000 )
    {
         r_x += 5;
    }

    lv_obj_move_to(rocket, r_x, r_y);
}
static void event_cb_3(lv_event_t * e)
{
    if(r_y >50 )
    {
         r_y -= 10;
    }

    lv_obj_move_to(rocket, r_x, r_y);
}


/**
 * Add click event to a button
 */
void lv_example_event_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 50, 50);
    lv_obj_set_pos(btn, 300,500);
    lv_obj_add_event_cb(btn, event_cb_1, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Left");
    lv_obj_center(label);
}
/**
 * Add click event to a button
 */
void lv_example_event_2(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 50, 50);
    lv_obj_set_pos(btn, 400,500);
    lv_obj_add_event_cb(btn, event_cb_2, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Right");
    lv_obj_center(label);
}
/**
 * Add click event to a button
 */
void lv_example_event_3(void)
{ 
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 50, 50);
    lv_obj_set_pos(btn, 350,400);
    //lv_obj_center(btn);
    lv_obj_add_event_cb(btn, event_cb_3, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Up!");
    lv_obj_center(label);
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
        result = cyhal_system_set_isr(GFXSS_GPU_IRQ, GFXSS_GPU_IRQ,
                GPU_INT_PRIORITY, &gpu_irq_handler);
        if (CY_RSLT_SUCCESS != result)
        {
            printf("Error in registering GPU interrupt: 0x%lx\r\n",result);
            CY_ASSERT(0);
        }
        base->GFXSS_GPU.MXGPU.INTR_MASK =
                GFXSS_GPU_MXGPU_INTR_MASK_CORE_MASK_Msk;
        /* Enable interrupt in NVIC. */
        NVIC_EnableIRQ(GFXSS_GPU_IRQ);

        /* 10.1" display initialization */
        //wf101jtyahmnb0_init(GFXSS_GFXSS_MIPIDSI);
        result=display_init();
       			if (CY_RSLT_SUCCESS != result) {
       				CY_ASSERT(0);
       			}

        /* Allocate memory for VGLite from the vglite_heap_base */
        vg_lite_init_mem(register_mem_base, GPU_MEM_BASE,
                         (volatile void *) vglite_heap_base,
                         (uint32_t)VGLITE_HEAP_SIZE);

        /* Initialize the memory and data structures needed for VGLite draw/blit
           functions */
        vglite_status = vg_lite_init((MY_DISP_HOR_RES) * (MEM_RESIZE_VAL),
                             (MY_DISP_VER_RES) * (MEM_RESIZE_VAL));

        if (VG_LITE_SUCCESS == vglite_status)
        {
            /* Initialize LVGL library */
            lv_init();
            lv_port_disp_init();
            //lv_port_indev_init();
            s_lvgl_initialized = true;
            /* Run the Music demo */
            //lv_demo_music();
            //lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x343247), 0);
            /*Create an object with the new style*/
            // lv_obj_t * obj = lv_img_create(lv_scr_act());
            // //lv_obj_add_style(obj, &style, 0);

            // LV_IMG_DECLARE(BG1);
            // lv_img_set_src(obj, &BG1);

            // lv_obj_center(obj); 

             lv_demo_img_bg();

            //ui_init();
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
        // static uint32_t tick = 0;
        // tick = lv_tick_get();
        // if (cyhal_ipc_queue_count(&task1_queue2))
        // {
        //     if (CY_RSLT_SUCCESS == (cyhal_ipc_queue_get(&task1_queue2, &task1_msg, CYHAL_IPC_NEVER_TIMEOUT)))
        //     {
        //         printf("CM55 Direction : %d\n\r", (unsigned int) task1_msg);
        //     }
        // }
        lv_task_handler();
        cyhal_system_delay_ms(GFX_TASK_DELAY_MS);
        // printf("\nElapsed time :%d",lv_tick_elaps(tick));
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
#if 1
        /* Create the IPC Task */
    task_return &= xTaskCreate(ipc_task, IPC_TASK_NAME,
                              IPC_TASK_STACK_SIZE, NULL,
                              IPC_TASK_PRIORITY, NULL);
#endif
    if (pdPASS == task_return)
    {
#if 1
        cyhal_ipc_init(CYHAL_IPC_NEVER_TIMEOUT);

        result = cyhal_ipc_queue_get_handle(&task1_queue1, CMD_CHANNEL_NUM, QUEUE1_NUM);
        result = cyhal_ipc_queue_get_handle(&task1_queue2, MSG_CHANNEL_NUM, QUEUE2_NUM);
#endif
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
    static uint32_t count =0;

    if (s_lvgl_initialized)
    {
        /* Call lv_tick_inc periodically for the internal timing of LVGL */
        lv_tick_inc(TICK_VAL);
        
        //TODO : move this to game control logic
        if(count % 500 == 0)
        {
            if(r_y <550)
            {
                r_y +=1;    
                #ifndef TEST_IMU_CONTROL
                lv_obj_move_to(rocket, r_x, r_y);
                #endif
            }
        }
        #ifdef TEST_IMU_CONTROL
        if(count % 50 == 0)
        {
            if( ipc_direction == LEFT)
            {
                if(r_x >50 )
                {
                    r_x -= 1;
                }
            }
            else if ( ipc_direction == RIGHT)
            {
                if(r_x <1000 )
                {
                    r_x += 1;
                }
            }
            else if (ipc_direction == UP)
            {
                if(r_y >50 )
                {
                    r_y -= 1;
                }
            }
            lv_obj_move_to(rocket, r_x, r_y);
        }
        #endif
    }
    count++;
}
#if 1
/*******************************************************************************
* Function Name: ipc_task 
********************************************************************************
* Summary:
*   This is the FreeRTOS task callback function.
*   It takes action on the IPC messages
*
* Parameters:
*  void *arg
*
* Return:
*  void
*
*******************************************************************************/
static void ipc_task(void *arg)
{
    printf("IPC task initialised\n\r");
    for(;;)
    {
        if (cyhal_ipc_queue_count(&task1_queue2))
        {
            if (CY_RSLT_SUCCESS == (cyhal_ipc_queue_get(&task1_queue2, &task1_msg, CYHAL_IPC_NEVER_TIMEOUT)))
            {
                #ifdef TEST_IMU_CONTROL
                ipc_direction = task1_msg;
                #endif
                printf("CM55 Direction : %d\n\r", (unsigned int) task1_msg);
            }
        }
        vTaskDelay(100);
    }

}
#endif
/* [] END OF FILE */
