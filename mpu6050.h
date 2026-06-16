#ifndef MPU6050_H
#define MPU6050_H

void mpu6050_init(void);
void mpu6050_read_accel(int16_t *x, int16_t *y, int16_t *z);

#endif