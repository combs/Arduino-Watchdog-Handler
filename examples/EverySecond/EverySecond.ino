
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

  delay(100);

}


// This function will get called every time the watchdog handler fires


ISR(WDT_vect) {

//  Anything you use in here needs to be declared with "volatile" keyword

//  Track the passage of time.
  secondsSinceStartup++;

}
