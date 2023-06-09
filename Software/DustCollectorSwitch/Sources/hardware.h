/**
 * @file      hardware.h (180.ARM_Peripherals/Sources/hardware.h)
 *
 * Main header file for USBDM library.
 * Generated code is included via this file.
 *
 * @version  V4.12.1.270
 * @date     1 December 2021
 */

/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */

#ifndef INCLUDE_USBDM_HARDWARE_H_
#define INCLUDE_USBDM_HARDWARE_H_

#include "error.h"
#include "pin_mapping.h"
#include "delay.h"
#include "console.h"

#include "gpio.h"
#include "pit.h"
#include "i2c.h"
#include "adc.h"
#include "spi.h"
#include "llwu.h"
#include "ftm.h"


namespace USBDM {

/**
 * Map all configured pins to peripheral signals.
 *
 * PCRs of allocated pins are set according to settings in Configure.usbdmProject
 *
 * @note Only the lower 16-bits of the PCR registers are initialised
 */
extern void mapAllPins();

/// ADC, Analogue Input
typedef Adc0                                                 UserAdc;                                      

/// A2
typedef Adc0::Channel<7>                                     CurrentSample;                                // PTD6(A2)

/// A0
typedef Adc0::Channel<14>                                    DelayControl;                                 // PTC0(A0)

/// A1
typedef Adc0::Channel<15>                                    HoldControl;                                  // PTC1(A1)

typedef Ftm0::Channel<0>                                     TimerChannel;                                 // PTC1(A1)

/// D5
typedef GpioTable_T<GpioAInfo, 1, ActiveHigh>                DelayLed;                                     // PTA1(D5)

/// D4
typedef GpioTable_T<GpioCInfo, 8, ActiveHigh>                HoldLed;                                      // PTC8(D4)

/// D13
typedef GpioTable_T<GpioDInfo, 1, ActiveHigh>                DustCollector;                                // PTD1(D13)

/// D0
typedef GpioTable_T<GpioEInfo, 1, ActiveHigh>                Digital_D0;                                   // PTE1(D0)

/// User I2C
typedef I2c0                                                 UserI2c;                                      

typedef Llwu::Pin<LlwuPin_Pte1>                              Wakeup_D0;                                    // PTE1(D0)

/// PIT, Programmable Interrupt Timer
typedef Pit                                                  UserTimer;                                    

/// User SPI
typedef Spi0                                                 UserSpi;                                      

} // End namespace USBDM

#endif /* INCLUDE_USBDM_HARDWARE_H_ */
