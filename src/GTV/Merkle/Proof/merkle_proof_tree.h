#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_PROOF_TREE_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_PROOF_TREE_H_

#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>

#include "../../abstract_value.h"
#include "../binary_tree.h"
#include "../path.h"

namespace chromia {
namespace postchain {
namespace gtv {

class MerkleProofElement {
  public:
    enum class Type {
        kProofNode,
        kProofNodeSimple,
        kProofValueLeaf,
        kProofHashedLeaf,
        kProofNodeArrayHead,
        kProofNodeDictHead
    };
    virtual Type GetType() = 0;
};

class ProofNode : public MerkleProofElement {
  public:
    ProofNode(unsigned char prefix, std::shared_ptr<MerkleProofElement> left,
              std::shared_ptr<MerkleProofElement> right)
        : prefix_(prefix), left_(left), right_(right) {}

    const unsigned char Prefix() { return prefix_; }
    const std::shared_ptr<MerkleProofElement> Left() { return left_; }
    const std::shared_ptr<MerkleProofElement> Right() { return right_; }
    virtual Type GetType() override { return Type::kProofNode; }

  protected:
    unsigned char prefix_;
    std::shared_ptr<MerkleProofElement> left_;
    std::shared_ptr<MerkleProofElement> right_;
};

class ProofNodeSimple : public ProofNode {
  public:
    ProofNodeSimple(std::shared_ptr<MerkleProofElement> left,
                    std::shared_ptr<MerkleProofElement> right)
        : ProofNode(hash_prefix::kNode, left, right) {}
    virtual Type GetType() override { return Type::kProofNodeSimple; }
};

class ProofValueLeaf : public MerkleProofElement {
  public:
    ProofValueLeaf(std::shared_ptr<AbstractValue> content,
                   std::shared_ptr<SearchablePathElement> path_elem)
        : content_(content), path_elem_(path_elem) {}

    const std::shared_ptr<AbstractValue> Content() const { return content_; }
    virtual Type GetType() override { return Type::kProofValueLeaf; }

  private:
    std::shared_ptr<AbstractValue> content_;
    std::shared_ptr<SearchablePathElement> path_elem_;
};

class ProofHashedLeaf : public MerkleProofElement {
  public:
    ProofHashedLeaf(std::vector<unsigned char> merkle_hash)
        : merkle_hash_(merkle_hash) {}

    bool operator==(const ProofHashedLeaf &b) const {
        return merkle_hash_ == b.merkle_hash_;
    }

    const std::vector<unsigned char> MerkleHash() { return merkle_hash_; }
    virtual Type GetType() override { return Type::kProofHashedLeaf; }

  private:
    std::vector<unsigned char> merkle_hash_;
};

class ProofNodeArrayHead : public ProofNode {
  public:
    ProofNodeArrayHead(
        std::shared_ptr<MerkleProofElement> left,
        std::shared_ptr<MerkleProofElement> right,
        std::shared_ptr<SearchablePathElement> path_elem = nullptr)
        : ProofNode(hash_prefix::kNodeArray, left, right),
          path_elem_(path_elem) {}

    virtual Type GetType() override { return Type::kProofNodeArrayHead; }

  private:
    std::shared_ptr<SearchablePathElement> path_elem_;
};

class ProofNodeDictHead : public ProofNode {
  public:
    ProofNodeDictHead(
        std::shared_ptr<MerkleProofElement> left,
        std::shared_ptr<MerkleProofElement> right,
        std::shared_ptr<SearchablePathElement> path_elem = nullptr)
        : ProofNode(hash_prefix::kNodeDict, left, right),
          path_elem_(path_elem) {}

    virtual Type GetType() override { return Type::kProofNodeDictHead; }

  private:
    std::shared_ptr<SearchablePathElement> path_elem_;
};

class MerkleProofTree {
  public:
    MerkleProofTree(std::shared_ptr<MerkleProofElement> root) : root_(root) {}
    const int MaxLevel() { return MaxLevelInternal(root_); }
    const std::shared_ptr<MerkleProofElement> Root() const { return root_; }

  private:
    const int MaxLevelInternal(std::shared_ptr<MerkleProofElement> node) {
        if (node->GetType() == MerkleProofElement::Type::kProofValueLeaf) {
            return 1;
        } else if (node->GetType() ==
                   MerkleProofElement::Type::kProofHashedLeaf) {
            return 1;
        } else {
            auto proof_node = std::dynamic_pointer_cast<ProofNode>(node);
            if (!proof_node) {
                std::stringstream ss;
                ss << "Should be able to handle node type "
                   << typeid(node.get()).name();
                throw new std::logic_error(ss.str());
            }

            int left = MaxLevelInternal(proof_node->Left());
            int right = MaxLevelInternal(proof_node->Right());
            return std::max(left, right) + 1;
        }
    }

    std::shared_ptr<MerkleProofElement> root_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif