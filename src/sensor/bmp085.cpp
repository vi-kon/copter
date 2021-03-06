#include "bmp085.h"
#include <stdio.h>
#include <Arduino.h>
#include "baro.h"
#include "../i2c.h"
#include "../main.h"
#include "../utils.h"

#ifdef BMP085

static struct {
    // BMP 085 registers
    int16_t ac1, ac2, ac3;
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

    uint32_t deadline;
    uint8_t state;
} bmp085_ctx;

void bmp085_read_calibration() {
    delay(10);

    uint8_t size = (uint8_t) ((uint8_t *) &bmp085_ctx.md - (uint8_t *) &bmp085_ctx.ac1 + sizeof(bmp085_ctx.ac1));

    i2c_read_reg_to_buf(BMP085_ADDRESS, 0xAA, (uint8_t *) &bmp085_ctx.ac1, size);

    // Fix endianness
    int16_t *p;

    for (p = &bmp085_ctx.ac1; p <= &bmp085_ctx.md; p++) {
        swap_endianness(p, sizeof(*p));
    }
}

/**
 * Start reading uncompensated temperature value
 */
void bmp085_ut_start() {
    i2c_write_reg(BMP085_ADDRESS, 0xF4, 0x2E);

    i2c_start((BMP085_ADDRESS << 1));
    // Set that we want read from 0xF6 register
    i2c_write(0xF6);
    i2c_stop();
}

/**
 * Read uncompressed temperature value
 */
void bmp085_ut_read() {
    i2c_start((BMP085_ADDRESS << 1) | 1);

    bmp085_ctx.ut.raw[1] = i2c_read_ack();
    bmp085_ctx.ut.raw[0] = i2c_read_nak();
}

/**
 * Start reading uncompensated pressure value
 */
void bmp085_up_start() {
    i2c_write_reg(BMP085_ADDRESS, 0xF4, 0x34 + (OSS << 6));

    i2c_start((BMP085_ADDRESS << 1));
    // Set that we want read from 0xF6 register
    i2c_write(0xF6);
    i2c_stop();
}

/**
 * Read uncompressed pressure value
 */
void bmp085_up_read() {
    i2c_start((BMP085_ADDRESS << 1) | 1);

    bmp085_ctx.up.raw[2] = i2c_read_ack();
    bmp085_ctx.up.raw[1] = i2c_read_ack();
    bmp085_ctx.up.raw[0] = i2c_read_nak();
}

void bmp085_calculate() {
    int32_t x1, x2, x3, b3, b4, b5, b6, b7, t, p;

    // Calculate temperature
    x1 = (((int32_t) bmp085_ctx.ut.val - bmp085_ctx.ac6) * bmp085_ctx.ac5) >> 15;
    x2 = ((int32_t) bmp085_ctx.mc << 11) / (x1 + bmp085_ctx.md);
    b5 = x1 + x2;
    t = (b5 + 8) >> 4; // temp in 0.1C

    // Calculate pressure
    b6 = b5 - 4000;
    x1 = (bmp085_ctx.b2 * (b6 * b6 >> 12)) >> 11;
    x2 = bmp085_ctx.ac2 * b6 >> 11;
    x3 = x1 + x2;
    b3 = ((bmp085_ctx.ac1 * 4 + x3) << OSS + 2) / 4;
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
    p = p + (x1 + x2 + 3791) >> 4; // press in Pa

    baroTemperature = t;
    baroPressure = p;
}

void bmp085_init() {
    delay(10);
    bmp085_read_calibration();
    delay(5);
    bmp085_ut_start();
    bmp085_ctx.deadline = currentTime + 5000;
}

uint8_t bmp085_update() {
    if (currentTime < bmp085_ctx.deadline) {
        return 0;
    }

    if (bmp085_ctx.state == 0) {
        bmp085_ut_read();
        bmp085_up_start();

        bmp085_ctx.state = 1;
        // 1.5ms margin according to the spec (25.5ms P conversion time pressure with OSS=3)
        bmp085_ctx.deadline += 27000;

        return 1;
    } else {
        bmp085_up_read();
        bmp085_ut_start();
        bmp085_calculate();
        bmp085_ctx.state = 0;

        // 1.5ms margin according to the spec (4.5ms T conversion time temperature)
        bmp085_ctx.deadline = currentTime + 6000;

        return 2;
    }
}

#endif