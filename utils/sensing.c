#include <stdint.h>
#include "sensing.h"
#include "cmod.h"
#include "oi.h"

uint8_t read1ByteSensorPacket(uint8_t packetId) {
    // Send the packet ID
    byteTx(CmdSensors);
    byteTx(packetId);
    // Read the packet byte
    return byteRx();
}
