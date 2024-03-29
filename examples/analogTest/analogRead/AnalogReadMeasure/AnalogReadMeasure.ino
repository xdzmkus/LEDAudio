
#define ANALOG_IN 35

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup()
{
#ifdef ADCSRA

    // ��������� ������� ������ ����������� ����� �� 38.4 ���, �� �������
    // ������������ (���������) ������������ ������� ������������� ����� 19 ���
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

#endif // ADCSRA

//    analogReference(EXTERNAL);

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