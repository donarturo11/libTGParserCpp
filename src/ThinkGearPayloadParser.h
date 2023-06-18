#ifndef TG_PAYLOAD_PARSER_H
#define TG_PAYLOAD_PARSER_H
#include "ThinkGearStreamParser.h"
#include "ThinkGearPayload.h"
#include <vector>
namespace libThinkGearCpp {
class ThinkGearPayloadParser
{
public:
    ThinkGearPayloadParser(ThinkGearPayloadPtr payload, ThinkGearStreamParser* parser);
    ~ThinkGearPayloadParser();
    void parseData();
private:
    void process(unsigned char c);
protected:
    ThinkGearPayloadPtr _payload;
    int _currentIndex;
    ThinkGearStreamParser *_parser;
};
}
#endif // TG_PAYLOAD_PARSER_H
