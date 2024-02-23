/*******************************************************************************
* File Name        : ILI2511.c
*
* Description      : This source file provides implementation for I2C based
*                    ILI2511 CTP driver.
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

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cyhal.h"
#include "cyhal_gpio.h"
#include "cycfg_pins.h"
#include "ILI2511.h"


/*******************************************************************************
* Macros
*******************************************************************************/
/* I2C slave address */
#define ILI2511_SLAVE_ADDR                   (0x41)
/* I2C bus frequency */
#define I2C_FREQ                             (400000UL)

#define I2C_MASTER_ADDR                      (0U)
#define WF101JTYAHMNB0_LCD_WIDTH             (1024U)
#define WF101JTYAHMNB0_LCD_HEIGHT            (600U)

#ifndef CYBSP_DISP_TP_INT
#define CYBSP_DISP_TP_INT                    (P17_2)
#endif
#define TP_INTERRUPT_PRIORITY                (3U)

/* ILI2511 command codes */
#define TOUCH_INFORMATION                    (0x10)
#define ILITEK_TP_CMD_GET_FIRMWARE_VER       (0x40)

#define ILITEK_CHIP_ID_CODE                  (0x06)
#define ILITEK_MAJOR_FIRMWARE_VER            (0x00)
#define ILITEK_MINOR_FIRMWARE_VER            (0x00)
#define ILITEK_RELEASE_FIRMWARE_VER          (0x00)
#define ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_4  (0x62)
#define ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_5  (0x90)
#define ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_6  (0x01)
#define ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_7  (0x02)

/* TP_CALIBRATION_OFFSET used to align with the actual coordinate */
#define TP_CALIBRATION_OFFSET                (4U)

#define TP_CALIBRATION_VAL                   (65535U)

#define RETRY_COUNT                          (5U)
#define I2C_TIMEOUT_MS                       (0U)
#define RST_PIN_LOW_HOLD_TIME_MS             (50U)
#define INITIALIZATION_TIME_MS               (100U)

#define RESET_VAL                            (0U)
#define BIT_MASK                             (0x3FFF)
#define BYTE_SHIFT                           (8U)


/*******************************************************************************
* Global Variables
*******************************************************************************/
cyhal_i2c_t i2c_master_obj;
cyhal_gpio_callback_data_t tp_int_callback_data;

static uint32_t last_touch_x = RESET_VAL;
static uint32_t last_touch_y = RESET_VAL;

volatile bool touch_event = false;

/* ILI511 firmware data bytes */
typedef enum
{
    ILITEK_CHIP_ID_CODE_POS,
    ILITEK_MAJOR_FIRMWARE_VER_POS,
    ILITEK_MINOR_FIRMWARE_VER_POS,
    ILITEK_RELEASE_FIRMWARE_VER_POS,
    ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_4_POS,
    ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_5_POS,
    ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_6_POS,
    ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_7_POS,
    ILITEK_FIRMWARE_NUM_BYTES
} ilitek_firmware_bytes_t;

/* ILI511 touch panel coordinate data bytes */
typedef enum
{
    TP_DATA_X_MSB_POS = 1,
    TP_DATA_X_LSB_POS,
    TP_DATA_Y_MSB_POS,
    TP_DATA_Y_LSB_POS,
    TP_DATA_NUM_BYTES = 31
} tp_data_bytes_t;

/*******************************************************************************
* Function Name: i2c_init
********************************************************************************
* Summary:
*  This function initializes I2C HAL resource in master mode to interact
*  with the touch panel (I2C slave).
*
* Parameters:
*  void
*
* Return:
*  cy_rslt_t: Result of the operation.
*
*******************************************************************************/
static cy_rslt_t i2c_init(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* Define the I2C master configuration structure */
    cyhal_i2c_cfg_t i2c_master_config =
    {
        .is_slave        = CYHAL_I2C_MODE_MASTER,
        .address         = I2C_MASTER_ADDR,   /* address is not used for master mode */
        .frequencyhal_hz = I2C_FREQ
    };

    /* Initialize I2C master, set the SDA and SCL pins and assign a new clock */
    result = cyhal_i2c_init(&i2c_master_obj, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);

    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Configure I2C Master */
    result = cyhal_i2c_configure(&i2c_master_obj, &i2c_master_config);

    cyhal_system_delay_ms(INITIALIZATION_TIME_MS);

    return result;
}

/*******************************************************************************
* Function Name: is_ili2511_alive
********************************************************************************
* Summary:
*  This function checks whether ILI2511 IC is alive or not.
*
* Parameters:
*  void
*
* Return:
*  cy_rslt_t: Result of the operation.
*
*******************************************************************************/
static cy_rslt_t is_ili2511_alive(void)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint8_t cmd = ILITEK_TP_CMD_GET_FIRMWARE_VER;
    uint8_t firmware_version[ILITEK_FIRMWARE_NUM_BYTES] = {(uint8_t) RESET_VAL};

    /* Send packet with command to the slave */
    status = cyhal_i2c_master_write(&i2c_master_obj, ILI2511_SLAVE_ADDR,
                                     &cmd, sizeof(cmd), I2C_TIMEOUT_MS, true);

    if (CY_RSLT_SUCCESS == status)
    {
        status = cyhal_i2c_master_read(&i2c_master_obj, ILI2511_SLAVE_ADDR,
                        firmware_version, sizeof(firmware_version), I2C_TIMEOUT_MS, true);
        if (CY_RSLT_SUCCESS == status)
        {
            /* To check whether the firmware version is valid or not */
            if ((ILITEK_CHIP_ID_CODE == firmware_version[ILITEK_CHIP_ID_CODE_POS])
                    && (ILITEK_MAJOR_FIRMWARE_VER == firmware_version[ILITEK_MAJOR_FIRMWARE_VER_POS])
                    && (ILITEK_MINOR_FIRMWARE_VER == firmware_version[ILITEK_MINOR_FIRMWARE_VER_POS])
                    && (ILITEK_RELEASE_FIRMWARE_VER == firmware_version[ILITEK_RELEASE_FIRMWARE_VER_POS])
                    && (ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_4 == firmware_version[ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_4_POS])
                    && (ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_5 == firmware_version[ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_5_POS])
                    && (ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_6 == firmware_version[ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_6_POS])
                    && (ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_7 == firmware_version[ILITEK_CUSTOMER_FIRMWARE_VER_BYTE_7_POS]))
            {
                status = CY_RSLT_SUCCESS;
            }
            else
            {
                status = CYHAL_I2C_RSLT_ERR_CMD_ERROR;
            }
        }
    }

    return status;
}

/*******************************************************************************
* Function Name: enable_ili2511
********************************************************************************
* Summary:
*  This function enables ILI2511 IC.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void enable_ili2511(void)
{
    cyhal_gpio_write(CYBSP_DISP_TP_RST, true);
    cyhal_system_delay_ms(RST_PIN_LOW_HOLD_TIME_MS);
    cyhal_gpio_write(CYBSP_DISP_TP_RST, false);
    cyhal_system_delay_ms(RST_PIN_LOW_HOLD_TIME_MS);
    cyhal_gpio_write(CYBSP_DISP_TP_RST, true);
    cyhal_system_delay_ms(INITIALIZATION_TIME_MS);
}

/*******************************************************************************
* Function Name: ili2511_read_raw_touch_data
********************************************************************************
* Summary:
*  This function reads the raw x, y coordinate data from the touch sensor.
*
* Parameters:
*  *touch_data: Pointer to the buffer to store touch data.
*  size: Size of the buffer to store touch data.
*
* Return:
*  cy_rslt_t: Result of the operation.
*
*******************************************************************************/
static cy_rslt_t ili2511_read_raw_touch_data(uint8_t *touch_data, uint16_t size)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint8_t cmd = TOUCH_INFORMATION;

    if (NULL != touch_data)
    {
        /* Send packet with command to the slave */
        status = cyhal_i2c_master_write(&i2c_master_obj, ILI2511_SLAVE_ADDR,
                &cmd, sizeof(cmd), I2C_TIMEOUT_MS, true);

        if (CY_RSLT_SUCCESS == status)
        {
            status = cyhal_i2c_master_read(&i2c_master_obj, ILI2511_SLAVE_ADDR,
                    touch_data, size, I2C_TIMEOUT_MS, true);
        }
        else
        {
            status = CYHAL_I2C_RSLT_ERR_CMD_ERROR;
        }
    }
    else
    {
        status = CYHAL_I2C_RSLT_ERR_BUFFERS_NULL_PTR;
    }

    return status;
}

/*******************************************************************************
* Function Name: tp_int_interrupt_handler
********************************************************************************
* Summary:
*  Touch panel interrupt handler to detect touch events on the sensor.
*  When a finger touches on the sensor surface, the INT pin will be pulled low
*  and we set the touch_event flag true.
*
* Parameters:
*  *handler_arg: Argument passed to the handler (not used)
*  event: Event responsible for the interrupt.
*
* Return:
*  void
*
*******************************************************************************/
static void tp_int_interrupt_handler(void *handler_arg, cyhal_gpio_event_t event)
{
    CY_UNUSED_PARAMETER(handler_arg);
    CY_UNUSED_PARAMETER(event);

    touch_event = true;
}

/*******************************************************************************
* Function Name: ili2511_init
********************************************************************************
* Summary:
*  This function performs the necessary initialization of I2C based
*  ILI2511 capacitive touch panel driver.
*
* Parameters:
*  void
*
* Return:
*  cy_rslt_t: Result of the operation.
*
*******************************************************************************/
cy_rslt_t ili2511_init(void)
{
    cy_rslt_t result    = CY_RSLT_SUCCESS;
    uint8_t retry_count = RETRY_COUNT;

    result = i2c_init();
    if (CY_RSLT_SUCCESS == result)
    {
        enable_ili2511();
        do
        {
            /* To check whether the ILI2511 touch IC is alive or not */
            result = is_ili2511_alive();
            if (CY_RSLT_SUCCESS != result)
            {
                retry_count--;
            }
            else
            {
                retry_count = RESET_VAL;
            }
        } while (retry_count);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* Initialize the Touch panel interrupt pin */
        result = cyhal_gpio_init(CYBSP_DISP_TP_INT, CYHAL_GPIO_DIR_INPUT,
                CYHAL_GPIO_DRIVE_PULLUP, true);

        if (CY_RSLT_SUCCESS == result)
        {
            /* Configure GPIO interrupt */
            tp_int_callback_data.callback = tp_int_interrupt_handler;

            cyhal_gpio_register_callback(CYBSP_DISP_TP_INT,
                                             &tp_int_callback_data);
            cyhal_gpio_enable_event(CYBSP_DISP_TP_INT, CYHAL_GPIO_IRQ_FALL,
                    TP_INTERRUPT_PRIORITY, true);
        }
    }

    return result;
}

/*******************************************************************************
* Function Name: ili2511_get_single_touch
********************************************************************************
* Summary:
*  This function calls "ili2511_read_raw_touch_data" to get raw touch x,y
*  coordinate data and processes it. It applies the calibration on this
*  processed data and returns the actual touch coordinate.
*
* Parameters:
*  *touch_x: Pointer to the X coordinate of touched location on touch panel.
*  *touch_y: Pointer to the Y coordinate of touched location on touch panel.
*
* Return:
*  cy_rslt_t: Result of the operation.
*
*******************************************************************************/
cy_rslt_t ili2511_get_single_touch(int *touch_x, int *touch_y)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint8_t   touch_data[TP_DATA_NUM_BYTES] = {(uint8_t)RESET_VAL};
    uint32_t  current_touch_x = RESET_VAL;
    uint32_t  current_touch_y = RESET_VAL;

    status = ili2511_read_raw_touch_data(touch_data, sizeof(touch_data));

    if (CY_RSLT_SUCCESS == status)
    {
        /* To convert the x touch data from 8 bits to 32 bits */
        current_touch_x = (touch_data[TP_DATA_X_MSB_POS] << (BYTE_SHIFT)) | touch_data[TP_DATA_X_LSB_POS];
        current_touch_x = current_touch_x & (BIT_MASK);
        current_touch_y = (touch_data[TP_DATA_Y_MSB_POS] << (BYTE_SHIFT)) | touch_data[TP_DATA_Y_LSB_POS];
        current_touch_y = current_touch_y & (BIT_MASK);

        /* To check the last touch position is equal to the current touch position */
        if ((last_touch_x != current_touch_x)
                || (last_touch_y != current_touch_y))
        {
            last_touch_x = current_touch_x;
            last_touch_y = current_touch_y;

            /* Calibration the x and y coordinates to map with the LCD display */
            *touch_x = (int)(((current_touch_x * (WF101JTYAHMNB0_LCD_WIDTH)) / TP_CALIBRATION_VAL) * TP_CALIBRATION_OFFSET);
            *touch_y = (int)(((current_touch_y * (WF101JTYAHMNB0_LCD_HEIGHT)) / TP_CALIBRATION_VAL) * TP_CALIBRATION_OFFSET);
        }
        else
        {
            status = (cy_rslt_t) CY_TOUCH_PANEL_NO_TOUCH;
        }
    }

    return status;
}

/*******************************************************************************
* Function Name: ili2511_deinit
********************************************************************************
* Summary:
*  This function de-initializes the ILI2511 touch IC.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ili2511_deinit(void)
{

    cyhal_i2c_clear(&i2c_master_obj);
    cyhal_i2c_free(&i2c_master_obj);

    cyhal_gpio_write(CYBSP_DISP_TP_RST, false);
    cyhal_system_delay_ms(INITIALIZATION_TIME_MS);
}

/* [] END OF FILE */
