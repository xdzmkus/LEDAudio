﻿#if defined(ESP8266)
#define LED_PIN D5  // leds pin
#define LEFT_PIN A0
#define RIGHT_PIN A0
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define LEFT_PIN 35
#define RIGHT_PIN 35
#else
#define LED_PIN 9   // leds pin
#define LEFT_PIN A0
#define RIGHT_PIN A1
#endif

bool whichChannel = false;

#define LOG_OUT 1   // use the log output function
#define FHT_N 256   // set to 256 point fht
#include <FHT.h>    // include the library

extern int fht_input[FHT_N];            // FHT input data buffer
extern uint8_t fht_log_out[FHT_N / 2];  // FHT log output magintude buffer

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 6000
#define BRIGHTNESS 120

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <MatrixLineConverters.h>
#include "LEDAudio.h"

void analyzeAudio(void);

ParabolicXBandConverter<uint8_t, uint8_t> audio(fht_log_out + 2, FHT_N / 2 - 2, MATRIX_W);
SpectrumMatrixLedEffect<ZigZagFromBottomRightToUpAndLeft, leds, MATRIX_W, MATRIX_H> effect(256, &audio, analyzeAudio);

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
#ifdef ADCSRA

    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

#endif // ADCSRA

#ifdef ARDUINO_AVR_MEGA2560

    analogReference(INTERNAL2V56);

#else

    analogReference(EXTERNAL);

#endif

    setup_LED();

    effect.start();
}

void loop()
{
    if (effect.isReady())
    {
        effect.paint();
        FastLED.show();
    }
}

void analyzeAudio()
{
    if (whichChannel)
    {
        for (int i = 0; i < FHT_N; i++)
        {
            fht_input[i] = analogRead(LEFT_PIN); // put real data into bins
        }
    }
    else
    {
        for (int i = 0; i < FHT_N; i++)
        {
            fht_input[i] = analogRead(RIGHT_PIN); // put real data into bins
        }
    }

    whichChannel = !whichChannel;

    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht

    audio.removeNotSound();
    audio.removeNoise();
    audio.gain();
}
