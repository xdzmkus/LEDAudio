#if defined(ESP8266)
#define LED_PIN D5  // leds pin
#define MIC_PIN A0
#elif defined(ESP32)
#define LED_PIN 16 // leds pin
#define MIC_PIN 35
#else
#define LED_PIN 9   // leds pin
#define MIC_PIN A0
#endif

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 6000
#define BRIGHTNESS 20

#include "arduinoFFT.h"

#define SAMPLES         256     // Must be a power of 2
#define SAMPLING_FREQ   19200   // Hz, depends on ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleFrequency/2

double vReal[SAMPLES];
double vImag[SAMPLES];

arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromTopLeftToBottomAndRight.hpp>
#include "LEDAudioEffects.h"

Fix32BandConverter<double, uint8_t> audio(vReal + 2, SAMPLES / 2 - 2);
SpectrumMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> effect(32, &audio);

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

    setup_LED();

    effect.start();

    audio.setLowGain(200);
}

void loop()
{
    if (effect.isReady())
    {
        analyzeAudio();

        audio.removeNotSound();

        effect.paint();
        FastLED.show();
    }
}

void analyzeAudio()
{
    // Sample the audio pin
    for (uint16_t i = 0; i < SAMPLES; i++)
    {
        vReal[i] = static_cast<double>(analogRead(MIC_PIN));
        vImag[i] = 0.0;
    }

    // Compute FFT
    FFT.DCRemoval();
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();
}
