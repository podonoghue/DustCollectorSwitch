/*
 * @file rtc.cpp (180.ARM_Peripherals/Startup_Code/rtc.cpp)
 *
 *  Created on: 21/09/2013
 *      Author: podonoghue
 */

#include <sys/time.h>
#include "derivative.h"
#include "rtc.h"
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
/*
 * To support time.h functions
 *
 * @param tp
 * @param tzp
 */
extern "C" __attribute__ ((__weak__))
int _gettimeofday(struct timeval *tp, void *) {
   // Start RTC if not already running
   USBDM::Rtc::initialise();
   tp->tv_sec  = USBDM::Rtc::getTime();
   tp->tv_usec = 0;
   return 0;
}

/*
 * To support time.h functions
 *
 * @param tp
 * @param tzp
 */
extern "C"
int settimeofday(const struct timeval *tp, const struct timezone *) {
   // Start RTC if not already running
   USBDM::Rtc::initialise();
   USBDM::Rtc::setTime(tp->tv_sec);
   return 0;
}
