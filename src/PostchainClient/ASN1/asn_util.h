#pragma once

#include <string>
#include <vector>

namespace chromia {
namespace postchain {
namespace asn1 {

class AsnUtil
{
public:
	static const unsigned char TAG_NULL = 0xa0;
	static const unsigned char TAG_BYTE_ARRAY = 0xa1;
	static const unsigned char TAG_STRING = 0xa2;
	static const unsigned char TAG_INTEGER = 0xa3;
	static const unsigned char TAG_ARRAY = 0xa5;
	static const unsigned char TAG_DICT = 0xa4;
	static const unsigned char TAG_SEQUENCE = 0x30;
};

}  // namespace asn1
}  // namespace postchain
}  // namespace chromia
