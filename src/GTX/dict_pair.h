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
	GTXValue* value_;

	DictPair(std::string name = "", GTXValue *value = nullptr);
	
	bool Equals(DictPair *value);

	std::vector<unsigned char> Encode();

	static DictPair Decode(Reader *sequence);
};
}  // namespace client
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTX_DICT_PAIR_H_