/*
* Spectrum32Band.cpp
*
*/

#include "Spectrum32Band.h"

Spectrum32Band::Spectrum32Band(uint8_t spectrum[], uint8_t samples) : IAudioConverter(spectrum, samples)
{
}

Spectrum32Band::~Spectrum32Band()
{
}

void Spectrum32Band::calcBandWidth(uint8_t band, uint8_t& idxPrev, uint8_t& idxCurr, uint8_t& idxNext)
{
    idxPrev = bands[band];
    idxCurr = bands[band + 1];
    idxNext = bands[band + 2];
}


