#include "operation.h"
#include "GTX/gtx.h"
#include "GTV/abstract_value_factory.h"

namespace chromia {
namespace postchain {

std::string PostchainOperation::GetName() const
{ 
	return op_name_; 
}

const std::vector<std::shared_ptr<client::GTXValue>>& PostchainOperation::GetArgs() const
{
	return args_;
}

const std::shared_ptr<ArrayValue>& PostchainOperation::GetRawArgs() const
{
	return raw_args_;
}

std::shared_ptr<AbstractValue> PostchainOperation::Raw()
{
	std::shared_ptr<ArrayValue> out = AbstractValueFactory::EmptyArray();
	out->Add(AbstractValueFactory::Build(this->op_name_));
	out->Add(this->raw_args_);

	/*
	for (auto & arg : this->raw_args_)
	{
		out.push_back(arg);
	}*/
	
	return out;
}

PostchainOperation::PostchainOperation(std::string op_name, std::shared_ptr<gtv::ArrayValue> args)
{
	this->op_name_ = op_name;
	this->raw_args_ = args;

	for (int i=0; i<args->Size(); i++)
	{
		this->args_.push_back(client::Gtx::ArgToGTXValue((*args)[i]));
	}
}

template <class T>
PostchainOperation& PostchainOperation::operator<<(const T& obj)
{
	args_.push_back(gtv::AbstractValueFactory::Build(obj));
	return *this;
}

}  // namespace postchain
}  // namespace chromia