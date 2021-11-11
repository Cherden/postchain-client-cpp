#ifndef POSTCHAIN_CLIENT_GTX_VALUE_H_
#define POSTCHAIN_CLIENT_GTX_VALUE_H_

#include <string>
#include <vector>
#include "../ASN1/reader.h"
#include "../ASN1/tag.h"

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

	std::vector<unsigned char> Encode();

	static std::shared_ptr<GTXValue> Decode(std::shared_ptr<asn1::Reader> sequence);

	static std::vector<unsigned char> TrimByteList(char* byteList, int length);

	std::string ToString()
	{
		switch (this->choice_)
		{
		case (GTXValueChoice::Null):
		{
			return "null";
		}
		case (GTXValueChoice::ByteArray):
		{
			return "0x" + PostchainUtil::ByteVectorToHexString(this->byte_array_);
		}
		case (GTXValueChoice::String):
		{
			return this->string_;
		}
		case (GTXValueChoice::Integer):
		{
			return std::to_string(this->integer_);
		}
		case (GTXValueChoice::Array):
		{
			std::string ret = "[";
			if (this->array_.size() == 0)
			{
				return ret + "]";
			}

			for(auto &elm : this->array_)
			{
				ret += elm->ToString() + ", ";
			}

			ret = ret.substr(0, ret.size() - 1);
			return ret + "]";
		}
		case (GTXValueChoice::Dict):
		{
			/*std::string ret = "[";
			if (Dict.Count == 0)
			{
				return ret + "]";
			}

			foreach(var elm in Dict)
			{
				ret += @"{{""" + elm.Name + @""": " + elm.Value.ToString() + "}, ";
			}

			return ret.Remove(ret.Length - 2) + "]";*/
			throw std::exception("GtxValue::ToString() Dict type not implemented");
			return "";
		}
		default:
		{
			return "";
		}
		}
	}
};
}  // namespace client
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTX_VALUE_H_