// Helper for setting up watchdog interrupt function.

// Define your interrupt routine in ISR(WDT_vect) { } 

// Any variables used in your interrupt routine should be declared 
// volatile to avoid sadness

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <WProgram.h>


void setup_watchdog(int timerPrescaler) {

  if (timerPrescaler > 9 ) timerPrescaler = 9; // Correct incoming amount if need be

  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); // Set the special 5th bit if necessary

#if defined __AVR_ATmega328P__ || defined __AVR_ATmega168p__

  // ATmega328p or ATmega168p -- Arduino Uno, etc.
  
  // This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); // Clear the watch dog reset
  WDTCSR |= (1<<WDCE) | (1<<WDE); // Set WD_change enable, set WD enable
  WDTCSR = bb; // Set new watchdog timeout value
  WDTCSR |= _BV(WDIE); 
  // Set the interrupt enable, this will keep unit from resetting after each int

#elif defined __AVR_ATtiny85__ || defined __AVR_ATtiny45__ || 
	defined __AVR_ATtiny25__
 
 // ATtiny85, 45, 25

  // This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); // Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // Set WD_change enable, set WD enable
  WDTCR = bb; // Set new watchdog timeout value
  WDTCR |= _BV(WDIE); 
  // Set the interrupt enable, this will keep unit from resetting after each int

#else 
  //??
#error "I don't know how to handle your AVR in setup_watchdog"
#endif


}

#endif


