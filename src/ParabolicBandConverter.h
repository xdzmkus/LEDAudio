/* 
* ParabolicBandConverter.h
*
*/

#ifndef __PARABOLICBANDCONVERTER_H__
#define __PARABOLICBANDCONVERTER_H__

#include "IAudioConverter.h"

class ParabolicBandConverter : public IAudioConverter
{
protected:

	const uint8_t bandsNum;

public:

	ParabolicBandConverter(uint8_t spectrum[], uint8_t samples, uint8_t bands);
	virtual ~ParabolicBandConverter();

protected:

	virtual void calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext);

private:
	ParabolicBandConverter(const ParabolicBandConverter&) = delete;
	ParabolicBandConverter& operator=(const ParabolicBandConverter&) = delete;
};

#endif
