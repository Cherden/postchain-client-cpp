#ifndef POSTCHAIN_CLIENT_GTV_NULL_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_NULL_VALUE_H_

#include "abstract_value.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

class NullValue : public AbstractValue {
  public:
    explicit NullValue(){};

  protected:
    unsigned char AddContent(asn1::Writer& asn1_buffer) override {
        asn1_buffer.WriteNull();
        return tag::kNull;
    }
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif