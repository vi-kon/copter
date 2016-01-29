#include <Arduino.h>
#include "bma180.h"
#include "../i2c.h"
#include "imu.h"

#ifdef BMA180

void bma180_init() {
    delay(10);

    // Write to ctrl_reg0 register ee_w value
    i2c_write_reg(BMA180_ADDRESS, BMA180_CTRLREG0, (1 << BMA180_EE_W));
    delay(5);

    uint8_t control = i2c_read_reg(BMA180_ADDRESS, BMA180_BW_TCS);
    // Clear bw register and save tcs register
    control = control & 0x0F; // 0000 1111
    // Set filter
    control = control | (BMA180_FILTER_LOW_10HZ << 4);
    i2c_write_reg(BMA180_ADDRESS, BMA180_BW_TCS, control);
    delay(5);

    control = i2c_read_reg(BMA180_ADDRESS, BMA180_TCO_Z);
    // Clear mode_config register and save tco_z register
    control = control & 0xFC; // 1111 1100
    // Set mode_config to 0x00 (Low noise mode)
    control = control | 0x00;
    i2c_write_reg(BMA180_ADDRESS, BMA180_TCO_Z, control);
    delay(5);

    control = i2c_read_reg(BMA180_ADDRESS, BMA180_OFFSET_LSB1);
    // Clear range register and save offset_x and smp_skip registers
    control = control & 0xF1; // 1111 0001
    // Set range
    control = control | (BMA180_RANGE_8G << 1);
    i2c_write_reg(BMA180_ADDRESS, BMA180_OFFSET_LSB1, control);
    delay(5);
}

void bma180_update() {
    uint8_t adc[6];

    i2c_read_reg_to_buf(BMA180_ADDRESS, BMA180_ACC_X_LSB, adc, 6);

    ACC_ORIENTATION((adc[1] << 8) | (adc[0] >> 4),
                    (adc[3] << 8) | (adc[2] >> 4),
                    (adc[5] << 8) | (adc[4] >> 4));
}

#endif