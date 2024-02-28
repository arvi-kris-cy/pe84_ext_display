/*******************************************************************************
* File Name        : main.c
*
* Description      : This source file contains the main routine for non-secure
*                    application running on CM33 CPU.
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

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_NAME                           ("CM33NS Task")
#define TASK_STACK_SIZE                     (configMINIMAL_STACK_SIZE)
#define TASK_PRIORITY                       (configMAX_PRIORITIES - 1)

/* The timeout value in microsecond used to wait for core to be booted
    Use value 0U for infinite wait till the core is booted successfully.*/
#define CM55_BOOT_WAIT_TIME_USEC            (10U)


/*******************************************************************************
* Function Name: cm33_ns_task
********************************************************************************
* Summary:
*  This is the FreeRTOS task callback function which suspends itself
*  (cm33_ns_task).
*
* Parameters:
*  void *arg: Pointer to the argument passed to the task (not used)
*
* Return:
*  void
*
*******************************************************************************/
static void cm33_ns_task(void *arg)
{
    CY_UNUSED_PARAMETER(arg);

    for (;;)
    {
        vTaskSuspend(NULL);
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM33 non-secure application. 
*    1. It initializes the device and board peripherals.
*    3. It creates the FreeRTOS application task 'cm33_ns_task'.
*    4. It starts the RTOS task scheduler.
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

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Create the FreeRTOS Task */
    result = xTaskCreate(cm33_ns_task, TASK_NAME, 
                        TASK_STACK_SIZE, NULL, 
                        TASK_PRIORITY, NULL);

    if (pdPASS == result)
    {
        /* CY_CM55_APP_BOOT_ADDR must be updated if CM55 memory layout is changed. */
        Cy_SysEnableCM55(CY_CM55_APP_BOOT_ADDR, CM55_BOOT_WAIT_TIME_USEC);

        /* Start the RTOS Scheduler */
        vTaskStartScheduler();

        /* Should never get here! */
        /* Halt the CPU if scheduler exits */
        CY_ASSERT(0);
    }

    return 0;
}

/* [] END OF FILE */
