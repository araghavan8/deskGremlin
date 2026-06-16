#include <avr/io.h>
#include "i2c.h"

void twi_init(void)
{
    /*
    Set TWBR value
        SCL frequency = 100 KHz
        Clock frequency = 16 MHz
    */
    TWBR = 72;
}

uint8_t twi_start(void)
{
    /*
    Result variable to return
    */
    uint8_t result;

    /*
    Set TWCR value for START condition
        TWINT - 1 (kicks off next I2C op)
        TWSTA - 1 (enable START condition)
        TWEN - 1 (enable I2C bus)
    */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /*
    Wait for TWINT to go high again (I2C op has finished)
    */
    while (!(TWCR & (1 << TWINT)))
    {
    }

    /*
    Check TWSR to see if operation worked
    */
    uint8_t status = TWSR & 0xF8;
    if (status == 0x08)
        result = 0;
    else if (status == 0x10)
        result = 0;
    else
        result = 1;

    return result;
}

uint8_t twi_write(uint8_t data)
{
    /*
    Result variable to return
    */
    uint8_t result;

    /*
    Load TWDR with data
    */
    TWDR = data;

    /*
    Trigger SEND by writing to TWCR
        TWINT - 1 (kicks off next I2C op)
        TWEN - 1 (enable I2C bus)
    */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /*
    Wait for TWINT to go high again (I2C op has completed)
    */
    while (!(TWCR & (1 << TWINT)))
    {
    }

    /*
    Check TWSR to see if operation worked
    */
    uint8_t status = TWSR & 0xF8;
    if (status == 0x18)
        result = 0;
    else if (status == 0x28)
        result = 0;
    else
        result = 1;

    return result;
}

uint8_t twi_read_ack(void)
{
    /*
    Read with ACK by writing to TWCR
        TWINT - 1 (kicks off next I2C op)
        TWEA - 1 (enable ACK)
        TWEN - 1 (enable I2C bus)
    */
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);

    /*
    Wait for TWINT to go high (I2C op has completed)
    */
    while (!(TWCR & (1 << TWINT)))
    {
    }

    /*
    Read from TWDR
    */
    return TWDR;
}

uint8_t twi_read_nack(void)
{
    /*
    Read with NACK by writing to TWCR
        TWINT - 1 (kicks off next I2C op)
        TWEA - 0 (disable ACK, use NACK)
        TWEN - 1 (enable I2C bus)
    */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /*
    Wait for TWINT to go high (I2C op has completed)
    */
    while (!(TWCR & (1 << TWINT)))
    {
    }

    /*
    Read from TWDR
    */
    return TWDR;
}

void twi_stop(void)
{
    /*
    Set TWCR value for STOP condition
        TWINT - 1 (kicks off next I2C op)
        TWSTO - 1 (enables STOP condition)
        TWEN - 1 (enables I2C bus)
    */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
