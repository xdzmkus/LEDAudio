/*
* ParabolicBandConverter.cpp
*
*/

#include "ParabolicBandConverter.h"

ParabolicBandConverter::ParabolicBandConverter(uint8_t spectrum[], uint8_t samples, uint8_t bands) : IAudioConverter(spectrum, samples), bandsNum(bands)
{
}

ParabolicBandConverter::~ParabolicBandConverter()
{
}

void ParabolicBandConverter::calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext)
{
    idxPrev = round((samplesNum - 1) * pow(static_cast<float>(band) / (bandsNum + 1), 2));
    idxCurr = round((samplesNum - 1) * pow(static_cast<float>(band) / (bandsNum), 2));
    idxNext = round((samplesNum - 1) * pow(static_cast<float>(band) / (bandsNum - 1), 2));
}



