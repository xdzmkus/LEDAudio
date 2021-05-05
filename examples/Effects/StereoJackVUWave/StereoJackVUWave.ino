#define LED_PIN 9

#define LEFT_PIN A0
#define RIGHT_PIN A1

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 6000
#define BRIGHTNESS 20

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromBottomRightToUpAndLeft.h>
#include "VUWaveMatrixLedEffect.h"

ZigZagFromBottomRightToUpAndLeft matrix(leds, MATRIX_W, MATRIX_H);
VUWaveMatrixLedEffect effect(&matrix, 30);

bool whichChannel = false;

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

    analogReference(INTERNAL2V56);

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
        uint16_t r = 0;
        uint16_t l = 0;
        analyzeAudio(r, l);
        effect.paint(r, l);
        FastLED.show();
    }
}

void analyzeAudio(uint16_t& r, uint16_t& l)
{
    r = l = 0;
    uint16_t rMin = -1;
    uint16_t lMin = -1;

    bool rightChannel = true;

    for (uint16_t i = 0; i < 256; i++)
    {
        uint16_t value = analogRead(rightChannel ? RIGHT_PIN : LEFT_PIN);

        if (rightChannel)
        {
            if (value > r) r = value;
            if (value < rMin) rMin = value;
        }
        else
        {
            if (value > l) l = value;
            if (value < lMin) lMin = value;
        }
        rightChannel = !rightChannel;
    }

    r -= rMin;
    l -= lMin;
}
