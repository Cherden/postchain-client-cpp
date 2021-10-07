#ifndef POSTCHAIN_CLIENT_GTV_MERKLE_PATH_H_
#define POSTCHAIN_CLIENT_GTV_MERKLE_PATH_H_

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "../abstract_value.h"
#include "../abstract_value_facotry.h"

namespace chromia {
namespace postchain {
namespace gtv {

class PathElement;

class Path {
  public:
    Path(std::vector<std::shared_ptr<PathElement>> path_elements)
        : path_elements_(path_elements) {}

    const std::shared_ptr<PathElement> GetCurrentPathElement() {
        return path_elements_[0];
    }

    const std::shared_ptr<Path> Tail() const {
        if (path_elements_.size() == 0) {
            throw std::logic_error("Impossible to tail this array");
        } else {
            return std::make_shared<Path>(
                std::vector<std::shared_ptr<PathElement>>(
                    path_elements_.begin() + 1, path_elements_.end()));
        }
    }

    bool operator==(const Path &b) const {
        return path_elements_ == b.path_elements_;
    }

    const std::vector<std::shared_ptr<PathElement>> PathElements() const {
        return path_elements_;
    }

  private:
    std::vector<std::shared_ptr<PathElement>> path_elements_;
};

class SearchablePathElement;

class PathElement {
  public:
    enum class Type {
        kPathElement,
        kPathLeafElement,
        kSearchablePathElement,
        kArrayPathElement,
        kDictPathElement
    };
    explicit PathElement(std::shared_ptr<SearchablePathElement> previous)
        : previous_(previous) {}

    const std::shared_ptr<SearchablePathElement> Previous() const {
        return previous_;
    }
    virtual Type GetType() = 0;

  private:
    std::shared_ptr<SearchablePathElement> previous_;
};

class PathLeafElement : public PathElement {
  public:
    explicit PathLeafElement(std::shared_ptr<SearchablePathElement> previous)
        : PathElement(previous) {}

    bool operator==(const PathLeafElement &b) const {
        return previous_ == b.previous_;
    }

  private:
    std::shared_ptr<SearchablePathElement> previous_;
};

class SearchablePathElement : public PathElement {
  public:
    SearchablePathElement(std::shared_ptr<SearchablePathElement> previous)
        : PathElement(previous) {}

    virtual std::shared_ptr<AbstractValue> GetSearchKey() = 0;
};

class ArrayPathElement : public SearchablePathElement {
  public:
    ArrayPathElement(std::shared_ptr<SearchablePathElement> previous, int index)
        : SearchablePathElement(previous), index_(index) {}

    std::shared_ptr<AbstractValue> GetSearchKey() override {
        return AbstractValueFactory::Build(index_);
    }

    bool operator==(const ArrayPathElement &b) const {
        return index_ == b.index_;
    }

  private:
    int index_;
};

class DictPathElement : public SearchablePathElement {
  public:
    DictPathElement(std::shared_ptr<SearchablePathElement> previous,
                    std::shared_ptr<AbstractValue> key)
        : SearchablePathElement(previous), key_(key) {}

    std::shared_ptr<AbstractValue> GetSearchKey() override { return key_; }

    bool operator==(const DictPathElement &b) const { return key_ == b.key_; }

  private:
    std::shared_ptr<AbstractValue> key_;
};

class PathSet {
  public:
    PathSet(std::set<std::shared_ptr<Path>> paths) : paths_(paths) {}
    const bool IsEmpty() const { return paths_.empty(); }

    const std::shared_ptr<PathElement> GetPathLeafOrElseAnyCurrentPathElement()
        const {
        std::shared_ptr<PathLeafElement> leaf_element = nullptr;
        std::shared_ptr<PathElement> current_element = nullptr;
        std::tuple<std::shared_ptr<Path>, std::shared_ptr<PathElement>>
            previous;
        for (auto path : paths_) {
            current_element = path->GetCurrentPathElement();
            auto new_leaf_element =
                std::dynamic_pointer_cast<PathLeafElement>(current_element);
            if (new_leaf_element) {
                leaf_element = new_leaf_element;
            }

            previous = ErrorCheckUnequalParent(path, current_element,
                                               std::get<0>(previous),
                                               std::get<1>(previous));
        }

        return leaf_element != nullptr ? leaf_element : current_element;
    }

    const std::tuple<std::shared_ptr<Path>, std::shared_ptr<PathElement>>
    ErrorCheckUnequalParent(
        std::shared_ptr<Path> current_path,
        std::shared_ptr<PathElement> current_element,
        std::shared_ptr<Path> previous_path,
        std::shared_ptr<PathElement> previous_element) const {
        if (previous_element != nullptr) {
            if (current_element->Previous() != previous_element->Previous()) {
                throw std::logic_error(
                    "Something is wrong, these paths do not have the same "
                    "parent");
            }
        }

        return std::make_tuple(current_path, current_element);
    }

    std::shared_ptr<PathSet> KeepOnlyArrayPaths() {
        std::set<std::shared_ptr<Path>> array_elements;
        for (auto path : paths_) {
            if (path->PathElements()[0]->GetType() ==
                PathElement::Type::kArrayPathElement) {
                array_elements.insert(path);
            }
        }

        return std::make_shared<PathSet>(array_elements);
    }

    std::shared_ptr<PathSet> KeepOnlyDictPaths() {
        std::set<std::shared_ptr<Path>> dict_elements;
        for (auto path : paths_) {
            if (path->PathElements()[0]->GetType() ==
                PathElement::Type::kArrayPathElement) {
                dict_elements.insert(path);
            }
        }

        return std::make_shared<PathSet>(dict_elements);
    }

    std::shared_ptr<PathSet> GetTailIfFirstElementIsArrayOfThisIndexFromList(
        int index) {
        std::set<std::shared_ptr<Path>> elements;
        for (auto path : paths_) {
            auto new_path = GetTail(AbstractValueFactory::Build(index), path);
            if (new_path != nullptr) {
                elements.insert(new_path);
            }
        }

        return std::make_shared<PathSet>(elements);
    }

    std::shared_ptr<PathSet> GetTailIfFirstElementIsDictOfThisKeyFromList(
        std::string key) {
        std::set<std::shared_ptr<Path>> elements;
        for (auto path : paths_) {
            auto new_path = GetTail(AbstractValueFactory::Build(key), path);
            if (new_path != nullptr) {
                elements.insert(new_path);
            }
        }

        return std::make_shared<PathSet>(elements);
    }

  private:
    std::shared_ptr<Path> GetTail(std::shared_ptr<AbstractValue> search_key,
                                  std::shared_ptr<Path> path) {
        if (search_key == nullptr) {
            throw std::logic_error("Have to provide a search key");
        }

        try {
            auto first_element =
                std::dynamic_pointer_cast<SearchablePathElement>(
                    path->PathElements()[0]);
            if (first_element && first_element->GetSearchKey() == search_key) {
                return path->Tail();
            }

        } catch (std::logic_error) {
            std::cout << "Why are we dropping first element of an empty path?"
                      << std::endl;
        }

        return nullptr;
    }

    std::set<std::shared_ptr<Path>> paths_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif