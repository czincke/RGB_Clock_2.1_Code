// Code for DS3231 RTC by CAZ
// Original Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!
// CAZ 2014-07-22 1.0.0 Code Modified
// CAZ 2016-03-31 1.1.0 Clean up Code

#ifndef _CAZ_RTCLIB_H_
#define _CAZ_RTCLIB_H_

// DS3231 RTC connected via I2C and the Wire library
class RTC_Time {
public:
	uint8_t bcd_second, bcd_minute, bcd_12hour, bcd_24hour, weekday, bcd_day, bcd_month, bcd_year;
	
	// Set time of RTC, offset in seconds
	// 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri, 7=Sat
	void    set_time (const char* date, const char* time, uint8_t dayofweek, int8_t offset);

	// Set time of RTC
	// set member values before running
	void    set_time ();

//	void    read_all();    // Read all but bcd_12hour

	// Read all except bcd_12hour
	// Return true if new minute
	boolean read_clock();

	// Set bcd_12hour with 12 hour DST/EST time
	// Returns true if EST
	boolean EST_DST();
};

#endif // _CAZ_RTCLIB_H_
