#include "postchain_util.h"

#include <algorithm>
#include <iomanip>
#include <ios>
#include <sstream>

namespace chromia {
namespace postchain {

std::vector<unsigned char> PostchainUtil::LongToByteVector(
    long long int number) {
    std::vector<unsigned char> bytes;

    bool is_negative = false;
    if (number < 0) {
        number = number * -1;
        is_negative = true;
    }

    bool add_one = is_negative;
    for (int i = 0; i < sizeof(long long); i++) {
        unsigned char byte = number & (unsigned char)0xff;
        if (is_negative) {
            byte = ~byte;
        }

        bytes.push_back(add_one ? byte + 1 : byte);
        if (byte != (unsigned char)0xff) {
            add_one = false;
        }

        number >>= 8;

        if (number == 0) {
            break;
        }
    }

    if (IsLittleEndian()) {
        std::reverse(bytes.begin(), bytes.end());
    }

    return bytes;
}

long long PostchainUtil::ByteVectorToLong(std::vector<unsigned char> bytes,
                                          bool mind_sign) {
    if (bytes.size() > sizeof(long long)) {
        std::stringstream ss;
        ss << "too many bytes for long long, expected max " << sizeof(long long)
           << ", got " << bytes.size() << std::endl;
        throw std::out_of_range(ss.str());
    }

    // pad out
    unsigned char padding_byte =
        bytes.front() >= (unsigned char)0x80 && !mind_sign ? 0xff : 0x00;
    for (int i = bytes.size(); i < sizeof(long long); i++) {
        bytes.insert(bytes.begin(), padding_byte);
    }

    long long number = 0;
    for (auto byte : bytes) {
        number = (number << 8) | byte;
    }

    return number;
}

std::vector<unsigned char> PostchainUtil::HexStringToByteVector(
    const std::string& hex_string) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hex_string.length(); i += 2) {
        unsigned char byte = stoi(hex_string.substr(i, 2), nullptr, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

std::string PostchainUtil::ByteVectorToHexString(
    const std::vector<unsigned char>& bytes) {
    std::stringstream ss;

    ss << std::hex << std::uppercase << std::setfill('0');
    for (auto byte : bytes) {
        ss << std::setw(2) << (int)byte;
    }

    return ss.str();
}

bool PostchainUtil::IsLittleEndian() {
    int n = 1;
    return *(char*)&n == 1;
}

}  // namespace postchain
}  // namespace chromia