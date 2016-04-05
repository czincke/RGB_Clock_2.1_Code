// 4 digit 7 segment RGB LED (12 hour) Time Display
// Uses 3 TLC5947 PWM Drivers and four 7 segment RGB LED 
// CAZ 2016-04-02 1.1.0 Library Created

#include "CAZ_7x4_RGB.h"


// Helper function: Is segment s, on or off for displaying value v
// If not ten's hour digit: lookup in table 'Dig2seg'
// if ten's hour digit: value>0 (v==0 is off) and only 1st 2 segments 
#define SEGMENT_ON_OFF ((d<3) ? PRB(Dig2seg[v][s]) : v && (s<2))


// Instance: Select ports and clear 'segments'
// Remeber to do 'begin()' in 'setup()'
CAZ_7x4_RGB::CAZ_7x4_RGB(uint8_t clk, uint8_t dat, uint8_t lat)
{
	_clk = clk;
	_dat = dat;
	_lat = lat;

	for (uint8_t d = 0; d<4; d++)
		for (uint8_t s = 0; s<7; s++)
			segments[d][s] = 0;
}


// Set ports
void CAZ_7x4_RGB::begin() {
  pinMode(_clk, OUTPUT);
  pinMode(_dat, OUTPUT);
  pinMode(_lat, OUTPUT);
  digitalWrite(_lat, LOW);
}


// Set segment s of digit d to color index c
void CAZ_7x4_RGB::set_segment(uint8_t d, uint8_t s, uint8_t c)
{
  segments[d][s] = c;  // Update record of color index for this segment
}


// Set value v to digit d using color index c
void CAZ_7x4_RGB::set_digit(uint8_t d, uint8_t v, uint8_t c)
{
	for (uint8_t s = 0; s<7; s++)
    segments[d][s] = SEGMENT_ON_OFF ? c : 0;
}


// Update display of digit d with value v using random colors, 
// Slowly updating each segments in random order 
void CAZ_7x4_RGB::write_digit_slow(uint8_t d, uint8_t v)
{
	uint8_t s, f, i, c;

	for (i = 6; i<0; i--) // randomize the sequencial order of the update of elements in this display 
	{
		c = random(7);
		f = order[i];
		    order[i] = order[c];
		               order[c] = f;
	}

	for (i = 0; i<7; i++) // For all 7 segments
	{
		s = order[i];       // s = which segment to modify
		f = 0;              // f = flag (0=no change has occured, 1=value changed)
		c= segments[d][s];  // c = color index of digit 'd', segment 's'

		if (SEGMENT_ON_OFF)           // If this segment should be ON
		{
			if (c == 0)                 // and segment currently OFF
			{
				c = random(1, N_COLORS);  // Assign random color index (but not 0)
				f = 1;                    // Set flag (change has occured)
			}
		}
		else           // If this segment should be OFF
			if (c != 0)  // and segment currently ON
			{
				c = 0;     // Assign pallete color 0 (OFF)
				f = 1;     // Set flag (change has occured)
			}
		if (f == 1)            // If change has occured
		{
      segments[d][s] = c;  // Set digit d, segment s to color index c
			write();             // Update display
			delay(250);          // Delay for artistic effect
		}
	}
}


// Update entire display quickly 
void CAZ_7x4_RGB::write(void) {
  uint8_t dsc, dig, seg, rgb, col;
  int8_t chan, bit;
  uint16_t pwm;

  digitalWrite(_lat, LOW);

  for (chan = 24 * 3 - 1; chan >= 0; chan--)  // For all 24 *3 PWM channels
  {
    dsc = PRB(Chan2Seg[chan]);     // Find which digit, Segment, & rgb for this channel
    seg = dsc >> 4;                // extract segment
    dig = (dsc & 0x0C) >> 2;       // extract digit
    rgb = (dsc & 0x03);            // extract rgb
    col = segments[dig][seg];      // current color index for this segment of display
    pwm = PRB(Palette[col][rgb]);  // PWM value for this color

    for (bit = 11; bit >= 0; bit--)  // 12 bits per channel, send MSB first
    {
      digitalWrite(_clk, LOW);

      if (pwm & (1 << bit))
        digitalWrite(_dat, HIGH);
      else
        digitalWrite(_dat, LOW);

      digitalWrite(_clk, HIGH);
    }
  }
  digitalWrite(_clk, LOW);

  digitalWrite(_lat, HIGH);  // Latch to PWM Drivers
  digitalWrite(_lat, LOW);
}
