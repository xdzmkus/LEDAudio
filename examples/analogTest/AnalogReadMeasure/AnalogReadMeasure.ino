
#define ANALOG_IN A0

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup()
{
    // поднимаем частоту опроса аналогового порта до 38.4 к√ц, по теореме
    //  отельникова (Ќайквиста) максимальна€ частота дискретизации будет 19 к√ц
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

    analogReference(EXTERNAL);

    Serial.begin(115200);
}

void loop()
{
    long t;

    t = micros();
    for (int i = 0; i < 1000; i++)
    {
        analogRead(ANALOG_IN);
    }
    t = micros() - t;  // elapsed time

    Serial.print("Time per sample: ");
    Serial.print((float)t / 1000);
    Serial.println(" us");

    Serial.print("Sampling frequency: ");
    Serial.print((float)1000000 / t);
    Serial.println(" KHz");

    delay(2000);
}