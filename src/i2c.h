#ifndef MULTIWII_I2C_H
#define MULTIWII_I2C_H

#include <stdint.h>

extern uint32_t i2c_errors_count;

void i2c_init();

void i2c_start(uint8_t address);

void i2c_rep_start(uint8_t address);

void i2c_stop();

void i2c_write(uint8_t twdr);

uint8_t i2c_read_ack();

uint8_t i2c_read_nak();

void i2c_read_reg_to_buf(uint8_t address, uint8_t reg, uint8_t *buf, uint8_t size);

void i2c_write_reg(uint8_t address, uint8_t reg, uint8_t val);

uint8_t i2c_read_reg(uint8_t address, uint8_t reg);

void i2c_read_reg_to_buf(uint8_t address, uint8_t reg, uint8_t *buf, uint8_t size);

#endif //MULTIWII_I2C_H
