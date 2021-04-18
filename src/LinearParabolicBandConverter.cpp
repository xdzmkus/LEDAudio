/*
* LinearParabolicBandConverter.cpp
*
*/

#include "LinearParabolicBandConverter.h"

LinearParabolicBandConverter::LinearParabolicBandConverter(uint8_t spectrum[], uint8_t samples, uint8_t bands) : IAudioConverter(spectrum, samples), bandsNum(bands)
{
}

LinearParabolicBandConverter::~LinearParabolicBandConverter()
{
}

IAudioConverter::BANDWIDTH LinearParabolicBandConverter::calcBandWidth(uint8_t band)
{
    uint8_t startSample = band + (samplesNum - bandsNum - 1);  // linear + parabolic
    // uint8_t startSample = (samplesNum - 1);  // only parabolic

    BANDWIDTH bandWidth = {};

    bandWidth.idxPrev = round(startSample * pow(static_cast<float>(band) / (bandsNum + 1), 2));
    bandWidth.idxCurr = round(startSample * pow(static_cast<float>(band) / (bandsNum), 2));
    bandWidth.idxNext = round(startSample * pow(static_cast<float>(band) / (bandsNum - 1), 2));

    return bandWidth;
}
