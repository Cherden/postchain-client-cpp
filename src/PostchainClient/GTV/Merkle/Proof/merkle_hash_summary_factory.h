#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_SUMMARY_FACTORY_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_SUMMARY_FACTORY_H_

#include <vector>

#include "../binary_tree_factory.h"
#include "../merkle_hash_calculator.h"
#include "merkle_hash_carrier.h"
#include "merkle_proof_tree_factory.h"

namespace chromia {
namespace postchain {
namespace gtv {

class MerkleHashSummaryFactory {
  public:
    MerkleHashSummaryFactory(
        std::shared_ptr<BinaryTreeFactory> tree_factory,
        std::shared_ptr<MerkleProofTreeFactory> proof_factory)
        : tree_factory_(tree_factory), proof_factory_(proof_factory) {}

    std::shared_ptr<MerkleHashSummary> CalculateMerkleRoot(
        std::shared_ptr<AbstractValue> value,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        auto binary_tree = tree_factory_->Build(value);
        auto proof_tree =
            proof_factory_->BuildFromBinaryTree(binary_tree, calculator);

        return CalculateMerkleRootOfTree(proof_tree, calculator);
    }

    std::shared_ptr<MerkleHashSummary> CalculateMerkleTreeRoot(
        std::shared_ptr<MerkleProofTree> tree,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        return CalculateMerkleRootOfTree(tree, calculator);
    }

    std::shared_ptr<MerkleHashSummary> CalculateMerkleRootOfTree(
        std::shared_ptr<MerkleProofTree> proof_tree,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        auto calculated_summary =
            CalculateMerkleRootInternal(proof_tree->Root(), calculator);

        return std::make_shared<MerkleHashSummary>(calculated_summary);
    }

    std::vector<unsigned char> CalculateMerkleRootInternal(
        std::shared_ptr<MerkleProofElement> current_element,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        if (current_element->GetType() ==
            MerkleProofElement::Type::kProofHashedLeaf) {
            auto leaf_element =
                std::dynamic_pointer_cast<ProofHashedLeaf>(current_element);
            return leaf_element->MerkleHash();
        } else if (current_element->GetType() ==
                   MerkleProofElement::Type::kProofValueLeaf) {
            auto value_element =
                std::dynamic_pointer_cast<ProofValueLeaf>(current_element);

            return calculator->CalculateLeafHash(value_element->Content());
        } else if (current_element->GetType() ==
                   MerkleProofElement::Type::kProofNode) {
            auto proof_element =
                std::dynamic_pointer_cast<ProofNode>(current_element);
            auto left =
                CalculateMerkleRootInternal(proof_element->Left(), calculator);
            auto right =
                CalculateMerkleRootInternal(proof_element->Right(), calculator);

            return calculator->CalculateNodeHash(proof_element->Prefix(), left,
                                                 right);
        } else {
            throw std::logic_error("Should have handled this type?");
        }
    }

    std::shared_ptr<MerkleProofTree> BuildProofTree(
        std::shared_ptr<AbstractValue> value,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        auto root = tree_factory_->Build(value);

        return proof_factory_->BuildFromBinaryTree(root, calculator);
    }

  private:
    std::shared_ptr<BinaryTreeFactory> tree_factory_;
    std::shared_ptr<MerkleProofTreeFactory> proof_factory_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif