#pragma once

#include "../Core/operation.h"
#include "../User/account.h"
#include "../../src/GTV/abstract_value_facotry.h"
#include "../../src/postchain_util.h"
#include "../Core/operation.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class AccountOperations
{
public:
	static std::shared_ptr<ft3::Operation> AddAuthDescriptor(std::string account_id, std::string auth_descriptor_id, std::shared_ptr<AuthDescriptor> auth_descriptor)
    {
		std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
		op_args->Add(AbstractValueFactory::Build(account_id));
		op_args->Add(AbstractValueFactory::Build(auth_descriptor_id));
		op_args->Add(auth_descriptor->ToGTV());
        return std::make_shared<ft3::Operation>("ft3.add_auth_descriptor", op_args);
    }

    static std::shared_ptr<ft3::Operation> Transfer(std::shared_ptr<AbstractValue> inputs, std::shared_ptr<AbstractValue> outputs)
    {
		std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
		op_args->Add(inputs);
		op_args->Add(outputs);
		return std::make_shared<ft3::Operation>("ft3.transfer", op_args);
    }

    static std::shared_ptr<ft3::Operation> XcTransfer(std::shared_ptr<AbstractValue> source, std::shared_ptr<AbstractValue> target, std::vector<std::string> hops)
    {
		std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
		op_args->Add(source);
		op_args->Add(target);
		op_args->Add(AbstractValueFactory::Build(hops));
		return std::make_shared<ft3::Operation>("ft3.xc.init_xfer", op_args);
    }
	
    static std::shared_ptr<ft3::Operation> DeleteAllAuthDescriptorsExclude(std::string account_id, std::string exclude_auth_descriptor_id)
    {
		std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
		op_args->Add(AbstractValueFactory::Build(account_id));
		op_args->Add(AbstractValueFactory::Build(exclude_auth_descriptor_id));
		return std::make_shared<ft3::Operation>("ft3.delete_all_auth_descriptors_exclude", op_args);
    }
	
    static std::shared_ptr<ft3::Operation> DeleteAuthDescriptor(string account_id, string auth_descriptor_id, string delete_auth_descriptor_id)
    {
		std::shared_ptr<ArrayValue> op_args = AbstractValueFactory::EmptyArray();
		op_args->Add(AbstractValueFactory::Build(account_id));
		op_args->Add(AbstractValueFactory::Build(auth_descriptor_id));
		op_args->Add(AbstractValueFactory::Build(delete_auth_descriptor_id));
		return std::make_shared<ft3::Operation>("ft3.delete_auth_descriptor", op_args);
    }
	
    static std::shared_ptr<ft3::Operation> Nop()
    {
		std::shared_ptr<ArrayValue> op_args;
		static int nonce = PostchainUtil::RandomIntInRange(0, 100000); //TO-DO check this static keyword
		op_args->Add(AbstractValueFactory::Build(std::to_string(nonce)));
        return std::make_shared<ft3::Operation>("nop", op_args);
    }

    static std::shared_ptr<ft3::Operation> Op(std::string name, std::shared_ptr<ArrayValue> args)
    {
		return nullptr;
		//return std::make_shared<ft3::Operation>(name, args);
    }
};
} // namespace ft3
} // namespace postchain
} // namespace chromia