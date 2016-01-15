#include <stdint.h>
#include "utils.h"

/**
 * Transfer series bytes from big endian to little endian
 */
void swap_endianness(void *buf, size_t size) {
    uint8_t temp;
    uint8_t *from;
    uint8_t *to;

    // Keep swapping bytes until the pointers have match each other
    for (from = (uint8_t *) buf, to = &from[size - 1]; from < to; from++, to--) {
        temp = *from;
        *from = *to;
        *to = temp;
    }
}