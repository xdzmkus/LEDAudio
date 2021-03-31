#define LED_PIN 9
#define AUDIO_IN A5

#define LOG_OUT 1   // use the log output function
#define FHT_N 256   // set to 256 point fht
#include <FHT.h>    // include the library

extern int fht_input[FHT_N];            // FHT input data buffer
extern uint8_t fht_log_out[FHT_N / 2];    // FHT log output magintude buffer

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 6000
#define BRIGHTNESS 20

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromBottomRightToUpAndLeft.h>
#include "SpectrumMatrixLedEffect.h"
#include "Fix32BandConverter.h"
#include "ParabolicBandConverter.h"

Fix32BandConverter audio(fht_log_out, FHT_N / 2);
//ParabolicBandConverter audio(fht_log_out, FHT_N / 2, 32);

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
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

    analogReference(INTERNAL);

//    Serial.begin(115200);

    setup_LED();

    effect.start();
}

void loop()
{
    analyzeAudio();

    if (effect.paint())
    {
        FastLED.show();
//        Serial.println(audio.getHiGain());
    }
}

void analyzeAudio()
{
    for (int i = 0; i < FHT_N; i++)
    {
        fht_input[i] = analogRead(AUDIO_IN); // put real data into bins
    }
    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht
}
