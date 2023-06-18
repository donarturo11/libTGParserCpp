#include "ThinkGearPayload.h"
#include <iostream>
namespace libThinkGearCpp {

ThinkGearPayload::ThinkGearPayload(int pLength)
{
    _length = pLength;
    _payloadSum = 0;
}

ThinkGearPayload::ThinkGearPayload(unsigned char *buffer, int pLength)
{
    _length = pLength;
    _payloadSum = 0;
    while (!completed()) {
        writeByte(*buffer++);
    }
}

ThinkGearPayload::~ThinkGearPayload()
{
}

void ThinkGearPayload::writeByte(unsigned char byte)
{
    if (completed()) return;
    _payload.push_back(byte);
    _payloadSum += (int) byte;
}

void ThinkGearPayload::clear()
{ 
    _payload.clear();
    _payloadSum = 0;
}

} // namespace libThinkGearCpp
