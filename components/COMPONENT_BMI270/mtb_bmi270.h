/*****************************************************************************
 * File Name        : mtb_bmi270.h
 *
 * Description      : This file contains the functions for interacting with the
 *                    motion sensor.
 *
 * Related Document : See README.md
 *
 *****************************************************************************
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
 *****************************************************************************/

#ifndef _MTB_BMI_270_H_
#define _MTB_BMI_270_H_

#include "bmi270.h"
#include "cy_result.h"
#include "cyhal_i2c.h"

#define READ_WRITE_LEN    (46U)

/*****************************************************************************
 * Function Name: mtb_bmi270_init
 *****************************************************************************
* Summary:
* This function initializes the I2C instance, configures the BMI270, and sets
* platform-dependent function pointers.
*
* Parameters:
*  dev    Structure instance of bmi2_dev
*
* Return:
*  cy_rslt_t Status of execution
*
*****************************************************************************/
cy_rslt_t mtb_bmi270_init(struct bmi2_dev *dev, cyhal_i2c_t *i2c_instance);

/*****************************************************************************
 * Function Name: mtb_bmi270_config
 *****************************************************************************
* Summary:
* This function configures the accelerometer and gyroscope with a 100 Hz output
* data rate
*
* Parameters:
*  dev    Structure instance of bmi2_dev
*
* Return:
*  cy_rslt_t Status of execution
*
*****************************************************************************/
cy_rslt_t mtb_bmi270_config(struct bmi2_dev *dev);

/*****************************************************************************
 * Function Name: mtb_bmi270_get_sensor_data
 *****************************************************************************
* Summary:
* This function gets the sensor data for accelerometer and gyroscope
*
* Parameters:
*  dev            Structure instance of bmi2_dev
*  sensor_data    Structure instance of bmi2_sens_data
*
* Return:
*  cy_rslt_t     Status of execution
*
*****************************************************************************/
cy_rslt_t mtb_bmi270_get_sensor_data(struct bmi2_dev *dev,
                                    struct bmi2_sens_data *sensor_data);

#endif /* _MTB_BMI_270_H_ */


/* [] END OF FILE */
