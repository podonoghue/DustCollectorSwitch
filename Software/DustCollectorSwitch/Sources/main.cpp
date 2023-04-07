/**
 ============================================================================
 * @file    main.cpp (180.ARM_Peripherals/Sources/main.cpp)
 * @brief   Dust extractor control
 *
 *  Created on: 7/4/2023
 *      Author: podonoghue
 ============================================================================
 */
#include "hardware.h"
#include "mcg.h"

using namespace USBDM;

constexpr AdcResolution ADC_RESOLUTION = AdcResolution_10bit_se; // ADC Resolution - 10-bit unsigned (single-ended mode)
constexpr Seconds       TICK_TIME      = 10_ms;

/**
 * Get Delay Control value in seconds [1..10]
 */
unsigned getDelayControl() {
   constexpr unsigned MIN_DELAY = 1;
   constexpr unsigned MAX_DELAY = 10;

   return MIN_DELAY + (DelayControl::readAnalogue()*(MAX_DELAY-MIN_DELAY))/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
}

/**
 * Get Hold Control value in seconds [2..20]
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

/**
 * Initialise I/O
 */
void initialise() {

   static constexpr PcrInit PORT_INIT(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);

   static constexpr Adc0BasicInfo::Init adcInit {
      AdcClockSource_Bus ,       // ADC Clock Source - Bus clock
      AdcClockDivider_4 ,        // Clock Divide Select - Divide by 4
      ADC_RESOLUTION ,           // ADC Resolution
      AdcMuxsel_B,
   };

   DustCollector::setOutput(PORT_INIT);
   HoldLed::setOutput(PORT_INIT);
   DelayLed::setOutput(PORT_INIT);

   UserAdc::configure(adcInit);

   CurrentSample::setInput();
   HoldControl::setInput();
   DelayControl::setInput();
}

/**
 * Simple state machine
 */
enum State {s_IDLE, s_DELAY, s_OPERATING, s_HOLD, };

static const char *getStateName(State state) {

   static const char *names[] = {
         "Idle", "Delay", "Operating", "Hold",
   };
   if (state > sizeofArray(names)) {
      return "Illegal";
   }
   return names[state];
}

int main() {
#ifdef DEBUG_BUILD
   console.writeln("\nStarting");
#endif

   initialise();

   // Update parameters on startup
   const unsigned delayTimeInTicks = getDelayControl() / TICK_TIME;
   const unsigned holdTimeInTicks  = getHoldControl()  / TICK_TIME;

#ifdef DEBUG_BUILD
   console.writeln("Delay time = ", delayTimeInTicks * TICK_TIME, " s");
   console.writeln("Hold  time = ", holdTimeInTicks * TICK_TIME, " s");
#endif

   State state     = s_IDLE;
   State lastState = s_OPERATING;

   unsigned tickCounter = 0;

   for(;;) {
#ifdef DEBUG_BUILD
      if (state != lastState) {
         console.writeln(getStateName(state));
         lastState = state;
      }
#endif
      switch(state) {
         default:
         case s_IDLE:
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
            if (!isLoadOn()) {
               // Load went away while delaying
               state = s_IDLE;
               DelayLed::off();
            }
            else if (++tickCounter>delayTimeInTicks) {
               // Completed delay time
               state = s_OPERATING;
               DelayLed::off();
            }
            break;
         case s_OPERATING:
            // Dust collector on
            DustCollector::on();

            if (!isLoadOn()) {
               // Load gone
               state = s_HOLD;
               tickCounter = 0;
            }
            break;
         case s_HOLD:
            HoldLed::on();
            if (isLoadOn()) {
               // Load back before delay expired
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
