/*
 *  @file system.cpp (from Stationery/Packages/180.ARM_Peripherals/Startup_Code/)
 *
 * Generic system initialization for Kinetis family
 *
 *  Created on: 23/9/2017
 */

#include <stdint.h>
#include "derivative.h"
#include "pmc.h"
#include "port.h"
#include "wdog.h"


/* This definition is overridden if Clock initialisation is provided */
__attribute__((__weak__))
void SystemCoreClockUpdate(void) {
}

/* These are overridden if actual clock code is provided */
/* Based on Kinetis internal clock used after reset */
__attribute__((__weak__))
uint32_t SystemCoreClock = 20000000;
__attribute__((__weak__))
uint32_t SystemBusClock  = 20000000;

#ifdef SIM_CLKDIV1_OUTDIV3_MASK
__attribute__((__weak__))
uint32_t SystemFlexbusClock = 20000000;
#endif

#ifdef SIM_CLKDIV_OUTDIV3_MASK
__attribute__((__weak__))
uint32_t SystemTimerClock = 20000000;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Actual Vector table */
extern int const __vector_table[];

/* This definition is overridden if Clock initialisation is provided */
__attribute__((__weak__))
void clock_initialise() {
}

/* This definition is overridden if UART initialisation is provided */
__attribute__((__weak__))
void console_initialise() {
}

/* This definition is overridden if RTC initialisation is provided */
__attribute__((__weak__))
void rtc_initialise() {
}

// Dummy hook routine for when CMSIS is not used.
__attribute__((__weak__))
void software_init_hook () {
}

#ifdef __cplusplus
}
#endif

#ifdef __NO_STARTFILES__
#warning Due to limited RAM the C library standard initialisation is not called - BSS and DATA are still initialised
void* __dso_handle;
#endif

#if defined(KINETIS_BOOTLOADER_CHECK)
void checkICP();
#endif

/**
 *  @brief Low-level initialise the system
 *
 *  Low level setup of the microcontroller system. \n
 *  Called very early in the initialisation. \n
 *  May NOT use globals etc (as will be overwritten by BSS initialization)
 */
void SystemInitLowLevel(void) {
   /*
    * This is generic initialization code
    * It may not be correct for a specific target
    */

#if (__VTOR_PRESENT != 0)
   /* Set the interrupt vector table position */
   SCB->VTOR = (uint32_t)__vector_table;
#endif

#ifdef SCB_CCR_DIV_0_TRP_Msk
   /* Enable trapping of divide by zero */
   SCB->CCR = SCB->CCR | SCB_CCR_DIV_0_TRP_Msk;
#endif

#ifdef RCM_MR_BOOTROM
   // Clear Boot ROM flag
   RCM->MR = RCM_MR_BOOTROM(3);
#endif
   /*
    * Initialise watchdog
    */
   USBDM::Wdog::disableWdog();
   
#if defined(KINETIS_BOOTLOADER_CHECK)
   /**
    * Hook for ICP code
    * Needed to be done before too much uC configuration
    */
   checkICP();
#endif
}

/**
 * @brief Initialise the system
 *
 * Setup the microcontroller system.
 */
__attribute__ ((constructor))
void SystemInit(void) {
   /*
    * This is generic initialization code
    * It may not be correct for a specific target
    */

#ifdef PMC_REGSC_ACKISO
   USBDM::Pmc::releasePins();
#endif

   /* Use Clock initialisation - if present */
   clock_initialise();

   /* Use UART initialisation - if present */
   console_initialise();

   /* Use RTC initialisation - if present */
   rtc_initialise();

#if defined(__VFP_FP__) && !defined(__SOFTFP__)
//#warning "Using FP hardware"

   /* Initialise FPU if present & in use */
   __asm__ (
         "  .equ CPACR, 0xE000ED88     \n"
         "                             \n"
         "  LDR.W R0, =CPACR           \n"  // CPACR address
         "  LDR R1, [R0]               \n"  // Read CPACR
         "  ORR R1, R1, #(0xF << 20)   \n"  // Enable CP10 and CP11 coprocessors
         "  STR R1, [R0]               \n"  // Write back the modified value to the CPACR
         "  DSB                        \n"  // Wait for store to complete"
         "  ISB                        \n"  // Reset pipeline now the FPU is enabled
   );
#endif
/* System startup code for peripherals */

   /* Configure Ports (Filters, PUPs and HDRIVE) */
   USBDM::Port::defaultConfigure();

}
