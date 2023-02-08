#pragma once

#include "../../../PostchainClient/GTV/abstract_value_factory.h"
#include "../../../PostchainClient/GTV/array_value.h"
#include "../../../PostchainClient/postchain_util.h"
#include "../../forward_declarations.h"
#include "../../Core/util.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

class SingleSignatureAuthDescriptor : public AuthDescriptor
{
public:
	std::shared_ptr<Flags> flags_;
	std::shared_ptr<IAuthdescriptorRule> auth_rule_;
	
	std::vector<std::vector<BYTE>> Signers() override
	{
		return std::vector<std::vector<BYTE>> {this->pubkey_};
	}

	std::string ID() override
	{
		return PostchainUtil::ByteVectorToHexString(this->Hash());
	}

	std::shared_ptr<IAuthdescriptorRule> Rule() override
	{
		return this->auth_rule_;
	}

	SingleSignatureAuthDescriptor(std::vector<BYTE> pubkey, std::vector<FlagsType> flags, std::shared_ptr<IAuthdescriptorRule> rule = nullptr);
	
	std::shared_ptr<gtv::ArrayValue> ToGTV();
	
	std::vector<BYTE> Hash();

private:
	std::vector<BYTE> pubkey_;
};
} // namespace ft3
} // namespace postchain
} // namespace chromia

