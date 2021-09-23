#ifndef ASNREADER_H_
#define ASNREADER_H_

#include <memory>
#include <string>
#include <vector>

namespace chromia {
namespace postchain {
namespace asn1 {

class Reader {
  public:
    explicit Reader(const std::vector<unsigned char> bytes);
    Reader(const Reader& old_obj) = delete;
    Reader& operator=(const Reader& other) = delete;

    unsigned char ReadChoice();
    void ReadNull();
    std::shared_ptr<Reader> ReadSequence();
    std::vector<unsigned char> ReadOctetString();
    std::string ReadUTF8String();
    long long ReadInteger();

    std::size_t RemainingBytes() { return bytes_.size(); }

  private:
    unsigned char ReadByte(const unsigned char* expected = nullptr);
    std::vector<unsigned char> ReadBytes(std::size_t length);
    std::size_t ReadLength();
    long long ReadIntegerInternal(unsigned int byte_amount,
                                  bool mind_sign = false);

    std::vector<unsigned char> bytes_;
};

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia

#endif  // ASNREADER_H_