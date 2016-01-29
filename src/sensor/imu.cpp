#include "imu.h"
#include "../config.h"

#ifdef BMA180

#include "bma180.h"

#endif

#if defined(ITG_3200) || defined(ITG3205) || defined(ITG_3050) || defined(MPU3050)

#include "itg3200.h"

#endif

#ifdef HMC5883

#include "hmc5883.h"

#endif

// Define global imu variable
imu_t imu;

void acc_init() {
#ifdef BMA180
    bma180_init();
#endif
}

void acc_update() {
#ifdef BMA180
    bma180_update();
#endif

    acc_common();
}

void acc_common() {

}

void gyro_init() {
#if defined(ITG_3200) || defined(ITG3205) || defined(ITG_3050) || defined(MPU3050)
    itg3205_init();
#endif
}

void gyro_update() {
#if defined(ITG_3200) || defined(ITG3205) || defined(ITG_3050) || defined(MPU3050)
    itg3205_update();
#endif

    gyro_common();
}

void gyro_common() {

}

void mag_init() {
#ifdef HMC5883
    hmc5883_init();
#endif
}

void mag_update() {
#ifdef HMC5883
    hmc5883_update();
#endif

    mag_common();
}

void mag_common() {

}