// Code for DS3231 RTC by CAZ
// Original Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!
// CAZ 2014-07-22 1.0.0 Code Modified
// CAZ 2016-03-31 1.1.0 Clean up Code

#include <Wire.h>
#include "CAZ_RTClib.h"

#define RTC_ADDRESS 0x68

// Helper functions
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }



boolean RTC_Time::read_clock()
{
  int8_t prev_minute = bcd_minute;

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(RTC_ADDRESS, 7);
  bcd_second = Wire.read();
  bcd_minute = Wire.read();
  bcd_24hour = Wire.read();
  weekday = Wire.read();
  bcd_day = Wire.read();
  bcd_month = Wire.read();
  bcd_year = Wire.read();

  return (prev_minute != bcd_minute);
//  return false;
}



//void RTC_Time::read_all() {
//  Wire.beginTransmission(RTC_ADDRESS);
//  Wire.write(0);	
//  Wire.endTransmission();
//
//  Wire.requestFrom(RTC_ADDRESS, 7);
//  bcd_second = Wire.read();
//  bcd_minute = Wire.read();
//  bcd_24hour = Wire.read();
//  weekday    = Wire.read();
//  bcd_day    = Wire.read();
//  bcd_month  = Wire.read();
//  bcd_year   = Wire.read();
//  Wire.endTransmission();
//}



boolean RTC_Time::EST_DST()
{
  int8_t offset = 11;                    // offset: 11 for DST, 10 for EST
  int8_t day    = bcd2bin(bcd_day);      // day = un-bcd date
  int8_t prevSun= day - weekday + 1;     // prevSun = date of previous sunday
  
  // EST in all of Jan, Feb, & Dec
  // if  Jan or Feb  or         Dec     EST
  if (bcd_month<0x03 || bcd_month>0x11) offset--;
  
  // Sping ahead on 2nd Sunday in March at 2am EST (3am DST)
  // if Mar and [ before 2nd Sun or (Before 3rd sun and day is sun and before 2am EST) ] 
  if (bcd_month==0x03 && (prevSun<8 || (prevSun<15 && prevSun==day && bcd_24hour<0x03) )) offset--;
  
  // Fall back on 1st Sunday in November at 2am DST
  // if Nov and after/on 1st sun and (after 2nd sun or after 1st sun or {on 1st sun} after 2am DST)
  if ((bcd_month == 0x11 && prevSun>0) && (prevSun>7 || day>prevSun || bcd_24hour >= 0x02)) offset--;

  // Conver DST 24 hr format (00 to 23) to EST/DST 12 hr format
  // DST: offset=11 for no change in time, EST: offset=10 to subtract 1 hour
  // ( 24hr+11 )%12 +1 results in 00->12, 01->01, ... 12->12, 13->01, ... 23->11
  bcd_12hour = bin2bcd( (bcd2bin(bcd_24hour)+offset)%12 +1);
  return (offset<11);
}



// A convenient constructor for using "the compiler's time":
// RTC_Time (__DATE__, __TIME__);
// dayofweek = ( 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat )
// sample input: date = "Dec 26 2009", time = "12:34:56", 
//                       0123456789A           01234567
void RTC_Time::set_time(const char* date, const char* time, const uint8_t dayofweek, int8_t offset)
{
	bcd_year = 16 * (date[9] - '0') + (date[10] - '0');
/*  switch (date[2])  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	  {
		case 'b': bcd_month = 0x02; break; // Feb
		case 'y': bcd_month = 0x05; break; // May
		case 'l': bcd_month = 0x07; break; // Jul
		case 'g': bcd_month = 0x08; break; // Aug
		case 'p': bcd_month = 0x09; break; // Sep
		case 't': bcd_month = 0x10; break; // Oct
		case 'v': bcd_month = 0x11; break; // Nov
		case 'c': bcd_month = 0x12; break; // Dec
		case 'n': bcd_month = (date[1]=='u') ? 0x06 : 0x01; break; // Jun : Jan
		case 'r': bcd_month = (date[0]=='M') ? 0x03 : 0x04; break; // Mar : Apr
	  }
	*/
	bcd_month = (date[1] == 'a') ? ((date[2] == 'n') ? 0x01 : ((date[2] == 'r') ? 0x03 : 0x05)) // Jan, Mar, May
		: (date[1] == 'e') ? ((date[2] == 'b') ? 0x02 : ((date[2] == 'p') ? 0x09 : 0x12)) // Feb, Sep, Dec
		: (date[1] == 'u') ? ((date[2] == 'n') ? 0x06 : ((date[2] == 'l') ? 0x07 : 0x08)) // Jun, Jul, Aug
		: ((date[2] == 'r') ? 0x04 : ((date[2] == 't') ? 0x10 : 0x11));// Apr, Oct, Nov

	bcd_day = 16 * (date[4] - '0') + (date[5] - '0');
	bcd_24hour = 16 * (time[0] - '0') + (time[1] - '0');
	bcd_minute = 16 * (time[3] - '0') + (time[4] - '0');
	bcd_second = bin2bcd(10 * (time[6] - '0') + (time[7] - '0') + offset);
	weekday = dayofweek;

//	uint8_t x = bcd_hour;
//	bcd_24hour = (EST_DST()) ? bin2bcd(bcd2bin(x) + 1) : x; // Adjust time to DST, if currently EST
	if (EST_DST()) bcd_24hour = bin2bcd(bcd2bin(bcd_24hour) + 1);
	set_time();
}



void RTC_Time::set_time() {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0);
  Wire.write(bcd_second);
  Wire.write(bcd_minute);
  Wire.write(bcd_24hour);
  Wire.write(weekday);
  Wire.write(bcd_day);
  Wire.write(bcd_month);
  Wire.write(bcd_year);
  Wire.endTransmission();
}
