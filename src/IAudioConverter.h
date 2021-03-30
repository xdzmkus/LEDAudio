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

	static const uint8_t LOW_PASS = 30;
	static const uint8_t HI_PASS = 150;

protected:

	uint8_t* spectrumValues;
	const uint8_t samplesNum;

private:
	
	long low_gain = LOW_PASS; // нижний уровень звука (порог чувствительности шумов)
	long hi_gain = HI_PASS;   // верхний уровень звука

public:

	IAudioConverter(uint8_t spectrum[], uint8_t samples);
	virtual ~IAudioConverter();
	
	long getLowGain();
	void setLowGain(long gain);

	long getHiGain();
	void setHiGain(long gain);

	virtual void filter(bool normalize = false);
	virtual float scale(uint8_t band);

protected:

	virtual void calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext) = 0;

private:

	IAudioConverter(const IAudioConverter&) = delete;
	IAudioConverter& operator=(const IAudioConverter&) = delete;
};

#endif
