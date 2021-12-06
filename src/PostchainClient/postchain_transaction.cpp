#include "postchain_transaction.h"
#include "GTX/gtx.h"
#include "postchain_util.h"
#include "../HTTP/httprequest.h"

namespace chromia {
namespace postchain {

	PostchainTransaction::PostchainTransaction(std::shared_ptr<client::Gtx> gtx, std::string base_url, std::string brid,
		std::function<void(std::string)> on_error)
{
	this->gtx_object_ = gtx;
	this->base_url_ = base_url;
	this->brid_ = brid;
	this->on_error_ = on_error;
}


void PostchainTransaction::AddOperation(std::string name, std::shared_ptr<gtv::ArrayValue> args)
{
	this->gtx_object_->AddOperationToGtx(name, args);
}


void PostchainTransaction::Sign(std::vector<byte> private_key, std::vector<byte> public_key)
{
	this->gtx_object_->Sign(private_key, public_key);
}


bool PostchainTransaction::IsSent()
{
	return this->sent_;
}


std::string PostchainTransaction::GetTxRID()
{
	std::vector<byte> buffer_to_sign = this->gtx_object_->GetBufferToSign();
	return PostchainUtil::ByteVectorToHexString(buffer_to_sign);
}

void PostchainTransaction::Post(std::function<void(std::string)> on_success)
{
	if (this->sent_)
	{
		this->on_error_("Tried to send tx twice");
	}
	else
	{
		std::string url = this->base_url_ + "/tx/" + this->brid_;
		std::string payload = "{\"tx\": \"" + Serialize() + "\"}";
		postchain::http::HttpRequest::SendPostRequest(url, payload, on_success, on_error_);
		this->sent_ = true;
	}
}

void PostchainTransaction::PostAndWait(std::function<void(std::string)> on_success)
{
	this->Post([] (std::string content) {
		content;
	});
	this->WaitConfirmation(on_success);
}

std::string PostchainTransaction::Serialize()
{
	return this->gtx_object_->Serialize();
}

std::vector<byte> PostchainTransaction::Encode()
{
	return this->gtx_object_->Encode();
}

void PostchainTransaction::WaitConfirmation(std::function<void(std::string)> on_success)
{
	std::string url = this->base_url_ + "/tx/" + this->brid_ + "/" + GetTxRID() + "/status";

	std::string req_content = "";
	std::string req_error = "";

	postchain::http::HttpRequest::SendGetRequestSync(url, req_content, req_error);

	this->error_ = true;

	if (req_error.size() > 0) {
		
		this->on_error_(req_error);
	}

	nlohmann::json json_content;

	try 
	{
		json_content = nlohmann::json::parse(req_content);
	}
	catch (std::exception e)
	{
		on_error_(e.what());
	}

	std::string status = PostchainUtil::GetSafeJSONString(json_content, "status");

	if (status.compare("confirmed") == 0) 
	{
		this->error_ = false;
		on_success(""); // TO-DO make something with this return string
	}
	else if (status.compare("waiting") == 0) 
	{
		PostchainUtil::SleepForMillis(500);
		WaitConfirmation(on_success);
	}
	else
	{
		this->on_error_("WaitConfirmation failed: " + status);
	}
}

}  // namespace postchain
}  // namespace chromia
