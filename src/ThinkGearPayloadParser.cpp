#include "ThinkGearPayloadParser.h"

namespace libThinkGearCpp {
ThinkGearPayloadParser::ThinkGearPayloadParser(ThinkGearPayloadPtr payload, ThinkGearStreamParser* parser)
: _currentIndex{0}, _parser{parser}
{
    _payload = std::move(payload);
	parseData();
}

ThinkGearPayloadParser::~ThinkGearPayloadParser()
{}

void ThinkGearPayloadParser::parseData()
{
    while (_currentIndex < _payload->size()) {
        process(_payload->at(_currentIndex));
    }
}

void ThinkGearPayloadParser::process(unsigned char c)
{
    tgdata_t data;
    data.code=_payload->at(_currentIndex);
    _currentIndex++;
    if (data.code < 0x80 ) {
        data.size = 1;
    } else {
        data.size = _payload->at(_currentIndex);
        _currentIndex++;
    }

    for (int i=0; i<data.size; i++) {
        data.value[i] = (_payload->at(_currentIndex));
        _currentIndex++;
    }
    _parser->pushToQueue(data);
}

} // namespace libThinkGearCpp
