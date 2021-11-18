#include "postchain_transaction.h"
#include "GTX/gtx.h"
#include "postchain_util.h"
#include "../HTTP/httprequest.h"
#include "CoreMinimal.h"

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

void PostchainTransaction::PostAndWait(std::function<void()> callback)
{
	if (this->sent_)
	{
		this->on_error_("Tried to send tx twice");
	}
	else
	{
		std::string payload = "{\"tx\": \"" + Serialize() + "\"}";

		std::string url = this->base_url_ + "/tx/" + this->brid_;
		UHttpRequest::SendPostRequest(url, payload, [callback](std::string content) { callback(); }, on_error_);
		this->sent_ = true;
	}
}

std::string PostchainTransaction::Serialize()
{
	return this->gtx_object_->Serialize();
}

std::vector<byte> PostchainTransaction::Encode()
{
	//TO-DO
	return {};
}

}  // namespace postchain
}  // namespace chromia
