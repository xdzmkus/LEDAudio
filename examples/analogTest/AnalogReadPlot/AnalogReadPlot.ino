
#define ANALOG_IN A0

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup()
{
    // ��������� ������� ������ ����������� ����� �� 38.4 ���, �� �������
    // ������������ (���������) ������������ ������� ������������� ����� 19 ���
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

//    analogReference(EXTERNAL);

    Serial.begin(115200);
}

void loop()
{
    Serial.println(analogRead(ANALOG_IN));
}
