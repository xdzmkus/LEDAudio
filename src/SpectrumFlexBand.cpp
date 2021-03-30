/*
* SpectrumFlexBand.cpp
*
*/

#include "SpectrumFlexBand.h"

SpectrumFlexBand::SpectrumFlexBand(uint8_t spectrum[], uint8_t samples, uint8_t bands) : IAudioConverter(spectrum, samples), bandsNum(bands)
{
}

SpectrumFlexBand::~SpectrumFlexBand()
{
}

void SpectrumFlexBand::calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext)
{
    idxPrev = (samplesNum - 1) * pow(static_cast<float>(band) / (bandsNum + 1), 2);
    idxCurr = (samplesNum - 1) * pow(static_cast<float>(band) / (bandsNum), 2);
    idxNext = (samplesNum - 1) * pow(static_cast<float>(band) / (bandsNum - 1), 2);
}



