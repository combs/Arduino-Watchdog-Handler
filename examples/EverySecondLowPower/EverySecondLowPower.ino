
#include "watchdogHandler.h"
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>


volatile unsigned long secondsSinceStartup = 0;
volatile unsigned long secondsLastUpdate = 0;
const unsigned long secondsUpdateFrequency = 60 * 30;


void setup(void) {

//  Disable the watchdog timer first thing, in case it is misconfigured

  wdt_disable();

  Serial.begin(115200);
  Serial.println("Hello!");

//  Just cool our jets for a sec here

  delay(2000);

//  Run ISR(WDT_vect) every second:

  setup_watchdog(WDTO_1S);

// Valid delays:
// 	WDTO_15MS
// 	WDTO_30MS
// 	WDTO_60MS
// 	WDTO_120MS
// 	WDTO_250MS
// 	WDTO_500MS
// 	WDTO_1S
// 	WDTO_2S
// 	WDTO_4S
// 	WDTO_8S

}


void loop(void) {

  //  Has it been enough time to update?

  if ( secondsSinceStartup - secondsLastUpdate > secondsUpdateFrequency) {

    Serial.print("It's been this many seconds: ");
    Serial.println(secondsSinceStartup);
    Serial.print("and millis() says: ");
    Serial.println(millis());

  }

  //  Go to sleep forever... or until the watchdog timer fires!

  sleeping();

}


// This function will get called every time the watchdog handler fires


ISR(WDT_vect) {

//  Anything you use in here needs to be declared with "volatile" keyword

//  Track the passage of time.
  secondsSinceStartup++;

}


void sleeping() {

//  The precise sequence of these instructions is important for the ATMEGA238

//  SLEEP_MODE_PWR_DOWN: Disable every clock domain and timer except for the watchdog timer.
//  Only way to wake up the device is the WDT and the external interrupt pins.
//  The millis() timers will be disabled during sleep, which is why we track seconds in the
//  watchdog timer routine.

//  You could shut down your external sensors & hardware here

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();
  sleep_cpu();
  sleep_disable();
  waking();

}

void waking() {

//  You could wake up your external sensors & hardware here

}
