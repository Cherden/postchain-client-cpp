#ifndef ASNTAG_H_
#define ASNTAG_H_

namespace chromia {
namespace postchain {
namespace asn1 {

namespace tag {
constexpr unsigned char kNull = 0x05;
constexpr unsigned char kOctetString = 0x04;
constexpr unsigned char kUTF8String = 0x0c;
constexpr unsigned char kInteger = 0x02;
constexpr unsigned char kSequence = 0x30;
};  // namespace tag

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia

#endif // ASNTAG_H_