/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for CM33 Secure Project.
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
#include "app_mem_map.h"
#include "cyhal.h"
#include "cybsp.h"
#ifdef ENABLE_HAL_RETARGET_IO
#include "cy_retarget_io.h"
#endif
#include "partition_ARMCM33.h"
#include "cy_ppc.h"
#include "cy_mpc.h"
#include "cy_ms_ctl.h"
#ifdef RRAM_BOOT
#include "s_smif_xip_init.h"
#endif
#include "ifx_se_platform.h"
#include "ifx_se_crc32.h"
/*****************************************************************************
* Macros
*****************************************************************************/
#define NS_APP_BOOT_ADDR                 CY_CM33_NS_APP_BOOT_ADDR
#define CM33_NS_SP_STORE                 (NS_APP_BOOT_ADDR)
#define CM33_NS_RESET_HANDLER_STORE      (NS_APP_BOOT_ADDR + 4)

/* Protection context for cm33 */
#define CM33_NS_PC_VALUE                 (5u)
#define CM33_S_PC_VALUE                  (2u)

#define CY_IPC_EP_CYPIPE_CM33_ADDR       (1UL)    //EP1 Index of Endpoint Array

cy_stc_ppc_init_t ppcInit;
cy_stc_ppc_attribute_t ppcAttribute;
cy_stc_ppc_pc_mask_t pcMaskConfig;

/*****************************************************************************
* Function Definitions
*****************************************************************************/
typedef void (*funcptr_void) (void) __attribute__((cmse_nonsecure_call));
void ipc0_cm33_s_isr(void);
void early_config_ppc0(void);
void config_ppc0(void);
void config_ppc1(void);
cy_en_ms_ctl_status_t config_set_cm33_ns_pc(void);
void config_mpc(void);
void config_smif0_xip_ns(void);
void config_smif1_xip_ns(void);
void release_uart(void);

/*****************************************************************************
* Function Name: main
******************************************************************************
* This is the main function for Cortex M33 CPU secure application
*****************************************************************************/
int main(void)
{
    uint32_t ns_stack;
    funcptr_void NonSecure_ResetHandler;
    cy_rslt_t result;

    /* Trustzone setup */
    TZ_SAU_Setup();

#if defined (__FPU_USED) && (__FPU_USED == 1U) && \
      defined (TZ_FPU_NS_USAGE) && (TZ_FPU_NS_USAGE == 1U)
    /*FPU init*/
    initFPU();
#endif

    /* Set up internal routing, pins, and clock-to-peripheral connections */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (CY_RSLT_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

#ifdef ENABLE_HAL_RETARGET_IO
    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("****************** "
           "PSoC Edge MCU: CM33 Secure App start "
           "****************** \r\n\n");


#endif


    /*Enables the PD1 Power Domain*/
    Cy_System_EnablePD1();

    /* SPERI Regsters - Enable APP_MMIO_TCM IP through PERI[1].PERI2_0.PERI_GROUP_STRUCT[4].SL_CTL register */
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_CM55_TCM_512K_PERI_NR, CY_MMIO_CM55_TCM_512K_GROUP_NR, \
                                 CY_MMIO_CM55_TCM_512K_SLAVE_NR, CY_MMIO_CM55_TCM_512K_CLK_HF_NR);
#ifdef RRAM_BOOT
    Cy_SysClk_PeriGroupSlaveInit(CY_MMIO_SMIF0_PERI_NR, CY_MMIO_SMIF0_GROUP_NR, CY_MMIO_SMIF0_SLAVE_NR, CY_MMIO_SMIF0_CLK_HF_NR);


     /* Smif Init and enable XIP */
    Cy_SMIFXIPInit();
#endif
    Cy_SysEnableSOCMEM(true);

    /* configure mpc */
    config_mpc();


    //configure smif as NS
    config_smif0_xip_ns();
    config_smif1_xip_ns();

    /* set nnlite pc for nnlite bus master */
    Cy_Ms_Ctl_SetActivePC(CPUSS_MS_ID_EXP_SYSCPUSS_MS_0, CY_MPC_PC_5);
    /* set nnlite pc for nnlite bus masters */
    Cy_Ms_Ctl_SetActivePC(CPUSS_MS_ID_CODE_MS_0, CY_MPC_PC_5);
    Cy_Ms_Ctl_SetActivePC(CPUSS_MS_ID_EXP_SYSCPUSS_MS_2, CY_MPC_PC_5);

    ns_stack = (uint32_t)(*((uint32_t*)CM33_NS_SP_STORE));
    __TZ_set_MSP_NS(ns_stack);

    NonSecure_ResetHandler = (funcptr_void)(*((uint32_t*)CM33_NS_RESET_HANDLER_STORE));
#ifdef ENABLE_HAL_RETARGET_IO
    printf("CM33 NS image address 0x%lx, ns_stack 0x%lx, handler %p\n", (uint32_t)CM33_NS_SP_STORE, ns_stack, NonSecure_ResetHandler);
    /* De-Initialize retarget-io */
    release_uart();
#endif

    /* Configure PPC0 */
    config_ppc0();

    /* Configure PPC0 */
    config_ppc1();

    /* change pc value for cm33-ns */
    config_set_cm33_ns_pc();

    /* Start non-secure application */
    NonSecure_ResetHandler();

    for (;;)
    {
    }
}
/*****************************************************************************
* Function Name: config_sram0_ns
******************************************************************************
*
*****************************************************************************/
void config_sram0_ns(void)
{
    cy_stc_mpc_rot_cfg_t rotConfig;

    rotConfig.addrOffset = CY_SRAM0_NS_OFFSET;
    rotConfig.size = CY_SRAM0_NS_SIZE ;
    rotConfig.regionSize = CY_MPC_SIZE_4KB ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    rotConfig.pc = CY_MPC_PC_2 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC0_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_5 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC0_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC0_MPC0, &rotConfig);
}
/*****************************************************************************
* Function Name: config_sram1_ns
******************************************************************************
*
*****************************************************************************/
void config_sram1_ns(void)
{
    cy_stc_mpc_rot_cfg_t rotConfig;

    rotConfig.addrOffset = CY_SRAM1_NS_OFFSET;
    rotConfig.size = CY_SRAM1_NS_SIZE;
    rotConfig.regionSize = CY_MPC_SIZE_4KB ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    rotConfig.pc = CY_MPC_PC_2 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC1_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_5 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC1_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC1_MPC0, &rotConfig);

    /* configure shared memory starting from offset 0x7E00 and size 8K */
    /* Default: PC 0/1/2/4 access these memory as secure */
    /* Explictly configure MPC to make these regions as non-secure for PC 5/6 */
    rotConfig.addrOffset = CY_SRAM1_SHM_NS_OFFSET; //SRAM1_SHM_OFFSET;
    rotConfig.size = CY_SRAM1_SHM_NS_SIZE; //SRAM1_SHARED_SIZE ;
    rotConfig.regionSize = CY_MPC_SIZE_4KB ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    rotConfig.pc = CY_MPC_PC_5 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC1_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct(RAMC1_MPC0, &rotConfig);
}

/*****************************************************************************
* Function Name: config_rram_nvm_main_ns
******************************************************************************
*
*****************************************************************************/
void config_rram_nvm_main_ns(void)
{
/*
For each nibble:
- bit[0] : NS (0 indicates secure, 1 indicates non-secure)
- bit[1] : R (0 indicates read access not allowed. 1 indicates read access allowed)
- bit[2] : W (0 indicates write access not allowed, 1 indicates write access allowed)
- bit[3] : Reserved
*/
    // To configure first 128KB of RRAM MAIN_NVM as non-secure

    ifx_se_mpc_rot_cfg_t rotConfig;
    ifx_se_mpc_rot_cfg_crc_t rotConfigCrc;

    rotConfig.addr_offset = CY_RRAM_MAIN_NVM_NS_OFFSET;
    rotConfig.size = CY_RRAM_MAIN_NVM_NS_SIZE;
    rotConfig.region_size = CY_MPC_SIZE_4KB ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    rotConfig.pc = CY_MPC_PC_2 ;
    rotConfigCrc.mpc_config = rotConfig;
    rotConfigCrc.crc = IFX_CRC32_CALC((uint8_t *)(&rotConfigCrc), sizeof(rotConfigCrc) - sizeof(uint32_t));
    ifx_se_mpc_config_rot_mpc_struct(&rotConfigCrc, IFX_SE_NULL_CTX);

    rotConfig.pc = CY_MPC_PC_5 ;
    rotConfigCrc.mpc_config = rotConfig;
    rotConfigCrc.crc = IFX_CRC32_CALC((uint8_t *)(&rotConfigCrc), sizeof(rotConfigCrc) - sizeof(uint32_t));
    ifx_se_mpc_config_rot_mpc_struct(&rotConfigCrc, IFX_SE_NULL_CTX);

    rotConfig.pc = CY_MPC_PC_6 ;
    rotConfigCrc.mpc_config = rotConfig;
    rotConfigCrc.crc = IFX_CRC32_CALC((uint8_t *)(&rotConfigCrc), sizeof(rotConfigCrc) - sizeof(uint32_t));
    ifx_se_mpc_config_rot_mpc_struct(&rotConfigCrc, IFX_SE_NULL_CTX);

}

/*****************************************************************************
* Function Name: config_socmemram_ns
******************************************************************************
*
*****************************************************************************/
void config_socmemram_ns(void)
{
/*
For each nibble:
- bit[0] : NS (0 indicates secure, 1 indicates non-secure)
- bit[1] : R (0 indicates read access not allowed. 1 indicates read access allowed)
- bit[2] : W (0 indicates write access not allowed, 1 indicates write access allowed)
- bit[3] : Reseved
*/
// To configure entire SOCMEMSRAM as non-secure

    cy_stc_mpc_rot_cfg_t rotConfig;

    rotConfig.addrOffset = CY_SOCMEMSRAM_NS_OFFSET;
    rotConfig.size = CY_SOCMEMSRAM_NS_SIZE;
    rotConfig.regionSize = CY_MPC_SIZE_8KB ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    rotConfig.pc = CY_MPC_PC_2 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SOCMEM_SRAM_MPC0, &rotConfig);
    rotConfig.pc = CY_MPC_PC_5 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SOCMEM_SRAM_MPC0, &rotConfig);
    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SOCMEM_SRAM_MPC0, &rotConfig);
}

/*****************************************************************************
* Function Name: config_mpc
******************************************************************************
*
*****************************************************************************/
/** Configure MPC for Non-Sec Core */
void config_mpc(void)
{
    config_sram0_ns();
    config_sram1_ns();
    config_rram_nvm_main_ns();
    config_socmemram_ns();
    Cy_Mpc_SetViolationResponse((MPC_Type*)SOCMEM_SRAM_MPC0, true);
}


/*****************************************************************************
* Function Name: config_ppc0
******************************************************************************
* Note: See Product TRM for information about the PPC regions
*****************************************************************************/
void config_ppc0(void)
{
    PPC_Type* ppcPtr;
    cy_en_ppc_status_t ppcStatus;

    /* Configure PPC0 for CM33 access */
    ppcPtr = PPC0;

    // Initialize PPC
    ppcInit.respConfig = CY_PPC_BUS_ERR;
    ppcStatus = Cy_Ppc_InitPpc(ppcPtr, &ppcInit);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    ppcAttribute.secAttribute = CY_PPC_NON_SECURE;
    ppcAttribute.secPrivAttribute = CY_PPC_SEC_NONPRIV;
    ppcAttribute.nsPrivAttribute = CY_PPC_NON_SEC_NONPRIV;


    /*Peri 0,  Address 0x42000000*/
    ppcAttribute.startRegion = PROT_PERI0_MAIN;
    ppcAttribute.endRegion = PROT_PERI0_MAIN;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42004040 - 0x42004140 */
    ppcAttribute.startRegion = PROT_PERI0_GR1_GROUP;
    ppcAttribute.endRegion = PROT_PERI0_GR5_GROUP;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /*Peri 0,  Address 0x42008000 */
    ppcAttribute.startRegion = PROT_PERI0_TR;
    ppcAttribute.endRegion = PROT_PERI0_TR;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0,  Address 0x42040000*/
    ppcAttribute.startRegion = PROT_PERI0_PERI_PCLK0_MAIN;
    ppcAttribute.endRegion = PROT_PERI0_PERI_PCLK0_MAIN;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0,  Address 0x42204000*/
    ppcAttribute.startRegion = PROT_PERI0_RRAMC0_RRAM_EXTRA_AREA_RRAMC_GENERAL;
    ppcAttribute.endRegion = PROT_PERI0_RRAMC0_RRAM_EXTRA_AREA_RRAMC_GENERAL;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0,  Address 0x42210000 - 0x42210020 */
    ppcAttribute.startRegion = PROT_PERI0_RRAMC0_RRAMC0_RRAMC_USER;
    ppcAttribute.endRegion = PROT_PERI0_RRAMC0_RRAMC0_RRAMC_ALLUSER;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42213000*/
    ppcAttribute.startRegion = PROT_PERI0_RRAMC0_RRAM_SFR_RRAMC_SFR_USER;
    ppcAttribute.endRegion = PROT_PERI0_RRAMC0_RRAM_SFR_RRAMC_SFR_USER;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42220000 - 0x42230000 */
    ppcAttribute.startRegion = PROT_PERI0_M33SYSCPUSS;
    ppcAttribute.endRegion = PROT_PERI0_RAMC1_CM33;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42240100 - 0x42240200 */
    ppcAttribute.startRegion = PROT_PERI0_RAMC0_RAM_PWR;
    ppcAttribute.endRegion = PROT_PERI0_RAMC1_RAM_PWR;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);
    
    /* Peri 0, Address 0x42260000*/
    ppcAttribute.startRegion = PROT_PERI0_MXCM33_CM33;
    ppcAttribute.endRegion = PROT_PERI0_MXCM33_CM33;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);
    
    /* Peri 0, Address 0x42261004*/
    ppcAttribute.startRegion = PROT_PERI0_MXCM33_CM33_NS;
    ppcAttribute.endRegion = PROT_PERI0_MXCM33_CM33_NS;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42268000 - 0x42290000*/
    ppcAttribute.startRegion = PROT_PERI0_MXCM33_CM33_INT;
    ppcAttribute.endRegion = PROT_PERI0_CPUSS_ALL_PC;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42290120 - 0x42290200*/
    ppcAttribute.startRegion = PROT_PERI0_CPUSS_CM33_NS;
    ppcAttribute.endRegion = PROT_PERI0_CPUSS_MSC_INT;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42295004 - 0x42296000*/
    ppcAttribute.startRegion = PROT_PERI0_MS_PC0_PRIV_MIR;
    ppcAttribute.endRegion = PROT_PERI0_MSC_ACG;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x422a0040 - 0x422a01e0*/
    ppcAttribute.startRegion = PROT_PERI0_IPC0_STRUCT2_IPC;
    ppcAttribute.endRegion = PROT_PERI0_IPC0_STRUCT15_IPC;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x422a1040 - 0x42400800 */
    ppcAttribute.startRegion = PROT_PERI0_IPC0_INTR_STRUCT2_INTR;
    ppcAttribute.endRegion = PROT_PERI0_SRSS_HIB_DATA;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42401000*/
    ppcAttribute.startRegion = PROT_PERI0_SRSS_MAIN;
    ppcAttribute.endRegion = PROT_PERI0_SRSS_MAIN;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42404000 - 0x42430000*/
    ppcAttribute.startRegion = PROT_PERI0_RAM_TRIM_SRSS_SRAM;
    ppcAttribute.endRegion = PROT_PERI0_DEBUG600_DEBUG600;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42440000 - 0x42a50000*/
    ppcAttribute.startRegion = PROT_PERI0_CRYPTO_MAIN;
    ppcAttribute.endRegion = PROT_PERI0_I3C;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 0, Address 0x42c10000 - 0x42d80000*/
    ppcAttribute.startRegion = PROT_PERI0_ETH0;
    ppcAttribute.endRegion = PROT_PERI0_MXNNLITE_1_0;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);

    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    // Set PC Mask
    pcMaskConfig.startRegion = PROT_PERI0_MAIN;
    pcMaskConfig.endRegion = PROT_PERI0_MXNNLITE_1_0;
    pcMaskConfig.pcMask = 0x64U; // max 8 pcs
    ppcStatus = Cy_Ppc_SetPcMask(ppcPtr, &pcMaskConfig);

    if(ppcStatus!=CY_PPC_SUCCESS) CY_ASSERT(0);
}


/*****************************************************************************
* Function Name: config_ppc1
******************************************************************************
*Note: See Product TRM for information about the PPC regions
*****************************************************************************/
void config_ppc1(void)
{
    PPC_Type* ppcPtr;
    cy_en_ppc_status_t ppcStatus;

    ppcPtr = PPC1;

    // Initialize PPC
    ppcInit.respConfig = CY_PPC_BUS_ERR;
    ppcStatus = Cy_Ppc_InitPpc(ppcPtr, &ppcInit);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    ppcAttribute.secAttribute = CY_PPC_NON_SECURE;
    ppcAttribute.secPrivAttribute = CY_PPC_SEC_NONPRIV;
    ppcAttribute.nsPrivAttribute = CY_PPC_NON_SEC_NONPRIV;
    
    /* Peri 1, Address 0x44000000 - 0x44004110 */
    ppcAttribute.startRegion = PROT_PERI1_MAIN;
    ppcAttribute.endRegion = PROT_PERI1_GR4_GROUP;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 1, Address 0x44008000 */
    ppcAttribute.startRegion = PROT_PERI1_TR;
    ppcAttribute.endRegion = PROT_PERI1_TR;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 1, Address 0x44040000 - 0x44161004 */
    ppcAttribute.startRegion = PROT_PERI1_PERI_PCLK1_MAIN;
    ppcAttribute.endRegion = PROT_PERI1_MXCM55_CM55_NS;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);
 
    /* Peri 1, Address 0x44168000 - 0x441c1000 */
    ppcAttribute.startRegion = PROT_PERI1_MXCM55_CM55_INT;
    ppcAttribute.endRegion = PROT_PERI1_APPCPUSS_AP;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 1, Address 0x441c50d0 - 0x44440800 */
    ppcAttribute.startRegion = PROT_PERI1_MS_CTL_MS_PC13_PRIV_MIR;
    ppcAttribute.endRegion = PROT_PERI1_SMIF0_CORE1_DEVICE;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 1, Address 0x44424000 - 0x44640000 */
    ppcAttribute.startRegion = PROT_PERI1_SMIF0_CORE0_SMIF_HSIOM_SMIF_PRT0_PRT;
    ppcAttribute.endRegion = PROT_PERI1_SOCMEM_MAIN;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 1, Address 0x44640200 - 0x44640200 */
    ppcAttribute.startRegion = PROT_PERI1_SOCMEM_SOCMEM_PWR;
    ppcAttribute.endRegion = PROT_PERI1_SOCMEM_SOCMEM_PWR;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

    /* Peri 1, Address 0x44650000 - 0x48040000 */
    ppcAttribute.startRegion = PROT_PERI1_SOCMEM_MAIN_PORT;
    ppcAttribute.endRegion = PROT_PERI1_DTCM;
    ppcStatus = Cy_Ppc_ConfigAttrib(ppcPtr, &ppcAttribute);
    
    CY_ASSERT(ppcStatus==CY_PPC_SUCCESS);

     // Set PC Mask
    pcMaskConfig.startRegion = PROT_PERI1_MAIN;
    pcMaskConfig.endRegion = PROT_PERI1_DTCM;
    pcMaskConfig.pcMask = 0x64U; // max 8 pcs
    ppcStatus = Cy_Ppc_SetPcMask(ppcPtr, &pcMaskConfig);

    if(ppcStatus!=CY_PPC_SUCCESS) CY_ASSERT(0);
}


/*****************************************************************************
* Function Name: config_smif0_xip_ns
******************************************************************************
*
*****************************************************************************/
void config_smif0_xip_ns(void)
{
    cy_stc_mpc_rot_cfg_t rotConfig;

    rotConfig.addrOffset = CY_SMIF0_MAIN_NVM_NS_OFFSET;
    rotConfig.size = CY_SMIF0_MAIN_NVM_NS_SIZE;
    rotConfig.regionSize = CY_MPC_SIZE_128KB ;
    rotConfig.pc = CY_MPC_PC_2 ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE0_SLOW_AHB_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_5 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE0_SLOW_AHB_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE0_SLOW_AHB_MPC0, &rotConfig);
    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE0_AXI_MPC0, &rotConfig);
}

/*****************************************************************************
* Function Name: config_smif1_xip_ns
******************************************************************************
*
*****************************************************************************/
void config_smif1_xip_ns(void)
{
    cy_stc_mpc_rot_cfg_t rotConfig;
    cy_stc_smif_context_t SMIF_CORE1_Context;

    static const cy_stc_smif_config_t SMIF_0_CORE_1_config =
    {
        .mode = (uint32_t)CY_SMIF_NORMAL,
        .deselectDelay = SMIF_0_CORE_0_DESELECT_DELAY,
        .rxClockSel = (uint32_t)CY_SMIF_SEL_INVERTED_FEEDBACK_CLK,
        .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
        .enable_internal_dll = false
    };

    rotConfig.addrOffset = CY_SMIF1_MAIN_NVM_NS_OFFSET;
    rotConfig.size = CY_SMIF1_MAIN_NVM_NS_SIZE;
    rotConfig.regionSize = CY_MPC_SIZE_128KB ;
    rotConfig.pc = CY_MPC_PC_2 ;
    rotConfig.secure = CY_MPC_NON_SECURE ;
    rotConfig.access = CY_MPC_ACCESS_RW ;

    /* Enable IP with default configuration */
    (void) Cy_SMIF_Init(SMIF0_CORE1, &SMIF_0_CORE_1_config, TIMEOUT_1_MS, &SMIF_CORE1_Context);
    Cy_SMIF_Enable(SMIF0_CORE1, &SMIF_CORE1_Context);

    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE1_SLOW_AHB_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_5 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE1_SLOW_AHB_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE1_SLOW_AHB_MPC0, &rotConfig);

    rotConfig.pc = CY_MPC_PC_6 ;
    Cy_Mpc_ConfigRotMpcStruct((MPC_Type*)SMIF0_CORE1_AXI_MPC0, &rotConfig);

    /* Disable IP as MPC configuration is complete */
    Cy_SMIF_Disable(SMIF0_CORE1);
}


/*****************************************************************************
* Function Name: config_set_cm33_ns_pc
******************************************************************************
*
*****************************************************************************/
cy_en_ms_ctl_status_t config_set_cm33_ns_pc(void)
{
    cy_en_ms_ctl_status_t mscStatus;

    /* set pc2 handler */
    mscStatus = Cy_Ms_Ctl_SetPcHandler(CM33_S_PC_VALUE, ipc0_cm33_s_isr);

    if (mscStatus == CY_MS_CTL_SUCCESS)
    {
            Cy_Ms_Ctl_SetSavedPC(CPUSS_MS_ID_CM33_0, CM33_NS_PC_VALUE);
            /* set active pc */
            mscStatus = Cy_Ms_Ctl_SetActivePC(CPUSS_MS_ID_CM33_0, CM33_NS_PC_VALUE);
    }

    return mscStatus;
}
/*******************************************************************************
* Function Name: ipc0_cm33_s_isr
****************************************************************************//**
*
* This is the interrupt service routine for the system pipe.
*
*******************************************************************************/
void ipc0_cm33_s_isr(void)
{
    Cy_SysLib_Delay(10);
    Cy_IPC_Pipe_ExecuteCallback(CY_IPC_EP_CYPIPE_CM33_ADDR);

     //set pc value to cm33-ns before returing from this isr
    Cy_Ms_Ctl_SetActivePC(CPUSS_MS_ID_CM33_0, CM33_NS_PC_VALUE);
}
#ifdef ENABLE_HAL_RETARGET_IO
/*******************************************************************************
* Function Name: release_uart
****************************************************************************//**
*
*******************************************************************************/
void release_uart(void)
{
    cy_retarget_io_deinit();

    Cy_GPIO_SetHSIOM_SecPin(P6_7_PORT, P6_7_PIN, 1);
    Cy_GPIO_SetHSIOM_SecPin(P6_5_PORT, P6_5_PIN, 1);

}
#endif

/* [] END OF FILE */
