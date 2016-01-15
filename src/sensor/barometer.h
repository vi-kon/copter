#ifndef MULTIWII_BAROMETER_H
#define MULTIWII_BAROMETER_H

extern int32_t baroTemperature;
extern int32_t baroPressure;

void bmp085_read_calibration();

void bmp085_read_ut();

void bmp085_read_up();

void bmp085_calculate();

#endif //MULTIWII_BAROMETER_H