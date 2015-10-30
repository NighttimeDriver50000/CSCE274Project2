#include <stdint.h>
#include "timer.h"
#include "cmod.h"
#include "oi.h"

#include "driving.h"
#include "irobled.h"
#include "sensing.h"

#include "iroblife.h"

// Delay constant
#define IROB_PERIOD_MS  (100)


// Declare global variables 
uint8_t suppressButtons = 0;


// Called by irobPeriodic
void iroblifePeriodic(void) {
    // Get bumps and wheel drops sensor packet
    uint8_t bumps_wd = getSensorUint8(SenBumpDrop);
    // Variable for holding the new Create LED bits
    uint8_t newLedBits = 0x00;
    if (bumps_wd & MASK_BUMP_LEFT) {
        // The left bumper is active. Light the play LED.
        newLedBits |= PLAY_ROBOT_LED;
    }
    if (bumps_wd & MASK_BUMP_RIGHT) {
        // The right bumper is active. Light the advance LED.
        newLedBits |= ADVANCE_ROBOT_LED;
    }
    // Send the LED bits to the Create
    robotLedSetBits(newLedBits);
    if (bumps_wd & MASK_WHEEL_DROP) {
        // The Create was picked up or is about to go over a ledge. Abort.
        irobEnd();
    }
    // Toggle the command module LEDs
    byteTx(LEDBothToggle);
}


int main(void) {
    // Submit to irobPeriodic
    setIrobPeriodicImpl(&iroblifePeriodic);

    // Initialize the Create
    irobInit();

    // Initialize global variables
    suppressButtons = 0;

    // Infinite operation loop
    for(;;) {
        // Periodic execution
        irobPeriodic();
       
        // Delay for the loop
        delayAndUpdateSensors(IROB_PERIOD_MS);
    }
}

