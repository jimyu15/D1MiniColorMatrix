#include <Adafruit_GFX.h>


D1MiniColorMatrix::D1MiniColorMatrix() : Adafruit_GFX(8, 8), Adafruit_NeoPixel(4, D4, NEO_GRB + NEO_KHZ800)
{
	Serial.begin(115200);
	this->begin();
	this->setBrightness(80);
	matrixWhiteBal(15, 35, 31);

}


void D1MiniColorMatrix::matrixWhiteBal(uint8_t r, uint8_t g, uint8_t b)
{
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
	matrixBuffer[3 * (x + y * 8)] = gammatable[r];
	matrixBuffer[3 * (x + y * 8) + 1] = gammatable[g];
	matrixBuffer[3 * (x + y * 8) + 2] = gammatable[b];
}

void D1MiniColorMatrix::drawPixel(uint8_t x, uint8_t y, uint32_t color)
{
	drawPixel(x, y, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

void D1MiniColorMatrix::setCorner(uint8_t n, uint8_t r, uint8_t g, uint8_t b)
{
	this->setPixelColor(n, gammatable[r], gammatable[g], gammatable[b]);
}

void D1MiniColorMatrix::setCorner(uint8_t n, uint32_t color)
{
	this->setPixelColor(n, color);
}

void D1MiniColorMatrix::write()
{
	corners.show();
	Serial.write('I');
	Serial.write('N');
	for (int i = 0; i < 192; i++)
		Serial.write(matrixBuffer[i]);
	Serial.write('E');
	Serial.println();
}