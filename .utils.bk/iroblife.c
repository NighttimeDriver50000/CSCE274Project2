#include <stdlib.h>
#include <stdint.h>
#include "iroblife.h"

#include "timer.h"
#include "cmod.h"
#include "iroblib.h"
#include "oi.h"

#include "sensing.h"
#include "irobled.h"
#include "driving.h"

void irobPeriodicImplNull(void) {
}

void (*irobPeriodicImpl)(void) = &irobPeriodicImplNull;

void setIrobPeriodicImpl(void (*func)(void)) {
    irobPeriodicImpl = func;
}

void irobInit(void) {
    // Set up Create and module
    initializeCommandModule();
    // Set up USART-based sensing
    setupSensing();

    // Is the Robot on
    powerOnRobot();
    // Start the create
    byteTx(CmdStart);
    // Set the baud rate for the Create and Command Module
    baud(Baud57600);
    // Define some songs so that we know the robot is on.
    defineSongs();
    // Deprecated form of safe mode. I use it because it will
    // turn of all LEDs, so it's essentially a reset.
    byteTx(CmdControl);
    // We are operating in FULL mode.
    byteTx(CmdFull);

    // Make sure the robot stops. 
    // As a precaution for the robot and your grade.
    driveStop();

    // Play the reset song and wait while it plays.
    byteTx(CmdPlay);
    byteTx(RESET_SONG);
    delayMs(750);

    // Turn the power button on to red.
    irobledInit();
}

void irobPeriodic(void) {
    // Call the user's periodic function
    irobPeriodicImpl();
    // Exit if the black button on the command module is pressed.
    if(UserButtonPressed) {
        irobEnd();
    }
}

void irobEnd(void) {
    // Stop the Create
    driveStop();
    // Power off the Create
    powerOffRobot();
    // Exit the program
    exit(1);
}
