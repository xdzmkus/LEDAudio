
#define ANALOG_CHANNEL 0    // NOTE: do not use A0, A1 ... etc. Just use digit !!! 

void setup()
{
    Serial.begin(115200);                   // use the serial port
}

void loop()
{
    int ADCvalue = ADCsingleREAD(ANALOG_CHANNEL);

    Serial.println(ADCvalue);
}

int ADCsingleREAD(uint8_t analogChannel)
{
    ADMUX = analogChannel;                  // use #analogChannel ADC and AREF as the reference
//  ADMUX |= (1 << REFS0);                  // use AVcc as the reference
    ADMUX &= ~(1 << ADLAR);                 // clear for 10 bit resolution

    ADCSRA |= (1 << ADPS2) | (1 << ADPS0);  // 32 prescaler for 38.5 KHz
//  ADCSRA |= (1 << ADPS2);                 // 16 prescaler for 76.9 KHz
//  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);  // 8 prescaler for 153.8 KHz

    ADCSRA |= (1 << ADEN);                  // Enable the ADC
    ADCSRA |= (1 << ADSC);                  // Start the ADC conversion

    while (ADCSRA & (1 << ADSC));           // Wait for the ADC to finish 

    int ADCval = ADCL;
    ADCval = (ADCH << 8) + ADCval;          // ADCH is read so ADC can be updated again

    return ADCval;
}


