#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "irobserial.h"
#include "cmod.h"
#include "oi.h"
#include "timer.h"

uint8_t serialDestination = SERIAL_SWITCHING;

void setSerialDestination(uint8_t dest) {
    serialDestination = SERIAL_SWITCHING;
    // Which serial port should byteTx and byteRx talk to?
    // Ensure any pending bytes have been sent. Without this, the last byte
    // sent before calling this might seem to disappear.
    delayMs(10);
    // Configure the port.
    if (dest == SERIAL_CREATE) {
        PORTB &= ~0x10 ;
    } else {
        PORTB |= 0x10 ;
    }
    // Wait a bit to let things get back to normal. According to the docs, this
    // should be at least 10 times the amount of time needed to send one byte.
    // This is less than 1 millisecond. We are using a much longer delay to be
    // super extra sure.
    delayMs(10);
    serialDestination = dest;
}

uint8_t getSerialDestination(void) {
   return serialDestination; 
}

void irobnprintf(uint16_t size, const char* format, ...) {
    char formatted[size];
    char* fp = &formatted[0];
    va_list ap;
    va_start(ap, format);
    vsnprintf(fp, size, format, ap);
    va_end(ap);
    char c;
    while ((c = *(fp++)) != '\0') {
        byteTx(c);
    }
}
