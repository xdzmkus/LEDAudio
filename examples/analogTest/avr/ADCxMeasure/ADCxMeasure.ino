
#define ANALOG_CHANNEL 0    // NOTE: do not use A0, A1 ... etc. Just use digit !!! 

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup_ADC(uint8_t analogChannel)
{
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html
    // https://sites.google.com/site/qeewiki/books/avr-guide/analog-input

    sbi(DIDR0, analogChannel);      // Turn off the digital input for Ax

    ADMUX = analogChannel;  // Use #analogChannel ADC and AREF as the reference
//  sbi(ADMUX, REFS0);      // Set reference voltage to AVcc
//  sbi(ADMUX, ADLAR);      // Left align ADC value to 8 bits from ADCH register
    /* ADC Multiplexer Selection Register
                7 bit   6 bit   5 bit   4 bit   3 bit   2 bit   1 bit   0 bit
        ADMUX   REFS1   REFS0   ADLAR   -       MUX3    MUX2    MUX1    MUX0

        MUX 3...0   Single Ended Input
            0000    ADC0
            0001    ADC1
            0010    ADC2
            0011    ADC3
            0100    ADC4
            0101    ADC5
            0110    ADC6
            0111    ADC7
            1000    Temp Sensor (ATmega168/328 only)
            1001    (reserved)
             -
            1101    (reserved)
            1110    1.1V (ATmega168/328) 1.30V (ATmega8)
            1111    0V (GND)

        REFS1   REFS0   Voltage Reference Selection
            0   0       AREF, Internal Vref turned off
            0   1       AVcc with external capacitor on AREF pin
            1   0       Reserved
            1   1       Internal 1.1V(ATmega168/328) or 2.56V(ATmega8)
    */

    /* ADC Data Register
        If ADLAR is LOW(0)
                7 bit   6 bit   5 bit   4 bit   3 bit   2 bit   1 bit   0 bit
        ADCH                                                    ADC9    ADC8
        ADCL    ADC7    ADC6    ADC5    ADC4    ADC3    ADC2    ADC1    ADC0

        If ADLAR is HIGH(1)
                7 bit   6 bit   5 bit   4 bit   3 bit   2 bit   1 bit   0 bit
        ADCH    ADC9    ADC8    ADC7    ADC6    ADC5    ADC4    ADC3    ADC2
        ADCL    ADC1    ADC0
    */

    ADCSRA = 0;
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
    /* ADC Controland Status Register A
                7 bit   6 bit   5 bit   4 bit   3 bit   2 bit   1 bit   0 bit
        ADCSRA  ADEN    ADSC    ADATE   ADIF    ADIE    ADPS2   ADPS1   ADPS0

        Prescale    ADPS2 ADPS1 ADPS0   Clock freq (MHz)    Sampling rate (KHz)
            2       0     0     1       8                   615
            4       0     1     0       4                   307
            8       0     1     1       2                   153
            16      1     0     0       1                   76.8
            32      1     0     1       0.5                 38.4
            64      1     1     0       0.25                19.2
            128     1     1     1       0.125               9.6
    */

    ADCSRB = 0; // Clear ADCSRB register, 0 for free running mode
    /* ADC Control and Status Register B
                7 bit   6 bit   5 bit   4 bit   3 bit   2 bit   1 bit   0 bit
        ADCSRB  -       ACME    -       -       -       ADTS2   ADTS1   ADTS0
    */

    sbi(ADCSRA, ADEN);  // Enable the ADC
}

void setup()
{
    setup_ADC(ANALOG_CHANNEL);

    Serial.begin(115200);
}

void loop()
{
    long t = micros();
    for (int i = 0; i < 1000; i++)
    {
        sbi(ADCSRA, ADSC);              // Start the ADC conversion
        while (ADCSRA & (1 << ADSC));   // Wait for the ADC to finish 

        int ADCvalue = ADCL;
        ADCvalue = (ADCH << 8) + ADCvalue;    // ADCH is read so ADC can be updated again
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
