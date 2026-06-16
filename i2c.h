#ifndef I2C_H
#define I2C_H

void twi_init(void);
uint8_t twi_start(void);
uint8_t twi_write(uint8_t data);
uint8_t twi_read_ack(void);
uint8_t twi_read_nack(void);
void twi_stop(void);

#endif