/*******************************************************************************
* File Name: app_mem_map.h
* version 1.0
* Description: application memory mapping
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
********************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#ifndef APP_MEM_MAP_H
#define APP_MEM_MAP_H

#ifdef RRAM_BOOT

// SMIF0 QSPI
#define CY_SMIF0_MAIN_NVM_NS_OFFSET  0x00340000 //0x00280000
#define CY_SMIF0_MAIN_NVM_NS_SIZE    0x00CC0000 //0x04000000-0x00280000

/* 
// SRAM 0 S+NS
#define CY_SRAM0_NS_OFFSET           0x00055000 //0x00020000
#define CY_SRAM0_NS_SIZE             0x0002B000 //0x00060000
*/

//SRAM 1 NS
#define CY_SRAM1_NS_OFFSET           0x00000000
#define CY_SRAM1_NS_SIZE             0x0007E000

// SRAM 1 SHM
#define CY_SRAM1_SHM_NS_OFFSET       0x0007E000
#define CY_SRAM1_SHM_NS_SIZE         0x00002000

// RRAM
#define CY_RRAM_MAIN_NVM_NS_SIZE     0x00016000  //0x00001000
#define CY_RRAM_MAIN_NVM_NS_OFFSET   0x0002A000 //0x0001E000

// BOOT
#define CY_CM33_SEC_APP_BOOT_ADDR    0x1200F400
#define CY_CM33_NSC_ADDR             0x1201DF00
#define CY_CM33_NSC_SIZE             0x00000100
#define CY_CM33_NS_APP_BOOT_ADDR     0x60340400//0x60280400
#define CY_CM55_APP_BOOT_ADDR        0x60580400//0x60D00400

#else
    
#ifdef FLASH_BOOT

#define CY_SMIF0_MAIN_NVM_NS_OFFSET  0x00340000//0x00280000
#define CY_SMIF0_MAIN_NVM_NS_OFFSET  0x00CC0000
#define CY_CM33_SEC_APP_BOOT_ADDR    0x18100400
#define CY_CM33_NSC_ADDR             0x1810EF00
#define CY_CM33_NSC_SIZE             0x00000100
#define CY_CM33_NS_APP_BOOT_ADDR     0x60340400//0x60280400
#define CY_CM55_APP_BOOT_ADDR        0x60580400//0x60D00400

#endif //#ifdef FLASH_BOOT

#endif 

#endif