#include "lib2a.h"
#include "irobserial.h"
#include "sensing.h"
#include "oi.h"
#include "cmod.h"

char *getChargingState(void) {
    uint8_t state = getSensorUint8(SenChargeState);
    switch (state) {
        case 0:
            return "Not Charging";
        case 1:
            return "Reconditioning Charging";
        case 2:
            return "Full Charging";
        case 3:
            return "Trickle Charging";
        case 4:
            return "Waiting";
        case 5:
            return "Charging Fault Condition";
        default:
            return "Unknown";
    }
}

// Called by irobPeriodic
void iroblifePeriodic(void) {
    // Switch output to USB
    setSerialDestination(SERIAL_USB);
    // Output sensor values
    irobprintf("Charging State: %s\n", getChargingState());
    irobprintf("Voltage: %d mV\n", getSensorUint16(SenVolt1));
    irobprintf("Current: %d mA\n", getSensorInt16(SenCurr1));
    irobprintf("Battery Temperature: %d degC\n", getSensorInt8(SenTemp));
    irobprintf("Battery Charge: %d mAh\n", getSensorUint16(SenCharge1));
    irobprintf("Battery Capacity: %d mAh\n", getSensorUint16(SenCap1));
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
