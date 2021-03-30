/*
* SpectrumMatrixLedEffect.cpp
*
*/

#include "SpectrumMatrixLedEffect.h"

const char* const SpectrumMatrixLedEffect::name = "SPECTRUM";

SpectrumMatrixLedEffect::SpectrumMatrixLedEffect(ILedMatrix* matrixConverter, uint16_t Hz, IAudioConverter* audioConverter)
	: ILedEffect(Hz), matrix(matrixConverter), audio(audioConverter)
{
    reset();
}

SpectrumMatrixLedEffect::~SpectrumMatrixLedEffect()
{
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

    float maxValue = 0;

    for (uint8_t column = 0; column < matrix->getWidth(); column++)
    {
        float columnLevel = audio->scale(column);

        // найти максимум из пачки тонов
        if (columnLevel > maxValue) maxValue = columnLevel;

        // преобразовать значение величины спектра в диапазон 0..HEIGHT
        columnLevel = map(columnLevel, audio->getLowGain(), audio->getHiGain(), 0, matrix->getHeight());
        columnLevel = constrain(columnLevel, 0, matrix->getHeight());

        for (int y = 0; y < columnLevel; y++)
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
