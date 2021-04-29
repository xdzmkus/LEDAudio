/*
* ParabolicXBandConverter.hpp
*
*/

#ifndef __PARABOLICXBANDCONVERTER_HPP__
#define __PARABOLICXBANDCONVERTER_HPP__

#include "TAudioConverter.hpp"

template <typename SV, typename SN>
class ParabolicXBandConverter : public TAudioConverter<SV, SN>
{
protected:

	const uint8_t bandsNum;

public:

	ParabolicXBandConverter(SV spectrum[], SN samples, uint8_t bands) : TAudioConverter<SV, SN>(spectrum, samples), bandsNum(bands)
	{
	};

	virtual ~ParabolicXBandConverter()
	{
	};

	virtual float getBandValue(uint8_t band) override
	{
		// stupid guardian
		if (band >= bandsNum) return 0.0F;

		return TAudioConverter<SV, SN>::getBandValue(band);
	}

	virtual ISpectrumBandConverter::BANDWIDTH getBandWidth(uint8_t band) override
	{
		ISpectrumBandConverter::BANDWIDTH bandWidth = {};

		uint8_t startSample = band + (TAudioConverter<SV, SN>::samplesNum - bandsNum - 1);	// linear + parabolic
//		uint8_t startSample = (TAudioConverter<SV, SN>::samplesNum - 1);					// only parabolic

		bandWidth.idxPrev = round(startSample * pow(static_cast<float>(band) / (bandsNum + 1), 2));
		bandWidth.idxCurr = round(startSample * pow(static_cast<float>(band) / (bandsNum), 2));
		bandWidth.idxNext = round(startSample * pow(static_cast<float>(band) / (bandsNum - 1), 2));

		return bandWidth;
	}
};

#endif
