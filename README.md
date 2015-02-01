# Arduino-Watchdog-Handler
Quick Arduino library for setting up an AVR watchdog interrupt routine. 

Based off [this work](http://letsmakerobots.com/blog/project2501/notes-putting-arduino-sleep)--thanks!

## What it is

The ATMega328p processor used in many Arduinos has a "watchdog timer" that can trigger periodically. It can be used to reset an unresponsive program, or to call a function of your choice periodically.

The <[avr/wdt.h](http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html)> header included with the Arduino IDE has some nice functions for setting up the watchdog to reset your Arduino, but it's a little more complicated to set up the watchdog timer (WDT) to call your own function. This library makes it easier.

Currently, this library supports the ATMega328p, ATMega168p, ATTiny85, ATTiny45, and ATTiny25. 

## Why use it

The watchdog timer keeps running during many sleep modes on the Arduino. You can save a lot of battery power by using the watchdog timer and sleeping instead of delay() for periodic functions.

## How to use it

Install the library, then:

	#include "Watchdog.h"

In your setup routine, call:

	setup_watchdog(WDTO_1S);

You can choose values from [15ms up to 8 seconds](http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html).

Then it will call a specially-named function at the interval you chose:

	ISR(WDT_vect) {
	
	// Put whatever you like in here
	
	}

## Caveats

Any global variables used in the ISR(WDT_vect) function (really an Interrupt Service Routine) need to be declared volatile:

	volatile int my_variable;


The ISR(WDT_vect) routine may hang with some standard Arduino functions. In particular, anything involving delay() seems to kill it. 

You can always set a flag (my_variable=true) and take care of the delay-containing work in your loop function.


