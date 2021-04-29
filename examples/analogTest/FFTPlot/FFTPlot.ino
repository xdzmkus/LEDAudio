
#define MIC_PIN A0

#include "arduinoFFT.h"

#define SAMPLES         256     // Must be a power of 2
#define SAMPLING_FREQ   38400   // Hz, depends on ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleFrequency/2

double vReal[SAMPLES];
double vImag[SAMPLES];

arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

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

//  analogReference(EXTERNAL);
}

void loop()
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

    // Analyse FFT results
    for (uint16_t i = 0; i < (SAMPLES>>1); i++)
    {
        Serial.println(vReal[i], 0);
    }

    double f = 0;
    double v = 0;
    FFT.MajorPeak(&f, &v);

    Serial.print("peak: ");
    Serial.println(v);

    delay(500); /* Repeat after delay */
}
