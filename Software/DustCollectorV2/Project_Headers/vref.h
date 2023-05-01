/**
 * @file     vref.h (180.ARM_Peripherals/Project_Headers/vref.h)
 * @brief    Voltage Reference
 *
 * @version  V4.12.1.210
 * @date     13 April 2016
 *      Author: podonoghue
 */

#ifndef HEADER_VREF_H
#define HEADER_VREF_H
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "derivative.h"
#include "pin_mapping.h"

namespace USBDM {

/**
 * @addtogroup VREF_Group VREF, Voltage Reference
 * @brief C++ Class allowing access to Voltage Reference
 * @{
 */

/**
 * Template class representing a Voltage Reference
 *
 * @tparam info      Information class for VREF
 *
 * @code
 * using vref = VrefBase_T<VrefInfo>;
 *
 *  vref::initialise();
 *
 * @endcode
 */
template<class Info>
class VrefBase_T : public Info {

   /** Class to static check output is mapped to a pin - Assumes existence */
   template<int output> class CheckOutputIsMapped {

      // Check mapping - no need to check existence
      static constexpr bool Test1 = (Info::info[output].gpioBit >= 0);

      static_assert(Test1, "VREF output is not mapped to a pin - Modify Configure.usbdm");

   public:
      /** Dummy function to allow convenient in-line checking */
      static constexpr void check() {}
   };

private:
   /**
    * This class is not intended to be instantiated
    */
   VrefBase_T(const VrefBase_T&) = delete;
   VrefBase_T(VrefBase_T&&) = delete;

public:
   /** Hardware instance pointer */
   static constexpr HardwarePtr<VREF_Type> vref = Info::baseAddress;

   VrefBase_T() {
   };

public:
   /**
    * Configure VREF from values specified in init
   
    * @param init Class containing initialisation values
    */
   static void configure(const typename Info::Init &init) {
   
      enable();
   
      vref->TRM = (vref->TRM&~VREF_TRM_CHOPEN_MASK)|init.trm;
      vref->SC = init.sc & ~VREF_SC_REGEN_MASK;
   
      // Regulator must be enabled >300ns after other settings
      waitUS(1);
      vref->SC = init.sc;
   
      while ((vref->SC & VREF_SC_VREFST_MASK) == 0) {
         // Wait until stable
      }
   }

   /**
    * Configure VREF with default settings
    */
   static void defaultConfigure() {
   
      configure(Info::DefaultInitValue);
   }

   /**
    * Configure VREF from values specified
    *
    * @param vrefBuffer This selects the buffer modes for the Voltage Reference module
    * @param vrefReg    This controls the internal 1.75 V regulator which produce a constant
    *        internal voltage supply in order to reduce the sensitivity to external supply noise and variation
    *        If it is desired to keep the regulator enabled in very low power modes see PmcBandgapLowPowerEnable
    * @param vrefIcomp  This should be enabled to achieve the performance stated in the data sheet
    * @param vrefChop   Controls the internal chopping operation to minimise the internal analogue offset
    *        This option is enabled during factory trimming of the VREF voltage.
    *        This should be enabled to achieve the performance stated in the data sheet.
    *        If the chop oscillator is to be used in very low power modes, the system (bandgap)
    *        voltage reference must also be enabled. See PmcBandgapLowPowerEnable
    */
   static void configure(
         VrefBuffer vrefBuffer,
         VrefReg    vrefReg    = VrefReg_Enabled,
         VrefIcomp  vrefIcomp  = VrefIcomp_Enabled,
         VrefChop   vrefChop   = VrefChop_Enabled) {


      enable();
   
      if (vrefReg) {
         // Chop must be enabled if regulator enabled
         vrefChop = VrefChop_Enabled;
      }
      vref->TRM = (vref->TRM&~VREF_TRM_CHOPEN_MASK)|vrefChop;
      vref->SC = vrefBuffer|vrefIcomp;
   
      // Regulator must be enabled >300ns after other settings
      waitUS(1);
      vref->SC = vrefBuffer|vrefReg|vrefIcomp;
   
      while ((vref->SC & VREF_SC_VREFST_MASK) == 0) {
         // Wait until stable
      }
   }


   // Template _mapPinsOption.xml

   /**
    * Configures all mapped pins associated with VREF
    *
    * @note Locked pins will be unaffected
    */
   static void configureAllPins() {
   
      // Configure pins if selected and not already locked
      if constexpr (Info::mapPinsOnEnable) {
         Info::initPCRs();
      }
   }

   /**
    * Disabled all mapped pins associated with VREF
    *
    * @note Only the lower 16-bits of the PCR registers are modified
    *
    * @note Locked pins will be unaffected
    */
   static void disableAllPins() {
   
      // Disable pins if selected and not already locked
      if constexpr (Info::mapPinsOnEnable) {
         Info::clearPCRs();
      }
   }

   /**
    * Basic enable of VREF
    * Includes enabling clock and configuring all mapped pins if mapPinsOnEnable is selected in configuration
    */
   static void enable() {
      Info::enableClock();
      configureAllPins();
   }

   /**
    * Disables the clock to VREF and all mapped pins
    */
   static void disable() {
      
      vref->SC = 0;
      disableAllPins();
      Info::disableClock();
   }
// End Template _mapPinsOption.xml

   /**
    * Enable Vref output pin as Vref output.
    * Configures all Pin Control Register (PCR) values
    */
   static void setOutput() {

      CheckOutputIsMapped<Info::outputPin>::check();

      using Pcr = PcrTable_T<Info, Info::outputPin>;

      // Enable and map pin to Vref_out is needed
      Pcr::setPCR();
   }


   /**
    * Sets the voltage reference mode
    *
    * @param scValue Value for SC register e.g. VREF_SC_VREFEN_MASK|VREF_SC_REGEN_MASK|VREF_SC_ICOMPEN_MASK|VREF_SC_MO`DE_LV(2)
    */
   static void setMode(uint32_t scValue=Info::vref_sc|VREF_SC_VREFEN_MASK) {
      vref->SC   = scValue;
   }

};

   /**
    * Class representing VREF
    */
   class Vref : public VrefBase_T<VrefInfo> {};

/**
 * End VREF_Group
 * @}
 */

} // End namespace USBDM

#endif /* HEADER_VREF_H */
