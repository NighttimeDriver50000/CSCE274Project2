#include <stdint.h>
#include "timer.h"
#include "cmod.h"
#include "oi.h"

#include "sensing.h"
#include "iroblife.h"
#include "driving.h"

// Driving constants
#define SPEED           (500)
#define TURN_ANGLE      (30)

//! Previous IR sensor value
uint8_t irPrevious = 0;

//! True iff the robot should not move
uint8_t cannotRotateOrAdvance(void) {
    return getSensorUint8(SenBumpDrop) & MASK_WHEEL_DROP;
}

//! True iff the robot should not drive forward
uint8_t cannotAdvance(void) {
    return getSensorUint8(SenBumpDrop) || !(getSensorUint16(SenCliffLSig1) &&
            getSensorUint16(SenCliffFLSig1) && getSensorUint16(SenCliffFRSig1)
            && getSensorUint16(SenCliffRSig1));
}

void doWhileTurning(void) {
    // Get most recent sensor values
    updateSensors();
    if (cannotRotateOrAdvance()) {
        // Shouldn't be moving: just stop
        driveStop();
    }
    // Keep going
}

//! Called by irobPeriodic
void iroblifePeriodic(void) {
    // Get most recent sensor values
    updateSensors();
    // Get IR sensor value
    uint8_t ir = getSensorUint8(SenIRChar);
    if (cannotRotateOrAdvance()) {
        // Shouldn't be moving: just stop
        driveStop();
    } else {
        switch (ir) {
            case IR_FORWARD:
                if (cannotAdvance()) {
                    // Shouldn't be moving forward: just stop
                    driveStop();
                } else {
                    // Forward being pressed and able to move forward: drive!
                    drive(SPEED, RadStraight);
                }
                break;
            case IR_LEFT:
                // Turn left 
                drive(SPEED, RadCCW);
                break;
            case IR_RIGHT:
                // Turn right
                drive(SPEED, RadCW);
                break;
            default:
                // Movement button isn't being pressed
                switch (irPrevious) {
                    case IR_LEFT:
                        // Turn an extra 30 degrees
                        driveAngleTFunc(SPEED, RadCCW, TURN_ANGLE,
                                &doWhileTurning, UPDATE_SENSOR_DELAY_PERIOD,
                                UPDATE_SENSOR_DELAY_CUTOFF);
                        break;
                    case IR_RIGHT:
                        // Turn an extra 30 degrees
                        driveAngleTFunc(SPEED, RadCW, TURN_ANGLE,
                                &doWhileTurning, UPDATE_SENSOR_DELAY_PERIOD,
                                UPDATE_SENSOR_DELAY_CUTOFF);
                        break;
                    default:
                        // No buttons pressed and no overturning left: stop
                        driveStop();
                }
                break;
        }
    }
    // Bookkeeping
    irPrevious = ir;
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

        // Delay to avoid overflows
        delayMs(UPDATE_SENSOR_DELAY_PERIOD);
        // Wait if sensors coming in so next loop has clean start
        waitForSensors();
    }
}

