/*
* SpectrumMatrixLedEffect.cpp
*
*/

#include "SpectrumMatrixLedEffect.h"

#define LOW_PASS 30       // нижний порог чувствительности шумов (нет скачков при отсутствии звука)
#define INPUT_GAIN 1.5F   // коэффициент усиления входного сигнала
#define NORMALIZE 0       // нормализовать пики (столбики низких и высоких частот будут одинаковой длины при одинаковой громкости) (1 вкл, 0 выкл)
#define MAX_COEF 1.1F     // коэффициент, который делает "максимальные" пики чуть меньше максимума, для более приятного восприятия


const char* const SpectrumMatrixLedEffect::name = "SPECTRUM";

SpectrumMatrixLedEffect::SpectrumMatrixLedEffect(ILedMatrix* converter, uint16_t Hz, uint8_t spectrum[], uint8_t width)
	: ILedEffect(Hz), matrix(converter), spectrum(spectrum), width(width)
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

    for (uint8_t i = 0; i < width; i++)
    {
        // фильтруем весь спектр по минимальному LOW_PASS
        if (spectrum[i] < LOW_PASS)
            spectrum[i] = 0;

        // усиливаем сигнал
        spectrum[i] = INPUT_GAIN * spectrum[i];

        // уменьшаем громкость высоких частот (пропорционально частоте) если включено
        if (NORMALIZE)
            spectrum[i] = static_cast<float>(spectrum[i]) / (1.0F + static_cast<float>(i) / width);
    }

    float maxValue = 0;

    for (uint8_t column = 1; column < matrix->getWidth() - 1; column++)
    {
        uint8_t idxPrev = (width - 1) * pow(static_cast<float>(column-1) / (matrix->getWidth() - 1), 2);
        uint8_t idxCurr = (width - 1) * pow(static_cast<float>(column)   / (matrix->getWidth() - 1), 2);
        uint8_t idxNext = (width - 1) * pow(static_cast<float>(column+1) / (matrix->getWidth() - 1), 2);

        float columnLevel = spectrum[idxCurr];

        uint8_t delta = idxCurr - idxPrev;
        // от предыдущей полосы до текущей
        for (uint8_t i = 0; i < delta; i++)
        {
            columnLevel += static_cast<float>(i) / delta * spectrum[idxPrev + i];
        }
        delta = idxNext - idxCurr;
        // от текущей полосы до следующей
        for (uint8_t i = 0; i < delta; i++)
        {
            columnLevel += static_cast<float>(i) / delta * spectrum[idxNext - i];
        }

        // найти максимум из пачки тонов
        if (columnLevel > maxValue) maxValue = columnLevel;

        // преобразовать значение величины спектра в диапазон 0..HEIGHT с учётом настроек
        columnLevel = map(columnLevel, LOW_PASS, gain, 0, matrix->getHeight());
        columnLevel = constrain(columnLevel, 0, matrix->getHeight());

        for (int y = 0; y < columnLevel; y++)
        {
            if (y <= matrix->getHeight() / 2)
                matrix->getPixel(column, y) = CRGB::Green;
            else if (y <= 0.7F * matrix->getHeight())
                matrix->getPixel(column, y) = CRGB::Yellow;
            else if (y <= 0.85F * matrix->getHeight())
                matrix->getPixel(column, y) = CRGB::Orange;
            else
                matrix->getPixel(column, y) = CRGB::Red;
        }
    }

    maxValue_f = maxValue * k + maxValue_f * (1 - k);

    // если максимальное значение больше порога, взять его как максимум для отображения
    if (maxValue_f > LOW_PASS)
        gain = MAX_COEF * maxValue_f;
    // если нет, то взять порог побольше, чтобы шумы вообще не проходили
    else
        gain = 100;

	return true;
}
