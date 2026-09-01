#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_print_u16(uint16_t val);
void uart_print_i16(int16_t n);
void uart_print_hex8(uint8_t val);

#endif