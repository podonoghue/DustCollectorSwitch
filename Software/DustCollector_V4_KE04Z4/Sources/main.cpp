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
#include "ics.h"
#include "adc.h"

using namespace USBDM;

// ADC Resolution - 10-bit unsigned (single-ended mode)
constexpr AdcResolution ADC_RESOLUTION = AdcResolution_10bit_se;

// How often the load current is sampled (in ms)
constexpr unsigned       TICK_TIME      = 1;

// Number of samples to average current over
constexpr unsigned averagePeriodInTicks = round(20_ms/1_ms);

/**
 * Simple state machine
 */
enum State {s_IDLE, s_DELAY, s_OPERATING, s_HOLD};

// State machine state
static State state = s_IDLE;

// Parameters initialised on startup
static unsigned delayTimeInTicks;
static unsigned holdTimeInTicks;

#undef DEBUG_BUILD

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
 * Get detect threshold as raw ADC value
 */
Seconds getDetectThreshold() {
   return LevelControl::readAnalogue();
}


/**
 * Get Delay value in milliseconds [500ms..5s]
 */
unsigned getDelayControl() {

   return 1000;
}

/**
 * Get Hold value in milliseconds [1s..10s]
 */
unsigned getHoldControl() {
   constexpr unsigned MIN_HOLD = 1000;
   constexpr unsigned MAX_HOLD = 10000;

   return MIN_HOLD + (HoldControl::readAnalogue()*(MAX_HOLD-MIN_HOLD))/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
}

/**
 * Timer callback that:
 *  - Checks if the load is operating
 *  - Updates state
 */
void timerCallback() {

   Debug::toggle();

   // Current is averaged over ~20 ms

   // Sample within interval
   static unsigned sampleCount = 0;

   // Accumulated value over interval so far
   static unsigned accumulator = 0;

   // Accumulated value over entire interval (~20 ms)
   static unsigned totalOfSamples = 0;

   // Do averaging of absolute value of current over cycle
   int current = I_Sample::readAnalogue()-Reference::readAnalogue();
   if (current<0) {
      current = -current;
   }

   accumulator += current;
   if (++sampleCount > averagePeriodInTicks) {
      totalOfSamples = accumulator;
      accumulator    = 0;
      sampleCount    = 0;
   }

   // Threshold used to determine if load is operating
   const unsigned threshold = averagePeriodInTicks * getDetectThreshold();

   // Check if load present
   bool isLoadOn = totalOfSamples > threshold;

   DetectLed::write(isLoadOn);

   // Used to count time intervals
   static unsigned timeCounter = 0;

   switch(state) {
      default:
      case s_IDLE:
         // Make sure dust collector is off
         DustCollector::off();

         // Check for load
         if (isLoadOn) {
            state = s_DELAY;
            timeCounter = 0;
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
         else if (++timeCounter>delayTimeInTicks) {
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
            timeCounter = 0;
         }
         break;
      case s_HOLD:
         HoldLed::on();
         if (isLoadOn) {
            // Load back before delay expired
            state = s_OPERATING;
            HoldLed::off();
         }
         else if (++timeCounter>holdTimeInTicks) {
            // Completed hold time
            state = s_IDLE;
            HoldLed::off();
         }
         break;
   }
}

//using PollingTimerChannel = Pit::Channel<0>;

/**
 * Initialise I/O
 */
void initialise() {

   DustCollector::setOutput();
   DustCollector::setHdrive(HighDrive_on);
   HoldLed::setOutput();
   DelayLed::setOutput();
   DetectLed::setOutput();
   Debug::setOutput();

   static constexpr UserAdc::Init adcInit {
      AdcClockSource_BusClock,
      AdcClockDivider_DivBy2,
      ADC_RESOLUTION,
   };

   UserAdc::configure(adcInit);

   I_Sample::setInput();
   Reference::setInput();
   HoldControl::setInput();
   LevelControl::setInput();

   static constexpr UserTimer::ChannelInit timerInit {
      PollingTimerChannel::CHANNEL,

      PitChannelEnable_Enabled ,   // Timer Channel Enabled
      PitChannelIrq_Enabled ,      // Timer Interrupt Enabled
      NvicPriority_Normal ,        // IRQ level
      (TICK_TIME*24000000/1000)-1, // Time interval for channel ((milliseconds*fBus)/1000) - 1
      timerCallback,               // Call-back
      };
   UserTimer::configure(timerInit);

   Debug::setOutput();
}

int main() {

#ifdef DEBUG_BUILD
   console.writeln("\nStarting");
#endif

   initialise();

   // Update time parameters on startup
   delayTimeInTicks = getDelayControl() / TICK_TIME;
   holdTimeInTicks  = getHoldControl()  / TICK_TIME;

#ifdef DEBUG_BUILD
   State lastState = s_OPERATING;

   console.writeln("Detect threshold = ", (getDetectThreshold()*3.3)/UserAdc::getSingleEndedMaximum(ADC_RESOLUTION), " V");
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
