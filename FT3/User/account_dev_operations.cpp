#include "account_dev_operations.h"
#include "../Core/operation.h"
#include "account.h"

namespace chromia {
namespace postchain {
namespace ft3 {

std::shared_ptr<ft3::Operation> AccountDevOperations::Register(std::shared_ptr<AuthDescriptor> auth_descriptor)
{
	std::shared_ptr<ArrayValue> args = auth_descriptor->ToGTV();
	return std::make_shared<ft3::Operation>("ft3.dev_register_account", args);
}

std::shared_ptr<ft3::Operation> AccountDevOperations::FreeOp(std::string account_id)
{
	std::shared_ptr<ArrayValue> args;
	args->Add(AbstractValueFactory::Build(account_id));
	return std::make_shared<ft3::Operation>("ft3.dev_free_op", args);
}

std::shared_ptr<ft3::Operation> AccountDevOperations::GivePoints(std::string account_id, int points)
{
	std::shared_ptr<ArrayValue> args;
	args->Add(AbstractValueFactory::Build(account_id));
	args->Add(AbstractValueFactory::Build(points));
	return std::make_shared<ft3::Operation>("ft3.dev_give_points", args);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia