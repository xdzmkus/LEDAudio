/* 
* IAudioConverter.h
*
*/

#ifndef __IAUDIOCONVERTER_H__
#define __IAUDIOCONVERTER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class IAudioConverter
{
public:

	struct BANDWIDTH
	{
		uint8_t idxPrev;
		uint8_t idxCurr;
		uint8_t idxNext;
	};

	static const uint8_t NOISE_BAND_LIMIT = 5;
	static const uint8_t LOW_PASS = 30;
	static const uint8_t HI_PASS = 150;

protected:

	uint8_t* spectrumValues;
	const uint8_t samplesNum;

private:
	
	long low_gain = LOW_PASS; // low volume level (reduce noise)
	long hi_gain = HI_PASS;   // high volume level

public:

	IAudioConverter(uint8_t spectrum[], uint8_t samples);
	virtual ~IAudioConverter();
	
	long getLowGain();
	void setLowGain(long gain);

	long getHiGain();
	void setHiGain(long gain);

	virtual void removeNoise();
	virtual void removeNotSound();
	virtual void gain(float k = 1.5F);
	virtual void normalize();
	virtual float scale(uint8_t band);

protected:

	virtual BANDWIDTH calcBandWidth(uint8_t band) = 0;

private:

	IAudioConverter(const IAudioConverter&) = delete;
	IAudioConverter& operator=(const IAudioConverter&) = delete;
};

#endif
