#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include "RTClib.h"
#include "D1MiniColorMatrix.h"

D1MiniColorMatrix matrix;	//Setup the LED matrix module

uint16_t rgb(uint16_t rr, uint16_t gg, uint16_t bb)		//convert R, G, B into 16bit color data
{
	return ((rr >> 3) << 11) + ((gg >> 2) << 5) + (bb >> 3);
}

void setup()
{

}

void loop()
{
	DateTime now = matrix.now();		//Get current time data from RTC (real time clock) IC

	matrix.fillMatrix(0x0);				//Fill the matrix to black
	
	uint16_t c1, c2;

	c1 = rgb(0, 255, 255);		//Setting the color bar for the minute word
	c2 = rgb(0, 255, 0);		//Setting the color bar for the minute word
	switch (now.minute() / 5)	//Draw the minute word
	{
		case 0:
		break;

		case 1:
		case 11:
		matrix.drawWord(FIVE, c1, c2);
		break;

		case 2:
		case 10:
		matrix.drawWord(TEN, c1, c2);
		break;

		case 3:
		case 9:
		matrix.drawWord(AQUARTER, c1, c2);
		break;

		case 4:
		case 8:
		matrix.drawWord(TWENTY, c1, c2);
		break;

		case 5:
		case 7:
		matrix.drawWord(TWENTY, c1, c2);
		matrix.drawWord(FIVE, c1, c2);
		break;

		case 6:
		matrix.drawWord(HALF, c1, c2);
		break;

		default:
		break;
	}

	c1 = rgb(0, 255, 0);		//Setting the color bar for the to/past word
	c2 = rgb(255, 255, 0);		//Setting the color bar for the to/past word
	if (now.minute() >= 35)		//If minute >= 35 draw "TO"
		matrix.drawWord(TO, c1, c2);
	else if (now.minute() > 4)	//If 4 < minute < 35 draw "PAST"
		matrix.drawWord(PAST, c1, c2);

	c1 = rgb(255, 255, 0);	//Setting the color bar for the hour word
	c2 = rgb(255, 40, 0);	//Setting the color bar for the hour word

	matrix.drawWord((now.hour() + now.minute() / 35) % 12, c1, c2);	//Draw the hour word

	for (int i = 0; i < 4; i++)
		matrix.setCorner(i, (i < now.minute() % 5) ? rgb(255, 100, 0) : 0);	//Set the four corners showing the minute

	Serial.print("Touch: ");
	Serial.print(digitalRead(D5));
	Serial.print("\t");
	Serial.print(digitalRead(D6));
	Serial.print("\t");
	Serial.print(digitalRead(D7));
	Serial.print("\t");
	Serial.print(digitalRead(D8));
	Serial.print("\t");
	matrix.write();			//refresh and write the display
	
	delay(20);

}