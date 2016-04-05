// Set_Clock_and_Read_Back.ino
// Set date and time functions on DS1307 RTC connected via I2C and Wire lib
// via trinket and display onto 7segment REG leds
// 2014-07-20 CAZ Initial
// 2015-03-13 CAZ Updated DST Calculations
// 2015-04-02 CAZ Clean up code
// 2016-04-04 CAZ Using Visual Micro

// -------Order of display-------
//        Month  /  2 digit Year
//  Day of week  /  Day of month
// (1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat)
//      EST/DST  /  24-hour (DST time)
// 12-hour (E/D) /  Minutes
// Single Minute / Seconds  (... until next minute)

#include <Wire.h>
#include "CAZ_RTClib.h"
#include "CAZ_7x4_RGB.h"

// Values to set clock
//#define SET_CLOCK      // Define to set clock. Comment out to just read back
#define T_OFFSET  6    // Add these many seconds to compilation time
#define DAYOFWEEK 3    // 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat
#define WAIT_TIME 5000 // 5 seconds

// TLC5947 PWM Pins
#define DAT 1
#define CLK 4
#define LAT 3

CAZ_7x4_RGB display = CAZ_7x4_RGB(CLK, DAT, LAT);
RTC_Time rtc;


void setup () {
  Wire.begin();
  display.begin();
  display.write();
  #ifdef SET_CLOCK
//    rtc.set_time("Dec 26 2009", "12:34:01",DAYOFWEEK,T_OFFSET);
    rtc.set_time(__DATE__, __TIME__, DAYOFWEEK, T_OFFSET);
  #endif
}


void loop ()
{
  rtc.read_clock();

  //        Month  /  2 digit Year
  Set_Mins(1, rtc.bcd_year);
  Set_Hour(2, rtc.bcd_month);
  display.write();
  delay(WAIT_TIME);

  //  Day of week  /  Day of month
  Set_Mins(3, rtc.bcd_day);
  Set_Hour(4, rtc.weekday);// 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat
  display.write();
  delay(WAIT_TIME);

  //      EST/DST  /  24-hour (DST time)
  Set_Mins(5, rtc.bcd_24hour);
  Set_Hour(6, rtc.EST_DST()? 0x0E : 0x0d );
  display.write();
  delay(WAIT_TIME);

  // 12-hour (E/D) /  Minutes
  Set_Mins(7, rtc.bcd_minute);
  Set_Hour(8, rtc.bcd_12hour);
  display.write();
  delay(WAIT_TIME);

  // Single Minute / Seconds  (... until next minute)
  while (!rtc.read_clock())
  {
    rtc.EST_DST();
    Set_Mins(9, rtc.bcd_second);
    Set_Hour(7, rtc.bcd_minute & 0x0F);
    display.write();
  }
}


void Set_Mins(uint8_t c, uint8_t x)
{
  display.set_digit(0, x & 0x0F, c);
  display.set_digit(1, x >> 4,   c);
}


void Set_Hour(uint8_t c, uint8_t x)
{
  display.set_digit(2, x & 0x0F, c);
  display.set_digit(3, x >> 4,   c);
}
