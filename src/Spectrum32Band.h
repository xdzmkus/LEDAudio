/* 
* Spectrum32Band.h
*
*/

#ifndef __SPECTRUM32BAND_H__
#define __SPECTRUM32BAND_H__

#include "IAudioConverter.h"

class Spectrum32Band : public IAudioConverter
{
protected:

	const uint8_t bands[34] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 32, 36, 40, 44, 48, 52, 57, 62, 67, 72, 78, 84, 90, 96, 102, 108, 120, 127 };

public:

	Spectrum32Band(uint8_t spectrum[], uint8_t samples);
	virtual ~Spectrum32Band();
	
protected:

	virtual void calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext);

private:
	Spectrum32Band(const Spectrum32Band&) = delete;
	Spectrum32Band& operator=(const Spectrum32Band&) = delete;
};

#endif
