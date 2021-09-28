#ifndef POSTCHAIN_CLIENT_GTV_ARRAY_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_ARRAY_VALUE_H_
#include <memory>
#include <vector>

#include "abstract_value.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

class ArrayValue : public AbstractValue {
  public:
    explicit ArrayValue(){};
    explicit ArrayValue(std::vector<std::shared_ptr<AbstractValue>> value)
        : value_(value) {}

    void Add(std::shared_ptr<AbstractValue> value) { value_.push_back(value); }

  protected:
    unsigned char AddContent(asn1::Writer& asn1_buffer) override {
        asn1_buffer.PushSequence();
        for (auto value : value_) {
            asn1_buffer.WriteEncodedValue(value->Encode());
        }
        asn1_buffer.PopSequence();
        return tag::kArray;
    }

  private:
    std::vector<std::shared_ptr<AbstractValue>> value_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif