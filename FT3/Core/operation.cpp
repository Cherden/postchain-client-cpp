#include "operation.h"

namespace chromia {
namespace postchain {
namespace ft3 {

Operation::Operation(std::string name, std::shared_ptr<AbstractValue> args)
{
	this->name_ = name;
	this->args_ = args;
}

Operation Operation::Op(std::string name, std::shared_ptr<AbstractValue> args)
{
	return Operation(name, args);
}

} // namespace ft3
} // namespace postchain
} // namespace chromia