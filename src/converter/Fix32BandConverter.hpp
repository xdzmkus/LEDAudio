/* 
* Fix32BandConverter.hpp
*
*/

#ifndef __FIX32BANDCONVERTER_HPP__
#define __FIX32BANDCONVERTER_HPP__

#include "TAudioConverter.hpp"

template <typename SV, typename SN>
class Fix32BandConverter : public TAudioConverter<SV, SN>
{
protected:

	static const uint8_t bands[34] PROGMEM;

public:

	Fix32BandConverter(SV spectrum[], SN samples) : TAudioConverter<SV, SN>(spectrum, samples)
	{
	};

	virtual ~Fix32BandConverter()
	{
	};
	
	virtual float getBandValue(uint8_t band) override
	{
		// stupid guardian
		if (band >= 32) return 0.0F;

		ISpectrumBandConverter::BANDWIDTH bandWidth = getBandWidth(band);

		float toneLevel = 0;

		for (uint8_t i = bandWidth.idxPrev; i < bandWidth.idxNext; i++)
		{
			toneLevel += TAudioConverter<SV, SN>::spectrumValues[i];
		}

		return toneLevel;
	}

	virtual ISpectrumBandConverter::BANDWIDTH getBandWidth(uint8_t band) override
	{
		ISpectrumBandConverter::BANDWIDTH bandWidth = {};

		bandWidth.idxPrev = pgm_read_byte(&(bands[band]));
		bandWidth.idxCurr = pgm_read_byte(&(bands[band + 1]));
		bandWidth.idxNext = pgm_read_byte(&(bands[band + 2]));

		return bandWidth;
	}
};

template <typename SV, typename SN>
const uint8_t Fix32BandConverter<SV, SN>::bands[34] PROGMEM = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 32, 36, 40, 44, 48, 52, 57, 62, 67, 72, 78, 84, 90, 96, 102, 108, 120, 127 };

#endif
