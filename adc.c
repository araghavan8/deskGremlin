#include <avr/io.h>
#include "adc.h"

void adc_init(void)
{
    /* Set ADMUX value
        Voltage Reference - AVcc
        ADCLAR - No
        Analog Channel - ADC0
    */
    ADMUX = 1 << REFS0;

    /* Set ADCSRA value
        ADCEN - Yes
        ADCSC - No (set to 1 in read method)
        ADCATE - No
        ADIF - No (don't care value)
        ADIE - No
        ADCPS - 128
    */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read()
{
    /* Set ADCSC to 1 to start conversion */
    ADCSRA |= (1 << ADSC);

    while (!(ADCSRA & (1 << ADSC)))
        ;

    uint8_t low = ADCL;
    uint8_t high = ADCH;

    uint16_t result = ((uint16_t)high << 8) | (uint16_t)low;
    return result;
}