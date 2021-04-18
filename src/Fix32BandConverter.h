/* 
* Fix32BandConverter.h
*
*/

#ifndef __FIX32BANDCONVERTER_H__
#define __FIX32BANDCONVERTER_H__

#include "IAudioConverter.h"

class Fix32BandConverter : public IAudioConverter
{
protected:

	static const uint8_t bands[34] PROGMEM;

public:

	Fix32BandConverter(uint8_t spectrum[], uint8_t samples);
	virtual ~Fix32BandConverter();
	
	float scale(uint8_t band) override;

protected:

	virtual BANDWIDTH calcBandWidth(uint8_t band);

private:
	Fix32BandConverter(const Fix32BandConverter&) = delete;
	Fix32BandConverter& operator=(const Fix32BandConverter&) = delete;
};

#endif
