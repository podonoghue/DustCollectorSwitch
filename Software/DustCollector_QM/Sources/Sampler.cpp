/*
 * Sampler.cpp
 *
 *  Created on: 3 Dec 2023
 *      Author: peter
 */

#include "Sampler.h"
#include "hardware.h"

using namespace USBDM;

// ADC Resolution - 10-bit unsigned (single-ended mode)
constexpr AdcResolution ADC_RESOLUTION = AdcResolution_12bit_se;

// How often the load current is sampled (in us)
constexpr unsigned SAMPLE_TIME = 100;

// Number of samples to average current over (1 mains cycle)
constexpr unsigned EXPECTED_PERIOD_IN_TICKS = round(20_ms/(SAMPLE_TIME/1000000.0));

Sampler::Sampler() {}

Sampler::~Sampler() {
   UserTimer::disable();
}

/**
 * Get detect level as raw ADC value
 */
unsigned Sampler::getDetectLevel() {
   return LevelControl::readAnalogue()/4;
}

/**
 * Initialise the sampler hardware
 */
void Sampler::init() {

   Pins::setOutput();
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
      Ticks((SAMPLE_TIME*(48000000/1000000))-1),       // Time interval for channel (microseconds*(fBus)/1000000) - 1; Assume 48MHz bus
      timerCallback,                                 // Call-back
      };
   UserTimer::configure(timerChannelInit);

   Debug::setOutput();
}


/**
 * Get Delay time [.1s .. 4s]
 *
 * @return Delay time in milliseconds
 */
unsigned Sampler::getDelayTime() {

   constexpr unsigned MIN_DELAY = 100;
   constexpr unsigned MAX_DELAY = 4000;

   unsigned value = DelayControl::readAnalogue();
   value *= (MAX_DELAY-MIN_DELAY);
   value /= UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
   value += MIN_DELAY;

   return value;
}

/**
 * Get Hold time [1s..10s]
 *
 * @return Hold time in milliseconds
 */
unsigned Sampler::getHoldTime() {

   constexpr unsigned MIN_HOLD = 1000;
   constexpr unsigned MAX_HOLD = 10000;

   unsigned value = HoldControl::readAnalogue();
   value *= (MAX_HOLD-MIN_HOLD);
   value /= UserAdc::getSingleEndedMaximum(ADC_RESOLUTION);
   value += MIN_HOLD;

   return value;
}

/**
 * Timer callback that:
 *  - Checks if the load is operating
 */
void Sampler::timerCallback() {

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

   static int      zeroCrossingLevel  = +HYSTERESIS;
   static unsigned sampleCount        = 0;

   sampleCount++;

   static unsigned threshAccumulator;

   threshAccumulator += getDetectLevel();

   if (((current>zeroCrossingLevel)&&(zeroCrossingLevel>0))||(sampleCount>(EXPECTED_PERIOD_IN_TICKS+2))) {

      // New period
      zeroCrossingLevel = -HYSTERESIS;
      unsigned totalOfSamples    = accumulator/sampleCount;
      accumulator       = 0;

      samplesThisPeriod = sampleCount;
      sampleCount       = 0;

      // Calculate detectLevel used to determine if load is operating
      // This is a mean-squared value
      unsigned thresh = threshAccumulator/samplesThisPeriod;
      thresh *= thresh;
      threshAccumulator = 0;

      // Add hysteresis ~DETECT_HYSTERESIS %
      if (loadIsOn&&(detectLevel>0)) {
         thresh = (thresh*(200-DETECT_HYSTERESIS))/200;
      }
      else {
         thresh = (thresh*(200+DETECT_HYSTERESIS))/200;
      }
      detectLevel = thresh/2;

      // Check if load present
      loadIsOn = totalOfSamples > detectLevel;
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
}

unsigned Sampler::samplesThisPeriod = 0;
unsigned Sampler::detectLevel       = 0;
bool     Sampler::loadIsOn          = false;
unsigned Sampler::accumulator       = 0;
