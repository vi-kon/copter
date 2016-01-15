#ifndef MULTIWII_BAROMETER_H
#define MULTIWII_BAROMETER_H

extern int32_t baroTemperature;
extern int32_t baroPressure;


void bmp085_init();

uint8_t bmp085_update();

#endif //MULTIWII_BAROMETER_H