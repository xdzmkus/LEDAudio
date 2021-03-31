/*
* SpectrumMatrixLedEffect.cpp
*
*/

#include "SpectrumMatrixLedEffect.h"

const char* const SpectrumMatrixLedEffect::name = "SPECTRUM";

SpectrumMatrixLedEffect::SpectrumMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, IAudioConverter* audioConverter)
	: ILedEffect(Hz), matrix(matrixConverter), audio(audioConverter)
{
//    bands = new BAND_LEVELS[matrix->getWidth()];

    reset();
}

SpectrumMatrixLedEffect::~SpectrumMatrixLedEffect()
{
//    if (bands != nullptr)
//    {
//        delete[] bands;
//    }
}

void SpectrumMatrixLedEffect::reset()
{
	ILedEffect::reset();

	matrix->clearAllLeds();
}

bool SpectrumMatrixLedEffect::paint()
{
	if (!isReady())
		return false;
    
    matrix->clearAllLeds();

    audio->filter();

    uint16_t maxValue = 0;

    for (uint8_t column = 0; column < matrix->getWidth(); column++)
    {
        bands[column].level = audio->scale(column) *0.3F + static_cast<float>(bands[column].level) * 0.7F;

        // найти максимум из пачки тонов
        if (bands[column].level > maxValue)
            maxValue = bands[column].level;

        // преобразовать значение величины спектра в диапазон 0..HEIGHT
        long columnLevel = map(static_cast<long>(bands[column].level), audio->getLowGain(), audio->getHiGain(), 0, matrix->getHeight());
        columnLevel = constrain(columnLevel, 0, matrix->getHeight());

        for (uint8_t y = 0; y < columnLevel; y++)
        {
            if (y <= 0.5F * matrix->getHeight())
            {
                matrix->getPixel(column, y) = CRGB::Green;
            }
            else if (y <= 0.8F * matrix->getHeight())
            {
                matrix->getPixel(column, y) = CRGB::Yellow;
            }
            else
            {
                matrix->getPixel(column, y) = CRGB::Red;
            }
        }
    }

    // установить верхний порог на уровень текущей громкости
    if (maxValue < audio->getLowGain())
    {
        audio->setHiGain(audio->HI_PASS);
    }
    else
    {
        audio->setHiGain(1.1F * maxValue);
    }

	return true;
}
