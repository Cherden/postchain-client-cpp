#pragma once

#include <memory>
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class User
{
public:
	std::shared_ptr<KeyPair> key_pair_;
	std::shared_ptr<AuthDescriptor> auth_descriptor_;

	User(std::shared_ptr<KeyPair> key_pair, std::shared_ptr<AuthDescriptor> auth_descriptor);

	static std::shared_ptr<User> GenerateSingleSigUser(std::vector<FlagsType> flags);
};
} // namespace http
} // namespace postchain
} // namespace chromia