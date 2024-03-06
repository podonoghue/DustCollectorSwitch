/**
 * @file      hardware.cpp (generated from MK20D5.usbdmHardware)
 * @version   1.3.0
 * @brief     Pin initialisation for MK20DX128VLF5
 *
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */

#include "hardware.h"
// /HARDWARE/Includes not found

/**
 * Namespace enclosing USBDM classes
 */
namespace USBDM {

/**
 * @addtogroup USBDM_Group USBDM Peripheral Interface
 * @brief Hardware Peripheral Interface and library
 * @{
 */

/**
 * Startup code for C++ classes
 */
extern "C" void __attribute__((constructor)) cpp_initialise() {
   if constexpr (MapAllPinsOnStartup) {
      mapAllPins();
   }
}

#ifdef PORT_PCR_MUX
// /HARDWARE_CPP/Definitions not found

/**
 * Map all configured pins to peripheral signals.
 *
 * PCRs of allocated pins are set according to settings in Configure.usbdmProject
 *
 * @note Only the lower 16-bits of the PCR registers are initialised
 */
void mapAllPins() {
#if false


#endif


#if defined(PCC_PCCn_CGC_MASK)
   PCC->PCC_PORTA = PCC_PCCn_CGC_MASK;
   PCC->PCC_PORTB = PCC_PCCn_CGC_MASK;
   PCC->PCC_PORTC = PCC_PCCn_CGC_MASK;
#else
   enablePortClocks(USBDM::PORTA_CLOCK_MASK|USBDM::PORTB_CLOCK_MASK|USBDM::PORTC_CLOCK_MASK);
#endif

   PORTA->GPCLR = ForceLockedPins|0x0200UL|PORT_GPCLR_GPWE(0x001CUL);
   PORTA->GPCLR = ForceLockedPins|0x0300UL|PORT_GPCLR_GPWE(0x0020UL);
   PORTA->GPCLR = ForceLockedPins|0x0400UL|PORT_GPCLR_GPWE(0x0002UL);
   PORTA->GPCLR = ForceLockedPins|0x0600UL|PORT_GPCLR_GPWE(0x0001UL);
   PORTB->GPCLR = ForceLockedPins|0x0000UL|PORT_GPCLR_GPWE(0x00F0UL);
   PORTB->GPCLR = ForceLockedPins|0x0400UL|PORT_GPCLR_GPWE(0x0008UL);
   PORTB->GPCLR = ForceLockedPins|0x0500UL|PORT_GPCLR_GPWE(0x0007UL);

   if constexpr (ForceLockoutUnbondedPins) {
      PORTA->GPCLR = PinLock_Locked |0x0000UL|PORT_GPCLR_GPWE(0xFFC0UL); // Lockout unavailable pins
      PORTA->GPCHR = PinLock_Locked |0x0000UL|PORT_GPCHR_GPWE(0xFFFFUL); // Lockout unavailable pins
      PORTB->GPCLR = PinLock_Locked |0x0000UL|PORT_GPCLR_GPWE(0xFF00UL); // Lockout unavailable pins
      PORTB->GPCHR = PinLock_Locked |0x0000UL|PORT_GPCHR_GPWE(0xFFFFUL); // Lockout unavailable pins
      PORTC->GPCLR = PinLock_Locked |0x0000UL|PORT_GPCLR_GPWE(0xFFFFUL); // Lockout unavailable pins
      PORTC->GPCHR = PinLock_Locked |0x0000UL|PORT_GPCHR_GPWE(0xFFFFUL); // Lockout unavailable pins
   }

}
#endif 
/**
 * End group USBDM_Group
 * @}
 */
/*
 *  Static objects
 */
   /**
    * Callback table of programmatically set handlers for Pit
    */
   PitInfo::CallbackFunction PitInfo::sCallbacks[] = {
      PitBasicInfo::unhandledCallback,
      PitBasicInfo::unhandledCallback,
   };



} // End namespace USBDM
