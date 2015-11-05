#ifndef LIB2B_H
#define LIB2B_H

#include <stdint.h>

// Driving constants
#define SPEED           (500)
#define OVER_TURN_ANGLE (30)

//! True iff the robot should not move at all
uint8_t cannotRotateOrAdvance(void);

//! True iff the robot should not drive forward
uint8_t cannotAdvance(void);

//! Begin or continue driving forward if it is allowed, otherwise stop
void driveForwardIfAllowable(void);

//! Begin or continue turning
void turnContinuous(int16_t radius);

//! Called by overTurn periodically while turning.
//! Updates sensors and stops if unsafe to continue.
void doWhileTurning(void);

//! Turn an extra 30 degrees. Does not return until fully turned.
//! Calls doWhileTurning periodically for sensor updating and safety.
void overTurn(int16_t radius);

//! Called by irobPeriodic
void iroblifePeriodic(void);

#endif
