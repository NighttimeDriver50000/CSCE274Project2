#include "iroblife.h"
#include "sensing.h"

#include "lib2a.h"

// Delay constant
#define IROB_PERIOD_MS  (1000)

int main(void) {
    // Submit to irobPeriodic
    setIrobPeriodicImpl(&iroblifePeriodic);

    // Initialize the Create
    irobInit();

    // Infinite operation loop
    for(;;) {
        // Periodic execution
        irobPeriodic();
       
        // Delay for the loop; one second
        delayAndUpdateSensors(IROB_PERIOD_MS);
    }
}

