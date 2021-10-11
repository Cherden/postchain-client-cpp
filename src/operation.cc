#include "operation.h"

namespace chromia {
namespace postchain {

Operation::Operation(std::string name) : name_(name) {}

std::shared_ptr<gtv::AbstractValue> Operation::ToAbstractValue() const {
    auto array = gtv::AbstractValueFactory::EmptyArray();
    array->Add(gtv::AbstractValueFactory::Build(name_));
    for (auto arg : args_) {
        array->Add(arg);
    }

    return array;
}

}  // namespace postchain

}  // namespace chromia