#ifndef POSTCHAIN_CLIENT_TRANSACTION_H_
#define POSTCHAIN_CLIENT_TRANSACTION_H_

#include <memory>
#include <string>
#include <vector>

#include "GTV/abstract_value.h"
#include "GTV/abstract_value_factory.h"
#include "GTX/gtx.h"

#include "operation.h"
#include "common.h"

namespace chromia {
namespace postchain {

namespace client {
class Gtx;
}

class TxStatusResponse
{
public:
	std::string status_ = "";
	std::string reject_reason_ = "";
};

class PostchainTransaction {
public:

	PostchainTransaction(std::shared_ptr<client::Gtx> gtx, std::string base_url, std::string brid);
	
	void AddOperation(std::string name, std::shared_ptr<gtv::ArrayValue> args);

	void Sign(std::vector<byte> private_key, std::vector<byte> public_key);

	std::string Serialize()
	{
		return this->gtx_object_->Serialize();
	}

	std::string GetTxRID();

	void Post()
	{
		//TO-DO
	}

	void PostAndWait(std::function<void()> callback)
	{
		//TO-DO
	}

	std::vector<byte> Encode()
	{
		//TO-DO
	}

	bool IsSent();
private:
	std::shared_ptr<client::Gtx> gtx_object_;
	std::string base_url_;
	std::string brid_;
	bool sent_ = false;
	bool error_;

	// TODO implement the onerror callback
};
}  // namespace postchain
}  // namespace chromia

#endif