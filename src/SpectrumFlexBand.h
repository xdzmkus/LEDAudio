/* 
* SpectrumFlexBand.h
*
*/

#ifndef __SPECTRUMFLEXBAND_H__
#define __SPECTRUMFLEXBAND_H__

#include "IAudioConverter.h"

class SpectrumFlexBand : public IAudioConverter
{
protected:

	const uint8_t bandsNum;

public:

	SpectrumFlexBand(uint8_t spectrum[], uint8_t samples, uint8_t bands);
	virtual ~SpectrumFlexBand();

protected:

	virtual void calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext);

private:
	SpectrumFlexBand(const SpectrumFlexBand&) = delete;
	SpectrumFlexBand& operator=(const SpectrumFlexBand&) = delete;
};

#endif
