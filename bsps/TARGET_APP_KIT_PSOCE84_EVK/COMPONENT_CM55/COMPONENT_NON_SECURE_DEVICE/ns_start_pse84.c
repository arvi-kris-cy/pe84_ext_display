#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "startup_cat1d.h"
#include "cy_sysint.h"
#include "cy_syspm.h"
#include "cy_syslib.h"
#include "cmsis_compiler.h"

#define SCB_NS_CPACR_CP10_CP11_ENABLE      (0xFUL << 20u)

CY_MISRA_FP_BLOCK_START('MISRA C-2012 Rule 8.6', 3, \
'Checked manually. The definition is a part of linker script or application.')
CY_MISRA_DEVIATE_BLOCK_START('ARRAY_VS_SINGLETON', 1, \
'Checked manually. Using pointer as an array will not corrupt or misinterpret adjacent memory locations.')
CY_MISRA_DEVIATE_BLOCK_START('MISRA C-2012 Rule 18.1', 3, \
'Checked manually. Dereferencing a pointer to one beyond the end of an array will not result in undefined behaviour.')
CY_MISRA_DEVIATE_BLOCK_START('MISRA C-2012 Rule 18.3', 1, \
'Checked manually. Attempting to make comparisons between pointers will not result in undefined behaviour.')

/*
** Copyright (c) 2018 Arm Limited. All rights reserved.
*/

// Initial Setup
//----------------------------------------------------------------

#if defined (__ARMCC_VERSION)
extern uint32_t Region$$Table$$Base;
extern uint32_t Region$$Table$$Limit;
typedef  void(*pGenericFunction)(uint8_t *pSrc, uint8_t* pDst, uint32_t len);     /* typedef for the generic function pointers */
#endif

#define CY_SYSINT_EWIC_CTL            (0xE0047000U)	  /**< EWIC control register */
#define CY_SYSINT_EWIC_ENABLE_MSK     (0x1U) 		  /**< EWIC enable mask */

__WEAK void Reset_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
__WEAK void SVC_Handler(void);
void DebugMon_Handler(void);
__WEAK void PendSV_Handler(void);
__WEAK void SysTick_Handler(void);
void InterruptHandler(void);
void NMIException_Handler(void);
__WEAK void HardFault_Handler(void);
void delay_infinite(void);
void SysLib_FaultHandler(uint32_t const *faultStackAddr);
extern int main(void);

#if defined (__ARMCC_VERSION)
void __attribute__((optnone)) Cy_RuntimeInit(void);
#else
void Cy_RuntimeInit(void);
#endif

#if defined(__ARMCC_VERSION)
extern unsigned int Image$$ARM_LIB_STACK$$ZI$$Limit;
extern unsigned int Image$$ARM_LIB_STACK$$ZI$$Base;
extern void __main(void);
typedef void(* ExecFuncPtrRw)(void);
ExecFuncPtrRw __ns_vector_table_rw[VECTORTABLE_SIZE] __attribute__( ( section(".bss.noinit.RESET_RAM"))) __attribute__((aligned(VECTORTABLE_ALIGN)));
#elif defined (__GNUC__)
extern unsigned int __StackTop;
extern uint32_t __StackLimit;
typedef void(* ExecFuncPtrRw)(void);
ExecFuncPtrRw __ns_vector_table_rw[VECTORTABLE_SIZE]   __attribute__( ( section(".ram_vectors"))) __attribute__((aligned(VECTORTABLE_ALIGN)));
#elif defined (__ICCARM__)
extern unsigned int CSTACK$$Limit;                      /* for (default) One Region model */
extern void  __cmain();
ExecFuncPtrRw __ns_vector_table_rw[VECTORTABLE_SIZE]   __attribute__( ( section(".intvec_ram"))) __attribute__((aligned(VECTORTABLE_ALIGN)));
#else
    #error "An unsupported toolchain"
#endif  /* (__ARMCC_VERSION) */

void Cy_RuntimeInit(void)
{
    __PROGRAM_START();
}

void SysLib_FaultHandler(uint32_t const *faultStackAddr)
{
    Cy_SysLib_FaultHandler(faultStackAddr);
}

// Exception Vector Table & Handlers
//----------------------------------------------------------------
void NMIException_Handler(void)
{
    __asm volatile(
        "bkpt #10\n"
        "B .\n"
    );
}

__WEAK void HardFault_Handler(void)
{
    __asm (
        "MRS R0, CONTROL\n"
        "TST R0, #2\n"
        "ITE EQ\n"
        "MRSEQ R0, MSP\n"
        "MRSNE R0, PSP\n"
        "B SysLib_FaultHandler\n"
    );
}

void MemManage_Handler(void)        {while(true){}}
void BusFault_Handler(void)         {while(true){}}
void UsageFault_Handler(void)       {while(true){}}
__WEAK void SVC_Handler(void)       {while(true){}}
void DebugMon_Handler(void)         {while(true){}}
__WEAK void PendSV_Handler(void)    {while(true){}}
__WEAK void SysTick_Handler(void)   {while(true){}}

void InterruptHandler(void)
{
    __asm volatile(
        "bkpt #1\n"
        "B .\n"
    );
}

ExecFuncPtr __ns_vector_table[] __VECTOR_TABLE_ATTRIBUTE = {
    (ExecFuncPtr)&__INITIAL_SP,           // initial SP
    (ExecFuncPtr)Reset_Handler,           // initial PC/Reset
    (ExecFuncPtr)NMIException_Handler,
    (ExecFuncPtr)HardFault_Handler,
    (ExecFuncPtr)MemManage_Handler,       // Memory Manage Fault
    (ExecFuncPtr)BusFault_Handler,        // Bus Fault
    (ExecFuncPtr)UsageFault_Handler,      // Usage Fault
    0,                                    // Secire Fault
    0,                                    // RESERVED
    0,                                    // RESERVED
    0,                                    // RESERVED
    (ExecFuncPtr)SVC_Handler,             // SVC
    0,                                    // debug
    0,                                    // RESERVED
    (ExecFuncPtr)PendSV_Handler,          // Pend SV
    (ExecFuncPtr)SysTick_Handler,         // Secure systick
    /* External interrupts */
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler,
    (ExecFuncPtr)InterruptHandler
};

#define MPU_SRAM1_SHARED_MEM_REG_ID     0x1
#define MPU_SRAM1_SHARED_MEM_ATTR_IDX   0x1

#define MPU_SOCMEM_SHARED_MEM_REG_ID     0x2
#define MPU_SOCMEM_SHARED_MEM_ATTR_IDX   0x2

#define MPU_GPU_BUF_MEM_ATTR_IDX        0x3
#define MPU_GPU_BUF_MEM_REG_ID          0x3
void config_noncacheable_region(void)
{
    ARM_MPU_Disable();

    /* Program MAIR0 and MAIR1 */
    ARM_MPU_SetMemAttr  (   MPU_SRAM1_SHARED_MEM_ATTR_IDX,
                            ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));

    ARM_MPU_SetRegion   (   MPU_SRAM1_SHARED_MEM_REG_ID,
                            ARM_MPU_RBAR(SRAM1_NS_SAHB_SHARED_START, ARM_MPU_SH_INNER, 0UL, 1UL, 1UL),
                            ARM_MPU_RLAR((SRAM1_NS_SAHB_SHARED_START + SRAM1_SHARED_SIZE - 1UL), MPU_SRAM1_SHARED_MEM_ATTR_IDX));

    /* socmem shared mem */
    ARM_MPU_SetMemAttr  (   MPU_SOCMEM_SHARED_MEM_ATTR_IDX,
                            ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));

    ARM_MPU_SetRegion   (   MPU_SOCMEM_SHARED_MEM_REG_ID,
                            ARM_MPU_RBAR(SOCMEM_NS_SAHB_SHARED_START, ARM_MPU_SH_INNER, 0UL, 1UL, 1UL),
                            ARM_MPU_RLAR((SOCMEM_NS_SAHB_SHARED_START + SOCMEM_SHARED_MEM_SIZE - 1UL), MPU_SOCMEM_SHARED_MEM_ATTR_IDX));
    ARM_MPU_SetMemAttr  (   MPU_GPU_BUF_MEM_ATTR_IDX,
                            ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));

    ARM_MPU_SetRegion   (   MPU_GPU_BUF_MEM_REG_ID,
                            ARM_MPU_RBAR(SOCMEM_GPUBUF_START, ARM_MPU_SH_INNER, 0UL, 1UL, 1UL),
                            ARM_MPU_RLAR((SOCMEM_GPUBUF_START + SOCMEM_GPUBUF_SIZE - 1UL), MPU_GPU_BUF_MEM_ATTR_IDX));

    ARM_MPU_Enable(4);
}

void cy_cache_update(void)
{
    SCB_CleanDCache();
    SCB_InvalidateICache();
}

/* Provide empty __WEAK implementation for the low-level initialization
   routine required by the RTOS-enabled applications.
   clib-support library provides FreeRTOS-specific implementation:
   https://github.com/Infineon/clib-support */
__WEAK void cy_toolchain_init(void)
{
}

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
/* GCC: newlib crt0 _start executes software_init_hook.
   The cy_toolchain_init hook provided by clib-support library must execute
   after static data initialization and before static constructors. */
void software_init_hook();
void software_init_hook()
{
    cy_cache_update();
    cy_toolchain_init();
}
#elif defined(__ICCARM__)
/* Initialize data section */
void __iar_data_init3(void);

/* Call the constructors of all global objects */
void __iar_dynamic_initialization(void);

/* Define strong version to return zero for __iar_program_start
   to skip data sections initialization (__iar_data_init3). */
int __low_level_init(void);
int __low_level_init(void)
{
    return 0;
}
#else
/**/
#endif /* defined(__GNUC__) && !defined(__ARMCC_VERSION) */

// Reset Handler
__WEAK void Reset_Handler(void)
{

#if 0
    /* invalidate the cache */
    ICACHE0->CMD = ICACHE0->CMD | ICACHE_CMD_INV_Msk;
    /*wait for invalidation complete */
    while(ICACHE0->CMD & ICACHE_CMD_INV_Msk);
#endif

    /* Enable EWIC block */
    uint32_t *ptr = (uint32_t *)CY_SYSINT_EWIC_CTL;
    *ptr |= (uint32_t)CY_SYSINT_EWIC_ENABLE_MSK;

    /* Disable I cache */
//    ICACHE0->CTL = ICACHE0->CTL & (~ICACHE_CTL_CA_EN_Msk);

    /* Enable ECC */
    //ICACHE0->CTL = ICACHE0->CTL | ICACHE_CTL_ECC_EN_Msk;

    /* Enable I cache */
  //  ICACHE0->CTL = ICACHE0->CTL | ICACHE_CTL_CA_EN_Msk;

    __disable_irq();

    SCB_EnableICache();
    SCB_EnableDCache();

    config_noncacheable_region();

    for (uint32_t count = 0; count < VECTORTABLE_SIZE; count++)
    {
        __ns_vector_table_rw[count] =__ns_vector_table[count];
    }

    SCB->VTOR = (uint32_t)__ns_vector_table_rw;
    __DMB();
#if (defined (__FPU_USED) && (__FPU_USED == 1U)) || \
      (defined (__ARM_FEATURE_MVE) && (__ARM_FEATURE_MVE > 0U))
    SCB->CPACR |= SCB_NS_CPACR_CP10_CP11_ENABLE;
#endif


    __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

    SystemInit();

#if defined(__ICCARM__)
    /* Initialize data section */
    __iar_data_init3();

    /* Initialization hook for RTOS environment  */
    cy_toolchain_init();

    /* Call the constructors of all global objects */
    __iar_dynamic_initialization();
#endif

    Cy_RuntimeInit();
}

CY_MISRA_BLOCK_END('MISRA C-2012 Rule 18.3')
CY_MISRA_BLOCK_END('MISRA C-2012 Rule 18.1')
CY_MISRA_BLOCK_END('ARRAY_VS_SINGLETON')
CY_MISRA_BLOCK_END('MISRA C-2012 Rule 8.6')
