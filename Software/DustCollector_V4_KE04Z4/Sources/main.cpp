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
constexpr AdcResolution ADC_RESOLUTION = AdcResolution_12bit_se;

// How often the load current is sampled (in us)
constexpr unsigned       TICK_TIME      = 100;

// Number of samples to average current over
constexpr unsigned EXPECTED_PERIOD_IN_TICKS = round(20_ms/(TICK_TIME/1000000.0));

/**
 * State machine states
 */
enum State {s_IDLE, s_DELAY, s_OPERATING, s_HOLD};

// State machine state
static State state = s_IDLE;

// Parameters initialised on startup
static unsigned delayTimeInTicks;
static unsigned holdTimeInTicks;

//#undef DEBUG_BUILD

#if defined(DEBUG_BUILD)
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
 * Get detect level as raw ADC value
 */
unsigned getDetectLevel() {
   return LevelControl::readAnalogue()/4;
}

/**
 * Get Delay value in milliseconds [100ms..4s]
 */
unsigned getDelayControl() {
   constexpr unsigned MIN_DELAY = 100;
   constexpr unsigned MAX_DELAY = 4000;

   unsigned value = DelayControl::readAnalogue();
   value *= (MAX_DELAY-MIN_DELAY);
   value /= UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
   value += MIN_DELAY;

   return value;
}

/**
 * Get Hold value in milliseconds [1s..10s]
 */
unsigned getHoldControl() {
   constexpr unsigned MIN_HOLD = 1000;
   constexpr unsigned MAX_HOLD = 10000;

   unsigned value = HoldControl::readAnalogue();
   value *= (MAX_HOLD-MIN_HOLD);
   value /= UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
   value += MIN_HOLD;

   return value;
}

// Accumulated value over interval so far
static unsigned accumulator = 0;

// Accumulated value over entire interval (~20 ms)
static unsigned totalOfSamples = 0;

static bool isLoadOn = false;

static unsigned samplesThisPeriod = 100;

// detectLevel used to determine if load is operating
static unsigned detectLevel = 10000;

/**
 * Timer callback that:
 *  - Checks if the load is operating
 *  - Updates state
 */
void timerCallback() {
//   Debug::set();
   Debug::toggle();

   // Hysteresis used for zero crossing
   static constexpr int HYSTERESIS        = 100;

   // Hysteresis used for load detection (percentage)
   static constexpr int DETECT_HYSTERESIS = 1;

   // Instantaneous current sample
   int sample    = I_Sample::readAnalogue();

   // Instantaneous reference sample
   int reference = Reference::readAnalogue();

   // Instantaneous current sample
   int current   = (sample-reference);

   // Used to check ADC ranges
//   static int max = 0;
//   static int min = 10000;
//
//   if (sample>max) {
//      max = sample;
//   }
//   if (sample<min) {
//      min = sample;
//   }

   static int      zeroCrossingLevel  = +HYSTERESIS;
   static unsigned sampleCount        = 0;

   sampleCount++;

   static unsigned threshAccumulator;

   threshAccumulator += getDetectLevel();

   if (((current>zeroCrossingLevel)&&(zeroCrossingLevel>0))||(sampleCount>(EXPECTED_PERIOD_IN_TICKS+2))) {
      // Update controls
      delayTimeInTicks = getDelayControl() * (1000/TICK_TIME);
      holdTimeInTicks  = getHoldControl()  * (1000/TICK_TIME);

      // New period
      zeroCrossingLevel = -HYSTERESIS;
      totalOfSamples    = accumulator/sampleCount;
      accumulator       = 0;

      samplesThisPeriod = sampleCount;
      sampleCount       = 0;

      // Calculate detectLevel used to determine if load is operating
      // This is a mean-squared value
      unsigned thresh = threshAccumulator/samplesThisPeriod;
      thresh *= thresh;
      threshAccumulator = 0;

      // Add hysteresis ~DETECT_HYSTERESIS %
      if (isLoadOn&&(detectLevel>0)) {
         thresh = (thresh*(200-DETECT_HYSTERESIS))/200;
      }
      else {
         thresh = (thresh*(200+DETECT_HYSTERESIS))/200;
      }
      detectLevel = thresh/2;

      // Check if load present
      isLoadOn = totalOfSamples > detectLevel;
   }
   else if (current<zeroCrossingLevel) {
      // Do hysteresis
      zeroCrossingLevel = +HYSTERESIS;
   }
   if (current<0) {
      current = -current;
   }

   // Mean-squared
   accumulator += current*current;

//   DetectLed::write(isLoadOn);
//   Debug::write(zeroCrossingLevel>0);

   // Used to count time intervals in 'ticks'
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
         // Indicate in delay
         DelayLed::on();

         // Make sure Collector off
         DustCollector::off();

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
         // Indicate in hold
         HoldLed::on();

         // Dust collector on
         DustCollector::on();

         if (isLoadOn) {
            // Load returned before delay expired
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
//   Debug::clear();
}

//using PollingTimerChannel = Pit::Channel<0>;

/**
 * Initialise I/O
 */
void initialise() {

   Pins::setOutput();
   DustCollector::setHdrive(HighDrive_on);

//   HoldLed::setOutput();
//   DelayLed::setOutput();
//   DetectLed::setOutput();
//   Debug::setOutput();
//   DustCollector::setOutput();
   DustCollector::setHdrive(HighDrive_on);

   static constexpr UserAdc::Init adcInit {
      AdcResolution_12bit_se ,         // ADC resolution - 12-bit
      AdcClockSource_BusClock ,        // Input Clock Select - Bus clock
      AdcClockDivider_DivBy8 ,         // Clock Divide Select - Divide by 8
      AdcRefSel_VrefhAndVrefl ,        // Voltage Reference Selection - VREFH and VREFL
      AdcSample_Short ,                // Sample Time Configuration - Long sample
      AdcPower_Normal ,                // Low-Power Configuration - Normal

      // Configured ADC channels (GPIO disabled)
      I_Sample::CHANNEL,
      Reference::CHANNEL,
      HoldControl::CHANNEL,
      LevelControl::CHANNEL,
      };

   UserAdc::configure(adcInit);

   static constexpr UserTimer::Init timerInit {
      PitOperation_Enabled,
      PitDebugMode_StopInDebug,
   };
   UserTimer::configure(timerInit);

   static constexpr UserTimer::ChannelInit timerChannelInit {
      PollingTimerChannel::CHANNEL,

      PitChannelEnable_Enabled ,                     // Timer Channel Enabled
      PitChannelIrq_Enabled ,                        // Timer Interrupt Enabled
      NvicPriority_Normal ,                          // IRQ level
      Ticks((TICK_TIME*(SystemBusClock/1000000))-1), // Time interval for channel (microseconds*(fBus)/1000000) - 1
      timerCallback,                                 // Call-back
      };
   UserTimer::configure(timerChannelInit);

   Debug::setOutput();
}

int main() {

#if defined(DEBUG_BUILD)
   console.writeln("\nStarting");
#endif

   initialise();

//   for(;;) {
//      waitMS(100);
//      DelayLed::toggle();
//      HoldLed::toggle();
//      DustCollector::toggle();
//   }

   // Update time parameters on startup
   delayTimeInTicks = getDelayControl() * (1000/TICK_TIME);
   holdTimeInTicks  = getHoldControl()  * (1000/TICK_TIME);
   detectLevel      = getDetectLevel();

#if defined(DEBUG_BUILD)
   console.writeln("Delay time   = ", (delayTimeInTicks * TICK_TIME)/1000, " ms");
   console.writeln("Hold  time   = ", (holdTimeInTicks * TICK_TIME)/1000, " ms");
   console.writeln("Detect level = ", (detectLevel * 4000)/4096, " mV");
#endif

   for(int count=0;;count++) {
#if defined(DEBUG_BUILD)
      static State lastState = s_OPERATING;
      if (state != lastState) {
         // Report state change
         console.writeln(getStateName(state));
         lastState = state;
      }
      if ((count&0xFF)==0) {
         console.writeln(
               "samplesThisPeriod = ", samplesThisPeriod,
               ", detectLevel = ", detectLevel/1000,
               ", totalOfSamples = ", totalOfSamples/1000,
               ", isLoadOn = ", isLoadOn);
      }
#endif
      __asm__("nop");
   }
   return 0;
}
