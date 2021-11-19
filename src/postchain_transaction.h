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

	PostchainTransaction(std::shared_ptr<client::Gtx> gtx, std::string base_url, std::string brid, std::function<void(std::string)> on_error);
	
	void AddOperation(std::string name, std::shared_ptr<gtv::ArrayValue> args);

	void Sign(std::vector<byte> private_key, std::vector<byte> public_key);

	std::string Serialize();

	std::string GetTxRID();

	void Post(std::function<void(std::string)> on_success);

	void PostAndWait(std::function<void(std::string)> on_success);

	std::vector<byte> Encode();

	bool IsSent();
private:
	std::shared_ptr<client::Gtx> gtx_object_;
	std::string base_url_;
	std::string brid_;
	bool sent_ = false;
	bool error_;

	std::function<void(std::string)> on_error_ = nullptr;

	void WaitConfirmation(std::function<void(std::string)> on_success);

};
}  // namespace postchain
}  // namespace chromia

#endif