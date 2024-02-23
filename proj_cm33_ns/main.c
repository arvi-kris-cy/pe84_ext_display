/*****************************************************************************
* File Name        : main.c
*
* Description      : This source file contains the main routine for non-secure
*                    application in the CM33 CPU
*
* Related Document : See README.md
*
*******************************************************************************
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

#include "app_mem_map.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "motion_task.h"
#include <stdlib.h>
#include "cy_retarget_io.h"
#include "FreeRTOS.h"
#include "task.h"

#include "ipc_communication.h"
/******************************************************************************
 * Macros
 ******************************************************************************/
/* The timeout value in microsecond used to wait for core to be booted
    Use value 0U for infinite wait till the core is booted successfully.*/
#define CY_SYS_CORE_WAIT_USEC               (10U)

/******************************************************************************
 * Global variables
 ******************************************************************************/
#if 1

cyhal_ipc_t task0_queue1;
cyhal_ipc_t task0_queue2;
void *queue1_pool, *queue2_pool;
cyhal_ipc_queue_t *queue1_handle, *queue2_handle;
uint32_t random_number;
uint32_t task0_msg;         /* Value to be sent to the other core. */
uint32_t task0_cmd;

#endif

/*****************************************************************************
 * Function Name: main
 *****************************************************************************
 * Summary:
 * This is the main function for CM33 non-secure application.
 *    1. It initializes the device and board peripherals.
 *    2. It enables the  CM55 CPU using 'Cy_SysEnableCM55'
 *    3. It creates the FreeRTOS application task 'cm33_blinky_task'
 *    4. It starts the RTOS task scheduler.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  int
 *
 *****************************************************************************/

int main(void)
{
    cy_rslt_t result;

#if defined (CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
#if 1
    /* Allocating (shared) memory pools for queue1 and queue2 */
    CYHAL_IPC_QUEUE_POOL_ALLOC(queue1_pool, QUEUE1_ITEMS, sizeof(task0_cmd));
    CYHAL_IPC_QUEUE_POOL_ALLOC(queue2_pool, QUEUE2_ITEMS, sizeof(task0_msg));

    /* Allocating (shared) memory for queue handles */
    CYHAL_IPC_QUEUE_HANDLE_ALLOC(queue1_handle);
    CYHAL_IPC_QUEUE_HANDLE_ALLOC(queue2_handle);

    queue1_handle->channel_num = CMD_CHANNEL_NUM;
    queue1_handle->queue_num   = QUEUE1_NUM;
    queue1_handle->queue_pool  = queue1_pool;
    queue1_handle->num_items   = QUEUE1_ITEMS;
    queue1_handle->item_size   = sizeof(task0_cmd);

    queue2_handle->channel_num = MSG_CHANNEL_NUM;
    queue2_handle->queue_num   = QUEUE2_NUM;
    queue2_handle->queue_pool  = queue2_pool;
    queue2_handle->num_items   = QUEUE2_ITEMS;
    queue2_handle->item_size   = sizeof(task0_msg);

    result = cyhal_ipc_queue_init(&task0_queue1, queue1_handle);
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    result = cyhal_ipc_queue_init(&task0_queue2, queue2_handle);
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }
#endif
    /* If retarget-io init failed stop program execution */
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    /* Enable CM55. */
    /* CY_CM55_APP_BOOT_ADDR must be updated if CM55 memory layout is changed. */
    Cy_SysEnableCM55(CY_CM55_APP_BOOT_ADDR, CY_SYS_CORE_WAIT_USEC);

    result = create_motion_sensor_task();

    /* If the task creation failed stop the program execution */
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    /* To avoid compiler warning */
    CY_UNUSED_PARAMETER(result);

    /* Start the RTOS Scheduler */
    vTaskStartScheduler();

    return 0;
}

