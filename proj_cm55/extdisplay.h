/*******************************************************************************
 * File Name        : extdisplay.h
 *
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

#ifndef EXTDISPLAY_H_
#define EXTDISPLAY_H_

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "string.h"
#include "stdio.h"

/* I2C bus frequency */
#define I2C_FREQ                 (100000UL)

#define BUFFER_SIZE              (2UL)
#define ADAPTER_I2C_SCL          (P17_0)
#define ADAPTER_I2C_SDA          (P17_1)
#define HPD_PIN                  (P15_0) //D8
#define RESET_PIN                (P11_1) //D7

/* output mode */
#define _hdmi_output
#define _lvds_bypass

// #define _pattern_test_

#define _lane_swap_ // mipi input 3210
// #define _pn_swap_   //mipi input lane pn swap

#define dds_debug

/* lanes */
#define lane_cnt 2 /*0: 4lane; 1: 1lane; 2: 2lane; 3: 3lane;*/

typedef struct video_timing
{
    uint16_t hfp;
    uint16_t hs;
    uint16_t hbp;
    uint16_t hact;
    uint16_t htotal;
    uint16_t vfp;
    uint16_t vs;
    uint16_t vbp;
    uint16_t vact;
    uint16_t vtotal;
    bool h_polarity;
    bool v_polarity;
    uint16_t vic;
    uint8_t aspact_ratio; // 0=no data, 1=4:3, 2=16:9, 3=no data.
    uint32_t pclk_khz;
};

typedef enum
{
    I2S_2CH,
    I2S_8CH,
    SPDIF
} _Audio_Input_Mode;

#define Audio_Input_Mode I2S_2CH

#define AR_ND 0x00
#define AR_4_3 0x01
#define AR_16_9 0x02

typedef struct panel_parameter
{
    uint16_t hfp;
    uint16_t hs;
    uint16_t hbp;
    uint16_t hact;
    uint16_t htotal;
    uint16_t vfp;
    uint16_t vs;
    uint16_t vbp;
    uint16_t vact;
    uint16_t vtotal;
    uint32_t pclk_khz;
};

/*******************************************************************************
 * Function Definitions
 *******************************************************************************/
int ext_display_adapter_writebyte(uint8_t reg_addr, uint8_t value);
uint8_t ext_display_adapter_readbyte(uint8_t reg_addr);
int ext_display_adapter_init(void);
void ext_display_adapter_reset(void);
void ext_display_adapter_configure(void);

#endif /* EXTDISPLAY_H_ */
