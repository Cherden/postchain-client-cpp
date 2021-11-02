#ifndef POSTCHAIN_CLIENT_GTX_VALUE_H_
#define POSTCHAIN_CLIENT_GTX_VALUE_H_

#include <string>
#include <vector>
#include "../ASN1/reader.h"

using namespace chromia::postchain::asn1;

namespace chromia {
namespace postchain {
namespace client {

enum GTXValueChoice
{
	NotSet = -1,
	Null = 0,
	ByteArray = 1,
	String = 2,
	Integer = 3,
	Dict = 4,
	Array = 5
};

// Forward declaration
class DictPair;

class GTXValue {
public:
	GTXValueChoice choice_;
	std::vector<unsigned char> byte_array_;
	std::string string_;
	long integer_;
	std::vector<std::shared_ptr<DictPair>> dict_;
	std::vector<std::shared_ptr<GTXValue>> array_;

	GTXValue();

	bool Equals(GTXValue *obj);
	
	std::vector<unsigned char> Encode();

	//static GTXValue Decode(Reader sequence);

	static std::vector<unsigned char> TrimByteList(char* byteList, int length);

	// public object[] ToObjectArray(); TO-DO check if needed and valid in C++

	std::string ToString();
};
}  // namespace client
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTX_VALUE_H_