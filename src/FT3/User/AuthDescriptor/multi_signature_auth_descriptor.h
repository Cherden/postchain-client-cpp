#pragma once

#include "../../../PostchainClient/GTV/abstract_value_factory.h"
#include "../../../PostchainClient/GTV/array_value.h"
#include "../../../PostchainClient/postchain_util.h"
#include "../../forward_declarations.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

class MultiSignatureAuthDescriptor : public AuthDescriptor
{
public:
	std::vector<std::vector<byte>> pubkeys_;
	std::shared_ptr<Flags> flags_;
	int signature_required_;
	std::shared_ptr<IAuthdescriptorRule> auth_rule_;

	std::vector<std::vector<byte>> Signers() override
	{
		return this->pubkeys_;
	}

	std::string ID() override
	{
		return PostchainUtil::ByteVectorToHexString(this->Hash());
	}

	std::shared_ptr<IAuthdescriptorRule> Rule() override
	{
		return this->auth_rule_;
	}

	MultiSignatureAuthDescriptor(std::vector<std::vector<byte>> pubkeys, int signature_required, std::vector<FlagsType> flags, std::shared_ptr<IAuthdescriptorRule> rule = nullptr);

	std::shared_ptr<gtv::ArrayValue> ToGTV();

	std::vector<byte> Hash();
};
} // namespace ft3
} // namespace postchain
} // namespace chromia
