/**
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Dust extractor control
 *
 *  Created on: 10/1/2016
 *      Author: podonoghue
 ============================================================================
 */
#include "hardware.h"
#include "mcg.h"

using namespace USBDM;

constexpr AdcResolution ADC_RESOLUTION = AdcResolution_10bit_se; // ADC Resolution - 10-bit unsigned (single-ended mode)
constexpr Seconds       TICK_TIME      = 10_ms;

/**
 * Get Delay Control value in seconds
 */
unsigned getDelayControl() {
   constexpr unsigned MIN_DELAY = 1;
   constexpr unsigned MAX_DELAY = 10;

   return MIN_DELAY + (DelayControl::readAnalogue()*(MAX_DELAY-MIN_DELAY))/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
}

/**
 * Get Hold Control value in seconds
 */
unsigned getHoldControl() {
   constexpr unsigned MIN_HOLD  = 2;
   constexpr unsigned MAX_HOLD = 20;

   return MIN_HOLD + (HoldControl::readAnalogue()*(MAX_HOLD-MIN_HOLD))/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
}

/**
 * Check if the load is operating
 *
 * @return true => Load is operating
 */
bool isLoadOn() {

   // Threshold used to determine if load is operating
   constexpr int THRESHOLD =  UserAdc::getSingleEndedMaximum(ADC_RESOLUTION)/2;

   int current = CurrentSample::readAnalogue();

   return current > THRESHOLD;
}

void initialise() {

   static constexpr PcrInit PORT_INIT(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);

   static constexpr Adc0BasicInfo::Init adcInit {
      AdcClockSource_Bus ,       // ADC Clock Source - Bus clock
      AdcClockDivider_4 ,        // Clock Divide Select - Divide by 4
      ADC_RESOLUTION ,           // ADC Resolution
   };

   DustCollector::setOutput(PORT_INIT);
   HoldLed::setOutput(PORT_INIT);
   DelayLed::setOutput(PORT_INIT);

   UserAdc::configure(adcInit);

   CurrentSample::setInput();
   HoldControl::setInput();
   DelayControl::setInput();
}

enum State {s_IDLE, s_DELAY, s_OPERATING, s_HOLD, };

int main() {
   console.writeln("\nStarting");

   initialise();

   State state = s_IDLE;
   unsigned tickCounter = 0;
   unsigned delayTimeInTicks = 100;
   unsigned holdTimeInTicks  = 100;

   for(;;) {
      switch(state) {
         default:
         case s_IDLE:
            // Update parameters while idle
            delayTimeInTicks = getDelayControl() / TICK_TIME;
            holdTimeInTicks  = getHoldControl()  / TICK_TIME;

            // Make sure dust collector is off
            DustCollector::off();

            // Check for load
            if (isLoadOn()) {
               state = s_DELAY;
               tickCounter = 0;
            }
            break;
         case s_DELAY:
            DelayLed::on();
            // Check if load went away while delaying
            if (!isLoadOn()) {
               state = s_IDLE;
               DelayLed::off();
            }
            else if (++tickCounter>delayTimeInTicks) {
               // Completed delay
               state = s_OPERATING;
               DelayLed::off();
            }
            break;
         case s_OPERATING:
            // Dust collector on
            DustCollector::on();

            // Check if load gone
            if (!isLoadOn()) {
               state = s_HOLD;
               tickCounter = 0;
            }
            break;
         case s_HOLD:
            HoldLed::on();
            if (isLoadOn()) {
               // Check for load back before delay expired
               state = s_OPERATING;
               HoldLed::off();
            }
            else if (++tickCounter>holdTimeInTicks) {
               // Completed hold time
               state = s_IDLE;
               HoldLed::off();
            }
            break;
      }
      wait(TICK_TIME);
   }
   return 0;
}
