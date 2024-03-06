/**
 * @file delay.h (180.ARM_DeviceOptions/Project_Headers/delay.h)
 * @brief Simple busy-waiting delay routines using SYST counter
 *
 *  Created on: 5 Oct 2015
 *      Author: podonoghue
 */
#ifndef INCLUDE_USBDM_DELAY_H_
#define INCLUDE_USBDM_DELAY_H_
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stdint.h>
#include "derivative.h"
#include "pin_mapping.h"

#ifdef __cplusplus
namespace USBDM {

// Mask for maximum timer value
static constexpr uint32_t TIMER_MASK = ((1UL<<24)-1UL);
#else
// Mask for maximum timer value
#define TIMER_MASK ((1UL<<24)-1UL)
#include <stdbool.h>
#endif

#ifndef __CMSIS_RTOS

/**
 * Convert milliseconds to timer ticks
 *
 * @param[in]  ms Time in milliseconds
 *
 * @return Time value in timer ticks
 */
static __attribute__((always_inline)) inline int64_t convertMSToTicks(uint32_t ms) {
   return ((uint64_t)ms * SystemCoreClock) / 1000;
}

/**
 * Convert timer ticks to milliseconds
 *
 * @param[in]  ticks Time in ticks
 *
 * @return Time value in milliseconds
 */
static __attribute__((always_inline)) inline int64_t convertTicksToMS(uint32_t ticks) {
   return ((uint64_t)ticks * 1000 / SystemCoreClock);
}

/**
 * Enable timer
 *
 * Note this is only required if getTicks() is used by itself
 */
static __attribute__((always_inline)) inline void enableTimer() {
   // Reload with maximum value 2^24
   SysTick->LOAD = -1;

   // Enable without interrupts
   SysTick->CTRL =
         SysTick_CTRL_CLKSOURCE_Msk| // Processor clock
         SysTick_CTRL_ENABLE_Msk;    // Enabled
}

/**
 * Gets the number of ticks since the timer was started
 *
 * Note: the timer rolls over at TIMER_MASK ticks
 *
 * @return Number of ticks
 */
static __attribute__((always_inline)) inline uint32_t getTicks() {
   return SysTick->VAL;
}
#endif

/**
 * Simple delay routine
 *
 * @param[in] usToWait How many microseconds to busy-wait
 *
 * @note Uses busy-waiting
 */
void waitUS(uint32_t usToWait);

/**
 * Simple delay routine
 *
 * @param[in]  msToWait How many milliseconds to busy-wait
 *
 * @note Uses busy-waiting
 */
void waitMS(uint32_t msToWait);

/**
 * Simple delay routine
 *
 * @param[in]  seconds How many seconds to busy-wait
 *
 * @note Limited to 2^32 ms (71,582 minutes)
 * @note Uses busy-waiting
 */
void wait(Seconds seconds);

#ifdef __cplusplus
/**
 * Routine to wait for an event with timeout
 *
 * @param[in] usToWait How many microseconds to busy-wait
 * @param[in] testFn   Polling function indicating if waited for event has occurred
 *
 * @return Indicate if event occurred. true=>event, false=>no event
 *
 * Note: Accuracy is affected by execution time of function.
 */
bool waitUS(uint32_t usToWait, bool testFn(void));

/**
 * Routine to wait for an event with timeout
 *
 * @param[in] msToWait How many milliseconds to busy-wait
 * @param[in] testFn   Polling function indicating if waited for event has occurred
 *
 * @return Indicate if event occurred: true=>event, false=>no event
 *
 * Note: Accuracy is affected by execution time of function.
 */
bool waitMS(uint32_t msToWait, bool testFn(void));

/**
 * Routine to wait for an event with timeout
 *
 * @param[in] seconds  How many seconds to busy-wait
 * @param[in] testFn   Polling function indicating if waited for event has occurred
 *
 * @return Indicate if event occurred: true=>event, false=>no event
 *
 * Note: Accuracy is affected by execution time of function.
 */
bool wait(float seconds, bool testFn(void));

} // End namespace USBDM
#endif /* __cplusplus */

#endif /* INCLUDE_USBDM_DELAY_H_ */