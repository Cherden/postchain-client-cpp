#ifndef POSTCHAIN_CLIENT_GTV_TAG_H_
#define POSTCHAIN_CLIENT_GTV_TAG_H_

namespace chromia {
namespace postchain {
namespace gtv {

namespace tag {
constexpr unsigned char kNull = 0xa0;
constexpr unsigned char kOctetString = 0xa1;
constexpr unsigned char kUTF8String = 0xa2;
constexpr unsigned char kInteger = 0xa3;
constexpr unsigned char kDict = 0xa4;
constexpr unsigned char kArray = 0xa5;
};  // namespace tag

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTV_TAG_H_