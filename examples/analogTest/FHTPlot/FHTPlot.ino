#define LOG_OUT 1   // use the log output function
#define FHT_N 256   // set to 256 point fht
#include <FHT.h>

extern int fht_input[FHT_N];            // FHT input data buffer
extern uint8_t fht_log_out[FHT_N / 2];  // FHT log output magintude buffer

#define MIC_PIN A0

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

    unsigned long nowTime = millis();

    for (int i = 0; i < FHT_N; i++)
    {
        fht_input[i] = analogRead(MIC_PIN); // put real data into bins
    }

    unsigned long readTime = millis() - nowTime;

    Serial.print(F("Analog read time: ")); Serial.print(readTime); Serial.println(F(" ms"));

    nowTime = millis();

    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht

    unsigned long fftTime = millis() - nowTime;

    Serial.print(F("FFT compute time: ")); Serial.print(fftTime); Serial.println(F(" ms"));

}

void loop()
{
    for (int i = 0; i < FHT_N; i++)
    {
        fht_input[i] = analogRead(MIC_PIN); // put real data into bins
    }
    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht

    for (int curBin = 2; curBin < FHT_N / 2; ++curBin)
    {
        Serial.println(fht_log_out[curBin]);
    }

    delay(500);
}