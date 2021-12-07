#pragma once

#include "forward_declarations.h"
#include "GTV/abstract_value.h" 
#include "GTV/abstract_value_factory.h"
#include <string>

using namespace chromia::postchain::gtv;
using namespace chromia;

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

	QueryObject(std::string name, int content)
	{
		this->name_ = name;
		this->content_ = AbstractValueFactory::Build(content);
	}

	QueryObject(std::string name, std::string content)
	{
		this->name_ = name;
		this->content_ = AbstractValueFactory::Build(content);
	}

	QueryObject(std::string name, std::vector<unsigned char> content)
	{
		this->name_ = name;
		this->content_ = AbstractValueFactory::Build(content);
	}
};

}  // namespace postchain
}  // namespace chromia
