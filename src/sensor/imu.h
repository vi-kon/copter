#ifndef MULTIWII_IMU_H
#define MULTIWII_IMU_H

#include <stdint.h>

#define ROLL 0
#define PITCH 1
#define YAW 2

typedef struct {
    int16_t accADC[3];
    int16_t gyroADC[3];
    int16_t magADC[3];
} imu_t;

extern imu_t imu;

void acc_init();

void acc_update();

void acc_common();

void gyro_init();

void gyro_update();

void gyro_common();

void mag_init();

void mag_update();

void mag_common();

#endif //MULTIWII_IMU_H
