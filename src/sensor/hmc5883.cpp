#include <Arduino.h>
#include "hmc5883.h"
#include "../i2c.h"
#include "imu.h"

#ifdef HMC5883

void hmc5883_init() {
    // First measurement uses old gain settings
    i2c_write_reg(HMC5883_ADDRESS, HMC5883_CONF_B, HMC5883_GAIN_19 << 5);
    i2c_write_reg(HMC5883_ADDRESS, HMC5883_MODE, HMC5883_MODE_SINGLE_MEASUREMENT);
    delay(100);

    // Read first sample and discard it
    hmc5883_update();
}

void hmc5883_update() {
    uint8_t adc[6];

    i2c_read_reg_to_buf(HMC5883_ADDRESS, HMC5883_DATA_X_M, adc, 6);

    MAG_ORIENTATION((adc[1] << 8) | (adc[0] >> 4),
                    (adc[3] << 8) | (adc[2] >> 4),
                    (adc[5] << 8) | (adc[4] >> 4));
}

#endif