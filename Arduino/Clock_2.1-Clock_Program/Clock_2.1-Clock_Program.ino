// Clock_Program.ino
// 7 segment rgb led clock CORE Code (v2.1)
// clock program with daylight savings included
// 2014-07-14 CAZ
// 2015-03-13 CAZ Updated DST Calculations
// 2016-03-16 CAZ Updated to use PROGMEM & Individual Output pins
// 2016-04-03 CAZ Updated to use CAZ_RTClib and CAZ_7x4_RGB libraries
// 2016-04-04 CAZ Using Visual Micro

#include <Wire.h>
#include "CAZ_RTClib.h"
#include "CAZ_7x4_RGB.h"


// TLC5974 LED PWM controller Pins
#define DAT  1
#define CLK  4
#define LAT  3

// 4 Digit 7 Segment RGB Display using 3 TLC5947 PWM Drivers
CAZ_7x4_RGB display = CAZ_7x4_RGB(CLK, DAT, LAT);

// DS3231 RTC (Real Time Clock)
// Uses I2C protocol
RTC_Time rtc;


// Initialze clock
void setup()
{
  Wire.begin();
  display.begin();
  display.write();
  randomSeed(millis());
}


void loop ()
{
  if (rtc.read_clock())   // read clock. If a change in the time (Hour:Min) has occured...
  {
    rtc.EST_DST();                                      // Adjust for Daylight Savings
    display.write_digit_slow(0,rtc.bcd_minute & 0x0F);  // Update one's digit of minutes
    display.write_digit_slow(1,rtc.bcd_minute / 0x10);  // Update ten's digit of minutes
	  display.write_digit_slow(2,rtc.bcd_12hour & 0x0F);  // Update one's digit of hours
	  display.write_digit_slow(3,rtc.bcd_12hour / 0x10);  // Update ten's digit of hours
  }
  
  // Wait one second while blinking Trinket LED
  delay(900);
  digitalWrite(1, HIGH);
  delay(100);
  digitalWrite(1, LOW);
}
