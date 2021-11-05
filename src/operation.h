#ifndef POSTCHAIN_CLIENT_OPERATION_H_
#define POSTCHAIN_CLIENT_OPERATION_H_

#include <memory>
#include <string>
#include <vector>

#include "common.h"
#include "GTV/abstract_value.h"
#include "GTV/abstract_value_factory.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {

// Forward declaration
namespace client {
	class GTXValue;
}

class Operation {
  public:

    Operation(std::string name, std::shared_ptr<ArrayValue> args);

	std::shared_ptr<client::GTXValue> ToGtxValue();

	std::shared_ptr<AbstractValue> Raw();

	std::vector<byte> Encode();

	std::vector<byte> Decode();

	template <class T>
	Operation& operator<<(const T& obj);

	std::string GetName() const;

	const std::vector<std::shared_ptr<client::GTXValue>>& GetArgs() const;

  private:
    std::string op_name_;
	std::shared_ptr<ArrayValue> raw_args_;
    std::vector<std::shared_ptr<client::GTXValue>> args_;

	Operation();
};

}  // namespace postchain
}  // namespace chromia

#endif