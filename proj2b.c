#include "timer.h"
#include "sensing.h"
#include "iroblife.h"

#include "lib2b.h"

int main(void) {
    // Submit to irobPeriodic
    setIrobPeriodicImpl(&iroblifePeriodic);

    // Initialize the Create
    irobInit();

    // Infinite operation loop
    for(;;) {
        // Periodic execution
        irobPeriodic();

        // Delay to avoid overflows
        delayMs(UPDATE_SENSOR_DELAY_PERIOD);
        // Wait if sensors coming in so next loop has clean start
        waitForSensors();
    }
}

