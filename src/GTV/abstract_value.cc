#include "abstract_value.h"

#include "../postchain_util.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

std::vector<unsigned char> AbstractValue::Encode() {
    asn1::Writer asn1_buffer;
    std::vector<unsigned char> encoded_bytes;

    unsigned char tag = AddContent(asn1_buffer);
    encoded_bytes.push_back(tag);

    int encoded_size = asn1_buffer.GetEncodedLength();
    if (encoded_size < 0x80) {
        encoded_bytes.push_back((unsigned char)encoded_size);
    } else {
        std::vector<unsigned char> length_bytes =
            PostchainUtil::LongToByteVector(encoded_size);

        encoded_bytes.push_back((unsigned char)0x80 + length_bytes.size());

        std::copy(length_bytes.begin(), length_bytes.end(),
                  std::back_inserter(encoded_bytes));
    }

    std::vector<unsigned char> asn1_encoded = asn1_buffer.Encode();
    std::copy(asn1_encoded.begin(), asn1_encoded.end(),
              std::back_inserter(encoded_bytes));

    return encoded_bytes;
}

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia