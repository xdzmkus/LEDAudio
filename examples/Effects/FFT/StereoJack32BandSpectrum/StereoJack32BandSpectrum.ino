#define LED_PIN 9
#define LEFT_PIN A0
#define RIGHT_PIN A1

bool whichChannel = false;

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 6000
#define BRIGHTNESS 120

#include "arduinoFFT.h"

#define SAMPLES         512     // Must be a power of 2
#define SAMPLING_FREQ   38400   // Hz, depends on ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleFrequency/2

double vReal[SAMPLES];
double vImag[SAMPLES];

arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromBottomRightToUpAndLeft.h>
#include "SpectrumMatrixLedEffect.h"
#include "ParabolicXBandConverter.hpp"

ParabolicXBandConverter<double, uint8_t>  audio(vReal + 2, SAMPLES / 2 - 2, MATRIX_W);
ZigZagFromBottomRightToUpAndLeft matrix(leds, MATRIX_W, MATRIX_H);
SpectrumMatrixLedEffect* effect;

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W));
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
}

void setup()
{
    Serial.begin(115200);

#ifdef ADCSRA

    // поднимаем частоту опроса аналогового порта до 38.4 к√ц, по теореме
    //  отельникова (Ќайквиста) максимальна€ частота дискретизации будет 19 к√ц
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

#endif // ADCSRA

#ifdef ARDUINO_AVR_MEGA2560

    analogReference(INTERNAL1V1);

#else

    analogReference(INTERNAL);

#endif

    setup_LED();

    effect = new SpectrumMatrixLedEffect(&matrix, 256, &audio);

    if (effect == NULL)
        abort();

    effect->start();
}

void loop()
{
    if (effect->isReady())
    {
        analyzeAudio();
        effect->paint();
        FastLED.show();
    }
}

void analyzeAudio()
{
    if (whichChannel)
    {
        // Sample the audio pin
        for (uint16_t i = 0; i < SAMPLES; i++)
        {
            vReal[i] = static_cast<double>(analogRead(LEFT_PIN));
            vImag[i] = 0.0;
        }
    }
    else
    {
        // Sample the audio pin
        for (uint16_t i = 0; i < SAMPLES; i++)
        {
            vReal[i] = static_cast<double>(analogRead(RIGHT_PIN));
            vImag[i] = 0.0;
        }
    }

    whichChannel = !whichChannel;

    // Compute FFT
//    FFT.DCRemoval();
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();

    audio.removeNotSound();
//    audio.removeNoise();
//    audio.gain();
}
