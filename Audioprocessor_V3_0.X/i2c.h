#ifndef _I2C_H
#define	_I2C_H

#include <stdint.h>

#define I2C_CLK_SPEED 400000

void i2c_init(uint32_t fsys);
int i2c_write(uint8_t *data, uint16_t DataSz);
void i2c_read(uint8_t address, uint16_t registerAdress, uint8_t *buffer, uint8_t size);


#endif	/* _I2C_H */

