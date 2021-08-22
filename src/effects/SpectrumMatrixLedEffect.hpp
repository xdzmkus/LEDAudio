/* 
* SpectrumMatrixLedEffect.hpp
*
*/

#ifndef __SPECTRUMMATRIXLEDEFFECT_HPP__
#define __SPECTRUMMATRIXLEDEFFECT_HPP__

#include <EffectTimer.hpp>
#include <ILedEffect.hpp>
#include <ILedMatrix.hpp>
#include "converter/ISpectrumBandConverter.h"

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class SpectrumMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static const char* const name;

protected:

	ISpectrumBandConverter* audio;

private:

	MillisTimer fallTimer;

	static const unsigned long MAXIMUM_HOLD_TIME = 600;

	struct BAND
	{
		uint16_t level = 0;
		unsigned long maximumTime = 0;
		uint8_t maximumPosition = 0;
	}
	bands[width];

public:

	SpectrumMatrixLedEffect(uint16_t Hz, ISpectrumBandConverter* audioConverter);
	~SpectrumMatrixLedEffect();
	
	void reset() override;
	void paint() override;

	operator const char* () const {	return name; }

private:

	SpectrumMatrixLedEffect(const SpectrumMatrixLedEffect&) = delete;
	SpectrumMatrixLedEffect& operator=(const SpectrumMatrixLedEffect&) = delete;

};

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const char* const SpectrumMatrixLedEffect<MATRIX, ledLine, width, height>::name = "SPECTRUM";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SpectrumMatrixLedEffect<MATRIX, ledLine, width, height>::SpectrumMatrixLedEffect(uint16_t Hz, ISpectrumBandConverter* audioConverter)
	: ILedEffect(Hz), audio(audioConverter), fallTimer(50)
{
	fallTimer.start();

	reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
SpectrumMatrixLedEffect<MATRIX, ledLine, width, height>::~SpectrumMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void SpectrumMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();

	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void SpectrumMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	MATRIX<ledLine, width, height>::clearAllLeds();

	bool fallMaximus = fallTimer.isReady();

	uint16_t maxValue = 0;

	for (uint8_t column = 0; column < MATRIX<ledLine, width, height>::getWidth(); column++)
	{
		bands[column].level = audio->getBandValue(column) * 0.2F + static_cast<float>(bands[column].level) * 0.8F;

		// find the maximum of all bands
		if (bands[column].level > maxValue)
			maxValue = bands[column].level;

		// convert band level to column height
		long columnLevel = map(static_cast<long>(bands[column].level), audio->getLowGain(), audio->getHiGain(), 0, MATRIX<ledLine, width, height>::getHeight());
		columnLevel = constrain(columnLevel, 0, (MATRIX<ledLine, width, height>::getHeight()));

		for (uint8_t y = 0; y < columnLevel; y++)
		{
			if (y <= 0.5F * MATRIX<ledLine, width, height>::getHeight())
			{
				MATRIX<ledLine, width, height>::getPixel(column, y) = CRGB::Green;
			}
			else if (y <= 0.8F * MATRIX<ledLine, width, height>::getHeight())
			{
				MATRIX<ledLine, width, height>::getPixel(column, y) = CRGB::Yellow;
			}
			else
			{
				MATRIX<ledLine, width, height>::getPixel(column, y) = CRGB::Red;
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
			MATRIX<ledLine, width, height>::getPixel(column, bands[column].maximumPosition) = CRGB::Blue;
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

#endif
