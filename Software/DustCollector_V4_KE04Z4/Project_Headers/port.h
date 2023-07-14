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
 * @tparam bitNum  Index into tables describing individual PORT pins e.g. PTA = 0..7, PTB = 8..25 etc
 */
template<unsigned bitNum>
class Port_T : public PortInfo {

protected:
   using Info = PortInfo;
   

   /**
    * Determines the mask for the HDRVE register given port bit number
    *
    * @param bitNm Bit number - This also indicates the actual port e.g. PTA = 0..7, PTB = 8..25 etc
    *
    * @return Mask value for use with the HDRVE register
    */
   static constexpr auto calculateHdriveMask(unsigned bitNm) {
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
      if (highDriveIndex[bitNm]<0) {
         return 0;
      }
      return (1<<highDriveIndex[bitNm]);
   }
   
   static constexpr HardwarePtr<uint32_t> hdrve = PORT_BasePtr+offsetof(PORT_Type, HDRVE);
   static constexpr unsigned HDRIVE_MASK        = calculateHdriveMask(bitNum);
   /**
    * Determines the mask for the PUE[] register given port bit number
    *
    * @param bitNm Bit number - This also indicates the actual port e.g. PTA = 0..7, PTB = 8..25 etc
    *
    * @return Mask value for use with the PUE[] registers
    */
   static constexpr auto calculatePueMask(unsigned bitNm) {
      constexpr unsigned MAX_BIT = 71; // Pti7
      if (bitNm > MAX_BIT) {
         return 0;
      }
      return 1<<(bitNm%32);
   }
   
   static constexpr HardwarePtr<uint32_t> pue   = PORT_BasePtr+offsetof(PORT_Type, PUEL)+4*(bitNum/32);
   static constexpr unsigned MASK               = calculatePueMask(bitNum);

public:
   /**
   * Set pull device for port pin
   *
   * @param pullDirection PullDirection_none or PullDirection_up
   */
   static void setPup(PullDirection pullDirection) {
      static_assert(MASK!=0, "Pin not supported on this device");
      *pue = (*pue|(MASK&pullDirection))&(~MASK|pullDirection);
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

// /PORT/declarations not found 
} // namespace USBDM


#endif /* PROJECT_HEADERS_PORT_H_ */
