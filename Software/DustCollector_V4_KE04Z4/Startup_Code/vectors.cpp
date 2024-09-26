/*
 *  @file vectors.cpp (180.ARM_Peripherals/Startup_Code/vectors-cm0.cpp)
 *
 *  Generated from  vectors-cm0.cpp
 *
 *  Generic vectors Kinetis Cortex-M0 devices
 *
 *  Created on: 22/05/2017
 *      Author: podonoghue
 */
#include <stdint.h>
#include <string.h>
#include "derivative.h"
#include "hardware.h"

#include "pit.h"


using namespace USBDM;

/*
 * Vector table related
 */
typedef void( *const intfunc )( void );

#define WEAK_DEFAULT_HANDLER __attribute__ ((__nothrow__, __weak__, alias("Default_Handler")))
/**
 * Default handler for interrupts
 *
 * Most of the vector table is initialised to point at this handler.
 *
 * If you end up here it probably means:
 *   - Failed to enable the interrupt handler in the USBDM configuration (Configure.usbdmProject)
 *   - You have accidently enabled an interrupt source in a peripheral
 *   - Enabled the wrong interrupt source
 *   - Failed to install or create a handler for an interrupt you intended using e.g. mis-spelled the name.
 *     Compare your handler (C function) name to that used in the vector table.
 *
 * You can check 'vectorNum' below to determine the interrupt source.  Look this up in the vector table below.
 */
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
__attribute__((__interrupt__))
void Default_Handler(void) {

#ifdef SCB
   __attribute__((unused))
   volatile uint32_t vectorNum = (SCB->ICSR&SCB_ICSR_VECTACTIVE_Msk)>>SCB_ICSR_VECTACTIVE_Pos;
#endif

   while (1) {
      __asm__("bkpt");
   }
}
#pragma GCC diagnostic pop
}

typedef struct {
   unsigned int r0;
   unsigned int r1;
   unsigned int r2;
   unsigned int r3;
   unsigned int r12;
   void       (*lr)();
   void       (*pc)();
   unsigned int psr;
} ExceptionFrame;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
/*  Low-level exception handler
 *
 *  Interface from asm to C.
 *  Passes address of exception handler to C-level handler
 *
 *  See http://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html
 */
__attribute__((__naked__, __weak__, __interrupt__))
void HardFault_Handler(void) {
#if defined(DEBUG_BUILD) && false
   /*
    * Determines the active stack pointer and loads it into r0
    * This is used as the 1st argument to _HardFault_Handler(volatile ExceptionFrame *exceptionFrame)
    * and allows access to the saved processor state.
    * Other registers are unchanged and available in the usual register view
    */
   __asm__ volatile ("       mov r0,lr                                     \n"); // Check mode
   __asm__ volatile ("       mov r1,#4                                     \n");
   __asm__ volatile ("       and r0,r1                                     \n");
   __asm__ volatile ("       bne skip1                                     \n");
   __asm__ volatile ("       mrs r0,msp                                    \n"); // Get active SP in r0
   __asm__ volatile ("       b   skip2                                     \n");
   __asm__ volatile ("skip1:                                               \n");
   __asm__ volatile ("       mrs r0,psp                                    \n");
   __asm__ volatile ("skip2:                                               \n");
   __asm__ volatile ( "      mov   r1, lr                                  \n"); // Get LR=EXC_RETURN in r1
   __asm__ volatile ("       ldr r2, handler_addr_const                    \n"); // Go to C handler
   __asm__ volatile ("       bx r2                                         \n");
   __asm__ volatile ("      .align 4                                       \n");
   __asm__ volatile ("       handler_addr_const: .word _HardFault_Handler  \n");

#else
   while (1) {
      // Stop here for debugger
      __asm__("bkpt");
   }
#endif
}
#pragma GCC diagnostic pop

extern "C" {
#if defined(DEBUG_BUILD) && false
/******************************************************************************/
/* Hard fault handler in C with stack frame location as input parameter
 *
 * Assumed exception frame without floating-point storage
 *
 * @param exceptionFrame address of exception frame
 *
 * If you end up here you have probably done one of the following:
 *   - Accessed illegal/unimplemented memory e.g. gone off the end of an array
 *   - Accessed a disabled peripheral - Check you have enabled the clock
 *   - Accessed unaligned memory - unlikely I guess
 *
 */
__attribute__((__naked__))
void _HardFault_Handler(
      volatile ExceptionFrame *exceptionFrame __attribute__((__unused__)),
      uint32_t execReturn                     __attribute__((__unused__)) ) {

#if defined(DEBUG_BUILD) && USE_CONSOLE
   using namespace USBDM;

   console.setPadding(Padding_LeadingZeroes);
   console.setWidth(Width_8);
   console.writeln("\n[Hardfault]\n - Stack frame:\n");
   console.writeln("R0  = 0x", exceptionFrame->r0,  Radix_16);
   console.writeln("R1  = 0x", exceptionFrame->r1,  Radix_16);
   console.writeln("R2  = 0x", exceptionFrame->r2,  Radix_16);
   console.writeln("R3  = 0x", exceptionFrame->r3,  Radix_16);
   console.writeln("R12 = 0x", exceptionFrame->r12, Radix_16);
   console.writeln("LR  = 0x", (void*)(exceptionFrame->lr),  Radix_16);
   console.writeln("PC  = 0x", (void*)(exceptionFrame->pc),  Radix_16);
   console.writeln("PSR = 0x", exceptionFrame->psr, Radix_16);
   console.writeln("- Misc");
   console.write("LR/EXC_RETURN= 0x", execReturn,  Radix_16);
#endif

   while (1) {
      // Stop here for debugger
      __asm__("bkpt");
   }
}
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
void Reset_Handler(void) __attribute__((__interrupt__));
#pragma GCC diagnostic pop

extern uint32_t __StackTop;
}

/*
 * Each vector is assigned an unique name.  This is then 'weakly' assigned to the
 * default handler.
 * To install a handler, create a C linkage function with the name shown and it will override
 * the weak default.
 */
 #ifdef __cplusplus
extern "C" {
#endif
// Reset handler must have C linkage
void Reset_Handler(void);
#ifdef __cplusplus
}
#endif
void NMI_Handler(void)                        WEAK_DEFAULT_HANDLER;
void SVC_Handler(void)                        WEAK_DEFAULT_HANDLER;
void PendSV_Handler(void)                     WEAK_DEFAULT_HANDLER;
void SysTick_Handler(void)                    WEAK_DEFAULT_HANDLER;
void FTMRE_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void PMC_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void IRQ_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void I2C0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void SPI0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void UART0_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void ADC0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void ACMP0_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void FTM0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void FTM2_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void RTC_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void ACMP1_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void KBI0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void KBI1_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void ICS_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void WDOG_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void PWT_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;

typedef struct {
   uint32_t *initialSP;
   intfunc  handlers[45];
} VectorTable;

extern VectorTable const __vector_table;

__attribute__ ((section(".interrupt_vectors")))
VectorTable const __vector_table = {
                                               /*  Exc# Irq# */
   &__StackTop,                                /*    0   -16  Initial stack pointer                                                            */
   {
      Reset_Handler,                           /*    1,  -15  Reset Vector, invoked on Power up and warm reset                                 */
      NMI_Handler,                             /*    2,  -14  Non maskable Interrupt, cannot be stopped or preempted                           */
      HardFault_Handler,                       /*    3,  -13  Hard Fault, all classes of Fault                                                 */
      0,                                       /*    4,  -12                                                                                   */
      0,                                       /*    5,  -11                                                                                   */
      0,                                       /*    6,  -10                                                                                   */
      0,                                       /*    7,   -9                                                                                   */
      0,                                       /*    8,   -8                                                                                   */
      0,                                       /*    9,   -7                                                                                   */
      0,                                       /*   10,   -6                                                                                   */
      SVC_Handler,                             /*   11,   -5  System Service Call via SVC instruction                                          */
      0,                                       /*   12,   -4                                                                                   */
      0,                                       /*   13,   -3                                                                                   */
      PendSV_Handler,                          /*   14,   -2  Pendable request for system service                                              */
      SysTick_Handler,                         /*   15,   -1  System Tick Timer                                                                */

                                               /* External Interrupts */
      Default_Handler,                         /*   16,    0                                                                                   */
      Default_Handler,                         /*   17,    1                                                                                   */
      Default_Handler,                         /*   18,    2                                                                                   */
      Default_Handler,                         /*   19,    3                                                                                   */
      Default_Handler,                         /*   20,    4                                                                                   */
      FTMRE_IRQHandler,                        /*   21,    5  Flash Memory                                                                     */
      PMC_IRQHandler,                          /*   22,    6  Power Management Controller                                                      */
      IRQ_IRQHandler,                          /*   23,    7  External Interrupt                                                               */
      I2C0_IRQHandler,                         /*   24,    8  Inter-Integrated Circuit                                                         */
      Default_Handler,                         /*   25,    9                                                                                   */
      SPI0_IRQHandler,                         /*   26,   10  Serial Peripheral Interface                                                      */
      Default_Handler,                         /*   27,   11                                                                                   */
      UART0_IRQHandler,                        /*   28,   12  Serial Communication Interface                                                   */
      Default_Handler,                         /*   29,   13                                                                                   */
      Default_Handler,                         /*   30,   14                                                                                   */
      ADC0_IRQHandler,                         /*   31,   15  Analogue to Digital Converter                                                    */
      ACMP0_IRQHandler,                        /*   32,   16  Analogue comparator                                                              */
      FTM0_IRQHandler,                         /*   33,   17  FlexTimer Module                                                                 */
      Default_Handler,                         /*   34,   18                                                                                   */
      FTM2_IRQHandler,                         /*   35,   19  FlexTimer Module                                                                 */
      RTC_IRQHandler,                          /*   36,   20  Real Time Clock                                                                  */
      ACMP1_IRQHandler,                        /*   37,   21  Analogue comparator                                                              */
      Pit::irqHandler<Pit::IrqNum_Ch0>,        /*   38,   22  Periodic Interrupt Timer                                                         */
      Pit::irqHandler<Pit::IrqNum_Ch1>,        /*   39,   23  Periodic Interrupt Timer                                                         */
      KBI0_IRQHandler,                         /*   40,   24  Keyboard Interrupt                                                               */
      KBI1_IRQHandler,                         /*   41,   25  Keyboard Interrupt                                                               */
      Default_Handler,                         /*   42,   26                                                                                   */
      ICS_IRQHandler,                          /*   43,   27  Clock Management                                                                 */
      WDOG_IRQHandler,                         /*   44,   28  Watchdog Timer                                                                   */
      PWT_IRQHandler,                          /*   45,   29  Pulse Width Timer                                                                */
   }
};




