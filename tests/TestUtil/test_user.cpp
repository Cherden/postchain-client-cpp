//#ifdef FT3_TESTS

#include "test_user.h"
#include "../../FT3/User/user.h"
#include "../../FT3/User/account.h"
#include "../../FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "../../FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../FT3/Core/key_pair.h"
#include "../../src/common.h"

std::shared_ptr<User> TestUser::SingleSig(std::shared_ptr<IAuthdescriptorRule> rule = nullptr)
{
	std::shared_ptr<KeyPair> key_pair = std::make_shared<KeyPair>();
	std::shared_ptr<SingleSignatureAuthDescriptor> single_sig_auth_descriptor = std::make_shared<SingleSignatureAuthDescriptor>(
		key_pair->pub_key_,
		std::vector<FlagsType>{ FlagsType::eAccount, FlagsType::eTransfer },
		rule
		);
	return std::make_shared<User>(key_pair, single_sig_auth_descriptor);
}

std::shared_ptr<User> TestUser::MultiSig(int required_signatures, int number_of_participants, std::shared_ptr<IAuthdescriptorRule> rule = nullptr)
{
	std::vector<std::shared_ptr<KeyPair>> key_pairs;
	std::vector<std::vector<byte>> pub_keys;
	for (int i = 0; i < number_of_participants; i++)
	{
		auto key_pair = std::make_shared<KeyPair>();
		key_pairs.push_back(key_pair);
		pub_keys.push_back(key_pair->pub_key_);
	}

	std::shared_ptr<MultiSignatureAuthDescriptor> multi_signature_auth_descriptor = std::make_shared<MultiSignatureAuthDescriptor>(
		pub_keys,
		required_signatures,
		std::vector<FlagsType> {FlagsType::eAccount, FlagsType::eTransfer},
		rule
		);

	return std::make_shared<User>(key_pairs[0], multi_signature_auth_descriptor);
}

//#endif // FT3_TESTS