#ifndef MULTIWII_BMP085_H
#define MULTIWII_BMP085_H

#include <stdint.h>
#include "../config.h"

#ifdef BMP085

#define BMP085_ADDRESS 0x77
#define OSS 3

void bmp085_init();

uint8_t bmp085_update();

#endif

#endif //MULTIWII_BMP085_H
