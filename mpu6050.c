#include <avr/io.h>
#include "mpu6050.h"
#include "i2c.h"

void mpu6050_init(void)
{
    /*
    Init function for the IMU - Write 0 to register 0x6B to wake IMU from sleep
    */

    /*
    Start I2C bus
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x69
        R/W mode - 0 (W)
        Byte to write - 0x68 << 1 | 0 = 0xD2
    */
    twi_write(0xD2);

    /*
    Send register address for PWR_MGMT_1 register
    */
    twi_write(0x6B);

    /*
    Send data 0x00 to wake up device
    */
    twi_write(0x00);

    /*
    Stop I2C transactions
    */
    twi_stop();
}

void mpu6050_read_accel(int16_t *x, int16_t *y, int16_t *z)
{
    /*
    Read acceleration values from IMU to detect desk slams
    */

    /*
    Start I2C bus
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x69
        R/W mode - 0 (W)
        Byte to write - 0x69 << 1 | 0 = 0xD2
    */
    twi_write(0xD2);

    /*
    Send register address for first accel register
    */
    twi_write(0x3B);

    /*
    Repeated START to switch from W to R
    */
    twi_start();

    /*
    Point bus to right device and specify R/W mode
        Device address - 0x69
        R/W mode - 1 (R)
        Byte to write - 0x69 << 1 | 1 = 0xD3
    */
    twi_write(0xD3);

    /*
    Read acceleration registers
        0x3B - X high byte
        0x3C - X low byte
        0x3D - Y high byte
        0x3E - Y low byte
        0x3F - Z high byte
        0x40 - Z low byte
    */
    uint8_t x_high = twi_read_ack(); // 0x3B
    uint8_t x_low = twi_read_ack();  // 0x3C
    uint8_t y_high = twi_read_ack(); // 0x3D
    uint8_t y_low = twi_read_ack();  // 0x3E
    uint8_t z_high = twi_read_ack(); // 0x3F
    uint8_t z_low = twi_read_nack(); // 0x40

    /*
    Stop I2C transactions
    */
    twi_stop();

    /*
    Generate 16-bit values for X,Y,Z coords
    */
    *x = (int16_t)(x_high << 8 | x_low);
    *y = (int16_t)(y_high << 8 | y_low);
    *z = (int16_t)(z_high << 8 | z_low);
}