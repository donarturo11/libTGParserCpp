#include "ThinkGearStreamParser.h"
#include "ThinkGearPayloadParser.h"
#include "ThinkGearPayload.h"
#include "TGEnums.h"
#include <iostream>
#include <memory>
namespace libThinkGearCpp {
ThinkGearStreamParser::ThinkGearStreamParser(ParserTypes type, ThinkGearValueHandler *handler)
: _type{type}, _handler{handler}
{
    init();
}

ThinkGearStreamParser::~ThinkGearStreamParser()
{

}

void ThinkGearStreamParser::init()
{
    _queue_capacity = 1024;
    switch (_type) {
        case( ParserTypes::Packets ):
            _status = ParserStatus::Idle;
            break;
        case( ParserTypes::TwoByteRaw ):
            _status = ParserStatus::WaitHigh;
            break;
        default:
            _status = ParserStatus::Null;
    }
}

void ThinkGearStreamParser::parseByte(unsigned char byte)
{
    if (!isInited()) return;
    _lastByte = byte;
    switch (_status) {
        case( ParserStatus::Idle ): receiveSyncByte(); break;
        case( ParserStatus::Sync ): receiveSyncByte(); break;
        case( ParserStatus::PayloadLength ): initPayload(); break;
        case( ParserStatus::Payload ): writePayload(); break;
        case( ParserStatus::Checksum ): checkPayload(); break;
        case( ParserStatus::WaitHigh ): twoByteRawInit(); break;
        case( ParserStatus::WaitLow ): twoByteRawParse(); break;
        default: _receive_status = ReceiveStatus::Unrecognized;
    }
    
}

void ThinkGearStreamParser::receiveSyncByte()
{
    if (_lastByte != ParserCodes::SyncByte) {
        reset();
        return;
    }
    
    switch (_status) { 
        case ( ParserStatus::Idle ): _status = ParserStatus::Sync; break;
        case ( ParserStatus::Sync ): _status = ParserStatus::PayloadLength; break;
        default: reset(); return;
    }
}

void ThinkGearStreamParser::reset()
{
    _payload.release();
    _status = ParserStatus::Idle;
}

void ThinkGearStreamParser::initPayload()
{ 
    if (_lastByte > ParserCodes::SyncByte) {
        _receive_status = ReceiveStatus::PlenghtTooLarge;
        reset();
        return;
    } else if (_lastByte == ParserCodes::SyncByte) {
        _receive_status = ReceiveStatus::InvalidWithSync;
        reset();
        return;
    }
    _payload = std::make_unique<ThinkGearPayload>(_lastByte);
    _status = ParserStatus::Payload;
    
}

void ThinkGearStreamParser::writePayload()
{
    _payload->writeByte(_lastByte);
    if (_payload->completed()) _status = ParserStatus::Checksum;
}

void ThinkGearStreamParser::checkPayload()
{
    _payload->setChecksum(_lastByte);
    if (_payload->isValid()) {
        _receive_status = ReceiveStatus::Success;
        parsePayload();
    } else {
        _receive_status = ReceiveStatus::ChecksumFailed;
    }
    reset();
}

void ThinkGearStreamParser::parsePayload()
{
    ThinkGearPayloadParser payloadParser(std::move(_payload), this);
    reset();
}

void ThinkGearStreamParser::twoByteRawInit()
{
    _payload = std::make_unique<ThinkGearPayload>(4);
    _payload->writeByte(0x80);
    _payload->writeByte(0x02);
    _payload->writeByte(_lastByte);
}

void ThinkGearStreamParser::twoByteRawParse()
{
    _payload->writeByte(_lastByte);
    parsePayload();
}

void ThinkGearStreamParser::pushToQueue(tgdata_t data)
{
    if (_handler) _handler->pushData(data);
    if (_received_data.size() == _queue_capacity) _received_data.pop();
    _received_data.push(data);
}

tgdata_t ThinkGearStreamParser::getData()
{
    auto data = _received_data.front();
    _received_data.pop();
    return data;
}

std::vector<tgdata_t> ThinkGearStreamParser::getAllData()
{
    std::vector<tgdata_t> v;
    while (! _received_data.empty()) {
        v.push_back(getData());
    }
    return v;
}

} // namespace libThinkGearCpp
