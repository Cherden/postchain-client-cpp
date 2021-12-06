#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_HASH_CARRIER_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_HASH_CARRIER_H_

#include <memory>
#include <vector>

namespace chromia {
namespace postchain {
namespace gtv {

class MerkleHashSummary {
  public:
    MerkleHashSummary(std::vector<unsigned char> merkle_hash)
        : merkle_hash_(merkle_hash) {}

    bool operator==(const MerkleHashSummary &b) const {
        return merkle_hash_ == b.merkle_hash_;
    }

    const std::vector<unsigned char> MerkleHash() { return merkle_hash_; }

  protected:
    std::vector<unsigned char> merkle_hash_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif