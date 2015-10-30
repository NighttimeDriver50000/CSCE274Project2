#ifndef SENSING_H
#define SENSING_H

#include <stdint.h>

#define UPDATE_SENSOR_DELAY_CUTOFF      (10)


#define PACKET_BUMPS_AND_WHEEL_DROPS    (7)
#define MASK_WHEEL_DROP_CASTER          (1 << 4)
#define MASK_WHEEL_DROP_LEFT            (1 << 3)
#define MASK_WHEEL_DROP_RIGHT           (1 << 2)
#define MASK_WHEEL_DROP                 (0x1C)
#define MASK_BUMP_LEFT                  (1 << 1)
#define MASK_BUMP_RIGHT                 (1 << 0)
#define MASK_BUMP                       (0x03)

#define PACKET_BUTTONS                  (18)
#define MASK_BTN_ADVANCE                (1 << 2)
#define MASK_BTN_PLAY                   (1 << 0)

#define PACKET_ALL                      (6)

void requestPacket(uint8_t packetId);

//! Read in a 1-byte sensor packet.
/*!
 *  \deprecated {
 *      This uses the old, non-USART-based way of retrieving sensor data.
 *  }
 *  
 *  What is a sensor packet? A byte (or bytes) containing data from a set of
 *  sensors, often shifted and ORed together. See the Create Open Interface
 *  documentation for more.
 *
 *  Currently Available Sensor Packets  (v = read1ByteSensorPacket(packetId)):
 *      Bumps and Wheel Drops   (packetId = PACKET_BUMPS_AND_WHEEL_DROPS):
 *          Caster Drop         (v & MASK_WHEEL_DROP_CASTER)
 *          Left Wheel Drop     (v & MASK_WHEEL_DROP_LEFT)
 *          Right Wheel Drop    (v & MASK_WHEEL_DROP_RIGHT)
 *          Any Wheel Drop      (v & MASK_WHEEL_DROP)
 *          Left Bumper         (v & MASK_BUMP_LEFT)
 *          Right Bumper        (v & MASK_BUMP_RIGHT)
 *          Either Bumper       (v & MASK_BUMPER)
 *      Create Buttons          (packetId = PACKET_BUTTONS):
 *          Advance Button      (v & MASK_BTN_ADVANCE)
 *          Play Button         (v & MASK_BTN_PLAY)
 *
 *  \param packetId     The ID of the packet to retrieve, as defined by the
 *                      Create Open Interface.
 */
uint8_t read1ByteSensorPacket(uint8_t packetId);

void updateSensors(void);

void delayAndUpdateSensors(uint32_t time_ms);

#endif
