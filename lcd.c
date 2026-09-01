#include "lcd.h"
#include <util/delay.h>

/* Pin Definitions */
#define LCD_RS_PORT PORTD
#define LCD_RS_DDR DDRD 
#define LCD_RS_PIN PD7

#define LCD_EN_PORT PORTD
#define LCD_EN_DDR DDRD
#define LCD_EN_PIN PD6

#define LCD_D4_PORT PORTD
#define LCD_D4_DDR DDRD
#define LCD_D4_PIN PD5

#define LCD_D5_PORT PORTD
#define LCD_D5_DDR DDRD
#define LCD_D5_PIN PD4

#define LCD_D6_PORT PORTD
#define LCD_D6_DDR DDRD
#define LCD_D6_PIN PD3

#define LCD_D7_PORT PORTD
#define LCD_D7_DDR DDRD
#define LCD_D7_PIN PD2

/* Method definitions */

/*
    Write a nibble to the LCD register for a 4-bit interface
*/
static void lcd_send_nibble(uint8_t nibble)
{
    // Bit 0 -> D4
    if (nibble & (1 << 0))
        LCD_D4_PORT |= (1 << LCD_D4_PIN);
    else
        LCD_D4_PORT &= ~(1 << LCD_D4_PIN);

    // Bit 1 -> D5
    if (nibble & (1 << 1))
        LCD_D5_PORT |= (1 << LCD_D5_PIN);
    else
        LCD_D5_PORT &= ~(1 << LCD_D5_PIN);

    // Bit 2 -> D6
    if (nibble & (1 << 2))
        LCD_D6_PORT |= (1 << LCD_D6_PIN);
    else
        LCD_D6_PORT &= ~(1 << LCD_D6_PIN);

    // Bit 3 -> D7
    if (nibble & (1 << 3))
        LCD_D7_PORT |= (1 << LCD_D7_PIN);
    else
        LCD_D7_PORT &= ~(1 << LCD_D7_PIN);

    // Pulse EN to latch this nibble
    LCD_EN_PORT |= (1 << LCD_EN_PIN);
    _delay_us(1);
    LCD_EN_PORT &= ~(1 << LCD_EN_PIN);
    _delay_us(50);
};

/*
    Write a Byte to the LCD register
*/
static void lcd_send_byte(uint8_t byte, uint8_t rs)
{
    // Set RS high or low based on rs param
    if (rs)
        LCD_RS_PORT |= (1 << LCD_RS_PIN);
    else
        LCD_RS_PORT &= ~(1 << LCD_RS_PIN);

    // Set byte in two nibbles
    lcd_send_nibble(byte >> 4);         // high nibble
    lcd_send_nibble(byte & 0x0F);       // low nibble
};

/*
    Sends a command byte - RS = 0
*/
static void lcd_command(uint8_t cmd)
{
    lcd_send_byte(cmd, 0);
}

/*
    Sends a character byte - RS = 1
*/
static void lcd_write(uint8_t ch)
{
    lcd_send_byte(ch, 1);
}

/*
    Initialization sequence for LCD
*/
void lcd_init(void)
{
    // Set all LCD pins as output
    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    LCD_EN_DDR |= (1 << LCD_EN_PIN);
    LCD_D4_DDR |= (1 << LCD_D4_PIN);
    LCD_D5_DDR |= (1 << LCD_D5_PIN);
    LCD_D6_DDR |= (1 << LCD_D6_PIN);
    LCD_D7_DDR |= (1 << LCD_D7_PIN);

    // Wait >40ms after power up
    _delay_ms(50);

    // Send nibble 0x03 three times
    lcd_send_nibble(0x03);
    _delay_ms(5);
    lcd_send_nibble(0x03);
    _delay_us(110);
    lcd_send_nibble(0x03);
    _delay_us(110);

    // Send nibble 0x02 to switch to 4-bit mode
    lcd_send_nibble(0x02);

    // Full commands as bytes
    lcd_command(0x28);      // function set: 2 lines, 5x8, 4-bit
    lcd_command(0x0C);      // display on, cursor off, blink off
    lcd_command(0x01);      // clear display
    _delay_ms(2);           // clear needs >1.5ms
    lcd_command(0x06);      // entry mode: cursor right, no shift
}

/*
    Clear the LCD screen
*/
void lcd_clear(void)
{
    lcd_command(0x01);
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t address = (row == 0) ? col : (0x40 + col);
    lcd_command(0x80 | address);
}

void lcd_putc(char c)
{
    lcd_write(c);
}

void lcd_print(const char *str)
{
    for (int i=0; str[i] != '\0'; i++)
        lcd_write(str[i]);
}