#ifndef TGENUMS_H
#define TGENUMS_H
namespace libThinkGearCpp {
typedef enum {
    Null = 0x0,
    Idle,
    Sync,
    PayloadLength,
    Payload,
    Checksum,
    WaitHigh,
    WaitLow
} ParserStatus;
typedef enum {
    Battery=0x01,
    PoorSignal,
    HeartRate,
    Attention,
    Meditation,
    Raw8Bit,
    RawMarker,
    RawSignal=0x80,
    EegPowers,
    AsicEegPowerInt=0x83,
    SyncByte=0xAA,
    ExcodeByte=0x55
} ParserCodes;
typedef enum {
    Unrecognized=-5,
    InvalidWithSync,
    PlenghtTooLarge,
    ChecksumFailed,
    ParserIsNotInitialized,
    PacketIsNotComplete,
    Success
} ReceiveStatus;

typedef enum {
    None=0x0,
    Packets,
    TwoByteRaw
} ParserTypes;

} // namespace libThinkGearCpp

#endif // TGENUMS_H