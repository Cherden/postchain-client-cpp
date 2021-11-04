#ifndef POSTCHAIN_CLIENT_GTV_UTF8_STRING_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_UTF8_STRING_VALUE_H_
#include <string>

#include "abstract_value.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

class UTF8StringValue : public AbstractValue {
  public:
    explicit UTF8StringValue(std::string value) : value_(value) {}

	std::string GetValue() { return value_; };

  protected:
    unsigned char AddContent(asn1::Writer& asn1_buffer) override {
        asn1_buffer.WriteUTF8String(value_);
        return tag::kUTF8String;
    }

  private:
    std::string value_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif