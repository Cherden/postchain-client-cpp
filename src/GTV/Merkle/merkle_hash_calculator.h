#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_HASH_CALCULATOR_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_HASH_CALCULATOR_H_

#include <memory>
#include <vector>

#include "../../postchain_util.h"
#include "../abstract_value.h"
#include "binary_tree.h"

namespace chromia {
namespace postchain {
namespace gtv {

class MerkleHashCalculator {
  public:
    MerkleHashCalculator() {}

    const std::vector<unsigned char> CalculateNodeHash(
        unsigned char prefix, const std::vector<unsigned char> hash_left,
        const std::vector<unsigned char> hash_right) const {
        std::vector<unsigned char> buffer;
        buffer.push_back(prefix);
        std::copy(hash_left.begin(), hash_left.end(),
                  std::back_inserter(buffer));
        std::copy(hash_right.begin(), hash_right.end(),
                  std::back_inserter(buffer));

        return HashingFun(buffer);
    }

    const std::vector<unsigned char> CalculateLeafHash(
        const std::shared_ptr<AbstractValue> value) const {
        std::vector<unsigned char> buffer;
        buffer.push_back(hash_prefix::kLeaf);

        auto encoded_value = value->Encode();
        std::copy(encoded_value.begin(), encoded_value.end(),
                  std::back_inserter(buffer));

        return HashingFun(buffer);
    }

  protected:
    const std::vector<unsigned char> HashingFun(
        std::vector<unsigned char> buffer) const {
        std::string hashed_buffer = PostchainUtil::Sha256(buffer);

        return PostchainUtil::HexStringToByteVector(hashed_buffer);
    }
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif