/***************************************************************************//**
* \file pse84_common.ld
* \version 1.0.0
*
* Linker file for the GNU C compiler.
*
* The main purpose of the linker script is to describe how the sections in the
* input files should be mapped into the output file, and to control the memory
* layout of the output file.
*
* \note The entry point location starts at 0x14000000. The valid
* application image should be placed there.
*
* \note The linker files included with the PDL template projects must be generic
* and handle all common use cases. Your project may not use every section
* defined in the linker files. In that case you may see warnings during the
* build process. In your project, you can simply comment out or remove the
* relevant code in the linker file.
*
********************************************************************************
* \copyright
* Copyright 2023, Cypress Semiconductor Corporation (an Infineon company)
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

/*-----------------------------------------------------------------------------------*/
/* Common definitions for the device                                                 */
/*-----------------------------------------------------------------------------------*/

ITCM_NS_START                    = 0x00000000;
ITCM_S_START                     = 0x10000000;
ITCM_SIZE                        = 0x00040000;

DTCM_NS_START                    = 0x20000000;
DTCM_S_START                     = 0x30000000;
DTCM_SIZE                        = 0x00040000;

RRAM_NVM_MAIN_NS_START           = 0x22000000;
RRAM_NVM_MAIN_S_START            = 0x32000000;
RRAM_NVM_MAIN_NS_CBUS_START      = 0x02000000;
RRAM_NVM_MAIN_S_CBUS_START       = 0x12000000;
RRAM_NVM_MAIN_SIZE               = 0x00040000;

RRAM_NVM_SECURE_NS_START         = 0x23C00000;
RRAM_NVM_SECURE_S_START          = 0x33C00000;
RRAM_NVM_SECURE_NS_CBUS_START    = 0x03C00000;
RRAM_NVM_SECURE_S_CBUS_START     = 0x13C00000;
RRAM_NVM_SECURE_SIZE             = 0x00040000;

SRAM0_NS_SAHB_START              = 0x24000000;
SRAM0_S_SAHB_START               = 0x34000000;
SRAM0_NS_CBUS_START              = 0x04000000;
SRAM0_S_CBUS_START               = 0x14000000;
SRAM0_SIZE                       = 0x00080000;

SRAM1_NS_SAHB_START              = 0x24080000;
SRAM1_S_SAHB_START               = 0x34080000;
SRAM1_NS_CBUS_START              = 0x04080000;
SRAM1_S_CBUS_START               = 0x14080000;
SRAM1_SIZE                       = 0x00080000;

SOCMEMSRAM_NS_SAHB_START         = 0x26000000;
SOCMEMSRAM_S_SAHB_START          = 0x36000000;
SOCMEMSRAM_NS_CBUS_START         = 0x06000000;
SOCMEMSRAM_S_CBUS_START          = 0x16000000;
SOCMEMSRAM_SIZE                  = 0x00500000;

XIP0_NS_SAHB_START               = 0x60000000;
XIP0_S_SAHB_START                = 0x70000000;
XIP0_NS_CBUS_START               = 0x08000000;
XIP0_S_CBUS_START                = 0x18000000;
XIP0_SIZE                        = 0x04000000;

XIP1_NS_START                    = 0x64000000;
XIP1_S_START                     = 0x74000000;
XIP1_NS_CBUS_START               = 0x0C000000;
XIP1_S_CBUS_START                = 0x1C000000;
XIP1_SIZE                        = 0x04000000;

/*-----------------------------------------------------------------------------------*/

CM0P_RAMVECTORS_ALIGNMENT        = 1024;
CM33_RAMVECTORS_ALIGNMENT        = 1024;
CM55_RAMVECTORS_ALIGNMENT        = 1024;

CM33S_MSP_STACK_SIZE             = 0x00000800;
CM33NS_MSP_STACK_SIZE            = 0x00000800;
CM55_MSP_STACK_SIZE              = 0x00000800;

MSP_STACK_SIZE                   = 0x00000800;
HEAP_SIZE                        = 0x000F0000; /* 960 KB */
CODE_NSC_SIZE                    = 0x00000100;

CM33S_APP_HEADER_SIZE            = 0x00000400;
CM33NS_APP_HEADER_SIZE           = 0x00000400;
CM55NS_APP_HEADER_SIZE           = 0x00000400;

SRAM0_SHARED_SIZE                = 0x00002000;   /* 8K memory reserved for cm0 and cm33-sec IPC data */
SRAM0_SHARED_CM0_SIZE            = 0x00001000;   /* Out of 8K shared memory, 4K is reserved for CM0 */

SRAM1_SHARED_SIZE                = 0x00002000;   /* 8K memory reserved for cm33-s, cm33-ns and cm55 IPC data */
SRAM1_SHARED_CM33NS_SIZE         = 0x00000800;   /* Out of 8K shared memory, 2K is reserved for CM33-S */
SRAM1_SHARED_CM33S_SIZE          = 0x00000800;   /* Out of 8K shared memory, 2K is reserved for CM33-S */

SOCMEMSRAM_SHARED_SIZE           = 0x00100000;   /* 1M memory reserved for SHARED SOCMEM  */

XIP0_CM33S_IMAGE_OFFSET          = 0x00000000;
XIP0_CM33S_APP_OFFSET            = XIP0_CM33S_IMAGE_OFFSET + CM33S_APP_HEADER_SIZE;
XIP0_CM33S_APP_SIZE              = (51*1024) - CM33S_APP_HEADER_SIZE - CODE_NSC_SIZE;

XIP0_CM33NS_IMAGE_OFFSET         = 0x00340000;
XIP0_CM33NS_APP_OFFSET           = XIP0_CM33NS_IMAGE_OFFSET + CM33NS_APP_HEADER_SIZE;
XIP0_CM33NS_APP_SIZE             = 0x00232800 - CM33NS_APP_HEADER_SIZE;

XIP0_CM55NS_IMAGE_OFFSET         = 0x00580000;
XIP0_CM55NS_APP_OFFSET           = XIP0_CM55NS_IMAGE_OFFSET + CM55NS_APP_HEADER_SIZE;
XIP0_CM55NS_APP_SIZE             = 0x0032C800 - CM55NS_APP_HEADER_SIZE;

RRAM_CM33S_IMAGE_OFFSET          = 0x0000F000;
RRAM_CM33S_APP_OFFSET            = (RRAM_CM33S_IMAGE_OFFSET + CM33S_APP_HEADER_SIZE);
RRAM_CM33S_APP_SIZE              = 0x0001AB00; /* (108KB - 0x500) */

RRAM_CM33NS_IMAGE_OFFSET         = 0x0002A000;
RRAM_CM33NS_APP_OFFSET           = RRAM_CM33NS_IMAGE_OFFSET + CM33NS_APP_HEADER_SIZE;
RRAM_CM33NS_APP_SIZE             = 0;

RRAM_CM55NS_IMAGE_OFFSET         = 0x00040800;
RRAM_CM55NS_APP_OFFSET           = RRAM_CM55NS_IMAGE_OFFSET + CM55NS_APP_HEADER_SIZE;
RRAM_CM55NS_APP_SIZE             = 0;

SRAM0_CM33S_IMAGE_OFFSET         = 0x00000000;
SRAM0_CM33S_APP_OFFSET           = SRAM0_CM33S_IMAGE_OFFSET + CM33S_APP_HEADER_SIZE;
SRAM0_CM33S_APP_SIZE             = 0x0000FB00;

SRAM0_CM33NS_IMAGE_OFFSET        = 0x00000000;
SRAM0_CM33NS_APP_OFFSET          = SRAM0_CM33NS_IMAGE_OFFSET;
SRAM0_CM33NS_APP_SIZE            = 0x00000000;

SRAM0_CM55NS_IMAGE_OFFSET        = 0x00000000;
SRAM0_CM55NS_APP_OFFSET          = SRAM0_CM55NS_IMAGE_OFFSET;
SRAM0_CM55NS_APP_SIZE            = 0x00000000;

SRAM0_CM33S_DATA_OFFSET          = 0x00010000;
SRAM0_CM33S_DATA_OFFSET_FLASH_BOOT = 0x00010000;
SRAM0_CM33S_DATA_SIZE            = 0x00010000 - SRAM0_SHARED_SIZE;

SRAM0_SHARED_OFFSET              = SRAM0_CM33S_DATA_OFFSET + SRAM0_CM33S_DATA_SIZE;
SRAM0_SHARED_OFFSET_FLASH_BOOT   = SRAM0_CM33S_DATA_OFFSET_FLASH_BOOT + SRAM0_CM33S_DATA_SIZE;

SRAM0_CM33NS_DATA_OFFSET         	= 0x00010000 + SRAM0_CM33S_DATA_OFFSET;
SRAM0_CM33NS_DATA_OFFSET_FLASH_BOOT	= 0x00010000 + SRAM0_CM33S_DATA_OFFSET_FLASH_BOOT;
SRAM0_CM33NS_DATA_SIZE            = SRAM0_SIZE - (SRAM0_SHARED_SIZE + SRAM0_SHARED_OFFSET);
SRAM0_CM33NS_DATA_SIZE_FLASH_BOOT = SRAM0_SIZE - (SRAM0_SHARED_SIZE + SRAM0_SHARED_OFFSET_FLASH_BOOT);

SRAM1_CM33S_IMAGE_OFFSET         = 0x00000000;
SRAM1_CM33S_APP_OFFSET           = SRAM1_CM33S_IMAGE_OFFSET;
SRAM1_CM33S_APP_SIZE             = 0x00000000;

SRAM1_CM33NS_IMAGE_OFFSET        = 0x00000000;
SRAM1_CM33NS_APP_OFFSET          = SRAM1_CM33NS_IMAGE_OFFSET + CM33NS_APP_HEADER_SIZE;
SRAM1_CM33NS_APP_SIZE            = 0x00040000 - SRAM1_CM33NS_APP_OFFSET;

SRAM1_CM55NS_IMAGE_OFFSET        = 0x00000000;
SRAM1_CM55NS_APP_OFFSET          = SRAM1_CM55NS_IMAGE_OFFSET;
SRAM1_CM55NS_APP_SIZE            = 0x00040000;

SRAM1_CM33S_DATA_OFFSET          = 0x00000000;
SRAM1_CM33S_DATA_SIZE            = 0x00000000;

SRAM1_CM33NS_DATA_OFFSET         = 0x00040000;
SRAM1_CM33NS_DATA_SIZE           = 0x00040000;

SRAM1_CM55NS_DATA_OFFSET         = 0x00000000;
SRAM1_CM55NS_DATA_SIZE           = 0x00000000;

SOCMEMSRAM_CM33S_IMAGE_OFFSET    = 0x00000000;
SOCMEMSRAM_SEC_APP_OFFSET        = SOCMEMSRAM_CM33S_IMAGE_OFFSET;
SOCMEMSRAM_SEC_APP_SIZE          = 0x00000000;
/*
SOCMEM Partition
--------------------------------------------------------------------
PARTITION               OFFSET                 SIZE
--------------------------------------------------------------------
CM55NS_APP            CM55NS_IMAGE_OFFSET     APP_SIZE
GPUBUF                GPUBUF_OFFSET           GPUBUF_SIZE
SHARED_MEM            SHARED_OFFSET           SHARED_SIZE
CM55NS_DATA           CM55NS_DATA_OFFSET      DATA_SIZE
CM33NS_APP            CM33NS_APP_OFFSET       CM33NS_APP_SIZE
CM33NS_DATA           CM33NS_DATA_OFFSET      CM33NS_DATA_SIZE
*/
SOCMEMSRAM_CM55NS_IMAGE_OFFSET   = 0x00000000;
SOCMEMSRAM_CM55NS_APP_OFFSET     = SOCMEMSRAM_CM55NS_IMAGE_OFFSET + CM55NS_APP_HEADER_SIZE;
SOCMEMSRAM_CM55NS_APP_SIZE       = 0x00000000;

SOCMEMSRAM_GPUBUF_OFFSET         = 0x00000000;
SOCMEMSRAM_GPUBUF_SIZE           = 0x00300000; /* 3 MB */

SOCMEMSRAM_SHARED_OFFSET         = 0x00300000;
SOCMEMSRAM_SHARED_SIZE           = 0x00100000; /* kept to 1 MB to utilize most of SOCMEM for gfx */

SOCMEMSRAM_CM55NS_DATA_OFFSET    = 0x00400000;
SOCMEMSRAM_CM55NS_DATA_SIZE      = 0x00100000; /* kept to 1 MB to utilize most of SOCMEM for gfx */

SOCMEMSRAM_CM33NS_IMAGE_OFFSET   = 0x00000000;
SOCMEMSRAM_CM33NS_APP_OFFSET     = SOCMEMSRAM_CM33NS_IMAGE_OFFSET;
SOCMEMSRAM_CM33NS_APP_SIZE       = 0x00000000; /* kept to 0 KB to utilize most of SOCMEM for gfx */

SOCMEMSRAM_CM33NS_DATA_OFFSET    = 0x00000000;
SOCMEMSRAM_CM33NS_DATA_SIZE      = 0x00000000; /* kept to 0 KB to utilize most of SOCMEM for gfx */
