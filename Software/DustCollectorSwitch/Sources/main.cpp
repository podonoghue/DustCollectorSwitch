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

// ADC Resolution - 10-bit unsigned (single-ended mode)
constexpr AdcResolution ADC_RESOLUTION = AdcResolution_10bit_se;

// How often the load current is sampled
constexpr Seconds       TICK_TIME      = 1_ms;

// Number of samples to average current over
constexpr unsigned averagePeriodInTicks = round(20_ms/1_ms);

/**
 * Simple state machine
 */
enum State {s_IDLE, s_DELAY, s_OPERATING, s_HOLD, };

// State machine state
static State state     = s_IDLE;

// Parameters initialised on startup
static unsigned delayTimeInTicks;
static unsigned holdTimeInTicks;

#ifdef DEBUG_BUILD
static const char *getStateName(State state) {

   static const char *names[] = {
         "Idle", "Delay", "Operating", "Hold",
   };
   if (state > sizeofArray(names)) {
      return "Illegal";
   }
   return names[state];
}
#endif

/**
 * Get Delay value in seconds [500ms..5s]
 */
Seconds getDelayControl() {
   constexpr Seconds MIN_DELAY = 500_ms;
   constexpr Seconds MAX_DELAY = 5_s;

   return MIN_DELAY + (DelayControl::readAnalogue()*(MAX_DELAY-MIN_DELAY))/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
}

/**
 * Get Hold value in seconds [1s..10s]
 */
Seconds getHoldControl() {
   constexpr Seconds MIN_HOLD = 1_s;
   constexpr Seconds MAX_HOLD = 10_s;

   return MIN_HOLD + (HoldControl::readAnalogue()*(MAX_HOLD-MIN_HOLD))/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
}

/**
 * Timer callback that:
 *  - Checks if the load is operating
 *  - Updates state
 *
 * @return true => Load is operating
 */
void timerOverflowCallback() {

   Digital_D0::toggle();

   static unsigned sampleCount   = 0;
   static unsigned accumulator = 0;
   static unsigned average     = 0;

   // Threshold used to determine if load is operating
   constexpr unsigned THRESHOLD =  averagePeriodInTicks * UserAdc::getSingleEndedMaximum(ADC_RESOLUTION)/5;

   // Do averaging of current over cycle
   int current = CurrentSample::readAnalogue();

   accumulator += current;
   if (++sampleCount > averagePeriodInTicks) {
      average     = accumulator;
      accumulator = 0;
      sampleCount = 0;
   }
   bool isLoadOn = average > THRESHOLD;

   // Used to count intervals
   static unsigned tickCounter = 0;

   switch(state) {
      default:
      case s_IDLE:
         // Make sure dust collector is off
         DustCollector::off();

         // Check for load
         if (isLoadOn) {
            state = s_DELAY;
            tickCounter = 0;
         }
         break;
      case s_DELAY:
         // Make sure dust collector is off
         DelayLed::on();

         if (!isLoadOn) {
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

         if (!isLoadOn) {
            // Load gone
            state = s_HOLD;
            tickCounter = 0;
         }
         break;
      case s_HOLD:
         HoldLed::on();
         if (isLoadOn) {
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
}

/**
 * Initialise I/O
 */
void initialise() {

   static constexpr PcrInit PORT_INIT(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);

   DustCollector::setOutput(PORT_INIT);
   HoldLed::setOutput(PORT_INIT);
   DelayLed::setOutput(PORT_INIT);

   static constexpr Adc0BasicInfo::Init adcInit {
      AdcClockSource_Bus ,       // ADC Clock Source - Bus clock
      AdcClockDivider_4 ,        // Clock Divide Select - Divide by 4
      ADC_RESOLUTION ,           // ADC Resolution
      AdcMuxsel_B,
   };

   UserAdc::configure(adcInit);

   CurrentSample::setInput();
   HoldControl::setInput();
   DelayControl::setInput();

   static constexpr UserTimer::Init timerInit {
      FtmMode_LeftAligned ,         // Left-aligned (count up and restart on interval)
      FtmOverflowAction_Interrupt , // Action on Counter overflow - Interrupt
      NvicPriority_Normal ,         // IRQ level
      FtmClockSource_SystemClock ,  // Clock Source - System clock
      TICK_TIME,                    // Overflow interval
   };
   UserTimer::configure(timerInit);
   UserTimer::setCallback(timerOverflowCallback);

   Digital_D0::setOutput();
}

int main() {
#ifdef DEBUG_BUILD
   console.writeln("\nStarting");
#endif

   initialise();

   // Update parameters on startup
   delayTimeInTicks = round(getDelayControl() / TICK_TIME);
   holdTimeInTicks  = round(getHoldControl()  / TICK_TIME);

#ifdef DEBUG_BUILD
   State lastState = s_OPERATING;

   console.writeln("Delay time       = ", delayTimeInTicks * TICK_TIME, " s");
   console.writeln("Hold  time       = ", holdTimeInTicks * TICK_TIME, " s");
   console.writeln("Average interval = ", averagePeriodInTicks * TICK_TIME, " s");
#endif

   for(;;) {
#ifdef DEBUG_BUILD
      if (state != lastState) {
         // Report state change
         console.writeln(getStateName(state));
         lastState = state;
      }
#endif
      __asm__("nop");
   }
   return 0;
}
