#ifndef D1MiniColorMatrix_h
#define D1MiniColorMatrix_h

#if ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"

#define SWIPEUP 8
#define SWIPEDOWN 9

#define FIVE    13        // these are in hexadecimal
#define TEN     14
#define AQUARTER 15
#define TWENTY   16
#define HALF     17
#define PAST     18
#define TO       19


const uint8_t gammatable[256] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
	115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
	144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
	177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
	215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
};

const uint64_t words[] = {
	0xF600,				//ZERO
	0x49,				//#define ONE      0x49
	0xC040,				//#define TWO      0xC040
	0x1F0000,			//#define THREE    0x1F0000
	0xF0,				//#define FOUR     0xF0
	0xF0000000,			//#define FIVE     0xF0000000
	0xE00000,			//#define SIX      0xE00000
	0x800F00,			//#define SEVEN    0x800F00
	0x1F000000,			//#define EIGHT    0x1F000000
	0xF,				//#define NINE     0xF
	0x1010100,			//#define TEN      0x1010100
	0x3F00,				//#define ELEVEN   0x3F00
	0xF600,				//#define TWELVE   0xF600
	0xF00000000000,		//#define MFIVE    0xF00000000000        // these are in hexadecimal
	0x5800000000000000,	//#define MTEN     0x5800000000000000
	0x80FE000000000000,	//#define AQUARTER 0x80FE000000000000
	0x7E00000000000000,	//#define TWENTY   0x7E00000000000000
	0xF0000000000,		//#define HALF     0xF0000000000
	0x7800000000,		//#define PAST     0x7800000000
	0xC00000000,		//#define TO       0xC00000000
};

const uint64_t digit0[] = {
  0x609090909090906,
  0x602020202020207,
  0x60909010204080f,
  0x609010602010906,
  0x60a0a0a0a0f0202,
  0xf08080e01010906,
  0x609080e09090906,
  0xf01010204040404,
  0x609090609090906,
  0x609090907010906,
};

const uint64_t digit1[] = {
  0x6090909090909060,
  0x6020202020202070,
  0x60909010204080f0,
  0x6090106020109060,
  0x60a0a0a0a0f02020,
  0xf08080e010109060,
  0x609080e090909060,
  0xf010102040404040,
  0x6090906090909060,
  0x6090909070109060,
};

const uint64_t smallDigit0[] = {
  0x40a0a0a0a04,
  0x40c0404040e,
  0xc020204080e,
  0xc020c02020c,
  0x2060a0a0e02,
  0xe080c02020c,
  0x6080c0a0a04,
  0xe0204040404,
  0x40a040a0a04,
  0x40a0a06020c,
};
const uint64_t smallDigit1[] = {
  0x205050505020,
  0x206020202070,
  0x601010204070,
  0x601060101060,
  0x103050507010,
  0x704060101060,
  0x304060505020,
  0x701020202020,
  0x205020505020,
  0x205050301060,
};

const uint8_t touchPin[4] = {D5, D6, D7, D8};

class D1MiniColorMatrix : public Adafruit_GFX, public Adafruit_NeoPixel {

public:
	D1MiniColorMatrix();

	void fillMatrix(uint16_t color);

	void matrixWhiteBal(uint8_t r, uint8_t g, uint8_t b);

	void setCorner(uint8_t n, uint8_t r, uint8_t g, uint8_t b);

	void setCorner(uint8_t n, uint16_t color);

	void drawPixel(int16_t x, int16_t y, uint16_t color);

	void drawPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);

	void drawMatrix(uint64_t bitmap, uint16_t color);

	void drawMatrix(uint64_t bitmap, uint16_t color1, uint16_t color2);

	void drawWord(uint8_t number, uint16_t color1, uint16_t color2);

	void write();

	uint8_t readTouch();

	DateTime now();

	RTC_DS3231 rtc;

private:
	volatile int touchPos = 0, touchStat = 0;

	volatile uint32_t touchTime = 0;

	uint8_t matrixBuffer[192];

	void touchInit();

	int getTouchPos();

	void touchPoll();

	static uint8_t getR(uint16_t color);

	static uint8_t getG(uint16_t color);

	static uint8_t getB(uint16_t color);
};

#endif