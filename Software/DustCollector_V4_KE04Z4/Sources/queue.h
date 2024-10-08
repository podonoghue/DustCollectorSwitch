/**
 * @file uart_queue.h (180.ARM_Peripherals/Project_Headers/uart_queue.h)
 *
 *  Created on: 12Nov.,2016
 *      Author: podonoghue
 */

#ifndef INCLUDE_USBDM_QUEUE_H_
#define INCLUDE_USBDM_QUEUE_H_

#include "pin_mapping.h"

namespace USBDM {

/**
 * Possible events
 */
enum Event {e_NONE, e_SWITCH_PRESSED, e_LOAD_ON, e_LOAD_OFF, e_TICK};

/**
 * Simple queue implementation
 *
 * @tparam QUEUE_SIZE Size of queue
 */
template<int QUEUE_SIZE>
class Queue {
   volatile Event    fBuff[QUEUE_SIZE];
   volatile Event    *fHead, *fTail;
   volatile int      fNumberOfElements;

public:
   /**
    * Create empty Queue
    */
   constexpr Queue() : fHead(fBuff), fTail(fBuff), fNumberOfElements(0) {
   }

   /**
    * Clear queue i.e. make empty
    */
   void clear() {
      USBDM::CriticalSection cs;
      fHead             = fBuff;
      fTail             = fBuff;
      fNumberOfElements = 0;
   }
   /**
    * Check if empty
    *
    * @return true => empty
    */
   bool isEmpty() {
      return fNumberOfElements == 0;
   }
   /**
    * Check if full
    *
    * @return true => full
    */
   bool isFull() {
      return fNumberOfElements == QUEUE_SIZE;
   }
   /**
    * Add element to queue
    *
    * @param[in]  element Element to add
    */
   void enQueue(Event element) {
      bool success = enQueueDiscardOnFull(element);
      (void)success;
      usbdm_assert(success, "Queue full");
   }
   /**
    * Add element to queue. Discards on full.
    *
    * @param[in]  element Element to add
    *
    * @return true  => Element enqueued
    * @return false => Queue full, element not added
    */
   bool enQueueDiscardOnFull(Event element) {
      USBDM::CriticalSection cs;
      bool hasSpace = !isFull();
      if (hasSpace) {
         *fTail = element;
         fTail = fTail + 1;
         fNumberOfElements = fNumberOfElements + 1;
         if (fTail>=(fBuff+QUEUE_SIZE)) {
            fTail = fBuff;
         }
      }
      return hasSpace;
   }
   /**
    * Remove & return element from queue
    *
    * @return Event from queue or e_NONE if empty
    */
   Event deQueue() {
      USBDM::CriticalSection cs;
//      usbdm_assert(!isEmpty(), "Queue empty");
      if (isEmpty()) {
         return Event::e_NONE;
      }
      Event t = *fHead;
      fHead = fHead + 1;
      fNumberOfElements = fNumberOfElements -1;
      if (fHead>=(fBuff+QUEUE_SIZE)) {
         fHead = fBuff;
      }
      return t;
   }
};

} // End namespace USBDM

#endif /* INCLUDE_USBDM_QUEUE_H_ */
