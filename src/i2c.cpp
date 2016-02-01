#include <avr/io.h>
#include "i2c.h"

uint32_t i2c_errors_count;

/**
 * Set I2C baud rate
 */
void inline i2c_baud_rate(uint32_t baudRate) {
    TWBR = (uint8_t) ((F_CPU / baudRate) - 16) / 2;
}

/**
 * Send TWCR transmission and wait while transmission ends
 */
void __attribute__ ((noinline)) i2c_transmission(uint8_t twcr) {
    TWCR = twcr;
    uint8_t count = 255;
    // Wait until transmission ends
    while (!(TWCR & (1 << TWINT))) {
        count--;
        if (count == 0) {
            // Force reset TWINT register
            TWCR = 0;
            i2c_errors_count++;

            break;
        }
    }
}

/**
 * Init I2C settings
 */
void i2c_init() {
    TWSR = 0;
    i2c_baud_rate(400000);
    TWCR = (1 << TWEN);
    i2c_errors_count = 0;
}

/**
 * Send start bit to I2C
 */
void i2c_start(uint8_t address) {
    // Send start signal
    i2c_transmission((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
    // Send device address
    TWDR = address;
    // Wait until transmission end
    i2c_transmission((1 << TWINT) | (1 << TWEN));
}

/**
 * Send start bit on open connection to I2C
 */
void i2c_rep_start(uint8_t address) {
    i2c_start(address);
}

/**
 * Send stop bit to I2C
 */
void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

/**
 * Send byte to I2C
 */
void i2c_write(uint8_t twdr) {
    TWDR = twdr;
    i2c_transmission((1 << TWINT) | (1 << TWEN));
}

/**
 * Read data from I2C and send ACK to accept more data
 */
uint8_t i2c_read_ack() {
    i2c_transmission((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
    return TWDR;
}

/**
 * Read data from I2C and send NAK to close more data
 */
uint8_t i2c_read_nak() {
    i2c_transmission((1 << TWINT) | (1 << TWEN));
    uint8_t twdr = TWDR;
    i2c_stop();
    return twdr;
}

/**
 * Write register value to I2C device
 */
void i2c_write_reg(uint8_t address, uint8_t reg, uint8_t val) {
    // Start communication with device in write mode
    i2c_start((address << 1));
    // Select register
    i2c_write(reg);
    // Write data to register
    i2c_write(val);
    // Stop communication with device
    i2c_stop();
}

/**
 * Read register value from I2C device
 */
uint8_t i2c_read_reg(uint8_t address, uint8_t reg) {
    uint8_t buf;
    i2c_read_reg_to_buf(address, reg, &buf, 1);

    return buf;
}

/**
 * Read register value from I2C device to buffer
 */
void i2c_read_reg_to_buf(uint8_t address, uint8_t reg, uint8_t *buf, uint8_t size) {
    // Start communication with device in write mode
    i2c_start((address << 1));
    // Register selection
    i2c_write(reg);
    // Switch to read mode
    i2c_rep_start((address << 1) | 1);
    uint8_t *b = buf;

    while (--size) {
        *b++ = i2c_read_ack();
    }
    *b = i2c_read_nak();

    i2c_stop();
}