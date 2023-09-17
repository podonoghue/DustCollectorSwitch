/**
 * @file     port.h (180.ARM_Peripherals/Project_Headers/port_MKE.h)
 * @brief    PORT routines
 *
 * @version  V4.12.1.270
 * @date     10 July 2023
 */

#ifndef PROJECT_HEADERS_PORT_H_
#define PROJECT_HEADERS_PORT_H_

#include "pin_mapping.h"

namespace USBDM {

enum PullDirection : uint32_t {
   PullDirection_none = 0,
   PullDirection_up   = 0xFFFFFFFFU,
};

enum HighDrive : uint32_t {
   HighDrive_off = 0,
   HighDrive_on  = 0xFFFFFFFFU,
};

/**
 * Class holding shared code for the PORT
 */
class Port : public PortInfo {

   using Info = PortInfo;

public:
   /* template /PORT/InitMethod */
   /**
    * Configure with default settings.
    * Configuration determined from Configure.usbdmProject
    */
   static inline void defaultConfigure() {
   
      // Update settings
      configure(Info::DefaultInitValue);
   
      port->PUEL     = Info::puel;
      port->HDRVE    = Info::hdrve;
   }
   
   /**
    * Configure PORT from values specified in init
    *
    * @param init Class containing initialisation values
    */
   static void configure(const typename Info::Init &init) {
   
      // Initialise remaining registers
      port->IOFLT    = init.ioflt;
   }
   

};

/**
 * Class representing a PORT pin
 *
 * @tparam pinIndex  Index into tables describing individual PORT pins e.g. PTA = 0..7, PTB = 8..25 etc
 */
template<PinIndex pinIndex>
class Port_T : public PortInfo {

protected:
   using Info = PortInfo;
   

   /**
    * Determines the mask for the HDRVE register given port bit number
    *
    * @param pinInd Bit number - This also indicates the actual port e.g. PTA = 0..7, PTB = 8..25 etc
    *
    * @return Mask value for use with the HDRVE register
    */
   static constexpr auto calculateHdriveMask(PinIndex pinInd) {
      constexpr int8_t highDriveIndex[] = {
         /*             0       1       2       3       4       5       6       7 */
         /* PTA */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
         /* PTB */     -1,     -1,     -1,     -1,     -1,      1,     -1,     -1,   
         /* PTC */     -1,      2,     -1,     -1,     -1,      3,     -1,     -1,   
         /* PTD */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
         /* PTE */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
         /* PTF */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
         /* PTG */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
         /* PTH */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
         /* PTI */     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,   
   

      };
      if (highDriveIndex[unsigned(pinInd)]<0) {
         return 0;
      }
      return (1<<highDriveIndex[unsigned(pinInd)]);
   }
   
   static constexpr HardwarePtr<uint32_t> hdrve = PORT_BasePtr+offsetof(PORT_Type, HDRVE);
   static constexpr unsigned HDRIVE_MASK        = calculateHdriveMask(pinIndex);
   /**
    * Determines the mask for the PUE[] register given port bit number
    *
    * @param pinInd Bit number - This also indicates the actual port e.g. PTA = 0..7, PTB = 8..25 etc
    *
    * @return Mask value for use with the PUE[] registers
    */
   static constexpr auto calculatePueMask(PinIndex pinInd) {
      if (pinInd > PinIndex::MAX_PIN_INDEX) {
         return 0;
      }
      return 1<<(unsigned(pinInd)%32);
   }
   
   static constexpr HardwarePtr<uint32_t> puel  = PORT_BasePtr+offsetof(PORT_Type, PUEL)+4*(unsigned(pinIndex)/32);
   static constexpr unsigned MASK               = calculatePueMask(pinIndex);

public:
   /**
   * Set pull device for port pin
   *
   * @param pullDirection PullDirection_none or PullDirection_up
   */
   static void setPup(PullDirection pullDirection) {
      static_assert(MASK!=0, "Pin not supported on this device");
      *puel = (*puel|(MASK&pullDirection))&(~MASK|pullDirection);
   }
   
   /**
    * Set high-drive for port pin
    *
    * @param highDrive HighDrive_on or HighDrive_off
    */
   static void setHdrive(HighDrive highDrive) {
      static_assert(HDRIVE_MASK!=0, "High-drive not supported on this pin");
      *hdrve = (*hdrve|(HDRIVE_MASK&highDrive))&(~HDRIVE_MASK|highDrive);
   }

};

/**
 * Class representing a field of PORT pins
 *
 * @tparam pinIndexLeft  Index identifying left-most PORT pins e.g. PTA = 0..7, PTB = 8..25 etc
 * @tparam pinIndexRight Index identifying left-most PORT pins e.g. PTA = 0..7, PTB = 8..25 etc
 */
template<PinIndex pinIndexLeft, PinIndex pinIndexRight>
class PortField_T : private Port_T<pinIndexLeft> {

   // Restrict to same Port i.e. 8 bits wide
   // In practice it could extend across Ports A-B-C-D or E-F-G-H as they are accessed through the same GPIO register
   static_assert((pinIndexLeft<PinIndex::MAX_PIN_INDEX), "Illegal bit number for left in PortField");
   static_assert(((unsigned(pinIndexLeft)&~0b111)==(unsigned(pinIndexRight)&~0b111)) && (pinIndexLeft>=pinIndexRight),
      "Illegal bit number for left or right in PortField");

private:
   /**
    * This class is not intended to be instantiated
    */
   PortField_T(const PortField_T&) = delete;
   PortField_T(PortField_T&&) = delete;

   /// Left bit within used GPIO registers
   static constexpr PinNum Left  = unsigned(pinIndexLeft)%32;

   /// Right bit within used GPIO registers
   static constexpr PinNum Right = unsigned(pinIndexRight)%32;


protected:

   using Info = PortInfo;

public:
   /** Mask for the bits being manipulated within underlying port hardware */
   static constexpr uint32_t BITMASK = static_cast<uint32_t>((1ULL<<(Left-Right+1))-1)<<Right;
   
   /**
    * Set pull device for port pin
    *
    * @param pullDirection PullDirection_none or PullDirection_up
    */
   static void setPup(PullDirection pullDirection) {
   
      static_assert(BITMASK!=0, "Pin not supported on this device");
      *Port_T<pinIndexLeft>::puel = (*Port_T<pinIndexLeft>::puel|(BITMASK&pullDirection))&(~BITMASK|pullDirection);
   }
};

   /**
    * Class representing individual pin options for pins in Port A
    *
    * @tparam pinNum Indicates pin within the port e.g. A3 = > A<3>
    * @tparam polarity Polarity of the pin. Either ActiveHigh or ActiveLow
    */
   template <PinNum pinNum>
      class PortA : public Port_T<PinIndex(pinNum+0)> {};
   
   /**
    * Class representing pin options for a field within Port A
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the port e.g. PTA[5..3] => A<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the port e.g. PTA[5..3] => A<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <PinNum leftPinNum, PinNum rightPinNum, Polarity polarity=ActiveHigh>
       class PortAField : public PortField_T<PinIndex(leftPinNum+0), PinIndex(rightPinNum+0)> {};

   /**
    * Class representing individual pin options for pins in Port B
    *
    * @tparam pinNum Indicates pin within the port e.g. B3 = > B<3>
    * @tparam polarity Polarity of the pin. Either ActiveHigh or ActiveLow
    */
   template <PinNum pinNum>
      class PortB : public Port_T<PinIndex(pinNum+8)> {};
   
   /**
    * Class representing pin options for a field within Port B
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the port e.g. PTB[5..3] => B<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the port e.g. PTB[5..3] => B<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <PinNum leftPinNum, PinNum rightPinNum, Polarity polarity=ActiveHigh>
       class PortBField : public PortField_T<PinIndex(leftPinNum+8), PinIndex(rightPinNum+8)> {};

   /**
    * Class representing individual pin options for pins in Port C
    *
    * @tparam pinNum Indicates pin within the port e.g. C3 = > C<3>
    * @tparam polarity Polarity of the pin. Either ActiveHigh or ActiveLow
    */
   template <PinNum pinNum>
      class PortC : public Port_T<PinIndex(pinNum+16)> {};
   
   /**
    * Class representing pin options for a field within Port C
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the port e.g. PTC[5..3] => C<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the port e.g. PTC[5..3] => C<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <PinNum leftPinNum, PinNum rightPinNum, Polarity polarity=ActiveHigh>
       class PortCField : public PortField_T<PinIndex(leftPinNum+16), PinIndex(rightPinNum+16)> {};

 
} // namespace USBDM


#endif /* PROJECT_HEADERS_PORT_H_ */
