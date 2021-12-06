#pragma once

#include "../../PostchainClient/GTV/abstract_value.h"
#include <vector>

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {
class Operation
{
public:
	std::string name_;
	std::shared_ptr<ArrayValue> args_;

	Operation(std::string name, std::shared_ptr<ArrayValue> args);

	static std::shared_ptr<Operation> Op(std::string name, std::shared_ptr<ArrayValue> args);

};
} // namespace ft3
} // namespace postchain
} // namespace chromia