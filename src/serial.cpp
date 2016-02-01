#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"

static volatile uint8_t serialHeadRX[SERIAL_PORT_NUMBER];
static uint8_t serialBufferRX[SERIAL_PORT_NUMBER][SERIAL_BUFFER_RX];
static volatile uint8_t serialTailRX[SERIAL_PORT_NUMBER];

static volatile uint8_t serialHeadTX[SERIAL_PORT_NUMBER];
static uint8_t serialBufferTX[SERIAL_PORT_NUMBER][SERIAL_BUFFER_TX];
static volatile uint8_t serialTailTX[SERIAL_PORT_NUMBER];

/**
 * Calculate next index for serial buffer
 */
uint8_t inline next_index(uint8_t index, uint8_t size) {
    if (++index >= size) {
        return 0;
    }
    return index;
}

// Register UART interrupts to receive simple character from serial port

#if SERIAL_PORT_NUMBER >= 1

ISR(USART0_RX_vect) {
    uint8_t tail = serialTailRX[0];
    // Read single character from serial
    serialBufferRX[0][tail] = UDR0;
    // Update header position after successful serial read
    serialTailRX[0] = next_index(tail, SERIAL_BUFFER_RX);
}

#endif

#if SERIAL_PORT_NUMBER >= 2

ISR(USART1_RX_vect) {
    uint8_t tail = serialTailRX[1];
    // Read single character from serial
    serialBufferRX[1][tail] = UDR1;
    // Update header position after successful serial read
    serialTailRX[1] = next_index(tail, SERIAL_BUFFER_RX);
}

#endif

#if SERIAL_PORT_NUMBER >= 3

ISR(USART2_RX_vect) {
    uint8_t tail = serialTailRX[2];
    // Read single character from serial
    serialBufferRX[2][tail] = UDR2;
    // Update header position after successful serial read
    serialTailRX[2] = next_index(tail, SERIAL_BUFFER_RX);
}

#endif

#if SERIAL_PORT_NUMBER >= 4

ISR(USART3_RX_vect) {
    uint8_t tail = serialTailRX[3];
    // Read single character from serial
    serialBufferRX[3][tail] = UDR3;
    // Update header position after successful serial read
    serialTailRX[3] = next_index(tail, SERIAL_BUFFER_RX);
}

#endif

// Reqister UART interrupts to transmit simple character to serial port

#if SERIAL_PORT_NUMBER >= 1

ISR(USART0_UDRE_vect) {
    uint8_t head = serialHeadTX[0];

    // Check if data available for transmitting
    if (serialTailTX[0] != head) {
        UDR0 = serialBufferTX[0][head];
        head = next_index(head, SERIAL_BUFFER_TX);
        serialHeadTX[0] = head;
    }

    // Check if all data is transmitted
    if (serialTailTX[0] == head) {
        // Disable data register interrupt
        UCSR0B &= ~(1 << UDRIE0);
    }
}

#endif

#if SERIAL_PORT_NUMBER >= 2

ISR(USART1_UDRE_vect) {
    uint8_t head = serialHeadTX[1];

    // Check if data available for transmitting
    if (serialTailTX[1] != head) {
        UDR1 = serialBufferTX[1][head];
        head = next_index(head, SERIAL_BUFFER_TX);
        serialHeadTX[1] = head;
    }

    // Check if all data is transmitted
    if (serialTailTX[1] == head) {
        // Disable data register interrupt
        UCSR1B &= ~(1 << UDRIE1);
    }
}

#endif

#if SERIAL_PORT_NUMBER >= 3

ISR(USART2_UDRE_vect) {
    uint8_t head = serialHeadTX[2];

    // Check if data available for transmitting
    if (serialTailTX[2] != head) {
        UDR2 = serialBufferTX[2][head];
        head = next_index(head, SERIAL_BUFFER_TX);
        serialHeadTX[2] = head;
    }

    // Check if all data is transmitted
    if (serialTailTX[2] == head) {
        // Disable data register interrupt
        UCSR2B &= ~(1 << UDRIE2);
    }
}

#endif

#if SERIAL_PORT_NUMBER >= 4

ISR(USART3_UDRE_vect) {
    uint8_t head = serialHeadTX[3];

    // Check if data available for transmitting
    if (serialTailTX[3] != head) {
        UDR3 = serialBufferTX[3][head];
        head = next_index(head, SERIAL_BUFFER_TX);
        serialHeadTX[3] = head;
    }

    // Check if all data is transmitted
    if (serialTailTX[3] == head) {
        // Disable data register interrupt
        UCSR3B &= ~(1 << UDRIE3);
    }
}

#endif

/**
 * Open given serial port with baud rate
 */
void serial_open(uint8_t port, uint32_t baud) {
    // Asynchronous Double Speed mode
    uint8_t h = (uint8_t) ((F_CPU / (8 * baud) - 1) >> 8);
    uint8_t l = (uint8_t) ((F_CPU / (8 * baud) - 1));

    switch (port) {
#if SERIAL_PORT_NUMBER >= 1
        case 0:
            UCSR0A = (1 << U2X0);
            UBRR0H = h;
            UBRR0L = l;
            // Enable RX, TX and RX interrupt
            UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
            break;
#endif
#if SERIAL_PORT_NUMBER >= 2
        case 1:
            UCSR1A = (1 << U2X1);
            UBRR1H = h;
            UBRR1L = l;
            // Enable RX, TX and RX interrupt
            UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
            break;
#endif
#if SERIAL_PORT_NUMBER >= 3
        case 2:
            UCSR2A = (1 << U2X2);
            UBRR2H = h;
            UBRR2L = l;
            // Enable RX, TX and RX interrupt
            UCSR2B |= (1 << RXEN2) | (1 << TXEN2) | (1 << RXCIE2);
            break;
#endif
#if SERIAL_PORT_NUMBER >= 4
        case 3:
            UCSR3A = (1 << U2X3);
            UBRR3H = h;
            UBRR3L = l;
            // Enable RX, TX and RX interrupt
            UCSR3B |= (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3);
            break;
#endif
        default:
            // Nothing to do if port is not defined
            break;
    }
}

/**
 * Close given serial port
 */
void serial_close(uint8_t port) {
    switch (port) {
#if SERIAL_PORT_NUMBER >= 1
        case 0:
            // Disable RX, TX, RX interrupt and data register empty interrupt
            UCSR0B &= ~((1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0));
            break;
#endif
#if SERIAL_PORT_NUMBER >= 2
        case 1:
            // Disable RX, TX, RX interrupt and data register empty interrupt
            UCSR1B &= ~((1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1) | (1 << UDRIE1));
            break;
#endif
#if SERIAL_PORT_NUMBER >= 3
        case 2:
            // Disable RX, TX, RX interrupt and data register empty interrupt
            UCSR2B &= ~((1 << RXEN2) | (1 << TXEN2) | (1 << RXCIE2) | (1 << UDRIE2));
            break;
#endif
#if SERIAL_PORT_NUMBER >= 4
        case 3:
            // Disable RX, TX, RX interrupt and data register empty interrupt
            UCSR3B &= ~((1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3) | (1 << UDRIE3));
            break;
#endif
        default:
            // Nothing to do if port is not defined
            break;
    }
}

/**
 * Check how many character is available on given serial port
 */
uint8_t serial_available(uint8_t port) {
    return (serialHeadRX[port] - serialTailRX[port]) % SERIAL_BUFFER_RX;
}

/**
 * Read single character from given serial port
 */
uint8_t serial_read(uint8_t port) {
    uint8_t head = serialHeadRX[port];
    uint8_t c = serialBufferRX[port][head];
    serialHeadRX[port] = next_index(head, SERIAL_BUFFER_RX);

    return c;
}

/**
 * Write single character to given serial port
 */
void serial_write(uint8_t port, uint8_t c) {
    uint8_t tail = serialTailTX[port];
    serialBufferTX[port][tail] = c;
    serialTailTX[port] = next_index(tail, SERIAL_BUFFER_TX);
}

/**
 * Start sending data from buffer to given serial port
 */
void serial_send_data(uint8_t port) {
    switch (port) {
        case 0:
            UCSR0B |= (1 << UDRIE0);
            break;
        case 1:
            UCSR1B |= (1 << UDRIE1);
            break;
        case 2:
            UCSR2B |= (1 << UDRIE2);
            break;
        case 3:
            UCSR3B |= (1 << UDRIE3);
            break;
        default:
            // Nothing to do if port is not defined
            break;
    }
}