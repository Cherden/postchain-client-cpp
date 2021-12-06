#include "auth_descriptor_factory.h"
#include "../account.h"
#include "single_signature_auth_descriptor.h"
#include "multi_signature_auth_descriptor.h"
#include <vector>
#include "../../../PostchainClient/ASN1/reader.h"
#include "../../../PostchainClient/GTX/gtx_value.h"

using namespace chromia::postchain::asn1;
using namespace chromia::postchain::client;

namespace chromia {
namespace postchain {
namespace ft3 {

std::shared_ptr<AuthDescriptor> AuthDescriptorFactory::Create(AuthType type, std::vector<byte> args)
{
	switch (type)
	{
	case AuthType::eSingleSig:
		return std::dynamic_pointer_cast<AuthDescriptor>(this->CreateSingleSig(args));
	case AuthType::eMultiSig:
		return std::dynamic_pointer_cast<AuthDescriptor>(this->CreateMultiSig(args));
	}
	return nullptr;
}

std::shared_ptr<SingleSignatureAuthDescriptor> AuthDescriptorFactory::CreateSingleSig(std::vector<byte> args)
{
	Reader gtx_transaction(args);
	std::shared_ptr<client::GTXValue> gtx_value = GTXValue::Decode(&gtx_transaction);
	std::vector<FlagsType> flags;

	if (gtx_value->array_.size() < 2)
	{
		throw std::exception("AuthDescriptorFactory::CreateSingleSig (gtx_value->array_.size() == 0)");
	}
	
	for (std::shared_ptr<client::GTXValue> &flag : gtx_value->array_[0]->array_)
	{
		flags.push_back(FT3Util::StringToFlagType(flag->ToString()));
	}

	std::shared_ptr<SingleSignatureAuthDescriptor> ret = std::make_shared<SingleSignatureAuthDescriptor>(
		PostchainUtil::HexStringToByteVector(gtx_value->array_[1]->string_),
		flags);

	return ret;
}

std::shared_ptr<MultiSignatureAuthDescriptor> AuthDescriptorFactory::CreateMultiSig(std::vector<byte> args)
{
	Reader gtx_transaction(args);
	std::shared_ptr<client::GTXValue> gtx_value = GTXValue::Decode(&gtx_transaction);
	std::vector<FlagsType> flags;
	std::vector<std::vector<byte>> pubkeys;

	int signature_required = (int) gtx_value->array_[1]->integer_;

	if (gtx_value->array_.size() < 3)
	{
		throw std::exception("AuthDescriptorFactory::CreateSingleSig (gtx_value->array_.size() == 0)");
	}

	for (std::shared_ptr<client::GTXValue> &flag : gtx_value->array_[0]->array_)
	{
		flags.push_back(FT3Util::StringToFlagType(flag->ToString()));
	}

	for(auto& pubkey : gtx_value->array_[2]->array_)
	{
		pubkeys.push_back(PostchainUtil::HexStringToByteVector(pubkey->string_));
	}

	std::shared_ptr<MultiSignatureAuthDescriptor> ret = std::make_shared<MultiSignatureAuthDescriptor>(
		pubkeys, signature_required, flags);

	return ret;
}

} // namespace http
} // namespace postchain
} // namespace chromia