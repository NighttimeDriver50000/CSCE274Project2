#include <stdint.h>
#include "sensing.h"
#include "cmod.h"
#include "timer.h"
#include "oi.h"

volatile uint8_t usartActive = 0;
volatile uint8_t sensorIndex = 0;
volatile uint8_t sensorBuffer[Sen6Size];
volatile uint8_t sensors[Sen6Size];

void requestPacket(uint8_t packetId) {
    byteTx(CmdSensors);
    byteTx(packetId);
}

uint8_t read1ByteSensorPacket(uint8_t packetId) {
    // Send the packet ID
    requestPacket(packetId);
    // Read the packet byte
    return byteRx();
}

void setupSensing(void) {
    cli();
    UBRR0 = 19;
    UCSR0B = (_BV(RXCIE0) | _BV(TXEN0) | _BV(RXEN0));
    UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01));
    sei();
}

ISR(USART_RX_vect) {
    uint8_t tmpUDR0;
    tmpUDR0 = UDR0;
    if (usartActive) {
        sensorBuffer[sensorIndex++] = tmpUDR0;
        if (sensorIndex >= Sen6Size) {
            usartActive = 0;
        }
    }
}

void updateSensors(void) {
    if (!usartActive) {
        uint8_t i;
        for (i = 0; i < Sen6Size; i++) {
            sensors[i] = sensorBuffer[i];
        }
        requestPacket(PACKET_ALL);
        sensorIndex = 0;
        usartActive = 1;
    }
}

void delayAndUpdateSensors(uint32_t time_ms) {
    delayMsFunc(time_ms, &updateSensors, 1, UPDATE_SENSOR_DELAY_CUTOFF);
}
