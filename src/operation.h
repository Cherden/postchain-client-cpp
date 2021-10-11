#ifndef POSTCHAIN_CLIENT_OPERATION_H_
#define POSTCHAIN_CLIENT_OPERATION_H_

#include <memory>
#include <string>
#include <vector>

#include "GTV/abstract_value.h"
#include "GTV/abstract_value_facotry.h"

namespace chromia {
namespace postchain {

class Operation {
  public:
    explicit Operation(std::string name);

    template <class T>
    Operation& operator<<(const T& obj) {
        args_.push_back(gtv::AbstractValueFactory::Build(obj));
        return *this;
    }

    std::shared_ptr<gtv::AbstractValue> ToAbstractValue() const;

    std::string Name() const { return name_; }
    const std::vector<std::shared_ptr<gtv::AbstractValue>>& Args() const {
        return args_;
    }

  private:
    std::string name_;
    std::vector<std::shared_ptr<gtv::AbstractValue>> args_;
};

}  // namespace postchain
}  // namespace chromia

#endif