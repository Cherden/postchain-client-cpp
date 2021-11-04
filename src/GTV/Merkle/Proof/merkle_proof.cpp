#include "merkle_proof.h"

namespace chromia {
namespace postchain {
namespace gtv {

std::shared_ptr<BinaryTreeFactory> MerkleProof::tree_factory =
    std::make_shared<BinaryTreeFactory>();

std::shared_ptr<MerkleProofTreeFactory> MerkleProof::proof_factory =
    std::make_shared<MerkleProofTreeFactory>();

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia
