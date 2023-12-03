/*
 * Sampler.h
 *
 *  Created on: 3 Dec 2023
 *      Author: peter
 */

#ifndef SOURCES_SAMPLER_H_
#define SOURCES_SAMPLER_H_

class Sampler {
public:
   Sampler();
   virtual ~Sampler();

   Sampler(const Sampler &other)             = delete;
   Sampler(Sampler &&other)                  = delete;
   Sampler& operator=(const Sampler &other)  = delete;
   Sampler& operator=(Sampler &&other)       = delete;

   /**
    * Initialise the sampler hardware
    */
   static void init();

   /**
    * Get Delay time [.1s .. 4s]
    *
    * @return Delay time in milliseconds
    */
   static unsigned getDelayTime();

   /**
    * Get Hold time [1s..10s]
    *
    * @return Hold time in milliseconds
    */
   static unsigned getHoldTime();

   /**
    * Indicates is the load is operating
    *
    * @return True if operating
    */
   static bool isLoadOn() {
      return loadIsOn;
   }

private:
    // Accumulated value over interval so far
    static unsigned accumulator;

    // Accumulated value over entire interval (~20 ms)
    static unsigned totalOfSamples;

    // Outcome of measuring load current
    static bool loadIsOn;

    // Samples accumulated this period
    static unsigned samplesThisPeriod;

    // DetectLevel used to determine if load is operating
    static unsigned detectLevel;

    static unsigned getDetectLevel();
    static void     timerCallback();
};

#endif /* SOURCES_SAMPLER_H_ */
