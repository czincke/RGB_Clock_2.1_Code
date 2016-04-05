// LED_Test.ino
// Test the RGB LEDs connected to the clock
// 2016-04-02 CAZ Initial
// 2016-04-04 CAZ Using Visual Micro

#include "CAZ_7x4_RGB.h"

// TLC5974 LED PWM controller Pins
#define DAT  1
#define CLK  4
#define LAT  3
CAZ_7x4_RGB display = CAZ_7x4_RGB(CLK, DAT, LAT);

// White, Red, Green, Blue, Black
uint8_t rgbw[] = {9,1,4,6,0};

uint8_t d, s, i, c;

void setup () {
  display.begin(); 
}


void loop ()
{
  // Display '1888' in White, Red, Green, Blue, & Black
  for(i=0; i<5; i++)
  {
    c = rgbw[i];
    display.set_digit(0,8,c);
    display.set_digit(1,8,c);
    display.set_digit(2,8,c);
    display.set_digit(3,1,c);
    display.write();
    delay(2000);
  }

  // Display 0..F in White
  for (d=0; d<16; d++)
  {
    c = 9;
    display.set_digit(0,d,c);
    display.set_digit(1,d,c);
    display.set_digit(2,d,c);
    display.set_digit(3,d,c);
    display.write();
    delay(1000);    
  }

  // Clear Display
  c = 0;
  display.set_digit(0,8,c);
  display.set_digit(1,8,c);
  display.set_digit(2,8,c);
  display.set_digit(3,1,c);
  display.write();
  delay(1000);

  // Start with lowest digit, Scan from segment A to G, Do R,G,B,W on each single Segment
  for (d=0; d<4; d++)
  {
     for (s=0; s<7; s++)
     {
        for (i=0;i<5;i++)
        {
          c = rgbw[i];
          display.set_segment(d,s,c);
          display.write();
          delay(500);
        }
     }
  }
}
