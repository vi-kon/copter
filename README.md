# Fly controller for Arduino platform

**Note**: The package is still under development

## Currently status

* Added **BMA180** (accelerometer), **BMP085** (barometer), **HMC5883** (magnetometer) and **ITG3200** (gyroscope) sensors 
* Implemented **I2C** bus

To disable some sensor simple comment out from `config.h` header file. To init sensors simple call `sensor_init` method.

Some sample usage:

```c++
#include "Arduino.h"
#include "main.h"
#include "sensor/sensors.h"
#include "sensor/imu.h"

uint64_t currentTime;

void setup() {
    Serial1.begin(115200);

    Serial1.println("Setup");

    sensors_init();
}

void loop() {
    currentTime = (uint64_t) micros();

    char buf[100];

    sensor_update();
    Serial1.println("Acc");
    sprintf(buf, "Roll: %d Pitch: %d Yaw: %d", imu.accADC[ROLL], imu.accADC[PITCH], imu.accADC[YAW]);
    Serial1.println(buf);

    Serial1.println("Gyro");
    sprintf(buf, "Roll: %d Pitch: %d Yaw: %d", imu.gyroADC[ROLL], imu.gyroADC[PITCH], imu.gyroADC[YAW]);
    Serial1.println(buf);

    Serial1.println("Mag");
    sprintf(buf, "Roll: %d Pitch: %d Yaw: %d", imu.magADC[ROLL], imu.magADC[PITCH], imu.magADC[YAW]);
    Serial1.println(buf);

    delay(500);
}
```

# Sensor datasheets

* **BMA180** - [BMA180-DataSheet-v2.5.pdf](docs/sensor/BMA180-DataSheet-v2.5.pdf)
* **BMP085** - [BST-BMP085-DS000-05.pdf](docs/sensor/BST-BMP085-DS000-05.pdf)
* **HMC5883** - [HMC5883.pdf](docs/sensor/HMC5883.pdf)
* **ITG3200** - [PS-ITG-3200-00-01.4.pdf](docs/sensor/PS-ITG-3200-00-01.4.pdf)