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
    irobnprintf(20, "Hello, computer!\n");
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

