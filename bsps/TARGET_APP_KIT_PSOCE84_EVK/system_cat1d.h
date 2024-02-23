/***************************************************************************//**
* \file system_cat1d.h
* \version 1.0
*
* \brief CAT1D Device system header file.
*
********************************************************************************
* \copyright
* Copyright 2016-2021 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#ifndef _SYSTEM_CAT1D_H_
#define _SYSTEM_CAT1D_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "system_pse84.h"
#include "partition_cat1d.h"

#define CY_SYSTEM_CPU_M0P    (__CORTEX_M == 0U)    /**< CM0+ core CPU Code */
#define CY_SYSTEM_CPU_M55     (__CORTEX_M == 55U)   /**< CM55 core CPU Code */
#define CY_SYSTEM_CPU_M33     (__CORTEX_M == 33U)   /**< CM33 core CPU Code */


/**
* \addtogroup group_system_config_user_settings_macro_1d
* \{
*/

#if !defined (CY_CORTEX_M33_SEC_APPL_ADDR)
/** The Cortex-M33 startup driver identifier */
    #define CY_CORTEX_M33_SEC_APPL_ADDR               (CY_CM33_SEC_APP_BOOT_ADDR)
#endif /* (CY_CORTEX_M33_SEC_APPL_ADDR) */

#if !defined (CY_CORTEX_M33_NS_APPL_ADDR)
/** The Cortex-M33 startup driver identifier */
    #define CY_CORTEX_M33_NS_APPL_ADDR               (CY_CM33_SEC_APP_BOOT_ADDR)
#endif /* (CY_CORTEX_M33_SEC_APPL_ADDR) */

#if !defined (CY_CORTEX_M55_APPL_ADDR)
/** The Cortex-M55 startup driver identifier */
    #define CY_CORTEX_M55_APPL_ADDR                   (CY_CM55_APP_BOOT_ADDR)
#endif /* (CY_CORTEX_M55_APPL_ADDR) */

/** \} group_system_config_user_settings_macro_1d */

/**
  \brief System Clock Frequency (Core Clock)
*/
extern uint32_t SystemCoreClock;

/**
  \brief Setup the microcontroller system.
   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.
   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);


/* TODO: Do we need to put this block inside CM33 core flag? */
/**
* \addtogroup group_system_config_core_status_macro_1d
* \{
*/
#define CY_SYS_CORE_STATUS_ACTIVE     (0x0U)    /**< The core is in Active mode */
#define CY_SYS_CORE_STATUS_SLEEP      (0x1U)    /**< The core is in sleep mode */
#define CY_SYS_CORE_STATUS_DEEPSLEEP  (0x3U)    /**< The core is in deep sleep mode */
#define CY_SYS_CORE_STATUS_INVALID    (0x2U)    /**< Invalid core state */

#define CY_SYS_CORE_WAIT_INFINITE     (0x0U)    /**< Wait in a loop for Infinite micro second */

/** \} group_system_config_core_status_macro_1d */

/**
* \addtogroup group_system_config_cm55_functions_1d
* \{
*/

#if (CY_SYSTEM_CPU_M33 == 1UL) || defined(CY_DOXYGEN)
uint32_t Cy_SysGetCM55Status(void);
void Cy_SysEnableCM55(uint32_t vectorTableOffset, uint32_t waitus);
void Cy_SysResetCM55(uint32_t waitus);
void Cy_SysEnableSOCMEM(bool enable);
void Cy_System_EnablePD1(void);
#endif /* (CY_SYSTEM_CPU_M33 == 1UL) || defined(CY_DOXYGEN) */
#if (CY_SYSTEM_CPU_M55 == 1UL) || defined(CY_DOXYGEN)
void Cy_SysEnableU55(bool enable);
#endif /* (CY_SYSTEM_CPU_M55 == 1UL) || defined(CY_DOXYGEN) */


/** \} group_system_config_cm55_functions_1d */

/**
* \addtogroup group_system_config_cm33_functions_1d
* \{
*/
#if (CY_SYSTEM_CPU_M0P == 1UL) || defined(CY_DOXYGEN)
uint32_t Cy_SysGetCM33Status(void);
void Cy_SysEnableCM33(uint32_t vectorTableOffset, uint32_t waitus);
void Cy_SysResetCM33(uint32_t waitus);
#endif /* (CY_SYSTEM_CPU_M0P == 1UL) || defined(CY_DOXYGEN) */


/**
 *****************************************************************************
 ** \brief  CBUS address remap
 **  remap address for masters on CBUS
 **
 ** \param [in]  addr  pointer to be remap
 **
 ** \retval remap address
 **
 *****************************************************************************/
static inline uint32_t cy_CbusRemapAddr(const void *addr)
{
  uint32_t remapAddr, offset;

  /* SRAM Address */
  if (((uint32_t)addr >= CY_SRAM_BASE) &&
      ((uint32_t)addr < (CY_SRAM_BASE + CY_SRAM_SIZE)))
  {
    offset = (uint32_t)addr - CY_SRAM_BASE;
    remapAddr = CY_SRAM_CBUS_BASE + offset;
  } /* SOCMEM Address */
  else if (((uint32_t)addr >= CY_SOCMEM_RAM_BASE) &&
      ((uint32_t)addr < (CY_SOCMEM_RAM_BASE + CY_SOCMEM_RAM_SIZE)))
  {

    offset = (uint32_t)addr - CY_SOCMEM_RAM_BASE;
    remapAddr = CY_SOCMEM_RAM_CBUS_BASE + offset;
  }
  /* XIP is mapped with offset */
  else if (((uint32_t)addr >= CY_XIP_PORT0_BASE) &&
      ((uint32_t)addr < (CY_XIP_PORT1_BASE + CY_XIP_PORT1_SIZE)))
  {
    offset = (uint32_t)addr - CY_XIP_PORT0_BASE;
    remapAddr =  CY_XIP_PORT0_CBUS_BASE + offset;
  }/* no remapping, addr not in range */
  else
  {
    remapAddr = (uint32_t)addr;
  }
  return remapAddr;
}

/**
 *****************************************************************************
 ** \brief  AHB address remap
 **  remap address for masters
 **
 ** \param [in]  addr  pointer to be remap
 **
 ** \retval remap address
 **
 *****************************************************************************/
static inline uint32_t cy_AhbRemapAddr(const void *addr)
{
  uint32_t remapAddr, offset;

  /* SRAM Address */
  if (((uint32_t)addr >= CY_SRAM_CBUS_BASE) &&
      ((uint32_t)addr < (CY_SRAM_CBUS_BASE + CY_SRAM_SIZE)))
  {
    offset = (uint32_t)addr - CY_SRAM_CBUS_BASE;
    remapAddr = CY_SRAM_BASE + offset;
  } /* SOCMEM Address */
  else if (((uint32_t)addr >= CY_SOCMEM_RAM_CBUS_BASE) &&
      ((uint32_t)addr < (CY_SOCMEM_RAM_CBUS_BASE + CY_SOCMEM_RAM_SIZE)))
  {

    offset = (uint32_t)addr - CY_SOCMEM_RAM_CBUS_BASE;
    remapAddr = CY_SOCMEM_RAM_BASE + offset;
  }
  /* XIP is mapped with offset */
  else if (((uint32_t)addr >= CY_XIP_PORT0_CBUS_BASE) &&
      ((uint32_t)addr < (CY_XIP_PORT1_CBUS_BASE + CY_XIP_PORT1_SIZE)))
  {
    offset = (uint32_t)addr - CY_XIP_PORT0_CBUS_BASE;
    remapAddr =  CY_XIP_PORT0_BASE + offset;
  }/* no remapping, addr not in range */
  else
  {
    remapAddr = (uint32_t)addr;
  }
  return remapAddr;
}


#if defined(CORE_NAME_CM55_0)
/**
 *****************************************************************************
 ** \brief  DTCM address remap
 **  remap address for masters
 **
 ** \param [in]  addr  pointer to be remap
 **
 ** \retval remap address
 **
 *****************************************************************************/
#define CY_CM55_DTCM_REMAP_BASE 0x48040000UL

static inline uint32_t cy_DTCMRemapAddr(const void *addr)
{
  uint32_t remapAddr, offset;

  /* DTCM  Address */
  if (((uint32_t)addr >= CY_CM55_DTCM_INTERNAL_BASE) &&
      ((uint32_t)addr < (CY_CM55_DTCM_INTERNAL_BASE + CY_CM55_DTCM_INTERNAL_SIZE)))
  {
    offset = (uint32_t)addr - CY_CM55_DTCM_INTERNAL_BASE;
    remapAddr = CY_CM55_DTCM_REMAP_BASE + offset;
  } /* DTCM Address */
  else
  {
    remapAddr = (uint32_t)addr;
  }
  return remapAddr;
}
#endif

/**
 *****************************************************************************
 ** \brief  DTCM/CBUS address remap
 **  remap address for masters
 **
 ** \param [in]  addr  pointer to be remap
 **
 ** \retval remap address
 **
 *****************************************************************************/
static inline void * cy_PlatformRemapAddr(const void *addr)
{
    #if defined(CORE_NAME_CM33_0)
        return (void *)cy_AhbRemapAddr(addr);
    #endif

    #if defined(CORE_NAME_CM55_0)
        return (void *)cy_DTCMRemapAddr(addr);
    #endif

    return (void *)addr;
}
/** \} group_system_config_cm33_functions_1d */

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_CAT1D_H_ */


/* [] END OF FILE */



