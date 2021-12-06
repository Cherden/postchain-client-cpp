#ifndef POSTCHAIN_CLIENT_GTV_OCTET_STRING_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_OCTET_STRING_VALUE_H_
#include <vector>

#include "abstract_value.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

class OctetStringValue : public AbstractValue {
  public:
    explicit OctetStringValue(std::vector<unsigned char> value)
        : value_(value){};

	std::vector<unsigned char> GetValue() { return value_; }

  protected:
    unsigned char AddContent(asn1::Writer& asn1_buffer) override {
        asn1_buffer.WriteOctetString(value_);
        return tag::kOctetString;
    };

  private:
    std::vector<unsigned char> value_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif