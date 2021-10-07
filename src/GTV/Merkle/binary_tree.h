#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_BINARY_TREE_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_BINARY_TREE_H_

#include <sstream>

#include "../abstract_value.h"
#include "../array_value.h"
#include "../dict_value.h"
#include "path.h"

namespace chromia {
namespace postchain {
namespace gtv {

namespace hash_prefix {
constexpr unsigned char kNode = 0;
constexpr unsigned char kLeaf = 1;
constexpr unsigned char kNodeArray = 7;
constexpr unsigned char kNodeDict = 8;
}  // namespace hash_prefix

class BinaryTreeElement {
  public:
    enum class Type {
        kNode,
        kSubTreeRootNode,
        kLeaf,
        kEmptyLeaf,
        kArrayHeadNode,
        kDictHeadNode
    };
    void SetPathElement(std::shared_ptr<PathElement> path_element) {
        path_element_ = path_element;
    }
    const std::shared_ptr<PathElement> GetPathElement() {
        return path_element_;
    }

    const bool IsPath() { return path_element_ != nullptr; }
    const bool IsPathLeaf() {
        return path_element_ == nullptr
                   ? false
                   : path_element_->GetType() ==
                         PathElement::Type::kPathLeafElement;
    }
    const virtual int GetPrefixByte() = 0;
    virtual Type GetType() = 0;

  protected:
    std::shared_ptr<PathElement> path_element_ = nullptr;
};

class Node : public BinaryTreeElement {
  public:
    Node(std::shared_ptr<BinaryTreeElement> left,
         std::shared_ptr<BinaryTreeElement> right)
        : left_(left), right_(right) {}

    const std::shared_ptr<BinaryTreeElement> Left() { return left_; }
    const std::shared_ptr<BinaryTreeElement> Right() { return right_; }

    const int GetPrefixByte() override { return hash_prefix::kNode; }
    Type GetType() override { return Type::kNode; }

  protected:
    std::shared_ptr<BinaryTreeElement> left_;
    std::shared_ptr<BinaryTreeElement> right_;
};

class SubTreeRootNode : public Node {
  public:
    SubTreeRootNode(std::shared_ptr<BinaryTreeElement> left,
                    std::shared_ptr<BinaryTreeElement> right,
                    std::shared_ptr<AbstractValue> content,
                    std::shared_ptr<PathElement> path_elem = nullptr)
        : Node(left, right), content_(content) {
        SetPathElement(path_element_);
    }

    const std::shared_ptr<AbstractValue> Content() const { return content_; }
    Type GetType() override { return Type::kSubTreeRootNode; }

  protected:
    std::shared_ptr<AbstractValue> content_;
};

class Leaf : public BinaryTreeElement {
  public:
    Leaf(std::shared_ptr<AbstractValue> content,
         std::shared_ptr<PathElement> path_elem = nullptr)
        : content_(content) {
        if (path_elem != nullptr) {
            if (path_element_->GetType() ==
                PathElement::Type::kPathLeafElement) {
                SetPathElement(path_element_);
            } else {
                throw new std::logic_error(
                    "The path and object structure does not match! We are at a "
                    "leaf, but the path expects a sub structure.");
            }
        }
    }

    const std::shared_ptr<AbstractValue> Content() const { return content_; }
    const int GetPrefixByte() override { return hash_prefix::kLeaf; }
    Type GetType() override { return Type::kLeaf; };

  protected:
    std::shared_ptr<AbstractValue> content_;
};

class EmptyLeaf : public BinaryTreeElement {
  public:
    EmptyLeaf() {}
    const int GetPrefixByte() override { return hash_prefix::kNode; }
    Type GetType() override { return Type::kEmptyLeaf; }
};

class BinaryTree {
  public:
    BinaryTree(std::shared_ptr<BinaryTreeElement> root) : root_(root) {}
    const int MaxLevel() const { return MaxLevelInternal(root_); }
    const std::shared_ptr<BinaryTreeElement> Root() const { return root_; }

  protected:
    const int MaxLevelInternal(std::shared_ptr<BinaryTreeElement> node) const {
        if (node->GetType() == BinaryTreeElement::Type::kEmptyLeaf) {
            return 0;
        } else if (node->GetType() == BinaryTreeElement::Type::kLeaf) {
            return 1;
        } else {
            auto tree_node = std::dynamic_pointer_cast<Node>(node);
            if (!tree_node) {
                std::stringstream ss;
                ss << "Should be able to handle node type "
                   << typeid(node.get()).name();
                throw new std::logic_error(ss.str());
            }

            int left = MaxLevelInternal(tree_node->Left());
            int right = MaxLevelInternal(tree_node->Right());
            return std::max(left, right) + 1;
        }
    }

    std::shared_ptr<BinaryTreeElement> root_;
};

class ArrayHeadNode : public SubTreeRootNode {
  public:
    ArrayHeadNode(std::shared_ptr<BinaryTreeElement> left,
                  std::shared_ptr<BinaryTreeElement> right,
                  std::shared_ptr<ArrayValue> content, int size,
                  std::shared_ptr<PathElement> path_elem = nullptr)
        : SubTreeRootNode(left, right, content, path_elem), size_(size) {}

    const int GetPrefixByte() override { return hash_prefix::kNodeArray; }

  protected:
    int size_;
};

class DictHeadNode : public SubTreeRootNode {
  public:
    DictHeadNode(std::shared_ptr<BinaryTreeElement> left,
                 std::shared_ptr<BinaryTreeElement> right,
                 std::shared_ptr<DictValue> content, int size,
                 std::shared_ptr<PathElement> path_elem = nullptr)
        : SubTreeRootNode(left, right, content, path_elem), size_(size) {}

    const int GetPrefixByte() override { return hash_prefix::kNodeDict; }

  protected:
    int size_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif