#include "ThinkGearStreamParser.h"
namespace libThinkGearCpp {
extern "C" {
void tgstream_parser_init(tgstream_parser_t *self, int type, tgvalue_handler* handler)
{
    if (self->handler) self->handler =  new ValueHandler_C(handler);
    self->parser = new ThinkGearStreamParser((ParserTypes) type);
}

void tgstream_parser_delete(tgstream_parser_t *self)
{
    if (self->handler) delete self->handler;
    delete self->parser;
}

void tgstream_parser_parseByte(tgstream_parser_t *self, unsigned char c)
{
    self->parser->parseByte(c);
}

int  tgstream_parser_available(tgstream_parser_t *self)
{
    return self->parser->availableData();
}

tgdata_t tgstream_parser_getData(tgstream_parser_t *self)
{
    return self->parser->getData();
}

} // extern "C"

} // namespace