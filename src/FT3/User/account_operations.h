#pragma once

#include "../forward_declarations.h"
#include "../../PostchainClient/GTV/abstract_value_facotry.h"
#include "../../PostchainClient/postchain_util.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class AccountOperations
{
public:
	static std::shared_ptr<ft3::Operation> AddAuthDescriptor(std::string account_id, std::string auth_descriptor_id, std::shared_ptr<AuthDescriptor> auth_descriptor);

	static std::shared_ptr<ft3::Operation> Transfer(std::shared_ptr<AbstractValue> inputs, std::shared_ptr<AbstractValue> outputs);

	static std::shared_ptr<ft3::Operation> XcTransfer(std::shared_ptr<AbstractValue> source, std::shared_ptr<AbstractValue> target, std::vector<std::string> hops);
	
	static std::shared_ptr<ft3::Operation> DeleteAllAuthDescriptorsExclude(std::string account_id, std::string exclude_auth_descriptor_id);
	
	static std::shared_ptr<ft3::Operation> DeleteAuthDescriptor(std::string account_id, std::string auth_descriptor_id, std::string delete_auth_descriptor_id);
	
	static std::shared_ptr<ft3::Operation> Nop();

	static std::shared_ptr<ft3::Operation> Op(std::string name, std::shared_ptr<ArrayValue> args);
};
} // namespace ft3
} // namespace postchain
} // namespace chromia