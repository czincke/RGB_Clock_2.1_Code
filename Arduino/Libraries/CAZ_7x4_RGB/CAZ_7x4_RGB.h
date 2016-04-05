// 4 digit 7 segment RGB LED (12 hour) Time Display
// Uses 3 TLC5947 PWM Drivers and four 7 segment RGB LED 
// CAZ 2016-04-02 1.1.0 Library Created

#ifndef _CAZ_7X4_RGB_H_
#define _CAZ_7X4_RGB_H_

#include <Arduino.h>
//#include<avr\pgmspace.h>

// Macro to read from PROGMEM arrays
#define PRB(x) pgm_read_byte(&(x))

// Palete of Colors to use
#define N_COLORS 9
uint8_t const PROGMEM Palette[N_COLORS+1][3] = {
	// r    g    b
	{   0,   0,   0 },   // 0: Black (off)
	{ 100,   0,   0 },   // 1: Red
	{ 100,  50,   0 },   // 2: Orange
	{ 100, 100,   0 },   // 3: Yellow
	{   0, 100,   0 },   // 4: Green
	{   0, 100, 100 },   // 5: Cyan
	{   0,   0, 200 },   // 6: Blue
	{  50,   0, 100 },   // 7: Violet
	{ 100,   0, 100 },   // 8: Magenta
  { 100, 100, 100 } }; // 9: White


// Which LED Segments are on for a number
uint8_t const PROGMEM Dig2seg[16][7] = {
	// a b c d e f g
	{ 1,1,1,1,1,1,0 },   // Digit 0
	{ 0,1,1,0,0,0,0 },   // Digit 1
	{ 1,1,0,1,1,0,1 },   // Digit 2
	{ 1,1,1,1,0,0,1 },   // Digit 3
	{ 0,1,1,0,0,1,1 },   // Digit 4
	{ 1,0,1,1,0,1,1 },   // Digit 5
	{ 1,0,1,1,1,1,1 },   // Digit 6
	{ 1,1,1,0,0,0,0 },   // Digit 7
	{ 1,1,1,1,1,1,1 },   // Digit 8
	{ 1,1,1,0,0,1,1 },   // Digit 9
  { 1,1,1,0,1,1,1 },   // Digit A
  { 0,0,1,1,1,1,1 },   // Digit b
  { 1,0,0,1,1,1,0 },   // Digit C
  { 0,1,1,1,1,0,1 },   // Digit d
  { 1,0,0,1,1,1,1 },   // Digit E
  { 1,0,0,0,1,1,1 } }; // Digit F


//// Each digits LED segments to PWM Channel mapping
//uint8_t const PROGMEM Seg2chan[4][7][3] = {
//	//	   r,  g,  b
//	{	{ 57, 58, 59 },  // a (19) Minutes Ones
//		{ 54, 55, 56 },  // b (18)
//		{ 48, 49, 50 },  // c (16)
//		{ 69, 70, 71 },  // d (23)
//		{ 66, 67, 68 },  // e (22)
//		{ 63, 64, 65 },  // f (21)
//		{ 51, 52, 53 } },// g (17) Minutes Ones
//	{	{ 30, 31, 32 },  // a (10) Minutes Tens
//		{ 60, 61, 62 },  // b (20)
//		{ 27, 28, 29 },  // c ( 9)
//		{ 24, 25, 26 },  // d ( 8)
//		{ 45, 46, 47 },  // e (15)
//		{ 42, 43, 44 },  // f (14)
//		{ 33, 34, 35 } },// g (11) Minutes Tens
//	{	{ 39, 40, 41 },  // a (13) Hours Ones
//		{ 36, 37, 38 },  // b (12)
//		{  6,  7,  8 },  // c ( 2)
//		{  3,  4,  5 },  // d ( 1)
//		{  0,  1,  2 },  // e ( 0)
//		{ 21, 22, 23 },  // f ( 7)
//		{  9, 10, 11 } },// g ( 3) Hours Ones
//	{	{ 12, 13, 14 },  // a ( 4) Hours Tens
//		{ 18, 19, 20 },  // b ( 6)
//		{ 15, 16, 17 },  // c ( 5)
//		{ 15, 16, 17 },  // d ( 5)
//		{ 15, 16, 17 },  // e ( 5)
//		{ 15, 16, 17 },  // f ( 5)
//		{ 15, 16, 17 }}};// g ( 5) Hours Tens

// Helper function: Encode digit, segment, & rgb values
// 0 (3 bits Segment), (2 bits digit), (2 bits rgb)
#define SEGMAP(d,s,c) ((s<<4)+(d<<2)+c)

// PWM Channel to digit/segment/color mapping
uint8_t const PROGMEM Chan2Seg[3 * 24] = {
  SEGMAP(2,4,0), SEGMAP(2,4,1), SEGMAP(2,4,2), // 00: H0 e
  SEGMAP(2,3,0), SEGMAP(2,3,1), SEGMAP(2,3,2), // 01: H0 d
  SEGMAP(2,2,0), SEGMAP(2,2,1), SEGMAP(2,2,2), // 02: H0 c
  SEGMAP(2,6,0), SEGMAP(2,6,1), SEGMAP(2,6,2), // 03: H0 g
  SEGMAP(3,0,0), SEGMAP(3,0,1), SEGMAP(3,0,2), // 04: H1 b
  SEGMAP(3,2,0), SEGMAP(3,2,1), SEGMAP(3,2,2), // 05: H1 X
  SEGMAP(3,1,0), SEGMAP(3,1,1), SEGMAP(3,1,2), // 06: H1 c
  SEGMAP(2,5,0), SEGMAP(2,5,1), SEGMAP(2,5,2), // 07: H0 f
  SEGMAP(1,3,0), SEGMAP(1,3,1), SEGMAP(1,3,2), // 08: M1 d
  SEGMAP(1,2,0), SEGMAP(1,2,1), SEGMAP(1,2,2), // 09: M1 c
  SEGMAP(1,0,0), SEGMAP(1,0,1), SEGMAP(1,0,2), // 10: M1 a
  SEGMAP(1,6,0), SEGMAP(1,6,1), SEGMAP(1,6,2), // 11: M1 g
  SEGMAP(2,1,0), SEGMAP(2,1,1), SEGMAP(2,1,2), // 12: H0 b
  SEGMAP(2,0,0), SEGMAP(2,0,1), SEGMAP(2,0,2), // 13: H0 a
  SEGMAP(1,5,0), SEGMAP(1,5,1), SEGMAP(1,5,2), // 14: M1 f
  SEGMAP(1,4,0), SEGMAP(1,4,1), SEGMAP(1,4,2), // 15: M1 e
  SEGMAP(0,2,0), SEGMAP(0,2,1), SEGMAP(0,2,2), // 16: M0 c
  SEGMAP(0,6,0), SEGMAP(0,6,1), SEGMAP(0,6,2), // 17: M0 g
  SEGMAP(0,1,0), SEGMAP(0,1,1), SEGMAP(0,1,2), // 18: M0 b
  SEGMAP(0,0,0), SEGMAP(0,0,1), SEGMAP(0,0,2), // 19: M0 a
  SEGMAP(1,1,0), SEGMAP(1,1,1), SEGMAP(1,1,2), // 20: M1 b
  SEGMAP(0,5,0), SEGMAP(0,5,1), SEGMAP(0,5,2), // 21: M0 f
  SEGMAP(0,4,0), SEGMAP(0,4,1), SEGMAP(0,4,2), // 22: M0 e
  SEGMAP(0,3,0), SEGMAP(0,3,1), SEGMAP(0,3,2), // 23: M0 d
};



// 4 Digit 7 Segment RGB Display using 3 TLC5947 PWM Drivers
class CAZ_7x4_RGB  {
public:
  // Remeber to do 'begin()' in 'setup()'
  CAZ_7x4_RGB(uint8_t clk, uint8_t dat, uint8_t lat);

  // Set output pins, clear segments
  void begin();

  // Set segment s of digit d to color index c
  void set_segment(uint8_t d, uint8_t s, uint8_t c);

  // Set value v to digit d using color index c
  void set_digit(uint8_t d, uint8_t v, uint8_t c);

  // Update display of digit d with value v using random colors, 
  // Slowly updating each segments in random order 
  void write_digit_slow(uint8_t d, uint8_t v);

  // Update entire display quickly 
  void write(void);
  
private:
  uint8_t segments[4][7];  // Holds color index for each segment of each digit
	uint8_t order[7] = { 0,1,2,3,4,5,6 };  // Holds radomized order to update segments
	uint8_t _clk, _dat, _lat;
};

#endif // _CAZ_7X4_RGB_H_
