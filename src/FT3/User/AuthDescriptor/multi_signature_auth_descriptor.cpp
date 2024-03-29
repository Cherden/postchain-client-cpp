#pragma once

#include "multi_signature_auth_descriptor.h"
#include "../account.h"
#include "../../../PostchainClient/GTX/gtx.h"
#include "../../../PostchainClient/GTX/gtx_value.h"
#include "../../Core/util.h"

using namespace chromia::postchain::client;

namespace chromia {
namespace postchain {
namespace ft3 {

MultiSignatureAuthDescriptor::MultiSignatureAuthDescriptor(std::vector<std::vector<BYTE>> pubkeys, int signature_required, std::vector<FlagsType> flags, std::shared_ptr<IAuthdescriptorRule> rule)
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
	for (std::vector<BYTE> &pubkey : this->pubkeys_)
	{
		hexpubs.push_back(PostchainUtil::ByteVectorToHexString(pubkey));
	}

	std::shared_ptr<gtv::ArrayValue> gtv = AbstractValueFactory::EmptyArray();

	gtv->Add(AbstractValueFactory::Build(FT3Util::AuthTypeToString(AuthType::eMultiSig)));
	gtv->Add(AbstractValueFactory::Build(hexpubs));

	std::shared_ptr<gtv::ArrayValue> arr_0 = AbstractValueFactory::EmptyArray();
	arr_0->Add(this->flags_->ToGTV());
	arr_0->Add(AbstractValueFactory::Build(this->signature_required_));
	arr_0->Add(AbstractValueFactory::Build(hexpubs));
	gtv->Add(arr_0);
	
	if (this->auth_rule_ != nullptr)
	{
		gtv->Add(this->auth_rule_->ToGTV());
	}
	else
	{
		gtv->Add(AbstractValueFactory::Build(nullptr));
	}

	std::shared_ptr<GTXValue> gtx_value = Gtx::ArgToGTXValue(gtv);
	std::string gtx_str = gtx_value->ToString();

	return gtv;
}

std::vector<BYTE> MultiSignatureAuthDescriptor::Hash()
{
	std::vector<std::string> hexpubs;
	for (std::vector<BYTE> &pubkey : this->pubkeys_)
	{
		hexpubs.push_back(PostchainUtil::ByteVectorToHexString(pubkey));
	}

	std::shared_ptr<gtv::ArrayValue> gtv = AbstractValueFactory::EmptyArray();

	gtv->Add(AbstractValueFactory::Build(FT3Util::AuthTypeToString(AuthType::eMultiSig)));
	gtv->Add(AbstractValueFactory::Build(this->pubkeys_));

	std::shared_ptr<gtv::ArrayValue> arr_0 = AbstractValueFactory::EmptyArray();
	arr_0->Add(this->flags_->ToGTV());
	arr_0->Add(AbstractValueFactory::Build(this->signature_required_));
	arr_0->Add(AbstractValueFactory::Build(hexpubs));
	gtv->Add(arr_0);

	if (this->auth_rule_ != nullptr)
	{
		gtv->Add(this->auth_rule_->ToGTV());
	}
	else
	{
		gtv->Add(AbstractValueFactory::Build(nullptr));
	}

	std::vector<BYTE> hashed = AbstractValue::Hash(gtv);
	return hashed;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia
