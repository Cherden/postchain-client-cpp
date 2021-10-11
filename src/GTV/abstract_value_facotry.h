#ifndef POSTCHAIN_CLIENT_GTV_ABSTRACT_VALUE_FACOTRY_H_
#define POSTCHAIN_CLIENT_GTV_ABSTRACT_VALUE_FACOTRY_H_

#include <memory>
#include <vector>

#include "abstract_value.h"
#include "array_value.h"
#include "dict_value.h"
#include "integer_value.h"
#include "null_value.h"
#include "octet_string_value.h"
#include "utf8_string_value.h"

namespace chromia {
namespace postchain {
namespace gtv {

class AbstractValueFactory {
  public:
    static std::shared_ptr<ArrayValue> EmptyArray() {
        return std::make_shared<ArrayValue>();
    }

    static std::shared_ptr<ArrayValue> Build(
        const std::vector<std::shared_ptr<AbstractValue>>& values) {
        return std::make_shared<ArrayValue>(values);
    }

    template <class T>
    static std::shared_ptr<ArrayValue> Build(const std::vector<T>& values) {
        auto array = EmptyArray();
        for (auto value : values) {
            array->Add(Build(value));
        }
        return array;
    }

    template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
    static std::shared_ptr<AbstractValue> Build(const T value) {
        return std::make_shared<IntegerValue>(value);
    }

    static std::shared_ptr<AbstractValue> Build(const std::string value) {
        return std::make_shared<UTF8StringValue>(value);
    }

    static std::shared_ptr<AbstractValue> Build(const char* value) {
        return std::make_shared<UTF8StringValue>(value);
    }

    static std::shared_ptr<AbstractValue> Build(const nullptr_t) {
        return std::make_shared<NullValue>();
    }

    static std::shared_ptr<AbstractValue> Build(
        const std::vector<unsigned char> value) {
        return std::make_shared<OctetStringValue>(value);
    }
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif