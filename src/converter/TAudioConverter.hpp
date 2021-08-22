/*
* TAudioConverter.hpp
*
*/

#ifndef __TAUDIOCONVERTER_HPP__
#define __TAUDIOCONVERTER_HPP__

#include "ISpectrumBandConverter.h"

template <typename SV, typename SN>
class TAudioConverter : public ISpectrumBandConverter
{
public:

protected:

	SV* spectrumValues;
	const SN samplesNum;

public:

	TAudioConverter(SV spectrum[], SN samples);
	virtual ~TAudioConverter();

	virtual void removeNoise();
	virtual void removeNotSound(uint8_t valuedSamples = 5);
	virtual void gain(float k = 1.5F);
	virtual void normalize();

    virtual float getBandValue(uint8_t band) override;

private:

	TAudioConverter(const TAudioConverter&) = delete;
	TAudioConverter& operator=(const TAudioConverter&) = delete;
};

template <typename SV, typename SN>
TAudioConverter<SV, SN>::TAudioConverter(SV spectrum[], SN samples)
    : spectrumValues(spectrum), samplesNum(samples)
{
}

template <typename SV, typename SN>
TAudioConverter<SV, SN>::~TAudioConverter()
{
}

template <typename SV, typename SN>
void TAudioConverter<SV, SN>::removeNoise()
{
    for (SN i = 0; i < samplesNum; i++)
    {
        if (spectrumValues[i] < getLowGain())
            spectrumValues[i] = 0;
    }
}

template <typename SV, typename SN>
void TAudioConverter<SV, SN>::removeNotSound(uint8_t valuedSamples)
{
    uint8_t soundsSamples = 0;

    for (SN i = 0; i < samplesNum; i++)
    {
        if (spectrumValues[i] >= getLowGain())
            ++soundsSamples;
    }

    if (soundsSamples < valuedSamples)
        memset((void*)spectrumValues, 0, sizeof(SV)*samplesNum);

}

template <typename SV, typename SN>
void TAudioConverter<SV, SN>::gain(float k)
{
    for (SN i = 0; i < samplesNum; i++)
    {
        spectrumValues[i] = k * spectrumValues[i];
    }
}

template <typename SV, typename SN>
void TAudioConverter<SV, SN>::normalize()
{
    for (SN i = 0; i < samplesNum; i++)
    {
        // reduce level of high frequencies 
        spectrumValues[i] = static_cast<float>(spectrumValues[i]) / (1.0F + static_cast<float>(i) / samplesNum);
    }
}

template <typename SV, typename SN>
float TAudioConverter<SV, SN>::getBandValue(uint8_t band)
{
    BANDWIDTH bandWidth = getBandWidth(band);

    float toneLevel = spectrumValues[bandWidth.idxCurr];

    // from previous band to current
    uint8_t delta = bandWidth.idxCurr - bandWidth.idxPrev;
    for (uint8_t i = 0; i < delta; i++)
    {
        toneLevel += static_cast<float>(i) / delta * spectrumValues[bandWidth.idxPrev + i];
    }

    // from current to next band
    delta = bandWidth.idxNext - bandWidth.idxCurr;
    for (uint8_t i = 0; i < delta; i++)
    {
        toneLevel += static_cast<float>(i) / delta * spectrumValues[bandWidth.idxNext - i];
    }

    return toneLevel;
}

#endif
