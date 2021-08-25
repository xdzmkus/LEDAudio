/* 
* VUMeterMatrixLedEffect.hpp
*
*/

#ifndef __VUMETERMATRIXLEDEFFECT_HPP__
#define __VUMETERMATRIXLEDEFFECT_HPP__

#include <ILedEffect.hpp>
#include <ILedMatrix.hpp>

/**
* The audio preprocess procedure.
*
* @param r - right channel volume level
* @param l - left channel volume level
*/
typedef void (*VUMeterMatrixLedPreprocess)(uint16_t &r, uint16_t &l);

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class VUMeterMatrixLedEffect : public MATRIX<ledLine, width, height>, public ILedEffect
{
public:

	static const char* const name;

protected:

	const uint16_t NOISE_LEVEL;

	VUMeterMatrixLedPreprocess preprocess;

private:

	uint16_t maxLevel;

public:

	VUMeterMatrixLedEffect(uint16_t Hz, uint16_t noiseLevel = 255, VUMeterMatrixLedPreprocess pre = nullptr);
	~VUMeterMatrixLedEffect();
	
	void reset() override;

	void paint() override;

	operator const char* () const { return name; }

protected:

	void autoGain(uint16_t maxVU);

	virtual void drawLeftChannel(uint16_t currentVU);
	virtual void drawRightChannel(uint16_t currentVU);

	virtual void drawVUsignal(uint8_t x, uint8_t columnLevel);

	virtual uint8_t getColumnLevel(uint16_t currentVU);

private:

	VUMeterMatrixLedEffect(const VUMeterMatrixLedEffect&) = delete;
	VUMeterMatrixLedEffect& operator=(const VUMeterMatrixLedEffect&) = delete;
};


template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const char* const VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::name = "VUMETER";

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::VUMeterMatrixLedEffect(uint16_t Hz, uint16_t noiseLevel, VUMeterMatrixLedPreprocess pre)
	: ILedEffect(Hz), NOISE_LEVEL(noiseLevel), preprocess(pre), maxLevel(noiseLevel)
{
	reset();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::~VUMeterMatrixLedEffect()
{
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::reset()
{
	ILedEffect::reset();

	MATRIX<ledLine, width, height>::clearAllLeds();
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::paint()
{
	uint16_t rightVU = 0;
	uint16_t leftVU = 0;

	if (preprocess)
	{
		preprocess(rightVU, leftVU);
		autoGain(max(rightVU, leftVU));
	}
	else
	{
		rightVU = random(0, maxLevel);
		leftVU = random(0, maxLevel);
	}

	drawLeftChannel(leftVU);
	drawRightChannel(rightVU);
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::autoGain(uint16_t maxVU)
{
	if (maxLevel > NOISE_LEVEL) maxLevel -= 5;

	if (maxVU > maxLevel) maxLevel = maxVU;
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::drawLeftChannel(uint16_t currentVU)
{
	// shift left half of matrix
	(*this) << MATRIX<ledLine, width, height>::getWidth() / 2 - 1;

	drawVUsignal(MATRIX<ledLine, width, height>::getWidth() / 2 - 1, getColumnLevel(currentVU));
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::drawRightChannel(uint16_t currentVU)
{
	// shift right half of matrix
	(*this) >> MATRIX<ledLine, width, height>::getWidth() / 2;

	drawVUsignal(MATRIX<ledLine, width, height>::getWidth() / 2, getColumnLevel(currentVU));
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
void VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::drawVUsignal(uint8_t x, uint8_t columnLevel)
{
	// change column level to be symetric to center line
	if (MATRIX<ledLine, width, height>::getHeight() % 2 == 0)
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

	uint8_t startY = (MATRIX<ledLine, width, height>::getHeight() - columnLevel) / 2;

	// draw current value of channel at position x
	for (uint8_t y = 0; y < startY; y++)
	{
		MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Black;
	}
	for (uint8_t y = startY; y < columnLevel + startY; y++)
	{
		MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Green;
	}
	for (uint8_t y = columnLevel + startY; y < MATRIX<ledLine, width, height>::getHeight(); y++)
	{
		MATRIX<ledLine, width, height>::getPixel(x, y) = CRGB::Black;
	}
}

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
uint8_t VUMeterMatrixLedEffect<MATRIX, ledLine, width, height>::getColumnLevel(uint16_t currentVU)
{
	// convert channel level to column height
	long columnLevel = map(currentVU, 0, maxLevel, 0, MATRIX<ledLine, width, height>::getHeight());

	return constrain(columnLevel, 0, (MATRIX<ledLine, width, height>::getHeight()));
}

#endif
