#include "transaction.h"
#include "GTX/gtx.h"
#include "postchain_util.h"

namespace chromia {
namespace postchain {

Transaction::Transaction(std::shared_ptr<client::Gtx> gtx, std::string base_url, std::string brid)
{
	this->gtx_object_ = gtx;
	this->base_url_ = base_url;
	this->brid_ = brid;
}


void Transaction::AddOperation(std::string name, std::shared_ptr<gtv::ArrayValue> args)
{
	this->gtx_object_->AddOperationToGtx(name, args);
}


void Transaction::Sign(std::vector<byte> private_key, std::vector<byte> public_key)
{
	this->gtx_object_->Sign(private_key, public_key);
}


bool Transaction::IsSent()
{
	return this->sent_;
}


std::string Transaction::GetTxRID()
{
	std::vector<byte> buffer_to_sign = this->gtx_object_->GetBufferToSign();
	return PostchainUtil::ByteVectorToHexString(buffer_to_sign);
}

}  // namespace postchain
}  // namespace chromia
