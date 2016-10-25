#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include "RTClib.h"
#include "D1MiniColorMatrix.h"

D1MiniColorMatrix matrix;			//Setup the LED matrix module

void setup()
{
	matrix.setTextColor(0xFFFF, 0);	//Set text color to white, background to black
	matrix.setTextSize(1);			//Text size ratio 1
	matrix.setTextWrap(false);		//Disable the wrapping fuction to avoid problem when (x,y) position get negative
}

void loop()
{
	matrix.fillMatrix(0);								//Clear the matrix to black
	matrix.setCursor(millis() / 100 % 140 * -1 + 8, 0); //Move the cursor position left every 100 ms
	matrix.print("Happy Birthday, WZ!!");				//Print the string
	matrix.write();										//Refresh the display
	delay(10);
}