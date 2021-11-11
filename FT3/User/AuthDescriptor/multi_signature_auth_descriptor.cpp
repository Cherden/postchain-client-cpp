#pragma once

#include "multi_signature_auth_descriptor.h"
#include "../account.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

MultiSignatureAuthDescriptor::MultiSignatureAuthDescriptor(std::vector<std::vector<byte>> pubkeys, int signature_required, std::vector<FlagsType> flags, std::shared_ptr<IAuthdescriptorRule> rule)
{
	if (signature_required > pubkeys.size())
    {
        throw std::exception("Number of required signatures have to be less or equal to number of pubkeys");
    }

    this->pubkeys_ = pubkeys;
    this->signature_required_ = signature_required;
	this->flags_ = std::make_shared<Flags>(flags);
    this->auth_rule_ = rule;
}

std::shared_ptr<gtv::ArrayValue> MultiSignatureAuthDescriptor::ToGTV()
{
	std::vector<std::string> hexpubs;
	for (std::vector<byte> &pubkey : this->pubkeys_)
	{
		hexpubs.push_back(PostchainUtil::ByteVectorToHexString(pubkey));
	}

	std::shared_ptr<gtv::ArrayValue> gtv;

	gtv->Add(AbstractValueFactory::Build(FT3Util::AuthTypeToString(AuthType::eMultiSig)));
	gtv->Add(AbstractValueFactory::Build(hexpubs));

	std::shared_ptr<gtv::ArrayValue> arr_0 = AbstractValueFactory::EmptyArray();
	arr_0->Add(this->flags_->ToGTV());
	arr_0->Add(AbstractValueFactory::Build(this->signature_required_));
	arr_0->Add(AbstractValueFactory::Build(hexpubs));
	gtv->Add(arr_0);
	
	gtv->Add(this->auth_rule_->ToGTV());

	return gtv;
}

std::vector<byte> MultiSignatureAuthDescriptor::Hash()
{
	std::vector<std::string> hexpubs;
	for (std::vector<byte> &pubkey : this->pubkeys_)
	{
		hexpubs.push_back(PostchainUtil::ByteVectorToHexString(pubkey));
	}

	std::shared_ptr<gtv::ArrayValue> gtv;

	gtv->Add(AbstractValueFactory::Build(FT3Util::AuthTypeToString(AuthType::eMultiSig)));
	gtv->Add(AbstractValueFactory::Build(this->pubkeys_));

	std::shared_ptr<gtv::ArrayValue> arr_0 = AbstractValueFactory::EmptyArray();
	arr_0->Add(this->flags_->ToGTV());
	arr_0->Add(AbstractValueFactory::Build(this->signature_required_));
	arr_0->Add(AbstractValueFactory::Build(hexpubs));
	gtv->Add(arr_0);

	gtv->Add(this->auth_rule_->ToGTV());

	std::vector<byte> hashed = AbstractValue::Hash(gtv);
	return hashed;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
