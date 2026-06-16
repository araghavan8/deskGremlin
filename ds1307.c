#include <avr/io.h>
#include "i2c.h"
#include "ds1307.h"

void ds1307_init(void)
{
    /*
    Init function for the RTC - Write 0 to Clock Halt
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x68
        R/W mode - 0 (W)
        Byte to write - 0x68 << 1 | 0 = 0xD0
    */
    twi_write(0xD0);

    /*
    Send register address for seconds register
    */
    twi_write(0x00);

    /*
    Repeated START to switch from W to R
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x68
        R/W mode - 1 (R)
        Byte to write - 0x68 << 1 | 1 = 0xD1
    */
    twi_write(0xD1);

    /*
    Read seconds register
    */
    uint8_t seconds = twi_read_nack();

    /*
    Stop I2C transaction
    */
    twi_stop();

    /*
    Clear bit 7 to disable Clock Halt
    */
    seconds &= ~(1 << 7);

    /*
    START to switch from R to W
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x68
        R/W mode - 0 (W)
        Byte to write - 0x68 << 1 | 0 = 0xD0
    */
    twi_write(0xD0);

    /*
    Send register address for seconds register
    */
    twi_write(0x00);

    /*
    Write modified seconds back to register to clear Clock Halt
    */
    twi_write(seconds);

    /*
    Stop I2C transaction
    */
    twi_stop();
}

void ds1307_read_time(uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
{
    /*
    Start I2C bus
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x68
        R/W mode - 0 (W)
        Byte to write - 0x68 << 1 | 0 = 0xD0
    */
    twi_write(0xD0);

    /*
    Send the address of first register (seconds)
    */
    twi_write(0x00);

    /*
    Repeated START to switch to R
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x68
        R/W mode - 1 (R)
        Byte to write - 0x68 << 1 | 1 = 0xD1
    */
    twi_write(0xD1);

    /*
    Read registers to get time values
        0x00 - Seconds
        0x01 - Minutes
        0x02 - Hours
    */
    uint8_t seconds_bcd = twi_read_ack();
    uint8_t minutes_bcd = twi_read_ack();
    uint8_t hours_bcd = twi_read_nack();

    /*
    Stop I2C transaction
    */
    twi_stop();

    /*
    Convert time from BCD to normal integers
    */
    seconds_bcd &= 0x7F;
    *seconds = (seconds_bcd >> 4) * 10 + (seconds_bcd & 0x0F);
    *minutes = (minutes_bcd >> 4) * 10 + (minutes_bcd & 0x0F);
    *hours = (hours_bcd >> 4) * 10 + (hours_bcd & 0x0F);
}

void ds1307_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    /*
    Convert normal integers to BCD
    */
    uint8_t sec_bcd = ((seconds / 10) << 4) | (seconds % 10);
    uint8_t min_bcd = ((minutes / 10) << 4) | (minutes % 10);
    uint8_t hr_bcd = ((hours / 10) << 4) | (hours % 10);

    /*
    Start I2C transaction
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x68
        R/W mode - 0 (W)
        Byte to write - 0x68 << 1 | 0 = 0xD0
    */
    twi_write(0xD0);

    /*
    Send the address of first register (seconds)
    */
    twi_write(0x00);

    /*
    Hardcode current time values into registers
    */
    twi_write(sec_bcd);
    twi_write(min_bcd);
    twi_write(hr_bcd);

    /*
    Stop the I2C transaction
    */
    twi_stop();
}