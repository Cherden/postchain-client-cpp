#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_PROOF_TREE_FACTORY_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_PROOF_TREE_FACTORY_H_

#include <memory>
#include <sstream>
#include <vector>

#include "../merkle_hash_calculator.h"
#include "merkle_proof_tree.h"

namespace chromia {
namespace postchain {
namespace gtv {

class MerkleProofTreeFactory {
  public:
    MerkleProofTreeFactory() {}

    std::shared_ptr<MerkleProofTree> BuildFromBinaryTree(
        std::shared_ptr<BinaryTree> original_tree,
        std::shared_ptr<MerkleHashCalculator> calculator) {
        auto root_element =
            BuildFromBinaryTreeInternal(original_tree->Root(), calculator);

        return std::make_shared<MerkleProofTree>(root_element);
    }

    const std::shared_ptr<MerkleProofElement> ConvertNode(
        std::shared_ptr<Node> node,
        std::shared_ptr<MerkleHashCalculator> calculator) const {
        auto left = BuildFromBinaryTreeInternal(node->Left(), calculator);
        auto right = BuildFromBinaryTreeInternal(node->Right(), calculator);

        auto left_converted = std::dynamic_pointer_cast<ProofHashedLeaf>(left);
        auto right_converted =
            std::dynamic_pointer_cast<ProofHashedLeaf>(right);
        if (left_converted == nullptr || right_converted == nullptr) {
            return BuildNodeOfCorrectType(node, left, right);
        }

        auto added_hash = calculator->CalculateNodeHash(
            node->GetPrefixByte(), left_converted->MerkleHash(),
            right_converted->MerkleHash());
        return std::make_shared<ProofHashedLeaf>(added_hash);
    }

  private:
    const std::shared_ptr<MerkleProofElement> BuildFromBinaryTreeInternal(
        std::shared_ptr<BinaryTreeElement> current_element,
        std::shared_ptr<MerkleHashCalculator> calculator) const {
        if (current_element->GetType() == BinaryTreeElement::Type::kEmptyLeaf) {
            return std::make_shared<ProofHashedLeaf>(
                std::vector<unsigned char>(32, 0x00));
        } else if (current_element->GetType() ==
                   BinaryTreeElement::Type::kLeaf) {
            auto leaf_element =
                std::dynamic_pointer_cast<Leaf>(current_element);
            auto path_element = current_element->GetPathElement();
            if (path_element != nullptr) {
                if (path_element->GetType() ==
                    PathElement::Type::kPathLeafElement) {
                    return std::make_shared<ProofValueLeaf>(
                        leaf_element->Content(), path_element->Previous());
                } else {
                    throw std::logic_error(
                        "The path and structure don't match. We are at a leaf, "
                        "but path elem is not a leaf.");
                }
            } else {
                auto hash =
                    calculator->CalculateLeafHash(leaf_element->Content());

                return std::make_shared<ProofHashedLeaf>(hash);
            }
        } else if (current_element->GetType() ==
                   BinaryTreeElement::Type::kSubTreeRootNode) {
            auto path_element = current_element->GetPathElement();
            if (path_element != nullptr) {
                if (path_element->GetType() ==
                    PathElement::Type::kPathLeafElement) {
                    auto tree_node_element =
                        std::dynamic_pointer_cast<SubTreeRootNode>(
                            current_element);
                    return std::make_shared<ProofValueLeaf>(
                        tree_node_element->Content(), path_element->Previous());
                } else {
                    auto node =
                        std::dynamic_pointer_cast<Node>(current_element);
                    return ConvertNode(node, calculator);
                }
            } else {
                auto node = std::dynamic_pointer_cast<Node>(current_element);
                return ConvertNode(node, calculator);
            }
        } else if (current_element->GetType() ==
                   BinaryTreeElement::Type::kNode) {
            auto node = std::dynamic_pointer_cast<Node>(current_element);
            return ConvertNode(node, calculator);
        } else {
            std::stringstream ss;
            ss << "cannot handle element " << (int)current_element->GetType();
            throw std::logic_error(ss.str());
        }
    }

    const std::shared_ptr<SearchablePathElement> ExtractSearchablePathElement(
        std::shared_ptr<Node> node) const {
        auto path_element = node->GetPathElement();

        return path_element != nullptr ? path_element->Previous() : nullptr;
    }

    const std::shared_ptr<ProofNode> BuildNodeOfCorrectType(
        std::shared_ptr<Node> node, std::shared_ptr<MerkleProofElement> left,
        std::shared_ptr<MerkleProofElement> right) const {
        if (std::dynamic_pointer_cast<ArrayHeadNode>(node)) {
            return std::make_shared<ProofNodeArrayHead>(
                left, right, ExtractSearchablePathElement(node));
        } else if (std::dynamic_pointer_cast<DictHeadNode>(node)) {
            return std::make_shared<ProofNodeDictHead>(
                left, right, ExtractSearchablePathElement(node));
        } else {
            return std::make_shared<ProofNodeSimple>(left, right);
        }
    }
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif