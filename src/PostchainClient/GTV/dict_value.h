#ifndef POSTCHAIN_CLIENT_GTV_DICT_VALUE_H_
#define POSTCHAIN_CLIENT_GTV_DICT_VALUE_H_
#include <map>
#include <memory>
#include <string>

#include "abstract_value.h"
#include "tag.h"

namespace chromia {
namespace postchain {
namespace gtv {

class DictValue : public AbstractValue {
  public:
    explicit DictValue(
        std::map<std::string, std::shared_ptr<AbstractValue>> value)
        : value_(value) {}

    virtual ~DictValue() {};

    size_t Size() const { return value_.size(); }
    std::shared_ptr<AbstractValue> operator[](const std::string& key) const {
        return value_.at(key);
    }
    std::vector<std::string> Keys() const {
        std::vector<std::string> keys;
        for (auto element : value_) {
            keys.push_back(element.first);
        }
        return keys;
    }

  protected:
    unsigned char AddContent(asn1::Writer& asn1_buffer) override {
        asn1_buffer.PushSequence();
        for (auto value : value_) {
            asn1_buffer.PushSequence();
            asn1_buffer.WriteUTF8String(value.first);
            asn1_buffer.WriteEncodedValue(value.second->Encode());
            asn1_buffer.PopSequence();
        }
        asn1_buffer.PopSequence();
        return tag::kDict;
    }

  private:
    std::map<std::string, std::shared_ptr<AbstractValue>> value_;
};

}  // namespace gtv
}  // namespace postchain
}  // namespace chromia

#endif