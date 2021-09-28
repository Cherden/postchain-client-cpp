#ifndef POSTCHAIN_CLIENT_GTV_ABSTRACT_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_ABSTRACT_VALUE_H_

#include <vector>

#include "../ASN1/writer.h"

namespace chromia {
namespace postchain {
namespace gtv {

class AbstractValue {
  public:
    std::vector<unsigned char> Encode();
    static std::vector<unsigned char> Hash(AbstractValue& value);

  protected:
    virtual unsigned char AddContent(asn1::Writer& asn1_buffer) = 0;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif