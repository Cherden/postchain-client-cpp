#ifndef POSTCHAIN_CLIENT_ASN1_WRITER_H_
#define POSTCHAIN_CLIENT_ASN1_WRITER_H_

#include <string>
#include <vector>

namespace chromia {
namespace postchain {
namespace asn1 {

class Writer {
  public:
    explicit Writer();
    Writer(const Writer& old_obj) = delete;
    Writer& operator=(const Writer& other) = delete;

    void WriteNull();
    void WriteOctetString(const std::vector<unsigned char>& octet_string);
    void WriteUTF8String(const std::string& character_string);
    void WriteInteger(long long number);
    void WriteEncodedValue(const std::vector<unsigned char>& bytes);

    void PushSequence();
    void PopSequence();

    const int GetEncodedLength();
    const std::vector<unsigned char> Encode();

  private:
    template <typename Iter>
    void Write(unsigned char tag, Iter begin, Iter end);

    std::vector<unsigned char> GetLengthBytes(unsigned int length);
    Writer* GetOpenSequence();

    std::vector<unsigned char> buffer_;
    std::vector<Writer*> sequences_;
};

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_ASN1_WRITER_H_