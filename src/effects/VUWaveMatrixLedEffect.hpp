/*
* VUWaveMatrixLedEffect.hpp
*
*/

#ifndef __VUWAVEMATRIXLEDEFFECT_HPP__
#define __VUWAVEMATRIXLEDEFFECT_HPP__

#include "VUMeterMatrixLedEffect.hpp"

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class VUWaveMatrixLedEffect :  public VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>
{
public:

	static LedEffectName const name;

public:

	VUWaveMatrixLedEffect(uint16_t Hz, uint16_t noiseLevel = 256, VUMeterMatrixLedPreprocess pre = nullptr);
	~VUWaveMatrixLedEffect();

	operator LedEffectName () const { return name; }

protected:

	virtual void drawVUsignal(uint8_t x, uint8_t columnLevel) override;

private:

	VUWaveMatrixLedEffect(const VUWaveMatrixLedEffect&) = delete;
	VUWaveMatrixLedEffect& operator=(const VUWaveMatrixLedEffect&) = delete;

};

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
LedEffectName const VUWaveMatrixLedEffect<MATRIX, ledLine, width, height>::name = "VUWAVE";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
VUWaveMatrixLedEffect<MATRIX, ledLine, width, height>::VUWaveMatrixLedEffect(uint16_t Hz, uint16_t noiseLevel, VUMeterMatrixLedPreprocess pre)
	: VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>(Hz, noiseLevel, pre)
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
VUWaveMatrixLedEffect<MATRIX, ledLine, width, height>::~VUWaveMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUWaveMatrixLedEffect<MATRIX, ledLine, width, height>::drawVUsignal(uint8_t x, uint8_t columnLevel)
{
	// draw current value of channel at position x
	for (uint8_t y = 0; y < columnLevel; y++)
	{
		if (y <= 0.5F * MATRIX<ledLine, width, height>::getHeight())
		{
			MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Green;
		}
		else if (y <= 0.8F * MATRIX<ledLine, width, height>::getHeight())
		{
			MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Yellow;
		}
		else
		{
			MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Red;
		}
	}
	for (uint8_t y = columnLevel; y < MATRIX<ledLine, width, height>::getHeight(); y++)
	{
		MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Black;
	}
}

#endif