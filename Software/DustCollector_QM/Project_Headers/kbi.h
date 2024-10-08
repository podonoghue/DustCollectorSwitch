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

#if false // /KBI/enablePeripheralSupport

namespace USBDM {

template <class Info>
class KbiBase_T : public Info {

protected:
   /** Hardware instance */
   static constexpr HardwarePtr<KBI_Type> kbi = Info::baseAddress;

// No /KBI/protectedMethods found
public:
// No /KBI/publicMethods found

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




}; // namespace USBDM

#endif // /KBI/enablePeripheralSupport

#endif /* PROJECT_HEADERS_KBI_H_ */
