/***************************************************************************//**
* \file pse84_s_flash.ld
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
* Copyright (c) (2020-2022), Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
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

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)
ENTRY(S_Reset_Handler)

INCLUDE pse84_common.ld

CODE_XIP0_OFFSET            = XIP0_CM33S_APP_OFFSET;
CODE_XIP0_VMA               = XIP0_S_CBUS_START + CODE_XIP0_OFFSET;
CODE_XIP0_LMA               = XIP0_S_SAHB_START + CODE_XIP0_OFFSET;
CODE_XIP0_SIZE              = XIP0_CM33S_APP_SIZE;

CODE_SRAM0_OFFSET           = SRAM0_CM33S_APP_OFFSET;
CODE_SRAM0_VMA              = SRAM0_S_CBUS_START + CODE_SRAM0_OFFSET;
CODE_SRAM0_LMA              = SRAM0_S_SAHB_START + CODE_SRAM0_OFFSET;
CODE_SRAM0_SIZE             = SRAM0_CM33S_APP_SIZE;

CODE_RRAM_OFFSET            = RRAM_CM33S_APP_OFFSET;
CODE_RRAM_VMA               = RRAM_NVM_MAIN_S_CBUS_START + CODE_RRAM_OFFSET;
CODE_RRAM_LMA               = RRAM_NVM_MAIN_S_START + CODE_RRAM_OFFSET;
CODE_RRAM_SIZE              = RRAM_CM33S_APP_SIZE;

DATA_SRAM0_OFFSET           = SRAM0_CM33S_DATA_OFFSET_FLASH_BOOT;
DATA_SRAM0_VMA              = SRAM0_S_SAHB_START + DATA_SRAM0_OFFSET;
DATA_SRAM0_LMA              = SRAM0_S_SAHB_START + DATA_SRAM0_OFFSET;
DATA_SRAM0_SIZE             = SRAM0_CM33S_DATA_SIZE;

SRAM0_SHARED_LMA            = SRAM0_S_SAHB_START + SRAM0_SHARED_OFFSET_FLASH_BOOT + SRAM0_SHARED_CM0_SIZE;
SRAM0_SHARED_CM33_SIZE      = SRAM0_SHARED_SIZE - SRAM0_SHARED_CM0_SIZE;

SRAM1_SHARED_LMA            = SRAM1_S_SAHB_START + SRAM1_SIZE - SRAM1_SHARED_SIZE;

RAMVECTORS_ALIGNMENT        = CM33_RAMVECTORS_ALIGNMENT;
MSP_STACK_SIZE              = CM33S_MSP_STACK_SIZE;
/*----------------------------------------*/

CODE_3_VMA                  = CODE_XIP0_VMA;
CODE_3_LMA                  = CODE_XIP0_LMA;
CODE_3_SIZE                 = CODE_XIP0_SIZE;

CODE_2_VMA                  = CODE_SRAM0_VMA;
CODE_2_LMA                  = CODE_SRAM0_LMA;
CODE_2_SIZE                 = CODE_SRAM0_SIZE;

CODE_1_VMA                  = CODE_RRAM_VMA;
CODE_1_LMA                  = CODE_RRAM_LMA;
CODE_1_SIZE                 = CODE_RRAM_SIZE;

CODE_NSC_VMA                = CODE_1_VMA + CODE_1_SIZE;
CODE_NSC_LMA                = CODE_1_LMA + CODE_1_SIZE;

DATA_1_VMA                  = DATA_SRAM0_VMA;
DATA_1_LMA                  = DATA_SRAM0_LMA;
DATA_1_SIZE                 = DATA_SRAM0_SIZE;

XIP_VMA                     = XIP0_S_CBUS_START;
XIP_LMA                     = XIP0_S_SAHB_START;
XIP_SIZE                    = XIP0_SIZE;

SHARED_MEM0_LMA             = SRAM0_SHARED_LMA;
SHARED_MEM0_VMA             = SRAM0_SHARED_LMA;
SHARED_MEM0_SIZE            = SRAM0_SHARED_CM33_SIZE;

SHARED_MEM1_LMA             = SRAM1_SHARED_LMA;
SHARED_MEM1_VMA             = SRAM1_SHARED_LMA;
SHARED_MEM1_SIZE            = SRAM1_SHARED_CM33S_SIZE;

__app_text1_vma__           = CODE_1_VMA;
__app_text1_lma__           = CODE_1_LMA;

__app_data1_vma__           = DATA_1_VMA;


MEMORY
{
    code_1      (rx)    : ORIGIN = CODE_1_VMA,        LENGTH = CODE_1_SIZE
    codeNsc     (rx)    : ORIGIN = CODE_NSC_VMA,      LENGTH = CODE_NSC_SIZE
    data_1      (rwx)   : ORIGIN = DATA_1_VMA,        LENGTH = DATA_1_SIZE
    shared_mem0 (rwx)   : ORIGIN = SHARED_MEM0_LMA    LENGTH = SHARED_MEM0_SIZE
    shared_mem1 (rxw)   : ORIGIN = SHARED_MEM1_VMA    LENGTH = SHARED_MEM1_SIZE
}

SECTIONS
{
    .appText_1 __app_text1_vma__ : AT (__app_text1_lma__)
    {
        KEEP(*(.vectors))
        *(.text*)
        KEEP(*(.text))      /* Program code */
        KEEP(*(.init))
        KEEP(*(.fini))
        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        /* Read only data */
        *(.rodata .rodata.* .constdata .constdata.* .conststring .conststring.*)
    } > code_1

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > code_1

    __exidx_start = .;

    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > code_1

    __exidx_end = .;

    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__app_data1_lma__)
        LONG (__data_start__)
        LONG ((__data_end__ - __data_start__)/4)
        __copy_table_end__ = .;
    } > code_1

    /* To clear multiple BSS sections,
     * uncomment .zero.table section and,
     * define __STARTUP_CLEAR_BSS_MULTIPLE in CAT1B devices */
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG ((__bss_end__ - __bss_start__)/4)
        __zero_table_end__ = .;
    } > code_1

    .gnu.sgstubs : AT(CODE_NSC_LMA)  ALIGN (32)
    {
        . = ALIGN(32);
        _start_sg = .;
        *(.gnu.sgstubs*)
        . = ALIGN(32);
        _end_sg = .;
    } > codeNsc

    __app_data1_lma__ = (__app_text1_lma__ + __zero_table_end__ - __app_text1_vma__);
    .data __app_data1_vma__ : AT (__app_data1_lma__)
    {
        __data_start__ = .;

        . = ALIGN(4);
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);

        __data_end__ = .;
    } > data_1

    .ramVectors (NOLOAD):
    {
        . = ALIGN(RAMVECTORS_ALIGNMENT);
        __ram_vectors_start__ = .;
        KEEP(*(.ram_vectors))
        __ram_vectors_end__   = .;
    } > data_1

    .noinit (NOLOAD) : ALIGN(8)
    {
        KEEP(*(.noinit))
    } > data_1

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > data_1

    .heap (NOLOAD):
    {
        . = ALIGN(8);
        __HeapBase = .;
        __end__ = .;
        end = __end__;
        KEEP(*(.heap*))
        . = ALIGN(8);
        . = ORIGIN(data_1) + LENGTH(data_1) - MSP_STACK_SIZE;
        __HeapLimit = .;
    } > data_1

    .cy_sharedmem_sec (NOLOAD):
    {
        . = ALIGN(4);
        __sec_ram_start__ = .;
        KEEP(*(.cy_sharedmem_sec))
        . = ALIGN(4);
        __sec_ram_end__ = .;
    } > shared_mem0

    .cy_sharedmem (NOLOAD):
    {
        . = ALIGN(4);
        __public_ram_start__ = .;
        KEEP(*(.cy_sharedmem))
        . = ALIGN(4);
        __public_ram_end__ = .;
    } > shared_mem1

    . = ALIGN(32);
    __StackTop = ORIGIN(data_1) + LENGTH(data_1);
    __StackLimit = __StackTop - MSP_STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
