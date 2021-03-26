/*
* SpectrumMatrixLedEffect.cpp
*
*/

#include "SpectrumMatrixLedEffect.h"

#define LOW_PASS 30       // ������ ����� ���������������� ����� (��� ������� ��� ���������� �����)
#define INPUT_GAIN 1.5F   // ����������� �������� �������� �������
#define NORMALIZE 0       // ������������� ���� (�������� ������ � ������� ������ ����� ���������� ����� ��� ���������� ���������) (1 ���, 0 ����)
#define MAX_COEF 1.1F     // �����������, ������� ������ "������������" ���� ���� ������ ���������, ��� ����� ��������� ����������


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
        // ��������� ���� ������ �� ������������ LOW_PASS
        if (spectrum[i] < LOW_PASS)
            spectrum[i] = 0;

        // ��������� ������
        spectrum[i] = INPUT_GAIN * spectrum[i];

        // ��������� ��������� ������� ������ (��������������� �������) ���� ��������
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
        // �� ���������� ������ �� �������
        for (uint8_t i = 0; i < delta; i++)
        {
            columnLevel += static_cast<float>(i) / delta * spectrum[idxPrev + i];
        }
        delta = idxNext - idxCurr;
        // �� ������� ������ �� ���������
        for (uint8_t i = 0; i < delta; i++)
        {
            columnLevel += static_cast<float>(i) / delta * spectrum[idxNext - i];
        }

        // ����� �������� �� ����� �����
        if (columnLevel > maxValue) maxValue = columnLevel;

        // ������������� �������� �������� ������� � �������� 0..HEIGHT � ������ ��������
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

    // ���� ������������ �������� ������ ������, ����� ��� ��� �������� ��� �����������
    if (maxValue_f > LOW_PASS)
        gain = MAX_COEF * maxValue_f;
    // ���� ���, �� ����� ����� ��������, ����� ���� ������ �� ���������
    else
        gain = 100;

	return true;
}
