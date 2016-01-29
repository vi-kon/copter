#ifndef MULTIWII_BAROMETER_H
#define MULTIWII_BAROMETER_H

#include <stdint.h>

extern int16_t baroTemperature;
extern int32_t baroPressure;

void baro_init();

void baro_update();

void baro_common();

#endif //MULTIWII_BAROMETER_H