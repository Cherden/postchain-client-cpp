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
	static std::shared_ptr<ft3::Operation> Register(std::shared_ptr<AuthDescriptor> auth_descriptor)
    {
		std::shared_ptr<ArrayValue> args = auth_descriptor->ToGTV();
        return std::make_shared<ft3::Operation>("ft3.dev_register_account", args);
    }

    static std::shared_ptr<ft3::Operation> FreeOp(std::string account_id)
    {
		std::shared_ptr<ArrayValue> args;
		args->Add(AbstractValueFactory::Build(account_id));
		return std::make_shared<ft3::Operation>("ft3.dev_free_op", args);
    }

    static std::shared_ptr<ft3::Operation> GivePoints(std::string account_id, int points)
    {
		std::shared_ptr<ArrayValue> args;
		args->Add(AbstractValueFactory::Build(account_id));
		args->Add(AbstractValueFactory::Build(points));
		return std::make_shared<ft3::Operation>("ft3.dev_give_points", args);
    }
};
} // namespace ft3
} // namespace postchain
} // namespace chromia