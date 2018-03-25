# Arduino-Watchdog-Handler
Quick Arduino library for setting up an AVR watchdog interrupt routine.

Based off [this work](http://robotshop.com/letsmakerobots/notes-putting-arduino-sleep)--thanks!

## What it is

The ATMega328p processor used in many Arduinos has a "watchdog timer" that can trigger periodically. It can be used to reset an unresponsive program, or to call a function of your choice periodically.

The <[avr/wdt.h](http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html)> header included with the Arduino IDE has some nice functions for setting up the watchdog to reset your Arduino, but it's a little more complicated to set up the watchdog timer (WDT) to call your own function. This library makes it easier.

Currently, this library supports the ATMega328p, ATMega168p, ATTiny85, ATTiny45, and ATTiny25.

## Why use it

The watchdog timer keeps running during many sleep modes on the Arduino. You can save a lot of battery power by using the watchdog timer and sleeping instead of delay() for periodic functions.

## How to use it

Install the library, then:

	#include "Watchdog.h"
	#include <avr/wdt.h>

In your setup routine, add this as the very first line:

	wdt_disable();

Then when you're ready for a periodic function to start running, call:

	setup_watchdog(WDTO_1S);

You can choose values from [15ms up to 8 seconds](http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html).

Then it will call a specially-named function at the interval you chose:

	ISR(WDT_vect) {

	// Put whatever you like in here

	}

There's lots more information in the [ATMega328p datasheet](http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf).
## Caveats

- Any global variables used in the ISR(WDT_vect) function (really an Interrupt Service Routine) need to be declared volatile:

		volatile int my_variable;


- The ISR(WDT_vect) routine may hang with some standard Arduino functions. In particular, anything involving delay() seems to kill it.

	You can always set a flag (my_variable=true) and take care of the delay-containing work in your loop function.

- If you reset the Arduino, under some conditions, the watchdog timer may remain active after reset and misbehave. This is why it's best to call wdt_disable() as the first line of your setup() routine.

- I've read that the watchdog timer is not terribly precise.

- You can't go shorter than 15ms or longer than 8 seconds, although you can always keep a variable and accumulate up to the interval you want.

	Example: if you want a 45-second watchdog timer, set it to `WDTO_1S` and increment a variable until it reaches 45... then do the actual work.

- You'll still need to take care of any sleep/wake stuff.
