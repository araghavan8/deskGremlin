#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "lcd.h"

#define BUZZER_DDR  DDRB
#define BUZZER_PORT PORTB
#define BUZZER_PIN  PB4
#define BUZZER_ON()  BUZZER_PORT |= (1 << BUZZER_PIN)
#define BUZZER_OFF() BUZZER_PORT &= ~(1 << BUZZER_PIN)

#define RGB_DDR   DDRB
#define RGB_R_PIN PB3
#define RGB_G_PIN PB2
#define RGB_B_PIN PB1

static void beep(uint16_t ms)
{
    BUZZER_ON();
    for (uint16_t i = 0; i < ms; i++)
        _delay_ms(1);
    BUZZER_OFF();
}

static void rgb_set(uint8_t r, uint8_t g, uint8_t b)
{
    if (r) PORTB |= (1 << RGB_R_PIN); else PORTB &= ~(1 << RGB_R_PIN);
    if (g) PORTB |= (1 << RGB_G_PIN); else PORTB &= ~(1 << RGB_G_PIN);
    if (b) PORTB |= (1 << RGB_B_PIN); else PORTB &= ~(1 << RGB_B_PIN);
}

int main(void)
{
    DDRB |= (1 << PB5);

    uart_init();
    adc_init();
    timer_init();
    lcd_init();

    BUZZER_DDR |= (1 << BUZZER_PIN);
    BUZZER_PORT &= ~(1 << BUZZER_PIN);

    RGB_DDR |= (1 << RGB_R_PIN) | (1 << RGB_G_PIN) | (1 << RGB_B_PIN);

    sei();

    // Boot sequence
    rgb_set(0, 0, 1);           // blue while waking up
    lcd_print("  Waking up...");
    _delay_ms(1500);
    lcd_clear();
    
    rgb_set(1, 0, 0);
    beep(100);
    beep(100);
    beep(100);
    
    lcd_print(" Desk Gremlin");
    lcd_set_cursor(0, 1);
    lcd_print("  is alive! :)");
    beep(80);
    _delay_ms(1000);


    while (1)
    {
        if (tick_flag)
        {
            tick_flag = 0;

            PORTB ^= (1 << PB5);

            static uint8_t tick_count = 0;
            tick_count++;

            if (tick_count >= 10)
            {
                tick_count = 0;

                uint16_t light = adc_read();

                lcd_clear();
                lcd_set_cursor(0, 0);

                if (light > 600) {
                    // Dark — red
                    rgb_set(0, 1, 0);
                    lcd_print("  (x_x)  HELLO?");
                    lcd_set_cursor(0, 1);
                    lcd_print("It's so dark..");
                    beep(300);

                } else if (light < 150) {
                    // Very bright — white
                    rgb_set(1, 1, 1);
                    lcd_print("  (O_O)  OUCH!!");
                    lcd_set_cursor(0, 1);
                    lcd_print("Too bright!!");
                    beep(300);

                } else {
                    // Normal — green
                    rgb_set(1, 0, 0);
                    lcd_print("  (^_^)  Hi!");
                    lcd_set_cursor(0, 1);
                    lcd_print("There you are!!");
                }

                uart_puts("Light: ");
                uart_print_u16(light);
                uart_puts("\r\n");
            }
        }
    }

    return 0;
}