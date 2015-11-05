#include <stdint.h>
#include "timer.h"
#include "cmod.h"
#include "oi.h"

#include "sensing.h"
#include "irobserial.h"
#include "iroblife.h"

// Delay constant
#define IROB_PERIOD_MS  (1000)


// Called by irobPeriodic
void iroblifePeriodic(void) {
    setSerialDestination(SERIAL_USB);
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
    byteTx('\n');
    setSerialDestination(SERIAL_CREATE);
}


int main(void) {
    // Submit to irobPeriodic
    setIrobPeriodicImpl(&iroblifePeriodic);

    // Initialize the Create
    irobInit();

    // Infinite operation loop
    for(;;) {
        // Periodic execution
        irobPeriodic();
       
        // Delay for the loop
        delayAndUpdateSensors(IROB_PERIOD_MS);
    }
}

