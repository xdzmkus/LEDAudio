#define LED_PIN 9
#define LEFT_PIN A0
#define RIGHT_PIN A1

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

#include <ZigZagFromBottomRightToUpAndLeft.h>
#include "SpectrumMatrixLedEffect.h"
#include "Fix32BandConverter.h"
#include "LinearParabolicBandConverter.h"

//Fix32BandConverter audio(fht_log_out, FHT_N / 2);
LinearParabolicBandConverter audio(fht_log_out + 2, FHT_N / 2 - 2, MATRIX_W);
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
//  Serial.begin(115200);

#ifdef ARDUINO_AVR_MEGA2560

    analogReference(INTERNAL1V1);

#else

    analogReference(INTERNAL);

#endif

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

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

//	audio.removeNoise();
    audio.removeNotSound();
//  audio.gain();
//  audio.normalize();
}
