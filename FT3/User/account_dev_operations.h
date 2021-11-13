#pragma once

#include <memory>
#include "../../src/GTV/abstract_value_facotry.h"
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class AccountDevOperations
{
public:
	static std::shared_ptr<ft3::Operation> Register(std::shared_ptr<AuthDescriptor> auth_descriptor);

	static std::shared_ptr<ft3::Operation> FreeOp(std::string account_id);

	static std::shared_ptr<ft3::Operation> GivePoints(std::string account_id, int points);
};
} // namespace ft3
} // namespace postchain
} // namespace chromia