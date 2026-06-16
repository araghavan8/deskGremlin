#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

volatile uint8_t tick_flag = 0;

ISR(TIMER1_COMPA_vect)
{
    tick_flag = 1;
}

void timer_init(void)
{
    /* Set TCCR1B value
        ICNC1 - Disabled (Input Capture Noise Canceler)
        ICES1 - Disabled (Input Capture Edge Select)
        WGM - CTC using OCRA1
        CS - prescaler set to 256, interval is 100ms
    */
    TCCR1B = (1 << WGM12) | (1 << CS12);

    /* Set OCRA1 value
        Value set to 6249
    */
    OCR1A = 6249;

    /* Set TIMSKI1 value
        OCIE1A = 1
    */
    TIMSK1 = (1 << OCIE1A);
}
