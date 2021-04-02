/*
* IAudioConverter.cpp
*
*/

#include "IAudioConverter.h"

IAudioConverter::IAudioConverter(uint8_t spectrum[], uint8_t samples)
	: spectrumValues(spectrum), samplesNum(samples)
{
}

IAudioConverter::~IAudioConverter()
{
}

long IAudioConverter::getLowGain()
{
    return low_gain;
}

void IAudioConverter::setLowGain(long gain)
{
    low_gain = gain;
}

long IAudioConverter::getHiGain()
{
    return hi_gain;
}

void IAudioConverter::setHiGain(long gain)
{
    hi_gain = gain;
}

void IAudioConverter::filter(bool normalize)
{
    for (uint8_t i = 0; i < samplesNum; i++)
    {
        // remove noise
        if (spectrumValues[i] < low_gain) spectrumValues[i] = 0;

        // gain signal
        spectrumValues[i] = 1.5F * spectrumValues[i];

        // reduce level of high frequencies 
        if (normalize) spectrumValues[i] = static_cast<float>(spectrumValues[i]) / (1.0F + static_cast<float>(i) / samplesNum);
    }
}

float IAudioConverter::scale(uint8_t band)
{
    uint8_t idxPrev;
    uint8_t idxCurr;
    uint8_t idxNext;

    calcBandWidth(band, idxPrev, idxCurr, idxNext);

    float toneLevel = spectrumValues[idxCurr];

    uint8_t delta = idxCurr - idxPrev;
    // from previous band to current
    for (uint8_t i = 0; i < delta; i++)
    {
        toneLevel += static_cast<float>(i) / delta * spectrumValues[idxPrev + i];
    }
    delta = idxNext - idxCurr;
    // from current to next band
    for (uint8_t i = 0; i < delta; i++)
    {
        toneLevel += static_cast<float>(i) / delta * spectrumValues[idxNext - i];
    }

    return toneLevel;
}
