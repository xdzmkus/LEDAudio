
#define ANALOG_IN A0

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup()
{
#ifdef ADCSRA

    // поднимаем частоту опроса аналогового порта до 38.4 к√ц, по теореме
    //  отельникова (Ќайквиста) максимальна€ частота дискретизации будет 19 к√ц
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
    Serial.println(analogRead(ANALOG_IN));
}
