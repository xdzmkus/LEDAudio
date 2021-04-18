/*
* Fix32BandConverter.cpp
*
*/

#include "Fix32BandConverter.h"

const uint8_t Fix32BandConverter::bands[34] PROGMEM = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 32, 36, 40, 44, 48, 52, 57, 62, 67, 72, 78, 84, 90, 96, 102, 108, 120, 127 };

Fix32BandConverter::Fix32BandConverter(uint8_t spectrum[], uint8_t samples) : IAudioConverter(spectrum, samples)
{
}

Fix32BandConverter::~Fix32BandConverter()
{
}

float Fix32BandConverter::scale(uint8_t band)
{
    // stupid guardian
    if (band > 31)
        return 0.0F;

    return IAudioConverter::scale(band);
}

IAudioConverter::BANDWIDTH Fix32BandConverter::calcBandWidth(uint8_t band)
{
    BANDWIDTH bandWidth = {};

    bandWidth.idxPrev = pgm_read_byte(&(bands[band]));
    bandWidth.idxCurr = pgm_read_byte(&(bands[band + 1]));
    bandWidth.idxNext = pgm_read_byte(&(bands[band + 2]));

    return bandWidth;
}


