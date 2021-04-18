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

void IAudioConverter::removeNoise()
{
    for (uint8_t i = 0; i < samplesNum; i++)
    {
        if (spectrumValues[i] < LOW_PASS)
            spectrumValues[i] = 0;
    }
}

void IAudioConverter::removeNotSound()
{
    uint8_t soundsBands = 0;

    for (uint8_t i = 0; i < samplesNum; i++)
    {
        if (spectrumValues[i] >= LOW_PASS)
            ++soundsBands;
    }

    if (soundsBands < NOISE_BAND_LIMIT)
        memset((void*)spectrumValues, 0, samplesNum);

}

void IAudioConverter::gain(float k)
{
    for (uint8_t i = 0; i < samplesNum; i++)
    {
        spectrumValues[i] = k * static_cast<float>(spectrumValues[i]);
    }
}

void IAudioConverter::normalize()
{
    for (uint8_t i = 0; i < samplesNum; i++)
    {
        // reduce level of high frequencies 
        spectrumValues[i] = static_cast<float>(spectrumValues[i]) / (1.0F + static_cast<float>(i) / samplesNum);
    }
}

float IAudioConverter::scale(uint8_t band)
{
    BANDWIDTH bandWidth = calcBandWidth(band);

    float toneLevel = spectrumValues[bandWidth.idxCurr];

    uint8_t delta = bandWidth.idxCurr - bandWidth.idxPrev;
    // from previous band to current
    for (uint8_t i = 0; i < delta; i++)
    {
        toneLevel += static_cast<float>(i) / delta * spectrumValues[bandWidth.idxPrev + i];
    }
    delta = bandWidth.idxNext - bandWidth.idxCurr;
    // from current to next band
    for (uint8_t i = 0; i < delta; i++)
    {
        toneLevel += static_cast<float>(i) / delta * spectrumValues[bandWidth.idxNext - i];
    }

    return toneLevel;
}
