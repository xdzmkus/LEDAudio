#define LED_PIN 9
#define MIC_PIN A0

#define LOG_OUT 1   // use the log output function
#define FHT_N 256   // set to 256 point fht
#include <FHT.h>    // include the library

extern int fht_input[FHT_N];            // FHT input data buffer
extern uint8_t fht_log_out[FHT_N / 2];  // FHT log output magintude buffer

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 6000
#define BRIGHTNESS 20

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromBottomRightToUpAndLeft.h>
#include "SpectrumMatrixLedEffect.h"
#include "Fix32BandConverter.hpp"

Fix32BandConverter<uint8_t, uint8_t> audio(fht_log_out, (FHT_N / 2));

ZigZagFromBottomRightToUpAndLeft matrix(leds, MATRIX_W, MATRIX_H);
SpectrumMatrixLedEffect effect(&matrix, 256, &audio);

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

    effect.start();
}

void loop()
{
    if (effect.isReady())
    {
        analyzeAudio();
        effect.paint();
        FastLED.show();
    }
}

void analyzeAudio()
{
    for (int i = 0; i < FHT_N; i++)
    {
        fht_input[i] = analogRead(MIC_PIN); // put real data into bins
    }
    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht

    audio.removeNotSound();
    audio.removeNoise();
    audio.gain();
}
