#ifndef THINKGEAR_PAYLOAD_H
#define THINKGEAR_PAYLOAD_H

#include <vector>
#include <memory>
namespace libThinkGearCpp {

class ThinkGearPayload
{
public:
    ThinkGearPayload(int pLength);
    ThinkGearPayload(unsigned char* buffer, int pLength);
    ~ThinkGearPayload();
    void operator<< (unsigned char byte) { writeByte(byte); }
    unsigned char operator[](int i) { return at(i); }
    void writeByte(unsigned char byte);
    void clear();
    void setChecksum(char chksum) { _checksum = chksum; }
    unsigned char packetChecksum() const { return _checksum; }
    unsigned char expectedChecksum() const { return ((~_payloadSum) & 0xFF); }
    const unsigned char* data() { return _payload.data(); }
    int size() const { return _payload.size(); }
    unsigned char at(int i) const { return _payload.at(i); }
    int capacity() const { return _length; }
    bool completed() const { return _payload.size() == _length; }
    bool isValid() const { return packetChecksum() == expectedChecksum(); }
    bool isValid(char chksum) const { return chksum == expectedChecksum(); }
private:
    char calculateChecksum();
protected:
    int _length;
    int _payloadSum;
    std::vector<unsigned char> _payload;
    char _checksum;
};

typedef std::unique_ptr<ThinkGearPayload> ThinkGearPayloadPtr;

} // namespace libThinkGearCpp
#endif // THINKGEAR_PAYLOAD_H
