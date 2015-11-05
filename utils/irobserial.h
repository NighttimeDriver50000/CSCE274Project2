#ifndef IROBSERIAL_H
#define IROBSERIAL_H

#include <stdint.h>
#include <stdarg.h>

#define SERIAL_CREATE       (1)
#define SERIAL_USB          (2)
#define SERIAL_SWITCHING    (0xFF)

#define PRINTF_BUFFER_SIZE  (0xFF)

void setSerialDestination(uint8_t dest);

uint8_t getSerialDestination(void);

void irobprint(char* str);

void irobprintf(const char* format, ...);

void irobnprintf(uint16_t size, const char* format, ...);

#endif
