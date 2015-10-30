#ifndef DRIVING_H
#define DRIVING_H

#include <stdint.h>

// # BASIC COMMANDS #

//! Drive at a certain speed in a certain direction.
/*!
 *  Returns immediately.
 *
 *  Directions: straight, clockwise, counterclockwise.
 *
 *  \param velocity     The speed in mm/s.
 *  \param radius       Either RadStraight, RadCW, or RadCCW (see oi.h).
 */
void drive(int16_t velocity, int16_t radius);

//! Stop the robot.
void driveStop(void);


// # OPCODE-BASED COMMANDS #

//! Drive a certain distance at a certain speed.
/*!
 *  Drive a certain distance using the Create wait for distance opcode.
 *  
 *  \param velocity     The speed in mm/s.
 *  \param distance     The distance to travel in mm.
 */
void driveDistanceOp(int16_t velocity, int16_t distance);

//! Rotate a certain angle at a certain speed.
/*!
 *  Drive a certain angle using the Create wait for angle opcode.
 *
 *  \param velocity     The speed in mm/s.
 *  \param radius       Either RadCW or RadCCW (see oi.h).
 *  \param angle        The angle to rotate in degrees.
 */
void driveAngleOp(int16_t velocity, int16_t radius, int16_t angle);


// # TIMER-BASED COMMANDS #

//! Drive a certain distance at a certain speed.
/*!
 *  Drive a certain distance using a timer.
 *  
 *  \param velocity     The speed in mm/s.
 *  \param distance     The distance to travel in mm.
 *  \param func         The function to execute periodically.
 *  \param period_ms    The interval to execute the function.
 *  \param cutoff_ms    The number of milliseconds before the end to stop
 *                      attempting to start the function.
 */
void driveDistanceTFunc(int16_t velocity, int16_t distance, void (*func)(void),
        uint16_t period_ms, uint16_t cutoff_ms);

//! Drive a certain angle at a certain speed.
/*!
 *  Drive a certain angle using a timer.
 *
 *  \param velocity     The speed in mm/s.
 *  \param radius       Either RadCW or RadCCW (see oi.h).
 *  \param angle        The angle to rotate in degrees.
 *  \param func         The function to execute periodically.
 *  \param period_ms    The interval to execute the function.
 *  \param cutoff_ms    The number of milliseconds before the end to stop
 *                      attempting to start the function.
 */
void driveAngleTFunc(int16_t velocity, int16_t radius, int16_t angle,
        void (*func)(void), uint16_t period_ms, uint16_t cutoff_ms);

#endif
