#include "../../src/GTV/abstract_value.h"
#include <vector>

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {
class Operation
{
public:
	std::string name_;
	std::shared_ptr<AbstractValue> args_;

	Operation(std::string name, std::shared_ptr<AbstractValue> args);

	static Operation Op(std::string name, std::shared_ptr<AbstractValue> args);

};
} // namespace ft3
} // namespace postchain
} // namespace chromia