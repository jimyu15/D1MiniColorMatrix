# D1MiniColorMatrix

Arduino Library for the D1 Mini + Colorduino project.

##Installation
Copy the D1MiniColorMatrix folder to the library folder of your Arduino IDE.

These three additional libraries should be installed as well:

1. RTClib by Adafruit

2. Adafruit GFX Library by Adafruit

3. Adafruit NeoPixel by Adafruit

##Usage Reference

###Initialization

\#include \<Wire.h>

\#include \<Adafruit_GFX.h>

\#include \<Adafruit_NeoPixel.h>

\#include "RTClib.h"

\#include "D1MiniColorMatrix.h"

D1MiniColorMatrix matrix;

###Matrix Drawing functions

####void fillMatrix(uint16_t color)

_e.g._ matrix.fillMatrix(0xFFFF);

<br/>


####void drawPixel(int16_t x_position[0-7], int16_t y_position[0-7], uint16_t color)

_e.g._ matrix.drawPixel(3, 5, 0xF3FB);

<br/>


####void drawPixel(uint8_t x_position[0-7], uint8_t y_position[0-7], uint8_t red[0-255], uint8_t green[0-255], uint8_t blue[0-255])

_e.g._ matrix.drawPixel(3, 5, 255, 128, 30);

<br/>


####void drawMatrix(uint64_t bitmap, uint16_t color)

<br/>


####void drawMatrix(uint64_t bitmap, uint16_t color1, uint16_t color2)

<br/>


####void drawWord(uint8_t content, uint16_t color1, uint16_t color2)

Print the preset content for the word clock panel.

The content can be: [0-12], FIVE, TEN, AQUARTER, TWENTY, HALF, PAST, TO

_e.g._ matrix.drawWord(AQUARTER, 0xFFFF, 0xFFF0);

<br/>

####void write()

Refresh the LED matrix and the four corner LEDs.

_e.g._ matrix.write();

<br/>

###Corner LEDs Setting Functions

####void setCorner(uint8_t n[0-3], uint8_t r, uint8_t g, uint8_t b)

<br/>


####void setCorner(uint8_t n[0-3], uint16_t color)

<br/>


###Touch Sensing Functions

<br/>


####uint8_t touchPosition()

Returns the current position of the finger[0-7]. 0 represents no touching on the panel.

<br/>


####uint8_t pressPosition()

Returns the starting position of the last valid touch[0-7].

<br/>


####uint8_t releasePosition()

Returns the last position of the last valid touch[0-7]. 0 represents the finger has not left the touch panel.

<br/>


####uint32_t touchTime()

Returns the duration of the last valid touch[0-7]. 0 represents the finger has not left the touch panel.

<br/>


###Real Time Clock Functions

####DateTime now()
