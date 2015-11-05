#include "lib2a.h"
#include "irobserial.h"
#include "sensing.h"
#include "oi.h"
#include "cmod.h"

// Called by irobPeriodic
void iroblifePeriodic(void) {
    // Switch output to USB
    setSerialDestination(SERIAL_USB);
    // Output sensor values
    irobprintf("Charging State: %d\n", getSensorUint8(SenChargeState));
    irobprintf("Voltage: %d\n", getSensorUint16(SenVolt1));
    irobprintf("Current: %d\n", getSensorInt16(SenCurr1));
    irobprintf("Battery Temperature: %d\n", getSensorInt8(SenTemp));
    irobprintf("Battery Charge: %d\n", getSensorUint16(SenCharge1));
    irobprintf("Battery Capacity: %d\n", getSensorUint16(SenCap1));
    irobprintf("Wall Signal: %d\n", getSensorUint16(SenWallSig1));
    irobprintf("Cliff Left Signal: %d\n", getSensorUint16(SenCliffLSig1));
    irobprintf("Cliff Front Left Signal: %d\n", getSensorUint16(SenCliffFLSig1));
    irobprintf("Cliff Front Right Signal: %d\n", getSensorUint16(SenCliffFRSig1));
    irobprintf("Cliff Right Signal: %d\n", getSensorUint16(SenCliffRSig1));
    // Spacing
    byteTx('\n');
    // Switch output back to Create for updating sensor values
    setSerialDestination(SERIAL_CREATE);
}