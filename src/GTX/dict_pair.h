#ifndef POSTCHAIN_CLIENT_GTX_DICT_PAIR_H_
#define POSTCHAIN_CLIENT_GTX_DICT_PAIR_H_

#include <string>
#include "../ASN1/reader.h"

using namespace chromia::postchain::asn1;

namespace chromia {
namespace postchain {
namespace client {

// Forward declaration
class GTXValue;

class DictPair {
public:
	std::string name_;
	std::shared_ptr<GTXValue> value_;

	DictPair(std::string name = "", std::shared_ptr<GTXValue> value = nullptr);
	
	std::vector<unsigned char> Encode();

	static std::shared_ptr<DictPair> Decode(Reader* sequence);
};
}  // namespace client
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTX_DICT_PAIR_H_