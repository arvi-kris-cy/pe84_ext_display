/******************************************************************************
 * File Name:   motion_task.c
 *
 * Description: This file contains the task that initializes and configures the
 *              BMI270 Motion Sensor and displays the sensor orientation.
 *
 * Related Document: See README.md
 *
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

#include "motion_task.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cy_result.h"
#include "mtb_bmi270.h"
#include "semphr.h"
#include <math.h>
#if 1
#include "ipc_cm33_defs.h"
#endif

/*******************************************************************************
 * Global Variables
 ********************************************************************************/
#define printf(fmt, ...) (void)0
/* HAL structure for I2C */
static cyhal_i2c_t kit_i2c;

/* Motion sensor task handle */
static TaskHandle_t motion_sensor_task_handle;

/* Instance of BMI270 sensor structure */
struct bmi2_dev dev;

typedef enum
{
    LEFT=1,
    RIGHT,
    UP,
    DOWN,
    NO_DIRECTION
} edirection_t;

volatile edirection_t imu_gesture_direction;
/******************************************************************************
 * Macros
 ******************************************************************************/
#define DELAY_MS (1000U)
/* Task priority and stack size for the Motion sensor task */
#define TASK_MOTION_SENSOR_PRIORITY     (configMAX_PRIORITIES - 1)
#define TASK_MOTION_SENSOR_STACK_SIZE   (1024U)
/* I2C Clock frequency in Hz */
#define I2C_CLK_FREQ_HZ                 (400000U)
#define THRESHOLD                       20


/*******************************************************************************
 * Function Name: motion_sensor_init
 ********************************************************************************
 * Summary:
 *  Function that configures the I2C master interface and then initializes
 *  the motion sensor.
 *
 * Parameters:
 *  None
 *
 * Return:
 * result
 *
 *******************************************************************************/
static cy_rslt_t motion_sensor_init(void)
{
    cy_rslt_t result;

    /* I2C configuration structure */
    cyhal_i2c_cfg_t kit_i2c_cfg =
    {
            .is_slave = false,
            .address = 0,
            .frequencyhal_hz = I2C_CLK_FREQ_HZ
    };

    /* Initialize the I2C master interface for BMI270 motion sensor */
    result = cyhal_i2c_init(&kit_i2c, (cyhal_gpio_t) CYBSP_I2C_SDA,
            (cyhal_gpio_t) CYBSP_I2C_SCL, NULL);
    if(CY_RSLT_SUCCESS != result)
    {
        printf(" Error : I2C initialization failed !!\r\n");
        CY_ASSERT(0);
    }

    /* Configure the I2C master interface with the desired clock frequency */
    result = cyhal_i2c_configure(&kit_i2c, &kit_i2c_cfg);
    if(CY_RSLT_SUCCESS != result)
    {
        printf(" Error : I2C configuration failed !!\r\n");
        CY_ASSERT(0);
    }

    /* Initialize the BMI270 motion sensor */
    result = mtb_bmi270_init(&dev, &kit_i2c);
    if(CY_RSLT_SUCCESS != result)
    {
        printf(" Error : IMU sensor init failed !!\r\n");
        CY_ASSERT(0);
    }

    result = mtb_bmi270_config(&dev);
    if(CY_RSLT_SUCCESS != result)
    {
        printf(" Error : IMU sensor config failed !!\r\n");
        CY_ASSERT(0);
    }

    return result;
}


/*******************************************************************************
 * Function Name: motion_sensor_update_orientation
 ********************************************************************************
 * Summary:
 *  Function that updates the orientation status to one of the 6 types,
 *  'ORIENTATION_UP, ORIENTATION_DOWN, TOP_EDGE, BOTTOM_EDGE,
 *  LEFT_EDGE, and RIGHT_EDGE'. This functions detects the axis that is most perpendicular
 *  to the ground based on the absolute value of acceleration in that axis.
 *  The sign of the acceleration signifies whether the axis is facing the ground
 *  or the opposite.
 *
 * Return:
 *  CY_RSLT_SUCCESS upon successful orientation update, else a non-zero value
 *  that indicates the error.
 *
 *******************************************************************************/
static cy_rslt_t motion_sensor_update_orientation(void)
{
    /* Status variable */
    cy_rslt_t result = CY_RSLT_SUCCESS;

    struct bmi2_sens_data sensor_data = {0};

    /* Read x, y, z components of acceleration */
    result =  mtb_bmi270_get_sensor_data(&dev, &sensor_data);
    if (CY_RSLT_SUCCESS != result)
    {
        printf("Read data failed\r\n");
    }

        if (result == CY_RSLT_SUCCESS)
        {
            for(;;){
            mtb_bmi270_get_sensor_data(&dev, &sensor_data);
            //printf("ACCL: X:%6d Y:%6d Z:%6d\r\n", sensor_data.acc.x, sensor_data.acc.y, sensor_data.acc.z);
            //printf("GYRO : X:%6d Y:%6d Z:%6d\r\n\r\n", sensor_data.gyr.x, sensor_data.gyr.y, sensor_data.gyr.z);
            int roll = atan2(sensor_data.acc.y, sensor_data.acc.z) * 57.3;
            int pitch = atan2((- sensor_data.acc.x) , sqrt(sensor_data.acc.y* sensor_data.acc.y + sensor_data.acc.z * sensor_data.acc.z)) * 57.3;
            //printf("Pitch:%d \r",pitch);
            //printf("Roll:%d \r",roll);
            printMotionDirection(roll,pitch);
            cyhal_system_delay_ms(1000);
            }
        }
        return result;
 }


void printMotionDirection(int roll, int pitch) 
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    imu_gesture_direction =  NO_DIRECTION;

    if (roll > THRESHOLD)
    {
        imu_gesture_direction =  RIGHT;
        printf("\rMoving Right:");
        int Right = roll / THRESHOLD;
        for (int i = 0; i < Right; ++i) 
        {
              printf(">");
        }
        printf("\n");
    } 
    else if (roll < -THRESHOLD) 
    {
        imu_gesture_direction =  LEFT;
        printf("\rMoving Left: ");
        int Left = -roll / THRESHOLD;
        for (int i = 0; i < Left; ++i) 
        {
            printf("<");
        }
        printf("\n");
    }
    else
    {
        
    }
    if (pitch > THRESHOLD) 
    {
        imu_gesture_direction =  DOWN;
        printf("\rMoving Down: ");
		int Down = pitch / THRESHOLD;
		for (int i = 0; i < Down; ++i) 
        {
			printf(".\n");
		}
    } 
    else if (pitch < -THRESHOLD) 
    {
        imu_gesture_direction =  UP;
        printf("\rMoving Up: ");
        int Up = -pitch / THRESHOLD;
        for (int i = 0; i < Up; ++i) 
        {
            printf("^\n");
        }
    }
    else
    {

    }
//printf("\r\n OUT :CM33 direction: %d ",imu_gesture_direction);
#if 1
    if(imu_gesture_direction !=  NO_DIRECTION)
    {
        printf("\r\nIN : CM33 direction: %d ",imu_gesture_direction);
        task0_msg = imu_gesture_direction;
        result = cyhal_ipc_queue_put(&task0_queue2, &task0_msg, CYHAL_IPC_NEVER_TIMEOUT);
        if (CY_RSLT_SUCCESS != result)
        {
                CY_ASSERT(0);
        }
    }
#endif
}


/*******************************************************************************
 * Function Name: task_motion
 ********************************************************************************
 * Summary:
 *  Task that configures the Motion Sensor and processes the sensor data to
 *  display the sensor orientation.
 *
 * Parameters:
 *  void *pvParameters : Task parameter defined during task creation (unused)
 *
 * Return:
 *  None
 *
 *******************************************************************************/
static void task_motion(void* pvParameters)
{
    /* Status variable to indicate the result of various operations */
    cy_rslt_t result;
    //printf("In motion task \n\r");
    // task0_msg = 1157;

    // for(;;)
    // {
    //     //printf("Sending data to IPC queue \n\r");

    //     task0_msg += 1;
    //     result = cyhal_ipc_queue_put(&task0_queue2, &task0_msg, CYHAL_IPC_NEVER_TIMEOUT);
    //     if (CY_RSLT_SUCCESS != result)
    //     {
    //         CY_ASSERT(0);
    //         //printf("Sending data to IPC queue FAIL\n\r");
    //     }
    //     vTaskDelay(5000);
    // }


    /* Remove warning for unused parameter */
    CY_UNUSED_PARAMETER(pvParameters);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    /* Initialize BMI270 motion sensor and suspend the task upon failure */
    result = motion_sensor_init();
    if(CY_RSLT_SUCCESS != result)
    {
        printf(" Error : Motion Sensor initialization failed !!\n Check hardware connection\r\n");
        CY_ASSERT(0);
    }
    printf("     BMI270 Motion Sensor successfully initialized.\r\n");

    for(;;)
    {
        /* Get current orientation */
        motion_sensor_update_orientation();
        vTaskDelay( DELAY_MS/portTICK_PERIOD_MS );

    }
}


/*******************************************************************************
 * Function Name: create_motion_sensor_task
 ********************************************************************************
 * Summary:
 *  Function that creates the motion sensor task.
 *
 * Parameters:
 *  None
 *
 * Return:
 *  CY_RSLT_SUCCESS upon successful creation of the motion sensor task, else a
 *  non-zero value that indicates the error.
 *
 *******************************************************************************/
cy_rslt_t create_motion_sensor_task(void)
{
    BaseType_t status;

    status = xTaskCreate(task_motion, "Motion Sensor Task", TASK_MOTION_SENSOR_STACK_SIZE,
            NULL, TASK_MOTION_SENSOR_PRIORITY, &motion_sensor_task_handle);

    return (pdPASS == status) ? CY_RSLT_SUCCESS : (cy_rslt_t) status;
}



/* [] END OF FILE */
