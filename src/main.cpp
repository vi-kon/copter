#include "Arduino.h"
#include "i2c.h"
#include "sensor/barometer.h"

#define BMP085

uint32_t currentTime;

void setup() {
    Serial1.begin(115200);

    Serial1.println("Setup");

    uint8_t c = 5;
    while (c) {
        c--;
        i2c_init();
        bmp085_init();
        if (i2c_errors_count == 0) {
            Serial1.println("Success");
            break;
        }
    }
}

void loop() {
    currentTime = micros();

    if (bmp085_update() == 2) {
        Serial1.println("loop");
        Serial1.println(baroTemperature);
        Serial1.println(baroPressure);
    }
}