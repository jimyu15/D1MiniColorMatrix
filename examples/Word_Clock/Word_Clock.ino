#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include "RTClib.h"
#include "D1MiniColorMatrix.h"

D1MiniColorMatrix matrix;

uint16_t rgb(uint16_t rr, uint16_t gg, uint16_t bb)
{
	return ((rr >> 3) << 11) + ((gg >> 2) << 5) + (bb >> 3);
}

void setup()
{
	Serial.println("Hello!");

}

void loop()
{
	DateTime now = matrix.now();

	matrix.fillMatrix(0x0);
	
	uint16_t c1 = rgb(0, 255, 255), c2 = rgb(0, 0, 255);
	matrix.drawWord((now.hour() + now.minute() / 35) % 12, rgb(255, 255, 0), rgb(0, 255, 0));

	c1 = rgb(0, 255, 255);
	c2 = rgb(0, 0, 255);
	switch (now.minute() / 5)
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

	c1 = 0xFFFF;
	c2 = 0xFFFF;
	if (now.minute() >= 35)
		matrix.drawWord(TO, c1, c2);
	else
		matrix.drawWord(PAST, c1, c2);

	for (int i = 0; i < 4; i++)
		matrix.setCorner(i, (i < now.minute() % 5) ? rgb(255, 255, 255) : 0);

	matrix.write();
	
	delay(1000);

}