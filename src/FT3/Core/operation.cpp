#include "operation.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Operation::Operation(std::string name, std::shared_ptr<ArrayValue> args)
{
	this->name_ = name;
	this->args_ = args;
}

std::shared_ptr<Operation> Operation::Op(std::string name, std::shared_ptr<ArrayValue> args)
{
	return std::make_shared<Operation>(name, args);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia