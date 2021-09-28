#ifndef POSTCHAINUTIL_H_
#define POSTCHAINUTIL_H_

#include <string>
#include <vector>

namespace chromia {
namespace postchain {

class PostchainUtil {
  public:
    static std::vector<unsigned char> LongToByteVector(long long number);
    static long long ByteVectorToLong(std::vector<unsigned char> bytes,
                                      bool mind_sign = false);
    static std::vector<unsigned char> HexStringToByteVector(
        const std::string& hex_string);
    static std::string ByteVectorToHexString(
        const std::vector<unsigned char>& bytes);
    static bool IsLittleEndian();
};

}  // namespace postchain
}  // namespace chromia

#endif