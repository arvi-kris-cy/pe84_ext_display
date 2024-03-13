/*******************************************************************************
 * File Name        : gpu_enabled_app.h
 *
 * Description      : This file includes the macros and function prototypes used
 * in gpu_enabled_app.c
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


#ifndef GPU_ENABLED_APP_H_
#define GPU_ENABLED_APP_H_

#include "cyhal.h"
#include "cybsp.h"
#include "WF101JTYAHMNB0.h"
#include "vg_lite.h"
#include "vg_lite_platform.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


/*******************************************************************************
 * Macros
 *******************************************************************************/
#define GFX_TASK_NAME                       ("CM55 Gfx Task")
#define GFX_TASK_STACK_SIZE                 (configMINIMAL_STACK_SIZE * 8)
#define GFX_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)
#define GFX_TASK_DELAY_MS                   (20U)

#define COLOR_DEPTH                         (24U) //(16U)
#define BITS_PER_PIXEL                      (8U)
#define APP_BUFFER_COUNT                    (2U)
#define DEFAULT_GPU_CMD_BUFFER_SIZE         ((64U) * (1024U))      /* 64 KB */
#define GPU_TESSELLATION_BUFFER_SIZE        ((DISPLAY_H) * 128U)
#define FRAME_BUFFER_SIZE                   ((DISPLAY_W) * (DISPLAY_H) * \
        ((COLOR_DEPTH) / (BITS_PER_PIXEL)))

#define VGLITE_HEAP_SIZE                    (((FRAME_BUFFER_SIZE) * (APP_BUFFER_COUNT)) + \
        ((DEFAULT_GPU_CMD_BUFFER_SIZE) * (APP_BUFFER_COUNT)) + \
        ((GPU_TESSELLATION_BUFFER_SIZE) * (APP_BUFFER_COUNT)))

#define WHITE_COLOR                         (0x00FFFFFFU)
#define TARGET_NUM_FRAMES                   (60U) //(45U)

/* Display controller command commit mask */
#define DISPLAY_MASK                        (0x00000001U)

/* Rotation and scaling parameters */
#define DEF_X_SCALE                         (0.7f)
#define DEF_Y_SCALE                         (0.7f)
#define ZOOM_OUT_X_SCALE                    (1.25f)
#define ZOOM_OUT_Y_SCALE                    (1.25f)
#define ZOOM_IN_X_SCALE                     (0.8f)
#define ZOOM_IN_Y_SCALE                     (0.8f)
#define SCALING_LIMIT                       (5)
#define ROTATION_DEG                        (5.0f)

#define RESET_VAL                           (-1)


/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

void cm55_ns_gfx_task(void *arg);
void animate_image(void);
void draw(void);
void cleanup(void);

#endif /* GPU_ENABLED_APP_H_ */
