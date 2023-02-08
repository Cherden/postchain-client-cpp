#include "key_pair.h"

namespace chromia {
namespace postchain {
namespace ft3 {

KeyPair::KeyPair(std::vector<BYTE> private_key, std::vector<BYTE> pub_key)
{
	pub_key_ = pub_key;
	priv_key_ = private_key;
}

KeyPair::KeyPair(std::string private_key)
{
	if (private_key.length() > 0)
	{
		this->priv_key_ = PostchainUtil::HexStringToByteVector(private_key);

		if (!PostchainUtil::GenerateCompresedPublicKey(this->priv_key_, this->pub_key_))
		{
			throw new std::exception("KeyPair::Failed to generate public key");
		}
	}
	else
	{
		if (!PostchainUtil::GenerateKeyPair(this->priv_key_, this->pub_key_))
		{
			throw new std::exception("KeyPair::Failed to generate key pair");
		}
	}
}

} // namespace ft3
} // namespace postchain
} // namespace chromia