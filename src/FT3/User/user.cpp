#pragma once

#include "user.h"
#include "../Core/key_pair.h"
#include "../User/AuthDescriptor/auth_descriptor_rule.h"
#include "../User/account.h"

namespace chromia {
namespace postchain {
namespace ft3 {

User::User(std::shared_ptr<KeyPair> key_pair, std::shared_ptr<AuthDescriptor> auth_descriptor)
{
	this->key_pair_ = key_pair;
	this->auth_descriptor_ = auth_descriptor;
}

std::shared_ptr<User> User::GenerateSingleSigUser(std::vector<FlagsType> flags)
{
	if (flags.size() == 0)
	{
		flags.push_back(FlagsType::eAccount);
		flags.push_back(FlagsType::eTransfer);
	}

	std::shared_ptr<KeyPair> key_pair = std::make_shared<KeyPair>();

	return std::make_shared<User>(key_pair, std::make_shared<SingleSignatureAuthDescriptor>(key_pair->pub_key_, flags));
}

} // namespace http
} // namespace postchain
} // namespace chromia