#include "Arduino.h"
#include "i2c.h"
#include "sensor/barometer.h"

#define BMP085

void setup() {
    Serial1.begin(115200);

    Serial1.println("Setup");

    i2c_init();

    bmp085_read_calibration();

    uint8_t c = 5;

    while (c) {
        c--;
        i2c_init();
        delay(10);
        bmp085_read_calibration();
        delay(5);
        if (i2c_errors_count != 0) {
            Serial1.println("Success");
            break;
        }
    }
}

void loop() {
    delay(1000);

    Serial1.println("Loop");

    bmp085_read_ut();
//    delay(10);
//    bmp085_read_up();
//    delay(10);
//    bmp085_calculate();
//    delay(10);
//    Serial1.println(baroTemperature);
//    Serial1.println(baroPressure);

}