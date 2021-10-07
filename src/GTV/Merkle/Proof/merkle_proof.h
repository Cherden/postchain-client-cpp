#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_PROOF_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_PROOF_H_

#include <vector>

#include "../../abstract_value.h"
#include "../binary_tree_factory.h"
#include "../merkle_hash_calculator.h"
#include "merkle_hash_summary_factory.h"
#include "merkle_proof_tree_factory.h"

namespace chromia {
namespace postchain {
namespace gtv {

class MerkleProof {
  public:
    static std::vector<unsigned char> MerkleHash(
        std::shared_ptr<AbstractValue> value,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        MerkleHashSummaryFactory summary_factory(tree_factory, proof_factory);
        return summary_factory.CalculateMerkleRoot(value, calculator)
            ->MerkleHash();
    }

  private:
    static std::shared_ptr<BinaryTreeFactory> tree_factory;
    static std::shared_ptr<MerkleProofTreeFactory> proof_factory;
};

std::shared_ptr<BinaryTreeFactory> MerkleProof::tree_factory =
    std::make_shared<BinaryTreeFactory>();

std::shared_ptr<MerkleProofTreeFactory> MerkleProof::proof_factory =
    std::make_shared<MerkleProofTreeFactory>();

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif