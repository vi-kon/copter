#include "baro.h"
#include "../config.h"

#ifdef BMP085

#include "bmp085.h"

#endif

int16_t baroTemperature = 0;
int32_t baroPressure = 0;

void baro_init() {
#ifdef BMP085
    bmp085_init();
#endif // BMP085
}

void baro_update() {
#ifdef BMP085
    bmp085_update();
#endif // BMP085

    baro_common();
}

void baro_common() { }