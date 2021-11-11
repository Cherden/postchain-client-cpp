#ifndef POSTCHAIN_CLIENT_ASN1_TAG_H_
#define POSTCHAIN_CLIENT_ASN1_TAG_H_

namespace chromia {
namespace postchain {
namespace asn1 {

namespace tag {
constexpr unsigned char kNull = 0xa0;
constexpr unsigned char kOctetString = 0xa1;
constexpr unsigned char kUTF8String = 0xa2;
constexpr unsigned char kInteger = 0x03;
constexpr unsigned char kDict = 0xa4;
constexpr unsigned char kArray = 0xa5;
constexpr unsigned char kSequence = 0x30;
};  // namespace tag

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_ASN1_TAG_H_