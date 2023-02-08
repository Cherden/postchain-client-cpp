#ifndef POSTCHAIN_CLIENT_OPERATION_H_
#define POSTCHAIN_CLIENT_OPERATION_H_

#include <memory>
#include <string>
#include <vector>

#include "common.h"
#include "forward_declarations.h"
#include "GTV/abstract_value.h"
#include "GTV/abstract_value_factory.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {

class PostchainOperation {
  public:

	PostchainOperation(std::string name, std::shared_ptr<ArrayValue> args);

	std::shared_ptr<client::GTXValue> ToGtxValue();

	std::shared_ptr<AbstractValue> Raw();

	std::vector<BYTE> Encode();

	std::vector<BYTE> Decode();

	template <class T>
	PostchainOperation& operator<<(const T& obj);

	std::string GetName() const;

	const std::vector<std::shared_ptr<client::GTXValue>>& GetArgs() const;

	const std::shared_ptr<ArrayValue>& GetRawArgs() const;

  private:
    std::string op_name_;
	std::shared_ptr<ArrayValue> raw_args_;
    std::vector<std::shared_ptr<client::GTXValue>> args_;

	PostchainOperation();
};

}  // namespace postchain
}  // namespace chromia

#endif