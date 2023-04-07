/**
 * @file     i2c.h (180.ARM_Peripherals/Project_Headers/i2c.h)
 * @brief    I2C interface
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */
#ifndef HEADER_I2C_H
#define HEADER_I2C_H
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "pin_mapping.h"

#ifdef __CMSIS_RTOS
#include "cmsis.h"
#endif

namespace USBDM {

/**
 * @addtogroup I2C_Group I2C, Inter-Integrated-Circuit Interface
 * @brief C++ Class allowing access to I2C interface
 * @{
 */

/**
 * Type definition for interrupt call back
 * @param status Interrupt status value from SPI->SR
 */
typedef void (*I2cCallbackFunction)();

/**
 * I2C Operating mode
 */
enum I2cMode {
   I2cMode_Polled    = I2C_C1_IICIE(0),   //!< Operate in Polled mode
   I2cMode_Interrupt = I2C_C1_IICIE(1),   //!< Operate in Interrupt mode
};

/**
 * Virtual Base class for I2C interface
 */
class I2c {

public:
   /** States for the I2C state machine */
   enum I2C_State { i2c_idle, i2c_txData, i2c_rxData, i2c_rxAddress };

   I2C_State           state;               //!< State of current transaction

protected:
   static constexpr unsigned TIMEOUT_LIMIT = 100000;

   /** Callback to catch unhandled interrupt */
   static void unhandledCallback() {
      // Not considered an error as may be using polling
   }

   const HardwarePtr<I2C_Type> i2c;                 //!< I2C hardware instance
   const I2cMode               i2cMode;             //!< Mode of operation (I2cMode_Interrupt/I2cMode_Polled)
   uint16_t                    rxBytesRemaining;    //!< Number of receive bytes remaining in current transaction
   uint16_t                    txBytesRemaining;    //!< Number of transmit bytes remaining in current transaction
   uint8_t                    *rxDataPtr;           //!< Pointer to receive data for current transaction
   const uint8_t              *txDataPtr;           //!< Pointer to transmit data for current transaction
   uint8_t                     addressedDevice;     //!< Address of device being communicated with
   ErrorCode                   errorCode;           //!< Error code from last transaction

   /** I2C baud rate divisor table */
   static const uint16_t I2C_DIVISORS[4*16];

   /**
    * Construct I2C interface
    *
    * @param[in]  i2c     Base address of I2C hardware
    * @param[in]  i2cMode Mode of operation (I2cMode_Interrupt or I2cMode_Polled)
    */
   I2c(uint32_t i2c, I2cMode i2cMode) :
      state(i2c_idle), i2c(i2c), i2cMode(i2cMode), rxBytesRemaining(0),
      txBytesRemaining(0), rxDataPtr(0), txDataPtr(0), addressedDevice(0),
      errorCode(E_NO_ERROR) {
   }

   /**
    * Destructor
    */
   ~I2c() {}

   /**
    * Calculate value for baud rate register of I2C
    *
    * This is calculated from processor bus frequency and given bps
    *
    * @param[in]  bps            Interface speed in bits-per-second
    * @param[in]  clockFrequency Frequency of I2C input clock
    *
    * @return I2C_F value representing speed
    */
   static uint8_t getBPSValue(uint32_t bps, uint32_t clockFrequency);

   /**
    * Start Rx/Tx sequence by sending address byte
    *
    * @param[in]  address - address of slave to access
    */
   void sendAddress(uint8_t address);

   /**
    * Set baud factor value for interface
    *
    * This is calculated from processor frequency and given bits-per-second
    *
    * @param[in]  bps            - Interface speed in bits-per-second
    * @param[in]  clockFrequency - Frequency of I2C input clock
    */
   void setBPS(uint32_t bps, uint32_t clockFrequency) {
      i2c->F = getBPSValue(bps, clockFrequency);
   }

public:

#ifdef __CMSIS_RTOS
   /**
    * Obtain I2C MUTEX
    *
    * @param[in]  milliseconds How long to wait in milliseconds. Use osWaitForever for indefinite wait
    *
    * @return osOK: The mutex has been obtain.
    * @return osErrorTimeoutResource: The mutex could not be obtained in the given time.
    * @return osErrorResource: The mutex could not be obtained when no timeout was specified.
    * @return osErrorParameter: The parameter mutex_id is incorrect.
    * @return osErrorISR: osMutexWait cannot be called from interrupt service routines.
    */
   virtual osStatus startTransaction(int milliseconds=osWaitForever) = 0;

   /**
    * Release I2C MUTEX
    *
    * @return osOK: the mutex has been correctly released.
    * @return osErrorResource: the mutex was not obtained before.
    * @return osErrorISR: osMutexRelease cannot be called from interrupt service routines.
    */
   virtual osStatus endTransaction() = 0;
#else
   /**
    * Obtain I2C MUTEX - dummy
    */
   virtual ErrorCode startTransaction(int =0) {return E_NO_ERROR;};
   /**
    * Release I2C MUTEX - dummy
    */
   virtual ErrorCode endTransaction() {return E_NO_ERROR;};
#endif

   /**
    * Clear bus hang
    *
    * Generates I2C_SCL clock until I2C_SDA goes high.
    *
    * This is useful if a slave is part-way through a transaction when the master goes away!
    */
   virtual void busHangReset() = 0;

   /**
    * Wait for current sequence to complete
    */
   void waitWhileBusy(void) {
      I2C_State lastState = state;
      unsigned timeout = TIMEOUT_LIMIT;
      while ((state != i2c_idle) && (--timeout>0)) {
         if (state != lastState) {
            // Restart timeout
            timeout = TIMEOUT_LIMIT;
            lastState = state;
         }
         if ((i2c->C1&I2C_C1_IICIE_MASK) == 0) {
            poll();
         }
         else {
            __asm__("wfi");
         }
      }
      if (state != i2c_idle) {
         errorCode = E_TIMEOUT;
         busHangReset();
      }
   }

   /**
    * I2C state-machine poll function.
    * May be called by polling loop or interrupt handler.
    */
   virtual void poll(void);

   /**
    * Transmit message
    * Note: 0th byte of Tx is often register address.
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  size     Size of transmission data
    * @param[in]  data     Data to transmit
    *
    * @return E_NO_ERROR on success
    */
   ErrorCode transmit(uint8_t address, uint16_t size, const uint8_t data[]);

   /**
    * Transmit message.
    * Note: 0th byte of Tx is often register address.
    *
    * @tparam txSize number of bytes to transmit
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  data     Data to transmit (size of transmission is inferred from array size).
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned txSize>
   ErrorCode transmit(uint8_t address, const uint8_t (&data)[txSize]) {
      return transmit(address, txSize, data);
   }

   /**
    * Transmit message.
    * Note: 0th byte of Tx is often register address.
    *
    * @tparam N number of bytes to transmit (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  data     Data to transmit (size of transmission is inferred from array size).
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned N>
   ErrorCode transmit(uint8_t address, const std::array<uint8_t, N> &data) {
      return transmit(address, N, data.data());
   }

   /**
    * Transmit message.
    * Note: 0th byte of Tx is often register address.
    *
    * @tparam N number of bytes to transmit (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  data     Data to transmit (size of transmission is inferred from array size).
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned N>
   ErrorCode transmit(uint8_t address, const std::array<const uint8_t, N> &data) {
      return transmit(address, N, data.data());
   }

   /**
    * Receive message
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  size     Size of reception data
    * @param[out] data     Data buffer for reception
    *
    * @return E_NO_ERROR on success
    */
   ErrorCode receive(uint8_t address, uint16_t size,  uint8_t data[]);

   /**
    * Receive message
    *
    * @tparam rxSize number of bytes to receive (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[out] data     Data buffer for reception (size of reception is inferred from array size)
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned rxSize>
   ErrorCode receive(uint8_t address, uint8_t (&data)[rxSize]) {
      return receive(address, rxSize, data);
   }

   /**
    * Receive message
    *
    * @tparam rxSize number of bytes to receive (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[out] data     Data buffer for reception (size of reception is inferred from array size)
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned rxSize>
   ErrorCode receive(uint8_t address, std::array<uint8_t, rxSize> &data) {
      return receive(address, rxSize, data.data());
   }

   /**
    * Transmit message followed by receive message.
    * Note: 0th byte of Tx is often register address.
    *
    * Uses repeated-start.
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  txSize   Size of transmission data
    * @param[in]  txData   Data for transmission
    * @param[in]  rxSize   Size of reception data
    * @param[out] rxData   Date buffer for reception
    *
    * @return E_NO_ERROR on success
    */
   ErrorCode txRx(uint8_t address, uint16_t txSize, const uint8_t txData[], uint16_t rxSize, uint8_t rxData[] );

   /**
    * Transmit message followed by receive message.
    * Note: 0th byte of Tx is often register address.
    *
    * Uses repeated-start.
    *
    * @tparam TxSize Number of bytes to transmit (inferred)
    * @tparam RxSize Number of bytes to receive (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  txData   Data for transmission (Tx size inferred from array size)
    * @param[out] rxData   Date buffer for reception (Rx size inferred from array size)
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned TxSize, unsigned RxSize>
   ErrorCode txRx(uint8_t address, const uint8_t (&txData)[TxSize], uint8_t (&rxData)[RxSize] ) {
      return txRx(address, TxSize, txData, RxSize, rxData);
   }

   /**
    * Transmit message followed by receive message.
    * Note: 0th byte of Tx is often register address.
    *
    * Uses repeated-start.
    *
    * @tparam TxSize Number of bytes to transmit (inferred)
    * @tparam RxSize Number of bytes to receive (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  txData   Data for transmission (Tx size inferred from array size)
    * @param[out] rxData   Date buffer for reception (Rx size inferred from array size)
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned TxSize, unsigned RxSize>
   ErrorCode txRx(uint8_t address, const std::array<uint8_t, TxSize> &txData, std::array<uint8_t, RxSize> &rxData) {
      return txRx(address, TxSize, txData.data(), RxSize, rxData.data());
   }

   /**
    * Transmit message followed by receive message.
    * Note: 0th byte of Tx is often register address.
    *
    * Uses repeated-start.
    *
    * @tparam TxSize Number of bytes to transmit (inferred)
    * @tparam RxSize Number of bytes to receive (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  txData   Data for transmission (Tx size inferred from array size)
    * @param[out] rxData   Date buffer for reception (Rx size inferred from array size)
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned TxSize, unsigned RxSize>
   ErrorCode txRx(uint8_t address, const std::array<const uint8_t, TxSize> &txData, std::array<uint8_t, RxSize> &rxData) {
      return txRx(address, TxSize, txData.data(), RxSize, rxData.data());
   }

   /**
    * Transmit message followed by receive message.
    * Note: 0th byte of Tx is often register address.
    *
    * Uses repeated-start.
    *
    * @tparam RxSize Number of bytes to receive (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  txSize   Size of transmission data
    * @param[in]  txData   Data for transmission
    * @param[out] rxData   Date buffer for reception (Rx size inferred from array size)
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned RxSize>
   ErrorCode txRx(uint8_t address, uint16_t txSize, const uint8_t txData[], uint8_t (&rxData)[RxSize] ) {
      return txRx(address, txSize, txData, RxSize, rxData);
   }

   /**
    * Transmit message followed by receive message.
    * Note: 0th byte of Tx is often register address.
    *
    * Uses repeated-start.
    *
    * @tparam TxSize Number of bytes to transmit (inferred)
    *
    * @param[in]  address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]  txData   Data for transmission (Tx size inferred from array size)
    * @param[in]  rxSize   Size of reception data
    * @param[out] rxData   Date buffer for reception
    *
    * @return E_NO_ERROR on success
    */
   template<unsigned TxSize>
   ErrorCode txRx(uint8_t address, const uint8_t (&txData)[TxSize], uint16_t rxSize, uint8_t rxData[] ) {
      return txRx(address, TxSize, txData, rxSize, rxData);
   }

   /**
    * Transmit message followed by receive message.
    * Uses repeated-start.\n
    * Uses shared transmit and receive buffer
    *
    * @param[in]    address  Address of slave to communicate with (should include LSB = R/W bit = 0)
    * @param[in]    txSize   Size of transmission data
    * @param[in]    rxSize   Size of reception data
    * @param[inout] data     Data for transmission and reception
    *
    * @return E_NO_ERROR on success
    */
   ErrorCode txRx(uint8_t address, uint16_t txSize, uint16_t rxSize, uint8_t data[] );

};

/**
 * @brief Template class representing an I2C interface
 *
 * <b>Example</b>
 *
 * @code
 *  // Instantiate interface
 *  I2c *i2c0 = new USBDM::I2cBase_T<I2cInfo>();
 *
 *  // Transmit data
 *  const uint8_t txDataBuffer[] = {0x11, 0x22, 0x33, 0x44};
 *
 *  // Receive buffer
 *  uint8_t rxDataBuffer[5];
 *
 *  for(;;) {
 *     // Transmit block
 *     i2c->transmit(0x1D<<1, sizeof(txDataBuffer), txDataBuffer);
 *
 *     // Receive block
 *     i2c->receive((0x1D<<1)|1, sizeof(rxDataBuffer), rxDataBuffer);
 *
 *     // Transmit block followed by read block (using repeated-start)
 *     // Note rxDataBuffer may be the same as txDataBuffer
 *     i2c->txRx(0x1D<<1, sizeof(txDataBuffer), txDataBuffer, sizeof(rxDataBuffer), rxDataBuffer);
 *  }
 *  @endcode
 *
 * @tparam Info            Class describing I2C hardware
 */
template<class Info> class I2cBase_T : public I2c {

public:
   // Handle on I2C hardware
   static constexpr volatile I2C_Type *I2C = Info::i2c;

   /** Used by ISR to obtain handle of object */
   static I2c *thisPtr;

   /** Callback function for ISR */
   static I2cCallbackFunction sCallback;

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

#ifdef __CMSIS_RTOS
protected:
   /**
    * Mutex to protect access\n
    * Using a static accessor function avoids issues with static object initialisation order
    *
    * @return mutex
    */
   static CMSIS::Mutex &mutex() {
      static CMSIS::Mutex mutex;
      return mutex;
   }

public:
   /**
    * Obtain I2C mutex
    *
    * @param[in]  milliseconds How long to wait in milliseconds. Use osWaitForever for indefinite wait
    *
    * @return osOK: The mutex has been obtain.
    * @return osErrorTimeoutResource: The mutex could not be obtained in the given time.
    * @return osErrorResource: The mutex could not be obtained when no timeout was specified.
    * @return osErrorParameter: The parameter mutex_id is incorrect.
    * @return osErrorISR: osMutexWait cannot be called from interrupt service routines.
    */
   virtual osStatus startTransaction(int milliseconds=osWaitForever) override {
      return mutex().wait(milliseconds);
   }

   /**
    * Release I2C mutex
    *
    * @return osOK: the mutex has been correctly released.
    * @return osErrorResource: the mutex was not obtained before.
    * @return osErrorISR: osMutexRelease cannot be called from interrupt service routines.
    */
   virtual osStatus endTransaction() override {
      return mutex().release();
   }
#endif

public:
   // Template _mapPinsOption_on.xml

   /**
    * Configures all mapped pins associated with I2C
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
    * Disabled all mapped pins associated with I2C
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
    * Basic enable of I2C
    * Includes enabling clock and configuring all mapped pins if mapPinsOnEnable is selected in configuration
    */
   static void enable() {
      Info::enableClock();
      configureAllPins();
   }

   /**
    * Disables the clock to I2C and all mapped pins
    */
   static void disable() {
      disableNvicInterrupts();
      
      disableAllPins();
      Info::disableClock();
   }
// End Template _mapPinsOption_on.xml

   /**
    * Construct I2C interface
    *
    * @param[in]  bps        Tx/Rx rate
    * @param[in]  i2cMode    Mode of operation
    * @param[in]  myAddress  Address of this device on bus (not currently used)
    */
   I2cBase_T(unsigned bps=400000, I2cMode i2cMode=I2cMode_Polled, uint8_t myAddress=0) : I2c(Info::baseAddress, i2cMode) {

      // Check pin assignments
      static_assert(Info::info[Info::sclPin].gpioBit >= 0, "I2Cx_SCL has not been assigned to a pin - Modify Configure.usbdm");
      static_assert(Info::info[Info::sdaPin].gpioBit >= 0, "I2Cx_SDA has not been assigned to a pin - Modify Configure.usbdm");

//      busHangReset();

      init(myAddress);
      setBPS(bps);

      configureAllPins();
   }

   /**
    * Destructor
    */
   virtual ~I2cBase_T() {}

   /**
    * Set channel Callback function\n
    * This callback is executed when the I2C state machine returns to the IDLE state
    * at the end of a transaction.
    *
    * @param[in] callback Callback function to execute on interrupt.\n
    *                     Use nullptr to remove callback.
    */
   static __attribute__((always_inline)) void setCallback(I2cCallbackFunction callback) {
      static_assert(Info::irqHandlerInstalled, "I2C not configured for interrupts");
      if (callback == nullptr) {
         callback = I2c::unhandledCallback;
      }
      sCallback = callback;
   }

   /**
    * Set baud factor value for interface
    *
    * This is calculated from processor frequency and given bits-per-second
    *
    * @param[in]  bps Interface speed in bits-per-second
    */
   void setBPS(uint32_t bps) {
      I2c::setBPS(bps, Info::getInputClockFrequency());
   }

   /**
    * Initialise interface
    *
    * @param[in]  myAddress Address of self (not used)
    */
   void init(const uint8_t myAddress) {

      // Enable clock to I2C interface
      Info::enableClock();

      thisPtr = this;

      if (i2cMode&I2C_C1_IICIE_MASK) {
         NVIC_EnableIRQ(Info::irqNums[0]);
      }

      // Set slave address
      i2c->C2  = I2C_C2_AD(myAddress>>8);

      // Clear status
      i2c->S  = I2C_S_ARBL_MASK|I2C_S_IICIF_MASK;

      // Enable I2C peripheral
      i2c->C1 = I2C_C1_IICEN_MASK|i2cMode;

      // Default options
      i2c->A1  = myAddress&~1;
      i2c->FLT = I2C_FLT_FLT(2);
   }

   /**
    * Clear bus hang
    *
    * Generates I2C_SCL clock until I2C_SDA goes high followed by I2C STOP. \n
    * This is useful if a slave is part-way through a transaction when the master goes away!
    */
   virtual void busHangReset() {

      // Disable I2C to clear some status flags
      i2c->C1 = i2c->C1 & ~I2C_C1_IICEN_MASK;

      static auto delay = [] {
         for(int j=0; j<20; j++) {
            __asm__("nop");
         }
      };
      // I2C SCL (clock) Pin
      using sclGpio = GpioTable_T<Info, Info::sclPin, USBDM::ActiveHigh>;

      // I2C SDA (data) Pin
      using sdaGpio = GpioTable_T<Info, Info::sdaPin, USBDM::ActiveHigh>;

      // Re-map pins to GPIOs initially 3-state
      sclGpio::setInput();
      sdaGpio::setInput();

      // SCL & SDA data values are low but direction is manipulated to achieve open-drain operation
      sclGpio::low();
      sdaGpio::low();

      for (int i=0; i<9; i++) {
         // Set clock 3-state
         sclGpio::setIn();    // SCL=T, SDA=?
         delay();
         bool sda = sdaGpio::isHigh();
         // Set clock low
         sclGpio::setOut();   // SCL=0, SDA=?
         delay();
         // If data is high bus is OK
         if (sda) {
            break;
         }
      }
      // Generate stop on I2C bus
      sdaGpio::setOut(); // SCL=0, SDA=0
      delay();
      sclGpio::setIn();  // SCL=T, SDA=0
      delay();
      sdaGpio::setIn();  // SCL=T, SDA=T
      delay();

      // Enable I2C
      i2c->C1 = i2c->C1 | ~I2C_C1_IICEN_MASK;

      // Restore pin mapping
      Info::initPCRs();
   }

   static void irqHandler() {
      thisPtr->poll();
      if (thisPtr->state == I2C_State::i2c_idle) {
         sCallback();
      }
   }
};

template<class Info> I2cCallbackFunction I2cBase_T<Info>::sCallback = I2c::unhandledCallback;

/** Used by ISR to obtain handle of object */
template<class Info> I2c *I2cBase_T<Info>::thisPtr = 0;

   /**
    * Class representing I2C0
    */
   using I2c0 = I2cBase_T<I2c0Info>;

/**
 * End I2C_Group
 * @}
 */

} // End namespace USBDM

#endif /* HEADER_I2C_H */
