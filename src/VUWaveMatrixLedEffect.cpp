/*
* VUWaveMatrixLedEffect.cpp
*
*/
#include "VUWaveMatrixLedEffect.h"

const char* const VUWaveMatrixLedEffect::name = "VUWAVE";

VUWaveMatrixLedEffect::VUWaveMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, uint16_t noiseLevel)
	: VUMeterMatrixLedEffect(matrixConverter, Hz, noiseLevel)
{
}

VUWaveMatrixLedEffect::~VUWaveMatrixLedEffect()
{
}

void VUWaveMatrixLedEffect::drawVUsignal(uint8_t x, uint8_t columnLevel)
{
	// draw current value of channel at position x
	for (uint8_t y = 0; y < columnLevel; y++)
	{
		if (y <= 0.5F * matrix->getHeight())
		{
			matrix->getPixel(x, y) = CRGB::Green;
		}
		else if (y <= 0.8F * matrix->getHeight())
		{
			matrix->getPixel(x, y) = CRGB::Yellow;
		}
		else
		{
			matrix->getPixel(x, y) = CRGB::Red;
		}
	}
	for (uint8_t y = columnLevel; y < matrix->getHeight(); y++)
	{
		matrix->getPixel(x, y) = CRGB::Black;
	}
}
