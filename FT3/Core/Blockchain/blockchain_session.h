#pragma once

#include "../../User/user.h"
#include "../../User/account.h"
#include "./blockchain.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class BlockchainSession
{
public:
	std::shared_ptr<User> user_;
    std::shared_ptr<Blockchain> blockchain_;

    BlockchainSession(std::shared_ptr<User> user, std::shared_ptr<Blockchain> blockchain)
    {
        this->user_ = user;
        this->blockchain_ = blockchain;
    }

    void GetAccountById(string id, std::function<void(std::shared_ptr<Account>)> on_success, std::function<void(std::string)> on_error)
    {
        //Account.GetById(id, this, onSuccess, onError);
    }

   /* void GetAccountsByParticipantId(std::string id, Action<Account[]> onSuccess, Action<string> onError)
    {
        yield return Account.GetByParticipantId(id, this, onSuccess, onError);
    }

    public IEnumerator GetAccountsByAuthDescriptorId(string id, Action<Account[]> onSuccess, Action<string> onError)
    {
        yield return Account.GetByAuthDescriptorId(id, this, onSuccess, onError);
    }
	*/

    void Query(string query_name, std::vector<QueryObject> query_objects, 
		std::function<void(std::string)> on_success, std::function<void(string)> on_error)
    {
        this->blockchain_->Query(query_name, query_objects, on_success, on_error);
    }
	
    void Call(ft3::Operation operation, 
		std::function<void()> on_success, std::function<void(string)> on_error)
    {
        //this->blockchain_->Call(operation, this->user_, on_success, on_error);
    }
};

} // namespace ft3
} // namespace postchain
} // namespace chromia