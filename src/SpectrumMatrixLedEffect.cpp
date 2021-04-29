/*
* SpectrumMatrixLedEffect.cpp
*
*/

#include "SpectrumMatrixLedEffect.h"

const char* const SpectrumMatrixLedEffect::name = "SPECTRUM";

SpectrumMatrixLedEffect::SpectrumMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, ISpectrumBandConverter* audioConverter)
	: ILedEffect(Hz), matrix(matrixConverter), audio(audioConverter), fallTimer(50)
{
	bands = new BAND[matrix->getWidth()];

	fallTimer.start();

	reset();
}

SpectrumMatrixLedEffect::~SpectrumMatrixLedEffect()
{
	if (bands != nullptr)
	{
		delete[] bands;
	}
}

void SpectrumMatrixLedEffect::reset()
{
	ILedEffect::reset();

	matrix->clearAllLeds();
}

void SpectrumMatrixLedEffect::paint()
{
	matrix->clearAllLeds();

	bool fallMaximus = fallTimer.isReady();

	uint16_t maxValue = 0;

	for (uint8_t column = 0; column < matrix->getWidth(); column++)
	{
		bands[column].level = audio->getBandValue(column) * 0.2F + static_cast<float>(bands[column].level) * 0.8F;

		// find the maximum of all bands
		if (bands[column].level > maxValue)
			maxValue = bands[column].level;

		// convert band level to column height
		long columnLevel = map(static_cast<long>(bands[column].level), audio->getLowGain(), audio->getHiGain(), 0, matrix->getHeight());
		columnLevel = constrain(columnLevel, 0, matrix->getHeight());

		for (uint8_t y = 0; y < columnLevel; y++)
		{
			if (y <= 0.5F * matrix->getHeight())
			{
				matrix->getPixel(column, y) = CRGB::Green;
			}
			else if (y <= 0.8F * matrix->getHeight())
			{
				matrix->getPixel(column, y) = CRGB::Yellow;
			}
			else
			{
				matrix->getPixel(column, y) = CRGB::Red;
			}
		}

		// update position of column's maximum level
		if (columnLevel > 1 && (columnLevel - 1) >= bands[column].maximumPosition)
		{
			bands[column].maximumPosition = columnLevel - 1;
			bands[column].maximumTime = getClock();
		}

		// draw position of column's maximum level
		if (bands[column].maximumPosition >= 1)
		{
			matrix->getPixel(column, bands[column].maximumPosition) = CRGB::Blue;
			if (fallMaximus && (getClock() - bands[column].maximumTime) > MAXIMUM_HOLD_TIME)
			{
				bands[column].maximumPosition--;
			}
		}
	}

	// auto gain - set high gain level to current level
	if (maxValue < audio->getLowGain())
	{
		audio->setHiGain(audio->HI_PASS);
	}
	else
	{
		audio->setHiGain(1.1F * maxValue);
	}
}
