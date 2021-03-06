#ifndef POSTCHAIN_CLIENT_ASN1_READER_H_
#define POSTCHAIN_CLIENT_ASN1_READER_H_

#include <string>
#include <vector>

namespace chromia {
namespace postchain {
namespace asn1 {

class Reader {
  public:
    explicit Reader(const std::vector<unsigned char> bytes);
    Reader& operator=(const Reader& other) = delete;

    unsigned char ReadChoice();
    void ReadNull();
    Reader ReadSequence();
    std::vector<unsigned char> ReadOctetString();
    std::string ReadUTF8String();
    long long ReadInteger();

    std::size_t RemainingBytes() { return bytes_.size(); }

	std::size_t ReadLength();

  private:
    unsigned char ReadByte(const unsigned char* expected = nullptr);
    std::vector<unsigned char> ReadBytes(std::size_t length);
   
    long long ReadIntegerInternal(unsigned int byte_amount,
                                  bool mind_sign = false);

    std::vector<unsigned char> bytes_;
};

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_ASN1_READER_H_