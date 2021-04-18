/* 
* LinearParabolicBandConverter.h
*
*/

#ifndef __LINEARPARABOLICBANDCONVERTER_H__
#define __LINEARPARABOLICBANDCONVERTER_H__

#include "IAudioConverter.h"

class LinearParabolicBandConverter : public IAudioConverter
{
protected:

	const uint8_t bandsNum;

public:

	LinearParabolicBandConverter(uint8_t spectrum[], uint8_t samples, uint8_t bands);
	virtual ~LinearParabolicBandConverter();

protected:

	virtual BANDWIDTH calcBandWidth(uint8_t band);

private:
	LinearParabolicBandConverter(const LinearParabolicBandConverter&) = delete;
	LinearParabolicBandConverter& operator=(const LinearParabolicBandConverter&) = delete;
};

#endif
