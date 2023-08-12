/**
 * @file      kbi.h (180.ARM_Peripherals/Project_Headers/kbi-MKE.h)
 *
 * @brief    Abstraction layer for KBI interface
 *
 * @version  V4.12.
 * @date     13 July 2023
 */

#ifndef PROJECT_HEADERS_KBI_H_
#define PROJECT_HEADERS_KBI_H_

#include "pin_mapping.h"

namespace USBDM {

template <class Info>
class KbiBase_T : public Info {

protected:
   /** Hardware instance */
   static constexpr HardwarePtr<KBI_Type> kbi = Info::baseAddress;


public:

   /**
    * Configure with default settings.
    * Configuration determined from Configure.usbdmProject
    */
   static inline void defaultConfigure() {
   
      // Update settings
      configure(Info::DefaultInitValue);
   }
   
   /**
    * Configure KBI from values specified in init
    *
    * @param init Class containing initialisation values
    */
   static void configure(const typename Info::Init &init) {
      // ..........  Configure stuff ...........
   
      // Enable peripheral clock
      Info::enableClock();
   
      // ..........  Regs to init .......... ;
      kbi->SC    = KBI_SC_KBACK_MASK;
      kbi->PE    = init.pe;
      kbi->ES    = init.es;
      kbi->SC    = init.sc;
   }
   

   /**
    * Enable interrupts in NVIC
    */
   static void enableNvicInterrupts() {
      NVIC_EnableIRQ(Info::irqNums[0]);
   }

   /**
    * Enable and set priority of interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    *
    * @param[in]  nvicPriority  Interrupt priority
    */
   static void enableNvicInterrupts(NvicPriority nvicPriority) {
      enableNvicInterrupt(Info::irqNums[0], nvicPriority);
   }

   /**
    * Disable interrupts in NVIC
    */
   static void disableNvicInterrupts() {
      NVIC_DisableIRQ(Info::irqNums[0]);
   }

};


   /**
    * Class representing KBI0
    */
   class Kbi0 : public KbiBase_T<Kbi0Info> {};
   /**
    * Class representing KBI1
    */
   class Kbi1 : public KbiBase_T<Kbi1Info> {};


}; // namespace USBDM

#endif /* PROJECT_HEADERS_KBI_H_ */
