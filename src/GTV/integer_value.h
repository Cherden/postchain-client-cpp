#ifndef POSTCHAIN_CLIENT_GTV_INTEGER_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_INTEGER_VALUE_H_

#include "abstract_value.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

class IntegerValue : public AbstractValue {
  public:
    explicit IntegerValue(long long value) : value_(value) {}

  protected:
    unsigned char AddContent(asn1::Writer& asn1_buffer) override {
        asn1_buffer.WriteInteger(value_);
        return tag::kInteger;
    }

  private:
    long long value_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif