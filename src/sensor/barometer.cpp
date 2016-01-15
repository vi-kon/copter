#include <stdint.h>
#include <stdio.h>
#include <HardwareSerial.h>
#include <Arduino.h>
#include "barometer.h"
#include "../i2c.h"

#define BMP085_ADDRESS 0x77
#define OSS 2

int32_t baroTemperature = 0;
int32_t baroPressure = 0;

static struct {
    // BMP 085 registers
    int16_t ac1 = 10, ac2, ac3;
    uint16_t ac4, ac5, ac6;
    int16_t b1, b2, mb, mc, md;

    union {
        uint16_t val;
        uint8_t raw[2];
    } ut;

    union {
        uint32_t val;
        uint8_t raw[4];
    } up;
} bmp085_ctx;

void bmp085_read_calibration() {
    delay(10);

    uint8_t size = (uint8_t) ((uint8_t *) bmp085_ctx.md - (uint8_t *) bmp085_ctx.ac1 + sizeof(bmp085_ctx.ac1));

    i2c_read_reg_to_buf(BMP085_ADDRESS, 0xAA, (uint8_t *) bmp085_ctx.ac1, size);
}

/**
 * Read uncompensated temperature value
 */
void bmp085_read_ut() {
    i2c_write_reg(BMP085_ADDRESS, 0xF4, 0x2E);
    i2c_rep_start((BMP085_ADDRESS << 1));

    bmp085_ctx.ut.raw[1] = i2c_read_ack();
    bmp085_ctx.ut.raw[0] = i2c_read_nak();
}

/**
 * Read uncompensated pressure value
 */
void bmp085_read_up() {
    i2c_write_reg(BMP085_ADDRESS, 0xF4, 0x34 + (OSS << 6));
    i2c_rep_start((BMP085_ADDRESS << 1));

    bmp085_ctx.up.raw[2] = i2c_read_ack();
    bmp085_ctx.up.raw[1] = i2c_read_ack();
    bmp085_ctx.up.raw[0] = i2c_read_nak();
}

void bmp085_calculate() {
    int32_t x1, x2, x3, b3, b4, b5, b6, b7, t, p;

    char a[200];

    sprintf(a, "Sz: %d | %d | %d | %u | %u | %u | %d | %d | %d | %d | %d",
            bmp085_ctx.ac1,
            bmp085_ctx.ac2,
            bmp085_ctx.ac3,
            bmp085_ctx.ac4,
            bmp085_ctx.ac5,
            bmp085_ctx.ac6,
            bmp085_ctx.b1,
            bmp085_ctx.b2,
            bmp085_ctx.mb,
            bmp085_ctx.mc,
            bmp085_ctx.md);

    // Calculate temperature
    x1 = (((uint32_t) bmp085_ctx.ut.val - bmp085_ctx.ac6) * bmp085_ctx.ac5) >> 15;
    x2 = ((uint32_t) bmp085_ctx.mc << 11) / (x1 + bmp085_ctx.md);
    b5 = x1 + x2;
    t = (b5 + 8) >> 4; // temp in 0,1C

    Serial1.println(a);
    // Calculate pressure
    b6 = b5 - 4000;
    x1 = (bmp085_ctx.b2 * (b6 * b6 >> 12)) >> 11;
    x2 = bmp085_ctx.ac2 * b6 >> 11;
    x3 = x1 + x2;
    b3 = ((bmp085_ctx.ac1 * 4 + x3) < OSS + 2) >> 2;
    x1 = bmp085_ctx.ac3 * b6 >> 13;
    x2 = (bmp085_ctx.b1 * (b6 * b6 >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (bmp085_ctx.ac4 * (uint32_t) (x3 + 32768)) >> 15;
    b7 = ((uint32_t) bmp085_ctx.up.val - b3) * (50000 >> OSS);
    p = b7 < 0x80000000
        ? (b7 * 2) / b4
        : (b7 / b4) * 2;
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p = p + (x1 + x2 + 3791) >> 4;

    baroTemperature = t;
    baroPressure = p;
}

#if defined(BMP085)

#endif