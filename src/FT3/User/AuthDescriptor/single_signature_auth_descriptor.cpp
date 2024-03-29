#include "single_signature_auth_descriptor.h"
#include "../account.h"
#include "../../../PostchainClient/GTX/gtx.h"
#include "../../../PostchainClient/GTX/gtx_value.h"

using namespace chromia::postchain::client;

namespace chromia {
namespace postchain {
namespace ft3 {

SingleSignatureAuthDescriptor::SingleSignatureAuthDescriptor(std::vector<BYTE> pubkey, std::vector<FlagsType> flags, std::shared_ptr<IAuthdescriptorRule> rule)
{
	this->pubkey_ = pubkey;
	this->flags_ = std::make_shared<Flags>(flags);
	this->auth_rule_ = rule;
}

std::shared_ptr<gtv::ArrayValue> SingleSignatureAuthDescriptor::ToGTV()
{
	std::shared_ptr<gtv::ArrayValue> gtv = AbstractValueFactory::EmptyArray();

	gtv->Add(AbstractValueFactory::Build(FT3Util::AuthTypeToString(AuthType::eSingleSig)));

	std::shared_ptr<gtv::ArrayValue> arr_0 = AbstractValueFactory::EmptyArray();

	std::string pass_as_string = PostchainUtil::ByteVectorToHexString(this->pubkey_);
	arr_0->Add(AbstractValueFactory::Build(pass_as_string));
	gtv->Add(arr_0);

	std::shared_ptr<gtv::ArrayValue> arr_1 = AbstractValueFactory::EmptyArray();
	arr_1->Add(this->flags_->ToGTV());
	arr_1->Add(AbstractValueFactory::Build(PostchainUtil::ByteVectorToHexString(this->pubkey_)));
	gtv->Add(arr_1);

	if (this->auth_rule_ != nullptr)
	{
		gtv->Add(this->auth_rule_->ToGTV());
	}
	else 
	{
		gtv->Add(AbstractValueFactory::Build(nullptr));
	}

	return gtv;
}

std::vector<BYTE> SingleSignatureAuthDescriptor::Hash()
{
	std::shared_ptr<gtv::ArrayValue> gtv = AbstractValueFactory::EmptyArray();

	gtv->Add(AbstractValueFactory::Build(FT3Util::AuthTypeToString(AuthType::eSingleSig)));

	std::shared_ptr<gtv::ArrayValue> arr_0 = AbstractValueFactory::EmptyArray();
	arr_0->Add(AbstractValueFactory::Build(this->pubkey_));
	gtv->Add(arr_0);

	std::shared_ptr<gtv::ArrayValue> arr_1 = AbstractValueFactory::EmptyArray();
	arr_1->Add(this->flags_->ToGTV());
	arr_1->Add(AbstractValueFactory::Build(PostchainUtil::ByteVectorToHexString(this->pubkey_)));
	gtv->Add(arr_1);

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

