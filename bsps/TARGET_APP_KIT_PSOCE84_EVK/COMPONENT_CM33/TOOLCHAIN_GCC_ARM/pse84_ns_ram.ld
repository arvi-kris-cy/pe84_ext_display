/***************************************************************************//**
* \file pse84_ns_ram.ld
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
ENTRY(Reset_Handler)

INCLUDE pse84_common.ld

CODE_XIP0_OFFSET            = XIP0_CM33NS_APP_OFFSET;
CODE_XIP0_VMA               = XIP0_NS_CBUS_START + CODE_XIP0_OFFSET;
CODE_XIP0_LMA               = XIP0_NS_SAHB_START + CODE_XIP0_OFFSET;
CODE_XIP0_SIZE              = XIP0_CM33NS_APP_SIZE;

CODE_RRAM_OFFSET            = RRAM_CM33NS_APP_OFFSET;
CODE_RRAM_VMA               = RRAM_NVM_MAIN_NS_CBUS_START + CODE_RRAM_OFFSET;
CODE_RRAM_LMA               = RRAM_NVM_MAIN_NS_START + CODE_RRAM_OFFSET;
CODE_RRAM_SIZE              = RRAM_CM33NS_APP_SIZE;

CODE_SRAM0_OFFSET           = SRAM0_CM33NS_APP_OFFSET;
CODE_SRAM0_VMA              = SRAM0_NS_CBUS_START + CODE_SRAM0_OFFSET;
CODE_SRAM0_LMA              = SRAM0_NS_SAHB_START + CODE_SRAM0_OFFSET;
CODE_SRAM0_SIZE             = SRAM0_CM33NS_APP_SIZE;

CODE_SRAM1_OFFSET           = SRAM1_CM33NS_APP_OFFSET;
CODE_SRAM1_VMA              = SRAM1_NS_CBUS_START + CODE_SRAM1_OFFSET;
CODE_SRAM1_LMA              = SRAM1_NS_SAHB_START + CODE_SRAM1_OFFSET;
CODE_SRAM1_SIZE             = SRAM1_CM33NS_APP_SIZE;

CODE_SOCMEMSRAM_OFFSET      = SOCMEMSRAM_CM33NS_APP_OFFSET;
CODE_SOCMEMSRAM_VMA         = SOCMEMSRAM_NS_CBUS_START + CODE_SOCMEMSRAM_OFFSET;
CODE_SOCMEMSRAM_LMA         = SOCMEMSRAM_NS_SAHB_START + CODE_SOCMEMSRAM_OFFSET;
CODE_SOCMEMSRAM_SIZE        = SOCMEMSRAM_CM33NS_APP_SIZE;
SOCMEMSRAM_SHARED_LMA       = (SOCMEMSRAM_NS_SAHB_START + SOCMEMSRAM_SHARED_OFFSET);

DATA_SRAM0_OFFSET           = SRAM0_CM33NS_DATA_OFFSET;
DATA_SRAM0_VMA              = SRAM0_NS_SAHB_START + DATA_SRAM0_OFFSET;
DATA_SRAM0_LMA              = SRAM0_NS_SAHB_START + DATA_SRAM0_OFFSET;
DATA_SRAM0_SIZE             = SRAM0_CM33NS_DATA_SIZE;

DATA_SRAM1_OFFSET           = SRAM1_CM33NS_DATA_OFFSET;
DATA_SRAM1_VMA              = SRAM1_NS_SAHB_START + DATA_SRAM1_OFFSET;
DATA_SRAM1_LMA              = SRAM1_NS_SAHB_START + DATA_SRAM1_OFFSET;
DATA_SRAM1_SIZE             = SRAM1_CM33NS_DATA_SIZE - SRAM1_SHARED_SIZE;

DATA_SOCMEMSRAM_OFFSET      = SOCMEMSRAM_CM33NS_DATA_OFFSET;
DATA_SOCMEMSRAM_VMA         = SOCMEMSRAM_NS_SAHB_START + DATA_SOCMEMSRAM_OFFSET;
DATA_SOCMEMSRAM_LMA         = SOCMEMSRAM_NS_SAHB_START + DATA_SOCMEMSRAM_OFFSET;
DATA_SOCMEMSRAM_SIZE        = SOCMEMSRAM_CM33NS_DATA_SIZE;

SRAM0_SHARED_CM33_SIZE      = SRAM0_SHARED_SIZE - SRAM0_SHARED_CM0_SIZE;
SRAM0_SHARED_LMA            = SRAM0_S_SAHB_START + SRAM0_SIZE - SRAM0_SHARED_CM33_SIZE;
SRAM1_SHARED_LMA            = (SRAM1_NS_SAHB_START + SRAM1_SIZE - SRAM1_SHARED_SIZE) + SRAM1_SHARED_CM33S_SIZE;

RAMVECTORS_ALIGNMENT        = CM33_RAMVECTORS_ALIGNMENT;
/*-----------------------------------------------------------------------------------*/
/* Modify the following values according to the memory in which the code needs to go */
/* E.g. CODE_1_VMA should always point to the memory in which the main code sits.    */
/* If main code needs to reside in externl flash then set it to CODE_XIP0_VMA        */
/*-----------------------------------------------------------------------------------*/

CODE_3_VMA                  = CODE_XIP0_VMA;
CODE_3_LMA                  = CODE_XIP0_LMA;
CODE_3_SIZE                 = CODE_XIP0_SIZE;

CODE_4_VMA                  = CODE_RRAM_VMA;
CODE_4_LMA                  = CODE_RRAM_LMA;
CODE_4_SIZE                 = CODE_RRAM_SIZE;

CODE_2_VMA                  = CODE_SRAM0_VMA;
CODE_2_LMA                  = CODE_SRAM0_LMA;
CODE_2_SIZE                 = CODE_SRAM0_SIZE;

CODE_1_VMA                  = CODE_SRAM1_VMA;
CODE_1_LMA                  = CODE_SRAM1_LMA;
CODE_1_SIZE                 = CODE_SRAM1_SIZE;

CODE_5_VMA                  = CODE_SOCMEMSRAM_VMA;
CODE_5_LMA                  = CODE_SOCMEMSRAM_LMA;
CODE_5_SIZE                 = CODE_SOCMEMSRAM_SIZE;

DATA_2_VMA                  = DATA_SRAM0_VMA;
DATA_2_LMA                  = DATA_SRAM0_LMA;
DATA_2_SIZE                 = DATA_SRAM0_SIZE;

DATA_1_VMA                  = DATA_SRAM1_VMA;
DATA_1_LMA                  = DATA_SRAM1_LMA;
DATA_1_SIZE                 = DATA_SRAM1_SIZE;

DATA_3_VMA                  = DATA_SOCMEMSRAM_VMA;
DATA_3_LMA                  = DATA_SOCMEMSRAM_LMA;
DATA_3_SIZE                 = DATA_SOCMEMSRAM_SIZE;

SHARED_MEM0_LMA             = SRAM1_SHARED_LMA;
SHARED_MEM0_VMA             = SRAM1_SHARED_LMA;
SHARED_MEM0_SIZE            = SRAM1_SHARED_SIZE;

SHARED_MEM1_LMA             = SOCMEMSRAM_SHARED_LMA;
SHARED_MEM1_VMA             = SOCMEMSRAM_SHARED_LMA;
SHARED_MEM1_SIZE            = SOCMEMSRAM_SHARED_SIZE;

__app_text1_vma__           = CODE_1_VMA;

__app_text2_vma__           = CODE_2_VMA;

__app_text3_vma__           = CODE_3_VMA;

__app_text4_vma__           = CODE_4_VMA;

__app_text5_vma__           = CODE_5_VMA;

__data1_vma__               = DATA_1_VMA;
__data2_vma__               = DATA_2_VMA;
__data3_vma__               = DATA_3_VMA;

/*-----------------------------------------------------------------------------------*/

MEMORY
{
    code_1        (rx)     : ORIGIN = CODE_1_VMA,     LENGTH = CODE_1_SIZE
    code_2        (rx)     : ORIGIN = CODE_2_VMA,     LENGTH = CODE_2_SIZE
    code_3        (rx)     : ORIGIN = CODE_3_VMA,     LENGTH = CODE_3_SIZE
    code_4        (rx)     : ORIGIN = CODE_4_VMA,     LENGTH = CODE_4_SIZE
    code_5        (rx)     : ORIGIN = CODE_5_VMA,     LENGTH = CODE_5_SIZE
    data_1        (rwx)    : ORIGIN = DATA_1_VMA,     LENGTH = DATA_1_SIZE
    data_2        (rwx)    : ORIGIN = DATA_2_VMA,     LENGTH = DATA_2_SIZE
    data_3        (rwx)    : ORIGIN = DATA_3_VMA,     LENGTH = DATA_3_SIZE
    shared_mem0   (rwx)    : ORIGIN = SHARED_MEM0_LMA LENGTH = SHARED_MEM0_SIZE /* shared_ram0 */
    shared_mem1   (rxw)    : ORIGIN = SHARED_MEM1_LMA LENGTH = SHARED_MEM1_SIZE /* shared_ram1 */
}

SECTIONS
{
    __app_text1_vma__ = CODE_1_VMA;
	__app_text1_lma__ = CODE_1_LMA;
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
    . = ALIGN(4);

    __copy_table_start__ = .;

    .copy.table : AT (__copy_table_start__ + (__app_text1_lma__ - __app_text1_vma__))
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__data1_lma__)
        LONG (__data1_vma__)
        LONG ((__data_end__ - __data_start__)/4)
        __copy_table_end__ = .;
    } > code_1

    . = ALIGN(4);
    __zero_table_start__ = . ;
    .zero.table : AT (__zero_table_start__ + (__app_text1_lma__ - __app_text1_vma__))
    {
        LONG (__bss_start__)
        LONG ((__bss_end__ - __bss_start__)/4)
        __zero_table_end__ = .;
    } > code_1
    __app_text1_lma_count__ = ( . - __app_text1_vma__ + __app_text1_lma__);

    __app_text2_lma__ = __app_text1_lma_count__;
    .appText_2 __app_text2_vma__ : AT (__app_text2_lma__)
    {
    } > code_2
    __app_text1_lma_count__ = ( . - __app_text2_vma__ + __app_text1_lma_count__);

    __app_text3_lma__ = __app_text1_lma_count__;
    .appText_3 __app_text3_vma__ : AT (__app_text3_lma__)
    {
    } > code_3
    __app_text1_lma_count__ = ( . - __app_text3_vma__ + __app_text1_lma_count__);

    __app_text4_lma__ = __app_text1_lma_count__;
    .appText_4 __app_text4_vma__ : AT(__app_text4_lma__)
    {
    }  > code_4
    __app_text1_lma_count__ = ( . - __app_text4_vma__ + __app_text1_lma_count__);

    __app_text5_lma__ = __app_text1_lma_count__;
    .appText_5 __app_text5_vma__ : AT(__app_text5_lma__ )
    {
    }  > code_5
    __app_text1_lma_count__ = ( . - __app_text5_vma__ + __app_text1_lma_count__);

    .cy_sram_code :
    {
        . = ALIGN(4);
         __cy_sram_code_vma_start__ = .;
        KEEP(*(.cy_sram_code))
        KEEP(*(.text.cy_os_common))
        . = ALIGN(4);
        __cy_sram_code_vma_end__ = .;
    } > data_1

    __data1_lma__ = __app_text1_lma_count__;

    .data __data1_vma__ : AT (__data1_lma__)
    {
	    __data_start__ = .;
        *(.data*)      /* Data memory */

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
    __app_text1_lma_count__ = (. - __data1_vma__ + __app_text1_lma_count__);

    .ram_vectors (NOLOAD):
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

    .cy_sharedmem (NOLOAD):
    {
        . = ALIGN(4);
        __public_ram_start__ = .;
        KEEP(*(.cy_sharedmem))
        . = ALIGN(4);
        __public_ram_end__ = .;
    } > shared_mem0

    .cy_shared_socmem (NOLOAD):
    {
        . = ALIGN(4);
        __public_ram_socmem_start__ = .;
        KEEP(*(.cy_shared_socmem))
        . = ALIGN(4);
        __public_ram_socmem_end__ = .;
    } > shared_mem1

    . = ALIGN(32);
    __StackTop = ORIGIN(data_1) + LENGTH(data_1);
    __StackLimit = __StackTop - MSP_STACK_SIZE;
    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

    __data2_lma__ = __app_text1_lma_count__;
    .data_2 __data2_vma__ : AT(__data2_lma__)
    {
        . = ALIGN(4);
         __cy_sram0_datans_vma_start__ = .;
        KEEP(*(.cy_sram0_data_ns))
        . = ALIGN(4);
        __cy_sram0_datans_vma_end__ = .;
    }  > data_2
    __app_text1_lma_count__ = (. - __data2_vma__ + __app_text1_lma_count__);

    __data3_lma__ = __app_text1_lma_count__;
    .data_3 __data3_vma__ : AT(__data3_lma__)
    {
        . = ALIGN(4);
         __cy_socmem_data_vma_start__ = .;
        KEEP(*(.cy_socmem_data))
        . = ALIGN(4);
        __cy_socmem_data_vma_end__ = .;
    }  > data_3
}
