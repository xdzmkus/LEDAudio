/*
* VUMeterMatrixLedEffect.cpp
*
*/

#include "VUMeterMatrixLedEffect.h"

const char* const VUMeterMatrixLedEffect::name = "VUMETER";

VUMeterMatrixLedEffect::VUMeterMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, uint16_t noiseLevel)
	: ILedEffect(Hz), matrix(matrixConverter), NOISE_LEVEL(noiseLevel), maxLevel(noiseLevel)
{
	reset();
}

VUMeterMatrixLedEffect::~VUMeterMatrixLedEffect()
{
}

void VUMeterMatrixLedEffect::reset()
{
	ILedEffect::reset();

	matrix->clearAllLeds();
}

void VUMeterMatrixLedEffect::paint(uint16_t rightVU, uint16_t leftVU)
{
	drawLeftChannel(leftVU);
	drawRightChannel(rightVU);
}

void VUMeterMatrixLedEffect::autoGain(uint16_t maxVU)
{
	if (maxLevel > NOISE_LEVEL) maxLevel -= 5;

	if (maxVU > maxLevel) maxLevel = maxVU;
}

void VUMeterMatrixLedEffect::drawLeftChannel(uint16_t currentVU)
{
	// shift left half of matrix
	(*matrix) << matrix->getWidth() / 2 - 1;

	drawVUsignal(matrix->getWidth() / 2 - 1, getColumnLevel(currentVU));
}

void VUMeterMatrixLedEffect::drawRightChannel(uint16_t currentVU)
{
	// shift right half of matrix
	(*matrix) >> matrix->getWidth() / 2;

	drawVUsignal(matrix->getWidth() / 2, getColumnLevel(currentVU));
}

void VUMeterMatrixLedEffect::drawVUsignal(uint8_t x, uint8_t columnLevel)
{
	// change column level to be symetric to center line
	if (matrix->getHeight() % 2 == 0)
	{
		// to be always odd
		if (columnLevel % 2 == 1)
			++columnLevel;
	}
	else
	{
		// to be always even
		if (columnLevel > 1 && columnLevel % 2 == 0)
			--columnLevel;
	}

	uint8_t startY = (matrix->getHeight() - columnLevel) / 2;

	// draw current value of channel at position x
	for (uint8_t y = 0; y < startY; y++)
	{
		matrix->getPixel(x, y) = CRGB::Black;
	}
	for (uint8_t y = startY; y < columnLevel + startY; y++)
	{
		matrix->getPixel(x, y) = CRGB::Green;
	}
	for (uint8_t y = columnLevel + startY; y < matrix->getHeight(); y++)
	{
		matrix->getPixel(x, y) = CRGB::Black;
	}
}

uint8_t VUMeterMatrixLedEffect::getColumnLevel(uint16_t currentVU)
{
	// convert channel level to column height
	long columnLevel = map(currentVU, 0, maxLevel, 0, matrix->getHeight());

	return constrain(columnLevel, 0, matrix->getHeight());
}
