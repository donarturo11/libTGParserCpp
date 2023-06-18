#ifndef THINKGEARSTREAMPARSER_H
#define THINKGEARSTREAMPARSER_H
#if defined _WIN32 || defined __CYGWIN__
  #if defined(EXPORT)
    #define TGDLL __declspec(dllexport)
  #else
    #define TGDLL
  #endif
#else
  #if __GNUC__ >= 4
    #define TGDLL __attribute__( (visibility( "default" )) )
  #else
    #define TGDLL
  #endif
#endif

#include <vector>
#include <queue>
#include <memory>
#include "TGEnums.h"

#ifdef __cplusplus
extern "C" {
namespace libThinkGearCpp {
#endif
typedef struct _tgdata_t
{
    char code;
    int  size;
    char value[64];
} tgdata_t;
typedef struct _tgvalue_handler {
    void* customData;
    void (*handleData)(void* customData, tgdata_t d);
    void* handler_obj;
} tgvalue_handler;
#ifdef __cplusplus
}
}
#endif

namespace libThinkGearCpp {
class ThinkGearPayload;
class ThinkGearValueHandler
{
public:
    virtual ~ThinkGearValueHandler() {};
    virtual void pushData(tgdata_t data) = 0;
};

class TGDLL ThinkGearStreamParser
{
public:
    ThinkGearStreamParser(ParserTypes type = ParserTypes::Packets, ThinkGearValueHandler *_handler = 0);
    ~ThinkGearStreamParser();
    void parseByte(unsigned char byte);
    bool isInited() const { return _status != ParserStatus::Null; }
    int availableData() { return _received_data.size(); }
    tgdata_t getData();
    std::vector<tgdata_t> getAllData();
    void pushToQueue(tgdata_t data);
private:
    void init();
    void receiveSyncByte();
    void reset();
    void initPayload();
    void writePayload();
    void checkPayload();
    void parsePayload();
    void twoByteRawInit();
    void twoByteRawParse();
protected:
    ParserTypes     _type;
    ParserStatus    _status;
    ReceiveStatus   _receive_status;
    unsigned char   _lastByte;
    std::unique_ptr<ThinkGearPayload> _payload;
    ThinkGearValueHandler* _handler;
    std::queue<tgdata_t>  _received_data;
    int _queue_capacity;
};

class ValueHandler_C : public ThinkGearValueHandler
{
public:
    ValueHandler_C(tgvalue_handler* handler) : _handler(handler){
        _handler->handler_obj=this;
        }
    void pushData(tgdata_t data){ _handler->handleData(_handler, data); }
    tgvalue_handler* _handler;
};

extern "C" {
typedef struct _tgstream_parser_t {
    ThinkGearStreamParser *parser;
    ThinkGearValueHandler *handler;
} tgstream_parser_t;

TGDLL void tgstream_parser_init(tgstream_parser_t *self, int type, tgvalue_handler* handler);
TGDLL void tgstream_parser_delete(tgstream_parser_t *self);
TGDLL void tgstream_parser_parseByte(tgstream_parser_t *self, unsigned char c);
TGDLL int  tgstream_parser_available(tgstream_parser_t *self);
TGDLL tgdata_t tgstream_parser_getData(tgstream_parser_t *self);

}

} // namespace libThinkGearCpp

#endif // THINKGEARSTREAMPARSER_H
