#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_BINARY_TREE_FACTORY_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_BINARY_TREE_FACTORY_H_

#include <algorithm>
#include <memory>
#include <set>
#include <vector>

#include "../abstract_value.h"
#include "../abstract_value_facotry.h"
#include "../array_value.h"
#include "../dict_value.h"
#include "binary_tree.h"
#include "path.h"

namespace chromia {
namespace postchain {
namespace gtv {

class BinaryTreeFactory {
  public:
    std::shared_ptr<BinaryTreeElement> HandleLeaf(
        std::shared_ptr<AbstractValue> leaf, std::shared_ptr<PathSet> paths,
        bool is_root = false) {
        if (paths->IsEmpty() && !is_root) {
            return InnerHandleLeaf(leaf, GetEmptyPathSet());
        } else {
            return InnerHandleLeaf(leaf, paths);
        }
    }

    std::shared_ptr<BinaryTree> Build(std::shared_ptr<AbstractValue> data) {
        return BuildWithPath(data, GetEmptyPathSet());
    }

    std::shared_ptr<BinaryTree> BuildWithPath(
        std::shared_ptr<AbstractValue> data, std::shared_ptr<PathSet> paths) {
        auto result = HandleLeaf(data, paths, true);
        return std::make_shared<BinaryTree>(result);
    }

  private:
    std::shared_ptr<PathSet> GetEmptyPathSet() {
        return std::make_shared<PathSet>(std::set<std::shared_ptr<Path>>());
    }

    std::shared_ptr<BinaryTreeElement> InnerHandleLeaf(
        std::shared_ptr<AbstractValue> leaf, std::shared_ptr<PathSet> paths) {
        if (std::dynamic_pointer_cast<ArrayValue>(leaf)) {
            auto array = std::dynamic_pointer_cast<ArrayValue>(leaf);
            return BuildFromArray(array, paths);
        } else if (std::dynamic_pointer_cast<DictValue>(leaf)) {
            auto dict = std::dynamic_pointer_cast<DictValue>(leaf);
            return BuildFromDictionary(dict, paths);
        } else {
            return HandlePrimitiveLeaf(leaf, paths);
        }
    }

    std::shared_ptr<BinaryTreeElement> HandlePrimitiveLeaf(
        std::shared_ptr<AbstractValue> leaf, std::shared_ptr<PathSet> paths) {
        auto path_element = paths->GetPathLeafOrElseAnyCurrentPathElement();

        if (path_element != nullptr &&
            path_element->GetType() == PathElement::Type::kPathLeafElement) {
            throw std::logic_error("Path does not match the tree structure.");
        }

        return std::make_shared<Leaf>(leaf, path_element);
    }

    std::vector<std::shared_ptr<BinaryTreeElement>> BuildHigherLayer(
        int layer, std::vector<std::shared_ptr<BinaryTreeElement>> in_list) {
        if (in_list.size() == 0) {
            throw std::logic_error("Cannot work on empty arrays.");
        } else if (in_list.size() == 1) {
            return in_list;
        }

        std::vector<std::shared_ptr<BinaryTreeElement>> ret;
        int nr_of_nodes_to_create = in_list.size() / 2;
        std::shared_ptr<BinaryTreeElement> left_value = nullptr;
        bool is_left = true;
        for (auto element : in_list) {
            if (is_left) {
                left_value = element;
                is_left = false;
            } else {
                ret.push_back(std::make_shared<Node>(left_value, element));
                nr_of_nodes_to_create--;
                is_left = true;
                left_value = nullptr;
            }
        }

        if (!is_left) {
            ret.push_back(left_value);
        }

        if (nr_of_nodes_to_create != 0) {
            throw std::logic_error(
                "Why didn't we build exactly the correct amount?");
        }

        return BuildHigherLayer(layer + 1, ret);
    }

    std::shared_ptr<ArrayHeadNode> BuildFromArray(
        std::shared_ptr<ArrayValue> array, std::shared_ptr<PathSet> paths) {
        auto path_element = paths->GetPathLeafOrElseAnyCurrentPathElement();
        if (array->Size() == 0) {
            return std::make_shared<ArrayHeadNode>(
                std::make_shared<EmptyLeaf>(), std::make_shared<EmptyLeaf>(),
                array, 0, path_element);
        }

        auto leaf_array = BuildLeafElements(array, paths);
        auto result = BuildHigherLayer(1, leaf_array);

        auto org_root = result[0];
        if (org_root->GetType() == BinaryTreeElement::Type::kNode) {
            auto node_root = std::dynamic_pointer_cast<Node>(org_root);
            return std::make_shared<ArrayHeadNode>(node_root->Left(),
                                                   node_root->Right(), array,
                                                   array->Size(), path_element);
        } else if (org_root->GetType() == BinaryTreeElement::Type::kLeaf) {
            return BuildFromOneLeaf(array, org_root, path_element);
        } else {
            throw std::logic_error("Should not find element of this type here");
        }
    }

    std::shared_ptr<ArrayHeadNode> BuildFromOneLeaf(
        std::shared_ptr<ArrayValue> array,
        std::shared_ptr<BinaryTreeElement> org_root,
        std::shared_ptr<PathElement> path_element) {
        if (array->Size() > 1) {
            throw std::logic_error(
                "How come we got a leaf returned when we had elements is the "
                "args");
        } else {
            return std::make_shared<ArrayHeadNode>(
                org_root, std::make_shared<EmptyLeaf>(), array, array->Size(),
                path_element);
        }
    }

    std::vector<std::shared_ptr<BinaryTreeElement>> BuildLeafElements(
        std::shared_ptr<ArrayValue> leaf_list, std::shared_ptr<PathSet> paths) {
        std::vector<std::shared_ptr<BinaryTreeElement>> leaf_array;
        auto array_paths = paths->KeepOnlyArrayPaths();
        for (int i = 0; i < leaf_list->Size(); i++) {
            auto leaf_path =
                array_paths->GetTailIfFirstElementIsArrayOfThisIndexFromList(i);
            auto element = HandleLeaf((*leaf_list)[i], leaf_path);
            leaf_array.push_back(element);
        }

        return leaf_array;
    }

    std::shared_ptr<DictHeadNode> BuildFromDictionary(
        std::shared_ptr<DictValue> dict, std::shared_ptr<PathSet> paths) {
        auto path_element = paths->GetPathLeafOrElseAnyCurrentPathElement();

        auto keys = dict->Keys();
        if (keys.size() == 0) {
            return std::make_shared<DictHeadNode>(std::make_shared<EmptyLeaf>(),
                                                  std::make_shared<EmptyLeaf>(),
                                                  dict, 0, path_element);
        }
        std::sort(keys.begin(), keys.end());

        auto leaf_array = BuildLeafElementFromDict(keys, dict, paths);
        auto result = BuildHigherLayer(1, leaf_array);

        auto org_root = result[0];
        if (org_root->GetType() == BinaryTreeElement::Type::kNode) {
            auto node_root = std::dynamic_pointer_cast<Node>(org_root);
            return std::make_shared<DictHeadNode>(node_root->Left(),
                                                  node_root->Right(), dict,
                                                  dict->Size(), path_element);
        } else {
            throw std::logic_error("Should not find element of this type here");
        }
    }

    std::vector<std::shared_ptr<BinaryTreeElement>> BuildLeafElementFromDict(
        std::vector<std::string> keys, std::shared_ptr<DictValue> dict,
        std::shared_ptr<PathSet> paths) {
        std::vector<std::shared_ptr<BinaryTreeElement>> leaf_array;
        auto dict_paths = paths->KeepOnlyDictPaths();

        for (int i = 0; i < keys.size(); i++) {
            auto key = keys[i];
            auto key_element =
                HandleLeaf(AbstractValueFactory::Build(key), GetEmptyPathSet());
            leaf_array.push_back(key_element);

            auto content = (*dict)[key];
            auto leaf_paths =
                dict_paths->GetTailIfFirstElementIsDictOfThisKeyFromList(key);
            auto content_element = HandleLeaf(content, leaf_paths);
            leaf_array.push_back(content_element);
        }

        return leaf_array;
    }
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia
#endif