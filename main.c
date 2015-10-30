#include <stdint.h>
#include "timer.h"
#include "cmod.h"
#include "oi.h"

#include "driving.h"
#include "irobled.h"
#include "sensing.h"

#include "iroblife.h"

// Delay constants
#define TURN_DELAY_MS   (500)
#define IROB_PERIOD_MS  (100)
#define IROB_CUTOFF_MS  (10)

// Pentagon constants
#define PENTAGON_SIDE_LENGTH    (800)
#define PENTAGON_INNER_ANGLE    (90)
#define PENTAGON_SUPPLEMENT     (180 - PENTAGON_INNER_ANGLE)

// Error for pentagon constants
#define SIDE_LENGTH_ERROR   (5)
#define INNER_ANGLE_ERROR   (8)
#define SUPPLEMENT_ERROR    (5)

// Drive speed
#define DRIVE_SPEED         (50)
// Actually used pentagon constants (with error added)
#define DRIVE_SIDE_LENGTH   (PENTAGON_SIDE_LENGTH + SIDE_LENGTH_ERROR)
#define DRIVE_INNER_ANGLE   (PENTAGON_INNER_ANGLE + INNER_ANGLE_ERROR)
#define DRIVE_SUPPLEMENT    (PENTAGON_SUPPLEMENT + SUPPLEMENT_ERROR)


// Declare global variables 
uint8_t suppressButtons = 0;


// Called by irobPeriodic
void iroblifePeriodic(void) {
    // Get bumps and wheel drops sensor packet
    uint8_t bumps_wd = read1ByteSensorPacket(PACKET_BUMPS_AND_WHEEL_DROPS);
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


void drivePentagon(int16_t radius) {
    // Lock Create buttons
    suppressButtons = 1;
    // Wait to allow motors to settle
    delayMsFunc(TURN_DELAY_MS, &irobPeriodic, IROB_PERIOD_MS,
            IROB_CUTOFF_MS);
    uint8_t i;
    for (i = 0; i < 4; i++) { // A pentagon has five sides
        // Drive along a side
        driveDistanceTFunc(DRIVE_SPEED, DRIVE_SIDE_LENGTH, &irobPeriodic,
                IROB_PERIOD_MS, IROB_CUTOFF_MS);
        // Wait to allow motors to settle
        delayMsFunc(TURN_DELAY_MS, &irobPeriodic, IROB_PERIOD_MS,
                IROB_CUTOFF_MS);
        // Turn to face the next side
        driveAngleTFunc(DRIVE_SPEED, radius, DRIVE_SUPPLEMENT, &irobPeriodic,
                IROB_PERIOD_MS, IROB_CUTOFF_MS);
        // Wait to allow motors to settle
        delayMsFunc(TURN_DELAY_MS, &irobPeriodic, IROB_PERIOD_MS,
                IROB_CUTOFF_MS);
    }
    // Unlock Create buttons
    suppressButtons = 0;
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
       
        // Code not executed during delay loops
        if (!suppressButtons) {
            // Read the button packet from the Create
            uint8_t buttons = read1ByteSensorPacket(PACKET_BUTTONS);
            if (buttons & MASK_BTN_PLAY) {
                // The Play button is down. Drive in a Clockwise pentagon.
                drivePentagon(RadCW);
            } else if (buttons & MASK_BTN_ADVANCE) {
                // The Advance button is down. Drive in Counterclockwise pentagon.
                // Get into the correct angle
                driveAngleTFunc(DRIVE_SPEED, RadCW, DRIVE_INNER_ANGLE, &irobPeriodic,
                        IROB_PERIOD_MS, IROB_CUTOFF_MS);
                // Drive the pentagon
                drivePentagon(RadCCW);
                // Return to the original angle
                driveAngleTFunc(DRIVE_SPEED, RadCCW, DRIVE_INNER_ANGLE, &irobPeriodic,
                        IROB_PERIOD_MS, IROB_CUTOFF_MS);
            }
            // Make sure the create is stopped
            driveStop();
        }
        
        // Delay for the loop
        delayMs(IROB_PERIOD_MS);
    }
}

