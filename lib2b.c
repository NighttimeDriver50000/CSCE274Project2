#include "lib2b.h"
#include "sensing.h"
#include "oi.h"
#include "driving.h"

//! Previous IR sensor value
uint8_t irPrevious = 0;

//! True iff the robot should not move at all
uint8_t cannotRotateOrAdvance(void) {
    return getSensorUint8(SenBumpDrop) & MASK_WHEEL_DROP;
}

//! True iff the robot should not drive forward
uint8_t cannotAdvance(void) {
    return getSensorUint8(SenBumpDrop) || !(getSensorUint16(SenCliffLSig1) &&
            getSensorUint16(SenCliffFLSig1) && getSensorUint16(SenCliffFRSig1)
            && getSensorUint16(SenCliffRSig1));
}

//! Begin or continue driving forward if it is allowed, otherwise stop
void driveForwardIfAllowable(void) {
    if (cannotAdvance()) {
        // Shouldn't be moving forward: just stop
        driveStop();
    } else {
        // Forward being pressed and able to move forward: drive!
        drive(SPEED, RadStraight);
    }
}

//! Begin or continue turning
void turnContinuous(int16_t radius) {
    drive(SPEED, radius);
}

//! Called by overTurn periodically while turning.
//! Updates sensors and stops if unsafe to continue.
void doWhileTurning(void) {
    // Get most recent sensor values
    updateSensors();
    if (cannotRotateOrAdvance()) {
        // Shouldn't be moving: just stop
        driveStop();
    }
    // Keep going
}

//! Turn an extra 30 degrees. Does not return until fully turned.
//! Calls doWhileTurning periodically for sensor updating and safety.
void overTurn(int16_t radius) {
    driveAngleTFunc(SPEED, radius, OVER_TURN_ANGLE,
            &doWhileTurning, UPDATE_SENSOR_DELAY_PERIOD,
            UPDATE_SENSOR_DELAY_CUTOFF);
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
                // Drive forward if allowable
                driveForwardIfAllowable();
                break;
            case IR_LEFT:
                // Turn left 
                turnContinuous(RadCCW);
                break;
            case IR_RIGHT:
                // Turn right
                turnContinuous(RadCW);
                break;
            default:
                // Movement button isn't being pressed
                switch (irPrevious) {
                    case IR_LEFT:
                        // Turn an extra 30 degrees
                        overTurn(RadCCW);
                        break;
                    case IR_RIGHT:
                        // Turn an extra 30 degrees
                        overTurn(RadCW);
                        break;
                    default:
                        // No buttons pressed and no over-turning left: stop
                        driveStop();
                }
                break;
        }
    }
    // Bookkeeping
    irPrevious = ir;
}
