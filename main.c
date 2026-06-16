#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "i2c.h"
#include "mpu6050.h"
#include "ds1307.h"

int main(void)
{
    DDRB |= (1 << PB5);

    uart_init();
    adc_init();
    timer_init();
    twi_init();
    mpu6050_init();
    ds1307_init();

    sei();
    ds1307_set_time(13, 14, 0);

    uart_puts("Desk Gremlin is alive.\r\n");

    uint16_t count = 0;

    while (1)
    {
        if (tick_flag)
        {
            tick_flag = 0;

            static uint8_t tick_count = 0;
            tick_count++;

            PORTB ^= (1 << PB5); /* Toggle LED */

            if (tick_count >= 20)
            {
                tick_count = 0;

                uart_puts("Blink #");
                uart_print_u16(count);

                count++;

                uart_puts("\r\n");

                uart_puts("Light: ");
                uint16_t photoval = adc_read();
                uart_print_u16(photoval);

                uart_puts("\r\n");

                int16_t x, y, z;
                mpu6050_read_accel(&x, &y, &z);
                uart_puts("X :");
                uart_print_i16(x);
                uart_puts("Y :");
                uart_print_i16(y);
                uart_puts("Z :");
                uart_print_i16(z);

                uart_puts("\r\n");

                uint8_t hours, minutes, seconds;
                ds1307_read_time(&hours, &minutes, &seconds);
                uart_puts("Hours: ");
                uart_print_u16(hours);
                uart_puts("Minutes: ");
                uart_print_u16(minutes);
                uart_puts("Seconds: ");
                uart_print_u16(seconds);

                uart_puts("\r\n");
                uart_puts("\r\n");
                uart_puts("\r\n");
            }
        }
    }

    return 0;
}