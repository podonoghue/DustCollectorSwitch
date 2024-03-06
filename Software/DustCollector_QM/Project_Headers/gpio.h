/**
 * @file     gpio.h (180.ARM_Peripherals/Project_Headers/gpio-MKE.h)
 * @brief    General Purpose Input/Output
 *
 * @version  V4.12.1.300
 * @date     7 July 2022
 */

#ifndef HEADER_GPIO_H
#define HEADER_GPIO_H

/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stddef.h>
#include "derivative.h"
#include "bme.h"
#include "port.h"

/*
 * Default port information
 */
#ifndef FIXED_PORT_CLOCK_REG
#define FIXED_PORT_CLOCK_REG SCGC5
#endif

namespace USBDM {

#pragma GCC push_options
#pragma GCC optimize ("Os")

// For experimentation only
#if 1
#define BITSET(dest,mask)    bmeOr((dest),   (mask));
#define BITCLEAR(dest,mask)  bmeAnd((dest), ~(mask));
#define BITTOGGLE(dest,mask) bmeXor((dest),  (mask));
#else
#define BITSET(dest,mask)    (dest) = (dest) |  (mask);
#define BITCLEAR(dest,mask)  (dest) = (dest) & ~(mask);
#define BITTOGGLE(dest,mask) (dest) = (dest) ^  (mask);
#endif

/**
 * Class representing GPIO functionality
 */
class Gpio {

   /**
    * This class is not intended to be instantiated by copy
    */
   Gpio(const Gpio&) = delete;
   Gpio(Gpio&&) = delete;

public:
   /// Underlying GPIO hardware
   const    HardwarePtr<GPIO_Type>     gpio;

   /// Mask for GPIO bit being manipulated
   const    uint32_t                   bitMask;

   /// Mask to flip bit if active low (0 otherwise)
   const    uint32_t                   flipMask;

protected:
   /**
    * Class representing GPIO functionality
    *
    * @param gpio       GPIO hardware address
    * @param bitNo      Bit number within GPIO
    * @param polarity   Polarity of bit (ActiveHigh or ActiveLow)
    */
   constexpr Gpio(uint32_t gpio, PinNum bitNo, Polarity polarity) :
      gpio(gpio), bitMask(1<<bitNo), flipMask(isActiveLow(polarity)?bitMask:0) {
   }
   /**
    * Checks if polarity if active-low
    */
   static constexpr bool isActiveLow(Polarity polarity) {
      return (polarity == ActiveLow);
   }

   /**
    * Checks if polarity if active-high
    */
   static constexpr bool isActiveHigh(Polarity polarity) {
      return (polarity == ActiveHigh);
   }

public:
#if true
   /**
    * Set pin as digital I/O.
    * Pin is initially set as an input.
    *
    * @note Resets the pin output value to the inactive state
    * @note Modifies the Port Input Disable Register value (PIDR value).
    * @note Use SetIn() and SetOut() for a lightweight change of direction without affecting other pin settings.
    */
    void setInOut() const {
      // Enable pin input function
      gpio->PIDR = gpio->PIDR & ~bitMask;
      // Make input initially
      setIn();
      // Set inactive pin state (if later made output)
      setInactive();
   }
   /**
    * Enable pin as digital output with initial inactive level.

    *
    * @note Modifies the Port Input Disable Register value (PIDR value).
    * @note Resets the pin value to the inactive state
    * @note Use SetIn() and SetOut() for a lightweight change of direction without affecting other pin settings.
    */
    void setOutput() const {
      // Disable pin input function
      gpio->PIDR = gpio->PIDR | bitMask;
      // Set initial level before enabling pin drive
      setInactive();
      // Make pin an output
      setOut();
   }
   /**
    * @brief
    * Enable pin as digital input.

    *
    * @note Modifies the Port Input Disable Register value (PIDR value).
    * @note Use SetIn() and SetOut() for a lightweight change of direction without affecting other pin settings.
    */
    void setInput() const {
      // Enable pin as input
      gpio->PIDR = gpio->PIDR & ~bitMask;
      // Make pin an input
      setIn();
   }
   /**
    * Set pin as digital input
    *
    * @note Does not affect other pin settings
    */
    void setIn() const {
      // Make pin an input
      gpio->PDDR = gpio->PDDR & ~bitMask;
   }
   /**
    * Set pin as digital output
    *
    * @note Does not affect other pin settings
    */
    void setOut() const {
      // Make pin an output
      gpio->PDDR = gpio->PDDR | bitMask;
   }
   /**
    * Set pin. Pin will be high if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
    void high() const {
      gpio->PSOR = bitMask;
   }
   /**
    * Clear pin. Pin will be low if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
    void low() const {
      gpio->PCOR = bitMask;
   }
   /**
    * Set pin. Pin will be high if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
    void set() const {
      gpio->PSOR = bitMask;
   }
   /**
    * Clear pin. Pin will be low if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
    void clear() const {
      gpio->PCOR = bitMask;
   }
   /**
    * Toggle pin (if output)
    */
    void toggle() const {
      gpio->PTOR = bitMask;
   }
   /**
    * Read pin value
    *
    * @return true/false reflecting pin value.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
    bool readBit() const {
      return isHigh();
   }
   /**
    * Write boolean value to pin (if configured as output)
    *
    * @param[in] value true/false value
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
    void  __attribute__((always_inline)) writeBit(bool value) const {
      if (value) {
         set();
      }
      else {
         clear();
      }
   }
   /**
    * Set pin to active level (if configured as output).
    * Convenience method for setActive()
    *
    * @note Polarity _is_ significant
    */
    void __attribute__((always_inline)) on() const {
      setActive();
   }
   /**
    * Set pin to inactive level (if configured as output).
    * Convenience method for setInactive()
    *
    * @note Polarity _is_ significant
    */
    void __attribute__((always_inline)) off() const {
      setInactive();
   }
   /**
    * Write boolean value to pin (if configured as output)
    *
    * @param[in] value true/false value
    *
    * @note Polarity _is_ significant
    */
    void write(bool value) const {
      if (value) {
         setActive();
      }
      else {
         setInactive();
      }
   }
   /**
    * Checks if pin is high
    *
    * @return true/false reflecting value on pin
    *
    * @note This reads the PDIR
    * @note Polarity _is_ _not_ significant
    */
    bool isHigh() const {
      return (gpio->PDIR & bitMask) != 0;
   }
   /**
    * Checks if pin is low
    *
    * @return true/false reflecting value on pin
    *
    * @note This reads the PDIR
    * @note Polarity _is_ _not_ significant
    */
    bool isLow() const {
      return (gpio->PDIR & bitMask) == 0;
   }
   /**
    * Read pin value and return true if active level.
    * Equivalent to read()
    *
    * @return true/false reflecting if pin is active.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
    bool __attribute__((always_inline)) isActive() const {
      return read();
   }
   /**
    * Read pin value and return true if inactive level
    * Equivalent to !read()
    *
    * @return true/false reflecting if pin is inactive.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
    bool __attribute__((always_inline)) isInactive() const {
      return !read();
   }
   /**
    * Read pin value and return true if active level.

    * Convenience method equivalent to isActive()
    *
    * @return true/false reflecting if pin is active.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
    bool __attribute__((always_inline)) isPressed() const {
      return isActive();
   }
   /**
    * Read pin value and return true if inactive level.

    * Convenience method equivalent to isInactive()
    *
    * @return true/false reflecting if pin is inactive.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
    bool __attribute__((always_inline)) isReleased() const {
      return isInactive();
   }

   /**
    * Set pin to active level (if configured as output)
    *
    * @note Polarity _is_ significant
    *
    * ActiveHigh: bm = 1, fm = 0 PSOR <- 1 = bm^fm, PCOR <- 0 = fm
    * ActiveLow:  bm = 1, fm = 1 PSOR <- 0 = bm^fm, PCOR <- 1 = fm
    */
   void setActive() const {
      gpio->PSOR = bitMask^flipMask;
      gpio->PCOR = flipMask;
   }
   /**
    * Set pin to inactive level (if configured as output)
    *
    * @note Polarity _is_ significant
    *
    * ActiveHigh: bm = 1, fm = 0 PSOR <- 0 = fm, PCOR <- 1 = bm^fm
    * ActiveLow:  bm = 1, fm = 1 PSOR <- 1 = fm, PCOR <- 0 = bm^fm
    */
   void setInactive() const {
      gpio->PSOR = flipMask;
      gpio->PCOR = bitMask^flipMask;
   }
   /**
    * Read pin value
    *
    * @return true/false reflecting pin value.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
   bool read() const {
      return ((gpio->PDIR^flipMask) & bitMask) != 0;
   }
   /**
    * Read value being driven to pin (if configured as output)
    *
    * @return true/false reflecting value in output register.
    *
    * @note This reads the PDOR
    * @note Polarity _is_ significant
    */
   bool readState() const {
      return ((gpio->PDOR^flipMask) & bitMask) != 0;
   }
#endif
};

/**
 * @addtogroup GPIO_Group GPIO, Digital Input/Output
 * @brief General Purpose Input/Output
 * @{
 */

/**
 * @brief Template representing a pin with Digital I/O capability
 *
 * <b>Example</b>
 * @code
 * // Instantiate
 * using Pta3 = USBDM::Gpio_T<SIM_SCGC5_PORTA_MASK, PORTA_BasePtr, PORTA_IRQn, GPIOA_BasePtr, 3, ActiveHigh>;
 *
 * // Set as digital output
 * Pta3::setOutput();
 *
 * // Set pin high
 * Pta3::set();
 *
 * // Set pin low
 * Pta3::clear();
 *
 * // Toggle pin
 * Pta3::toggle();
 *
 * // Set pin to boolean value
 * Pta3::write(true);
 *
 * // Set pin to boolean value
 * Pta3::write(false);
 *
 * // Set as digital input
 * Pta3::setInput();
 *
 * // Read pin as boolean value
 * bool x = Pta3::read();
 *
 * @endcode
 *
 *
 * @tparam pinIndex  Index into tables describing individual pins e.g. PTA = 0..7, PTB = 8..25 etc
 * @tparam polarity  Polarity of pin. Either ActiveHigh or ActiveLow
 */
template<PinIndex pinIndex, Polarity polarity>
class Gpio_T : public Gpio, public Port_T<pinIndex> {

   // Restrict to available ports
   static_assert((pinIndex>=PinIndex::MIN_PIN_INDEX)&&(pinIndex<PinIndex::MAX_PIN_INDEX),
      "Illegal bit number for left or right in GpioField");


private:
   /**
    * This class is not intended to be instantiated
    */
   Gpio_T(const Gpio_T&) = delete;
   Gpio_T(Gpio_T&&) = delete;

protected:
//   constexpr Gpio_T() : Gpio(gpioAddress, PinNum, polarity) {};

public:

   /**
    * Get GPIO base register address from bit index
    *
    * @param pinInd Index of GPIO bit
    *
    * @return Base address of GPIO_Type hardware structure
    */
   static constexpr uint32_t getGpioAddress(PinIndex pinInd) {
      constexpr uint32_t gpioAddresses[] = {
            GPIOA_BasePtr,
      };
      return gpioAddresses[unsigned(pinInd) / 32];
   }

   // Pin index into tables describing individual pins e.g. PTA = 0..7, PTB = 8..25 etc
   static constexpr PinIndex PININDEX = pinIndex;

   // Bit number for port pin within individual port e.g. GPIOB[31..0]
   static constexpr PinNum BITNUM   = (unsigned(pinIndex)%32);

   // Bit mask for port pin within individual port e.g. GPIOB[31..0]
   static constexpr uint32_t BITMASK = (1<<BITNUM);

   static constexpr uint32_t gpioAddress = getGpioAddress(pinIndex);

   /** Get base address of GPIO hardware as pointer to struct */
   static constexpr HardwarePtr<GPIO_Type> gpio = gpioAddress;

   /// Base address of GPIO hardware
   static constexpr uint32_t gpioBase = gpioAddress;
   /// Address of PDOR register in GPIO
   static constexpr uint32_t gpioPDOR = gpioAddress+offsetof(GPIO_Type, PDOR);
   /// Address of PSOR register in GPIO
   static constexpr uint32_t gpioPSOR = gpioAddress+offsetof(GPIO_Type, PSOR);
   /// Address of PCOR register in GPIO
   static constexpr uint32_t gpioPCOR = gpioAddress+offsetof(GPIO_Type, PCOR);
   /// Address of PTOR register in GPIO
   static constexpr uint32_t gpioPTOR = gpioAddress+offsetof(GPIO_Type, PTOR);
   /// Address of PDDR register in GPIO
   static constexpr uint32_t gpioPDDR = gpioAddress+offsetof(GPIO_Type, PDDR);
   /// Address of PDIR register in GPIO
   static constexpr uint32_t gpioPDIR = gpioAddress+offsetof(GPIO_Type, PDIR);

   /** Polarity of pin */
   static constexpr Polarity POLARITY = polarity;

#if true
   /**
    * Set pin as digital I/O.
    * Pin is initially set as an input.
    *
    * @note Resets the pin output value to the inactive state
    * @note Modifies the Port Input Disable Register value (PIDR value).
    * @note Use SetIn() and SetOut() for a lightweight change of direction without affecting other pin settings.
    */
   static void setInOut()  {
      // Enable pin input function
      gpio->PIDR = gpio->PIDR & ~BITMASK;
      // Make input initially
      setIn();
      // Set inactive pin state (if later made output)
      setInactive();
   }
   /**
    * Enable pin as digital output with initial inactive level.

    *
    * @note Modifies the Port Input Disable Register value (PIDR value).
    * @note Resets the pin value to the inactive state
    * @note Use SetIn() and SetOut() for a lightweight change of direction without affecting other pin settings.
    */
   static void setOutput()  {
      // Disable pin input function
      gpio->PIDR = gpio->PIDR | BITMASK;
      // Set initial level before enabling pin drive
      setInactive();
      // Make pin an output
      setOut();
   }
   /**
    * @brief
    * Enable pin as digital input.

    *
    * @note Modifies the Port Input Disable Register value (PIDR value).
    * @note Use SetIn() and SetOut() for a lightweight change of direction without affecting other pin settings.
    */
   static void setInput()  {
      // Enable pin as input
      gpio->PIDR = gpio->PIDR & ~BITMASK;
      // Make pin an input
      setIn();
   }
   /**
    * Set pin as digital input
    *
    * @note Does not affect other pin settings
    */
   static void setIn()  {
      // Make pin an input
      gpio->PDDR = gpio->PDDR & ~BITMASK;
   }
   /**
    * Set pin as digital output
    *
    * @note Does not affect other pin settings
    */
   static void setOut()  {
      // Make pin an output
      gpio->PDDR = gpio->PDDR | BITMASK;
   }
   /**
    * Set pin. Pin will be high if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
   static void high()  {
      gpio->PSOR = BITMASK;
   }
   /**
    * Clear pin. Pin will be low if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
   static void low()  {
      gpio->PCOR = BITMASK;
   }
   /**
    * Set pin. Pin will be high if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
   static void set()  {
      gpio->PSOR = BITMASK;
   }
   /**
    * Clear pin. Pin will be low if configured as an output.
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
   static void clear()  {
      gpio->PCOR = BITMASK;
   }
   /**
    * Toggle pin (if output)
    */
   static void toggle()  {
      gpio->PTOR = BITMASK;
   }
   /**
    * Read pin value
    *
    * @return true/false reflecting pin value.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
   static bool readBit()  {
      return isHigh();
   }
   /**
    * Write boolean value to pin (if configured as output)
    *
    * @param[in] value true/false value
    *
    * @note Polarity _is_ _not_ significant
    * @note Do not use this method unless dealing with very low-level I/O
    */
   static void  __attribute__((always_inline)) writeBit(bool value)  {
      if (value) {
         set();
      }
      else {
         clear();
      }
   }
   /**
    * Set pin to active level (if configured as output).
    * Convenience method for setActive()
    *
    * @note Polarity _is_ significant
    */
   static void __attribute__((always_inline)) on()  {
      setActive();
   }
   /**
    * Set pin to inactive level (if configured as output).
    * Convenience method for setInactive()
    *
    * @note Polarity _is_ significant
    */
   static void __attribute__((always_inline)) off()  {
      setInactive();
   }
   /**
    * Write boolean value to pin (if configured as output)
    *
    * @param[in] value true/false value
    *
    * @note Polarity _is_ significant
    */
   static void write(bool value)  {
      if (value) {
         setActive();
      }
      else {
         setInactive();
      }
   }
   /**
    * Checks if pin is high
    *
    * @return true/false reflecting value on pin
    *
    * @note This reads the PDIR
    * @note Polarity _is_ _not_ significant
    */
   static bool isHigh()  {
      return (gpio->PDIR & BITMASK) != 0;
   }
   /**
    * Checks if pin is low
    *
    * @return true/false reflecting value on pin
    *
    * @note This reads the PDIR
    * @note Polarity _is_ _not_ significant
    */
   static bool isLow()  {
      return (gpio->PDIR & BITMASK) == 0;
   }
   /**
    * Read pin value and return true if active level.
    * Equivalent to read()
    *
    * @return true/false reflecting if pin is active.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
   static bool __attribute__((always_inline)) isActive()  {
      return read();
   }
   /**
    * Read pin value and return true if inactive level
    * Equivalent to !read()
    *
    * @return true/false reflecting if pin is inactive.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
   static bool __attribute__((always_inline)) isInactive()  {
      return !read();
   }
   /**
    * Read pin value and return true if active level.

    * Convenience method equivalent to isActive()
    *
    * @return true/false reflecting if pin is active.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
   static bool __attribute__((always_inline)) isPressed()  {
      return isActive();
   }
   /**
    * Read pin value and return true if inactive level.

    * Convenience method equivalent to isInactive()
    *
    * @return true/false reflecting if pin is inactive.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
   static bool __attribute__((always_inline)) isReleased()  {
      return isInactive();
   }
   /**
    * Set pin to active level (if configured as output)
    *
    * @note Polarity _is_ significant
    */
   static void setActive() {
      if constexpr (isActiveLow(polarity)) {
         clear();
      }
      else {
         set();
      }
   }
   /**
    * Set pin to inactive level (if configured as output)
    *
    * @note Polarity _is_ significant
    */
   static void setInactive() {
      if constexpr (isActiveLow(polarity)) {
         set();
      }
      else {
         clear();
      }
   }
   /**
    * Read pin value
    *
    * @return true/false reflecting pin value.
    *
    * @note This reads the PDIR
    * @note Polarity _is_ significant
    */
   static bool read() {
      if constexpr (isActiveLow(polarity)) {
         return isLow();
      }
      else {
         return isHigh();
      }
   }
   /**
    * Read value being driven to pin (if configured as output)
    *
    * @return true/false reflecting value in output register.
    *
    * @note This reads the PDOR
    * @note Polarity _is_ significant
    */
   static bool readState() {
#ifdef RELEASE_BUILD
      uint32_t t = bmeExtract(gpio->PDOR, BITNUM, 1);
#else
      uint32_t t = gpio->PDOR & BITMASK;
#endif
      if constexpr (isActiveLow(polarity)) {
         return !t;
      }
      else {
         return t;
      }
   }
#endif
};

/**
 * Class representing GPIO field functionality
 */
class GpioField {

public:
   /// Underlying GPIO hardware
   const HardwarePtr<GPIO_Type>  gpio;

   /// Mask for GPIO bits being manipulated
   const uint32_t                bitMask;

   /// Mask to flip bit values if active low (0 otherwise)
   const uint32_t                flipMask;

   /// Offset of bit field in GPIO hardware
   const PinNum                  right;

protected:
   /**
    * Class representing GPIO field functionality
    *
    * @param gpio       GPIO hardware address
    * @param bitMask    Bitmask for bits affected in port
    * @param right      Rightmost bit number in port
    * @param flipMask   Mask to flip bits in port (or use ActiveHigh/ActiveLow)
    */
   constexpr GpioField(uint32_t gpio, uint32_t bitMask, PinNum right, uint32_t flipMask) :
      gpio(gpio), bitMask(bitMask), flipMask(flipMask), right(right) {
   }

public:
   /**
    * Set all pins in field as inputs.
    *
    * @note Does not affect other pin settings.
    */
   void setIn() const {
      gpio->PDDR = gpio->PDDR & ~bitMask;
   }
   /**
    * Set all pins in field as outputs.
    *
    * @note Does not affect other pin settings.
    */
   void setOut() const {
      gpio->PDDR = gpio->PDDR | bitMask;
   }
   /**
    * Set individual pin directions
    *
    * @param[in] mask Mask for pin directions (1=>out, 0=>in)
    *
    * @note Does not affect other pin settings.
    */
   void setDirection(uint32_t mask) const {
      USBDM::CriticalSection cs;
      gpio->PDDR = (gpio->PDDR&~bitMask)|((mask<<right)&bitMask);
   }
   /**
    * Set bits in field output register
    *
    * @param[in] mask Mask to apply to the field (1 => set bit, 0 => unchanged)
    *
    * @note Polarity _is_ _not_ significant
    */
   void bitSet(const uint32_t mask) const {
      gpio->PSOR = (mask<<right)&bitMask;
   }
   /**
    * Clear bits in field output register
    *
    * @param[in] mask Mask to apply to the field (1 => clear bit, 0 => unchanged)
    *
    * @note Polarity _is_ _not_ significant
    */
   void bitClear(const uint32_t mask) const {
      gpio->PCOR = (mask<<right)&bitMask;
   }
   /**
    * Toggle bits in field output register
    *
    * @param[in] mask Mask to apply to the field (1 => toggle bit, 0 => unchanged)
    */
   void bitToggle(const uint32_t mask) const {
      gpio->PTOR = (mask<<right)&bitMask;
   }
   /**
    * Read field as unmodified bit field
    *
    * @return Value from field pins
    *
    * @note Polarity _is_ _not_ significant
    */
   uint32_t bitRead() const {
      return (gpio->PDIR & bitMask)>>right;
   }
   /**
    * Read field
    *
    * @return value from field pins
    *
    * @note Polarity _is_ significant
    */
   uint32_t read() const {
      return ((gpio->PDIR^flipMask)&bitMask)>>right;
   }
   /**
    * Read value being driven to field pins (if configured as output)
    *
    * @return value from field output register
    *
    * @note This reads the PDOR
    * @note Polarity _is_ significant
    */
   bool readState() const {
      return ((gpio->PDOR^flipMask)&bitMask)>>right;
   }
   /**
    * Write field
    *
    * @param[in] value to insert as field
    *
    * @note Polarity _is_ significant
    */
   void write(uint32_t value) const {
      USBDM::CriticalSection cs;
      gpio->PDOR = ((gpio->PDOR) & ~bitMask) | (((value<<right)^flipMask)&bitMask);
   }

   /**
    * Write bit field
    *
    * @param[in] value to insert as field
    *
    * @note Polarity _is_ _not_ significant
    */
   void bitWrite(uint32_t value) const {
      USBDM::CriticalSection cs;
      gpio->PDOR = ((gpio->PDOR) & ~bitMask) | ((value<<right)&bitMask);
   }
};

/**
 * @brief Template representing a field within a port
 *
 * <b>Example</b>
 * @code
 * // Instantiate object representing Port A 6 down to 3
 * using Pta6_3 = GpioField_T<GpioAInfo, 6, 3>;
 *
 * // Set as digital output
 * Pta6_3::setOutput();
 *
 * // Write value to field
 * Pta6_3::write(0x53);
 *
 * // Clear all of field
 * Pta6_3::bitClear();
 *
 * // Clear lower two bits of field
 * Pta6_3::bitClear(0x3);
 *
 * // Set lower two bits of field
 * Pta6_3::bitSet(0x3);
 *
 * // Set as digital input
 * Pta6_3::setInput();
 *
 * // Read pin as int value
 * int x = Pta6_3::read();
 * @endcode
 *
 * @tparam bitIndexLeft         Bit number of leftmost bit in GPIO (inclusive)
 * @tparam bitIndexRight        Bit number of rightmost bit in GPIO (inclusive)
 * @tparam FlipMask             Polarity of all bits in field. Either ActiveHigh, ActiveLow or a bitmask (0=>bit active-high, 1=>bit active-low)
 */
template<PinIndex bitIndexLeft, PinIndex bitIndexRight, uint32_t FlipMask=ActiveHigh>
class GpioField_T : public GpioField, public PortField_T<bitIndexLeft, bitIndexRight> {

   // Restrict to same Port i.e. 8 bits wide
   // In practice it could extend across Ports A-B-C-D or E-F-G-H as they are accessed through the same GPIO register
   static_assert((bitIndexLeft<PinIndex::MAX_PIN_INDEX)&&
         ((unsigned(bitIndexLeft)&~0b111)==(unsigned(bitIndexRight)&~0b111))&&
         (bitIndexLeft>=bitIndexRight),
      "Illegal bit number for left or right in GpioField");

private:
   /**
    * This class is not intended to be instantiated
    */
   GpioField_T(const GpioField_T&) = delete;
   GpioField_T(GpioField_T&&) = delete;

   void enablePortClocks() {
   }

public:
   /**
    * Get GPIO base register address from bit index
    *
    * @param pinInd Index of GPIO bit
    *
    * @return Base address of GPIO_Type hardware structure
    */
   static constexpr uint32_t getGpioAddress(PinIndex pinInd) {
      constexpr uint32_t gpioAddresses[] = {
            GPIOA_BasePtr,
      };
      return gpioAddresses[unsigned(pinInd) / 32];
   }

   /// Base address of GPIO hardware
   static constexpr uint32_t gpioAddress = getGpioAddress(bitIndexLeft);

   /// Left bit within used GPIO registers
   static constexpr PinNum Left  = unsigned(bitIndexLeft)%32;

   /// Right bit within used GPIO registers
   static constexpr PinNum Right = unsigned(bitIndexRight)%32;

   constexpr GpioField_T() : GpioField(gpioAddress, BITMASK, Right, FLIP_MASK) {}

   /** Get base address of GPIO hardware as pointer to struct */
   static constexpr HardwarePtr<GPIO_Type> gpio = gpioAddress;

   /// Base address of GPIO hardware
   static constexpr uint32_t gpioBase = gpioAddress;
   /// Address of PDOR register in GPIO
   static constexpr uint32_t gpioPDOR = gpioAddress+offsetof(GPIO_Type, PDOR);
   /// Address of PSOR register in GPIO
   static constexpr uint32_t gpioPSOR = gpioAddress+offsetof(GPIO_Type, PSOR);
   /// Address of PCOR register in GPIO
   static constexpr uint32_t gpioPCOR = gpioAddress+offsetof(GPIO_Type, PCOR);
   /// Address of PTOR register in GPIO
   static constexpr uint32_t gpioPTOR = gpioAddress+offsetof(GPIO_Type, PTOR);
   /// Address of PDDR register in GPIO
   static constexpr uint32_t gpioPDDR = gpioAddress+offsetof(GPIO_Type, PDDR);
   /// Address of PDIR register in GPIO
   static constexpr uint32_t gpioPDIR = gpioAddress+offsetof(GPIO_Type, PDIR);

public:
   /** Bit number of left bit within underlying port hardware */
   static constexpr unsigned LEFT = Left;

   /** Bit number of right bit within underlying port hardware */
   static constexpr unsigned RIGHT = Right;

   /** Mask for the bits being manipulated within underlying port hardware */
   static constexpr uint32_t BITMASK = static_cast<uint32_t>((1ULL<<(Left-Right+1))-1)<<Right;

   /** Mask to flip bits in field. Two special cases for later optimisation */
   static constexpr uint32_t FLIP_MASK =
         (((FlipMask<<Right)&BITMASK)==BITMASK)?0xFFFFFFFFUL:  // All active-low
         (((FlipMask<<Right)&BITMASK)==0)?0x00000000UL:        // All active-high
         (FlipMask<<Right);                                    // Mixed

   static_assert(((Left<=31)&&(Left>=Right)), "Illegal bit number for left or right in GpioField");
   static_assert((FlipMask==0xFFFFFFFFUL)||((((FlipMask<<Right)&BITMASK)>>Right)==FlipMask), "Illegal FlipMask (polarity) in GpioField");

   /**
    * Calculate Port bit-mask from field bit number
    *
    * @param PinNum  Bit number within field (left-right,0]
    *
    * @return Mask for given bit within underlying port hardware
    */
   static constexpr uint32_t mask(uint32_t PinNum) {
      return 1<<(PinNum+Right);
   }

   /**
    *  Disable Pins
    *  This sets the pins to MUX(0) which is specified for minimum leakage in low-power modes.
    *
    *  @note The clock is left enabled as shared with other pins.
    *  @note Mux(0) is also the Analogue MUX setting
    */
   static void disablePins() {
      // Enable clock to port
      enablePortClocks(); 

      // Disable pin input function
      BITSET(gpio->PIDR, BITMASK);

      // Disable pin output function
      BITCLEAR(gpio->PDDR, BITMASK);
   }

   /**
    * Set field as digital I/O.
    * Pins are initially set as an input.
    * Use setIn(), setOut() and setDirection() to change pin directions.
    *
    * @note Resets the pin output value to the inactive state
    */
   static void setInOut() {
      // Enable clock to port
      enablePortClocks();

      // Default to input

      // Enable pin input function
      BITCLEAR(gpio->PIDR, BITMASK);

      // Disable pin output function
      BITCLEAR(gpio->PDDR, BITMASK);

      // Default to output inactive
      write(0);
   }

   /**
    * Set all pins as digital outputs.
    *
    * @note Does not affect other pin settings
    */
   static void setOut() {
      BITSET(gpio->PDDR, BITMASK);
   }

   /**
    * Sets all pin as digital outputs.
    *
    * @note Use setOut(), setIn() or setDirection() for a lightweight change of direction without affecting other pin settings.
    */
   static void setOutput() {
      // Set initial level before enabling pin drive
      write(0);

      // Disable pin input function
      BITSET(gpio->PIDR, BITMASK);

      // Make pin an output
      BITSET(gpio->PDDR, BITMASK);
   }

   /**
    * Set all pins as digital inputs.
    *
    * @note Does not affect other pin settings
    */
   static void setIn() {
      BITCLEAR(gpio->PDDR, BITMASK);
   }

   /**
    * Set all pins as digital inputs.
    *
    * @note Use setOut(), setIn() or setDirection() for a lightweight change of direction without affecting other pin settings.
    */
   static void setInput() {
      setInOut();
   }

   /**
    * Set individual pin directions
    *
    * @param[in] mask Mask for pin directions (1=>out, 0=>in)
    *
    * @note Does not affect other pin settings
    */
   static void setDirection(uint32_t mask) {
      bmeInsert(gpio->PDDR, Right, Left-Right+1, mask);
   }
   /**
    * Set bits in field
    *
    * @param[in] mask Mask to apply to the field (1 => set bit, 0 => unchanged)
    *
    * @note Polarity _is_ _not_ significant
    */
   static void bitSet(const uint32_t mask) {
      gpio->PSOR = (mask<<Right)&BITMASK;
   }
   /**
    * Clear bits in field
    *
    * @param[in] mask Mask to apply to the field (1 => clear bit, 0 => unchanged)
    *
    * @note Polarity _is_ _not_ significant
    */
   static void bitClear(const uint32_t mask) {
      gpio->PCOR = (mask<<Right)&BITMASK;
   }
   /**
    * Toggle bits in field
    *
    * @param[in] mask Mask to apply to the field (1 => toggle bit, 0 => unchanged)
    */
   static void bitToggle(const uint32_t mask) {
      gpio->PTOR = (mask<<Right)&BITMASK;
   }
   /**
    * Read field as unmodified bit field
    *
    * @return value from field
    *
    * @note Polarity _is_ _not_ significant
    */
   static uint32_t bitRead() {
      return bmeExtract(gpio->PDIR, Right, Left-Right+1);
   }
   /**
    * Read field
    *
    * @return value from field
    *
    * @note Polarity _is_ significant
    */
   static uint32_t read() {
      if constexpr (FLIP_MASK==0) {
         return bmeExtract(gpio->PDIR, Right, Left-Right+1);
      }
      else {
         return bmeExtract(gpio->PDIR, Right, Left-Right+1)^(FLIP_MASK>>Right);
      }
   }
   /**
    * Read value being driven to field pins (if configured as output)
    *
    * @return value from field output
    *
    * @note This reads the PDOR
    * @note Polarity _is_ significant
    */
   static uint32_t readState() {
      if constexpr (FLIP_MASK==0) {
         return bmeExtract(gpio->PDOR, Right, Left-Right+1);
      }
      else {
         return bmeExtract(gpio->PDOR, Right, Left-Right+1)^(FLIP_MASK>>Right);
      }
   }
   /**
    * Write field
    *
    * @param[in] value to insert as field
    *
    * @note Polarity _is_ significant
    */
   static void write(uint32_t value) {
      if constexpr (FLIP_MASK == 0xFFFFFFFFUL) {
         value = ~value;
      }
      else if constexpr (FLIP_MASK != 0) {
         value = value^(FLIP_MASK>>Right);
      }
      bmeInsert(gpio->PDOR, Right, Left-Right+1, value);
   }

   /**
    * Write bit field
    *
    * @param[in] value to insert as field
    *
    * @note Polarity _is_ _not_ significant
    */
   static void bitWrite(uint32_t value) {
      bmeInsert(gpio->PDOR, Right, Left-Right+1, value);
   }
};

   /**
    * Class representing a field within Port A
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the GPIO e.g. GPIOA[5..3] => A<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the GPIO e.g. GPIOA[5..3] => A<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <unsigned leftPinNum, unsigned rightPinNum, Polarity polarity=ActiveHigh>
   class WideGpioFieldA : public GpioField_T<PinIndex(leftPinNum+0), PinIndex(rightPinNum+0), polarity> {};

   /**
    * Class representing individual pins in Port A
    *
    * @tparam pinNum Indicates pin within the port e.g. A3 = > A<3>
    * @tparam polarity Polarity of the pin. Either ActiveHigh or ActiveLow
    */
   template <PinNum pinNum, Polarity polarity=ActiveHigh>
      class GpioA : public Gpio_T<PinIndex(pinNum+0), polarity> {};
   
   /**
    * Class representing a field within Port A
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the port e.g. PTA[5..3] => A<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the port e.g. PTA[5..3] => A<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <PinNum leftPinNum, PinNum rightPinNum, Polarity polarity=ActiveHigh>
       class GpioAField : public GpioField_T<PinIndex(leftPinNum+0), PinIndex(rightPinNum+0), polarity> {};

   /**
    * Class representing individual pins in Port B
    *
    * @tparam pinNum Indicates pin within the port e.g. B3 = > B<3>
    * @tparam polarity Polarity of the pin. Either ActiveHigh or ActiveLow
    */
   template <PinNum pinNum, Polarity polarity=ActiveHigh>
      class GpioB : public Gpio_T<PinIndex(pinNum+8), polarity> {};
   
   /**
    * Class representing a field within Port B
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the port e.g. PTB[5..3] => B<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the port e.g. PTB[5..3] => B<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <PinNum leftPinNum, PinNum rightPinNum, Polarity polarity=ActiveHigh>
       class GpioBField : public GpioField_T<PinIndex(leftPinNum+8), PinIndex(rightPinNum+8), polarity> {};

   /**
    * Class representing individual pins in Port C
    *
    * @tparam pinNum Indicates pin within the port e.g. C3 = > C<3>
    * @tparam polarity Polarity of the pin. Either ActiveHigh or ActiveLow
    */
   template <PinNum pinNum, Polarity polarity=ActiveHigh>
      class GpioC : public Gpio_T<PinIndex(pinNum+16), polarity> {};
   
   /**
    * Class representing a field within Port C
    *
    * @tparam leftPinNum  Indicates left pin (inclusive) within the port e.g. PTC[5..3] => C<5,...>
    * @tparam rightPinNum Indicates right pin (inclusive) within the port e.g. PTC[5..3] => C<...,3>
    * @tparam polarity    Polarity of field. Either ActiveHigh or ActiveLow
    */
   template <PinNum leftPinNum, PinNum rightPinNum, Polarity polarity=ActiveHigh>
       class GpioCField : public GpioField_T<PinIndex(leftPinNum+16), PinIndex(rightPinNum+16), polarity> {};


/**
 * End GPIO_Group
 * @}
 */

#pragma GCC pop_options

} // End namespace USBDM

#undef BITSET
#undef BITCLEAR
#undef BITTOGGLE
#undef BITSET
#undef BITCLEAR
#undef BITTOGGLE

#endif /* HEADER_GPIO_H */
