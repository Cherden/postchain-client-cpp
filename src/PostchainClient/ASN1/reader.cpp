#include "reader.h"

#include <sstream>

#include "../postchain_util.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace asn1 {

Reader::Reader(const std::vector<unsigned char> bytes) : bytes_(bytes) {}

unsigned char Reader::ReadChoice() { return ReadByte(); }

Reader Reader::ReadSequence() {
    ReadByte(&tag::kSequence);

    auto length = ReadLength();
    std::vector<unsigned char> sequence(bytes_.begin(),
                                        bytes_.begin() + length);
    bytes_.erase(bytes_.begin(), bytes_.begin() + length);

    return Reader(sequence);
}

void Reader::ReadNull() {
    ReadByte(&tag::kNull);
    ReadByte();
}

std::vector<unsigned char> Reader::ReadOctetString() {
    ReadByte(&tag::kOctetString);
    auto length = ReadLength();

    return ReadBytes(length);
}

std::string Reader::ReadUTF8String() {
    ReadByte(&tag::kUTF8String);
    auto length = ReadLength();

    std::vector<unsigned char> buffer = ReadBytes(length);
    return std::string(buffer.begin(), buffer.end());
}

long long int Reader::ReadInteger() {
    ReadByte(&tag::kInteger);

    return ReadIntegerInternal(ReadLength());
}

unsigned char Reader::ReadByte(const unsigned char* expected) {
    if (bytes_.size() == 0) {
        throw std::logic_error("tried to read from empty buffer.");
    }

    unsigned char got = bytes_[0];
    if (expected != nullptr && *expected != got) {
        std::stringstream ss;
        ss << "expected byte x" << std::hex << (int)*expected << ", got x"
           << (int)got << std::dec;
        throw std::logic_error(ss.str());
    }

    bytes_.erase(bytes_.begin());

    return got;
}

std::vector<unsigned char> Reader::ReadBytes(std::size_t length) {
    std::vector<unsigned char> buffer;

    for (int i = 0; i < length; i++) {
        buffer.push_back(ReadByte());
    }

    return buffer;
}

std::size_t Reader::ReadLength() {
    unsigned char first = ReadByte();

    if (first < 0x80) {
        return first;
    } else {
        auto length = ReadIntegerInternal(first - 0x80, true);
        return length;
    }
}

long long Reader::ReadIntegerInternal(unsigned int byte_amount,
                                      bool mind_sign) {
    if (bytes_.size() < byte_amount) {
        throw std::logic_error("too few bytes left to read.");
    } else if (byte_amount > sizeof(long long)) {
        throw std::logic_error("tried to read too many bytes.");
    }

    std::vector<unsigned char> buffer{bytes_.begin(),
                                      bytes_.begin() + byte_amount};
    bytes_.erase(bytes_.begin(), bytes_.begin() + byte_amount);

    return PostchainUtil::ByteVectorToLong(buffer, mind_sign);
}

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia
