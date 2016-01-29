#include "../config.h"

#if defined(ITG_3200) || defined(ITG3205) || defined(ITG_3050) || defined(MPU3050)

#include <Arduino.h>
#include "itg3200.h"
#include "../i2c.h"
#include "imu.h"


void itg3205_init() {
    // Reset device
    i2c_write_reg(ITG3200_ADDRESS, ITG3200_PWR_MGM, (ITG3200_H_RESET << 7)); // 1000 0000
    delay(5);

    // Need add GYRO_DLPF_CFG
    i2c_write_reg(ITG3200_ADDRESS, ITG3200_DLPF_FS, (ITG3200_FS_SEL << 3)); // 0001 1000
    delay(5);

    // Set PLL with Z Gyro reference
    i2c_write_reg(ITG3200_ADDRESS, ITG3200_PWR_MGM, (ITG3200_CLK_SEL_PLL_Z_GYRO << 0)); // 0000 0011
    delay(5);
}

void itg3205_update() {
    uint8_t adc[6];

    i2c_read_reg_to_buf(ITG3200_ADDRESS, ITG3200_GYRO_XOUT_H, adc, 6);

    GYRO_ORIENTATION((adc[1] << 8) | (adc[0] >> 4),
                     (adc[3] << 8) | (adc[2] >> 4),
                     (adc[5] << 8) | (adc[4] >> 4));
}

#endif