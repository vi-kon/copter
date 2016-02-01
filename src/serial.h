#ifndef MULTIWII_SERIAL_H
#define MULTIWII_SERIAL_H

#include <stdint.h>

#define SERIAL_PORT_NUMBER 4

#define SERIAL_BUFFER_RX 200
#define SERIAL_BUFFER_TX 34

void serial_open(uint8_t port, uint32_t baud);

void serial_close(uint8_t port);

uint8_t serial_available(uint8_t port);

uint8_t serial_read(uint8_t port);

void serial_write(uint8_t port, uint8_t c);

void serial_send_data(uint8_t port);

#endif //MULTIWII_SERIAL_H
