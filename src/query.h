#ifndef POSTCHAIN_CLIENT_QUERY_H_
#define POSTCHAIN_CLIENT_QUERY_H_

#include "GTV/abstract_value.h" 
#include "GTV/abstract_value_factory.h" //TO-DO rename this

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {

struct QueryObject
{
	std::string name_;
	std::shared_ptr<AbstractValue> content_;

	QueryObject(std::string name, std::shared_ptr<AbstractValue> content)
	{
		this->name_ = name;
		this->content_ = content;
	}
};

}  // namespace postchain
}  // namespace chromia

#endif //POSTCHAIN_CLIENT_QUERY_H_