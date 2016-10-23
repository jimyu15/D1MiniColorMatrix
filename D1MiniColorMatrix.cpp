#include <Adafruit_NeoPixel.h>
#include <D1MiniColorMatrix.h>
#ifdef __AVR__
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #ifndef pgm_read_byte
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #endif
#endif

#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif

// Constructor for single matrix:
D1MiniColorMatrix::D1MiniColorMatrix() : Adafruit_GFX(8, 8), Adafruit_NeoPixel(4, D4, NEO_GRB + NEO_KHZ800)
{
	touchInit();
	Serial.begin(115200);
	begin();
	setBrightness(80);
	matrixWhiteBal(15, 35, 31);
	rtc.begin();


}




void D1MiniColorMatrix::matrixWhiteBal(uint8_t r, uint8_t g, uint8_t b)
{
	Serial.print("Matrix: ");
	Serial.write('I');
	Serial.write('D');
	Serial.write(r);
	Serial.write(g);
	Serial.write(b);
	Serial.write('E');
	Serial.println();
}

void D1MiniColorMatrix::drawPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
	if (x > 7 || x < 0 || y > 7 || y < 0)
		return;
	matrixBuffer[3 * (7 - x + y * 8)] = gammatable[r];
	matrixBuffer[3 * (7 - x + y * 8) + 1] = gammatable[g];
	matrixBuffer[3 * (7 - x + y * 8) + 2] = gammatable[b];
}

void D1MiniColorMatrix::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	drawPixel(x, y, getR(color), getG(color), getB(color));
}

void D1MiniColorMatrix::setCorner(uint8_t n, uint8_t r, uint8_t g, uint8_t b)
{
	setPixelColor(3 - n % 4, gammatable[r], gammatable[g], gammatable[b]);
}

void D1MiniColorMatrix::setCorner(uint8_t n, uint16_t color)
{
	setPixelColor(3 - n % 4, getR(color), getG(color), getB(color));
}

void D1MiniColorMatrix::write()
{
	Serial.print("Matrix: ");
	show();
	Serial.write('I');
	Serial.write('N');
	for (int i = 0; i < 192; i++)
		Serial.write(matrixBuffer[i]);
	Serial.write('E');
	Serial.println();
}

void D1MiniColorMatrix::fillMatrix(uint16_t color)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			drawPixel(j, i, color);
		}
	}
}

void D1MiniColorMatrix::drawMatrix(uint64_t bitmap, uint16_t color)
{
	uint64_t mask = 1;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (mask & bitmap)
				drawPixel(7 - j, 7 - i, color);
			mask *= 2;
		}
	}
}

void D1MiniColorMatrix::drawMatrix(uint64_t bitmap, uint16_t color1, uint16_t color2)
{
	int length = 0;
	for (int i = 0; i < 64; i++)
	{
		if (((uint64_t)1 << i) & bitmap)
			length++;
	}
	uint32_t *stripe = new uint32_t[length];
	for (int i = 0; i < length; i++)
	{
		uint32_t rr, gg, bb;
		rr = map (i, 0, length - 1, color2 >> 11, color1 >> 11);
		gg = map (i, 0, length - 1, (color2 >> 5) & 0x3F, (color1 >> 5) & 0x3F);
		bb = map (i, 0, length - 1, color2 & 0x1F, color1 & 0x1F);
		stripe[i] = (rr << 11) + (gg << 5) + bb;

	}
	int count = 0;
	uint64_t mask = 1;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (mask & bitmap)
				drawPixel(7 - j, 7 - i, stripe[count++]);
			mask *= 2;
		}
	}
	delete [] stripe;
}

void D1MiniColorMatrix::drawWord(uint8_t number, uint16_t color1, uint16_t color2)
{
	drawMatrix(words[number], color1, color2);
}

void D1MiniColorMatrix::touchInit() 
{
	for (int i = 0; i < 4; i++)
	{
		pinMode(touchPin[i], INPUT);
		//attachInterrupt(touchPin[i], reinterpret_cast<void (*)()>(&D1MiniColorMatrix::touchPoll), CHANGE);
	}
}

int D1MiniColorMatrix::getTouchPos()
{
	uint8_t touchRead = 0;
	for (int i = 0; i < 4; i++)
	{
		touchRead *= 2;
		touchRead += digitalRead(touchPin[i]);
	}
	switch (touchRead)
	{
		case B0001:
		return 1;
		break;

		case B0011:
		return 2;
		break;

		case B0010:
		case B0111:
		return 3;
		break;

		case B0110:
		return 4;
		break;

		case B0100:
		case B1110:
		return 5;
		break;

		case B1100:
		return 6;
		break;

		case B1000:
		return 7;
		break;

		default:
		return 0;
		break;
	}
}

void D1MiniColorMatrix::touchPoll()
{
	static uint8_t swiped = 2;
	int touchRead = getTouchPos();
	
	if (touchPos == 0 && touchRead > 0)
	{
		touchTime = millis();
		touchPos = touchRead;
		swiped = 2;
		return;
	}

	else if (touchRead == 0 && touchPos > 0)
	{
		if (millis() - touchTime > 20 && millis() - touchTime < 500)
			touchStat = touchPos;
		swiped = 0;
	}

	else if (touchPos)
	{
		switch (touchRead - touchPos)
		{
			case 1:
			if (swiped)
				swiped--;
			else
				touchStat = SWIPEUP;
			break;

			case -1:
			if (swiped)
				swiped--;
			else
				touchStat = SWIPEDOWN;
			break;

			default:
			break;
		}
	}

	
	touchPos = touchRead;

}

uint8_t D1MiniColorMatrix::readTouch()
{
	uint8_t st = touchStat;
	touchStat = 0;
	return st;
}

uint8_t D1MiniColorMatrix::getR(uint16_t color)
{
	return map((color >> 11) & 0x1F, 0, 0x1F, 0, 255);
}


uint8_t D1MiniColorMatrix::getG(uint16_t color)
{
	return map((color >> 5) & 0x3F, 0, 0x3F, 0, 255);
}


uint8_t D1MiniColorMatrix::getB(uint16_t color)
{
	return map(color & 0x1F, 0, 0x1F, 0, 255);
}

DateTime D1MiniColorMatrix::now()
{
	return rtc.now();
}