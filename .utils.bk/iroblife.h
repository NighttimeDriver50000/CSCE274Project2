#ifndef IROBLIFE_H
#define IROBLIFE_H

/*
 *  The irobPeriodic function in this library calls a function given to
 *  setIrobPeriodicImpl. The default value does nothing, but you can give
 *  it another function as a hook for periodically executed code.
 */

//! Default periodic function. Does nothing.
void irobPeriodicImplNull(void);
//! Set the function that irobPeriodic calls.
void setIrobPeriodicImpl(void (*func)(void));

//! Initialize the Create. Call this at the beginning of your main.
void irobInit(void);
//! Periodic operations. Call this in your main loop.
//! Calls the function last given to setIrobPeriodicImpl.
void irobPeriodic(void);
//! Stops and shuts down the Create, then exits. Call this to end the program.
void irobEnd(void);

#endif
