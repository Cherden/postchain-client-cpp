#include "writer.h"

#include <memory>

#include "../postchain_util.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace asn1 {

Writer::Writer() {}

void Writer::WriteNull() {
    std::vector<unsigned char>& buffer = GetOpenSequence()->buffer_;

    buffer.push_back(tag::kNull);
    buffer.push_back(0x00);
}

void Writer::WriteOctetString(const std::vector<unsigned char>& octet_string) {
    Write(tag::kOctetString, octet_string.begin(), octet_string.end());
}

void Writer::WriteUTF8String(const std::string& character_string) {
    Write(tag::kUTF8String, character_string.begin(), character_string.end());
}

void Writer::WriteInteger(long long number) {
    std::vector<unsigned char> bytes = PostchainUtil::LongToByteVector(number);

    Write(tag::kInteger, bytes.begin(), bytes.end());
}

void Writer::WriteEncodedValue(const std::vector<unsigned char>& bytes) {
    std::vector<unsigned char>& buffer = GetOpenSequence()->buffer_;

    std::copy(bytes.begin(), bytes.end(), std::back_inserter(buffer));
}

void Writer::PushSequence() { sequences_.push_back(new Writer()); }

void Writer::PopSequence() {
    auto writer = GetOpenSequence();
    sequences_.pop_back();

    auto content = writer->Encode();
    Write(tag::kSequence, content.begin(), content.end());
    delete writer;
}

template <typename Iter>
void Writer::Write(unsigned char tag, Iter begin, Iter end) {
    std::vector<unsigned char>& buffer = GetOpenSequence()->buffer_;

    buffer.push_back(tag);

    auto count = std::distance(begin, end);
    auto length_bytes = GetLengthBytes(count);

    std::copy(length_bytes.begin(), length_bytes.end(),
              std::back_inserter(buffer));
    std::copy(begin, end, std::back_inserter(buffer));
}

const int Writer::GetEncodedLength() {
    return GetOpenSequence()->buffer_.size();
}

const std::vector<unsigned char> Writer::Encode() {
    if (sequences_.size() != 0) {
        throw std::logic_error("tried to encode with open sequence.");
    }

    return buffer_;
}

std::vector<unsigned char> Writer::GetLengthBytes(unsigned int length) {
    if (length < 128) {
        return std::vector<unsigned char>{(unsigned char)length};
    } else {
        auto length_bytes = PostchainUtil::LongToByteVector(length);

        std::vector<unsigned char> return_bytes{
            (unsigned char)(0x80 + length_bytes.size())};
        std::copy(length_bytes.begin(), length_bytes.end(),
                  std::back_inserter(return_bytes));

        return return_bytes;
    }
}

Writer* Writer::GetOpenSequence() {
    return sequences_.size() == 0 ? this : sequences_.back();
}

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia