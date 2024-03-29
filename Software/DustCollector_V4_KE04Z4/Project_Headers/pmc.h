/**
 * @file     pmc.h (180.ARM_Peripherals/Project_Headers/pmc.h)
 * @brief    Power Management Controller
 *
 * @version  V4.12.1.210
 * @date     30 September 2017
 */

#ifndef HEADER_PMC_H
#define HEADER_PMC_H
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "pin_mapping.h"

namespace USBDM {

/**
 * @addtogroup PMC_Group PMC, Power Management Controller
 * @brief Peripheral information for Power Management Controller
 * @{
 */

#if false // /PMC/enablePeripheralSupport
/**
 * Template class providing interface to Power Management Controller
 *
 * @tparam info      Information class for PMC
 *
 * @code
 * using pmc = PmcBase_T<PmcInfo>;
 *
 *  pmc::defaultConfigure();
 *
 * @endcode
 */
class PmcBase : public PmcInfo {

protected:
// No protected methods found

protected:
   /** Hardware instance */
   static constexpr HardwarePtr<PMC_Type> pmc = baseAddress;

public:
// /PMC/publicMethods not found

   /**
    * Enable interrupts in NVIC
    */
   static void enableNvicInterrupts() {
      NVIC_EnableIRQ(irqNums[0]);
   }

   /**
    * Enable and set priority of interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    *
    * @param[in]  nvicPriority  Interrupt priority
    */
   static void enableNvicInterrupts(NvicPriority nvicPriority) {
      enableNvicInterrupt(irqNums[0], nvicPriority);
   }

   /**
    * Disable interrupts in NVIC
    */
   static void disableNvicInterrupts() {
      NVIC_DisableIRQ(irqNums[0]);
   }

};

// No static declarations found

#endif // /PMC/enablePeripheralSupport

/**
 * End PMC_Group
 * @}
 */
} // End namespace USBDM

#endif /* HEADER_PMC_H */
