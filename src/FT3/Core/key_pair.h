#pragma once

#include "../../PostchainClient/common.h"
#include "../../PostchainClient/postchain_util.h"
#include <vector>

namespace chromia {
namespace postchain {
namespace ft3 {

class KeyPair
{
public:
	std::vector<BYTE> pub_key_;
	std::vector<BYTE> priv_key_;

	KeyPair(std::vector<BYTE> private_key, std::vector<BYTE> pub_key);

	KeyPair(std::string private_key = "");

};

} // namespace ft3
} // namespace postchain
} // namespace chromia