// ISpectrumBandConverter.h

#ifndef _ISpectrumBandConverter_h
#define _ISpectrumBandConverter_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class ISpectrumBandConverter
{
public:

	struct BANDWIDTH
	{
		uint8_t idxPrev;
		uint8_t idxCurr;
		uint8_t idxNext;
	};

	static const long LOW_PASS = 30;
	static const long HI_PASS = 150;

private:

	long low_gain = LOW_PASS; // low volume level (reduce noise)
	long hi_gain = HI_PASS;   // high volume level

public:

	long getLowGain()
	{
		return low_gain;
	};
	void setLowGain(long gain)
	{
		low_gain = gain;
	};

	long getHiGain()
	{
		return hi_gain;
	};
	void setHiGain(long gain)
	{
		hi_gain = gain;
	};

	virtual float getBandValue(uint8_t band) = 0;
	virtual BANDWIDTH getBandWidth(uint8_t band) = 0;
};
#endif

