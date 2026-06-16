#include <avr/io.h>
#include "uart.h"

#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

void uart_init(void)
{
    /* Set baud rate in the USART Baud Rate Register */
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)(UBRR_VAL);

    /* Enable transmitter (bit TXEN0 in UCSR0B) */
    UCSR0B = (1 << TXEN0);

    /* Frame format: 8 data bits, 1 stop bit, no parity */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putc(char c)
{
    /* Wait until the transmit buffer is empty */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Write the byte to the data register */
    UDR0 = c;
}

void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putc(*s);
        s++;
    }
}

void uart_print_u16(uint16_t val)
{
    /* Covert a number to a string */
    char buf[6];
    uint16_t n = val;
    int i = 0;

    if (n == 0)
    {
        buf[i++] = '0';
    }
    else
    {
        char tmp[6];
        int j = 0;
        while (n > 0)
        {
            tmp[j++] = '0' + (n % 10);
            n /= 10;
        }
        while (j > 0)
        {
            buf[i++] = tmp[--j];
        }
    }
    buf[i] = '\0';
    uart_puts(buf);
    uart_puts("\r\n");
}

void uart_print_i16(int16_t n)
{
    if (n < 0)
    {
        uart_putc('-');
        n = -n;
    }
    uart_print_u16((uint16_t)n);
}