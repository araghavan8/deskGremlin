#ifndef DS1307_H
#define DS1307_H

void ds1307_init(void);
void ds1307_read_time(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
void ds1307_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif