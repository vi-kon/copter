#include <stdint.h>
#include "sensors.h"
#include "../i2c.h"
#include "imu.h"
#include "baro.h"

void sensors_init() {
    // Try several times initialize sensors with no errors
    uint8_t c = 5;
    while (c) {
        c--;
        i2c_init();
        baro_init();
        acc_init();
        gyro_init();
        mag_init();
        if (i2c_errors_count == 0) {
            break;
        }
    }
}

void sensor_update() {
    baro_update();
    acc_update();
    gyro_update();
    mag_update();
}