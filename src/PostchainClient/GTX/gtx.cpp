#include "gtx.h"
#include "gtx_value.h"
#include "dict_pair.h"
#include "../ASN1/reader.h"
#include "../ASN1/writer.h"
#include "../operation.h"
#include "../postchain_util.h"
#include "../GTV/Merkle/Proof/merkle_proof.h"
#include "../postchain_util.h"

namespace chromia {
namespace postchain {
namespace client {
	
Gtx::Gtx()
{

}


Gtx::Gtx(const std::string& blockchain_rid)
{
	this->blockchain_rid_ = blockchain_rid;
}


std::shared_ptr<GTXValue> Gtx::ArgToGTXValue(std::shared_ptr<gtv::AbstractValue> arg)
{
	std::shared_ptr<GTXValue> gtx_value = std::make_shared<GTXValue>();

	if (std::dynamic_pointer_cast<gtv::NullValue>(arg))
	{
		gtx_value->choice_ = GTXValueChoice::Null;
	}
	else if (std::dynamic_pointer_cast<gtv::IntegerValue>(arg))
	{
		gtx_value->choice_ = GTXValueChoice::Integer;
		gtx_value->integer_ = (std::dynamic_pointer_cast<gtv::IntegerValue>(arg))->GetValue();
	}
	else if (std::dynamic_pointer_cast<gtv::OctetStringValue>(arg))
	{
		gtx_value->choice_ = GTXValueChoice::ByteArray;
		gtx_value->byte_array_ = (std::dynamic_pointer_cast<gtv::OctetStringValue>(arg))->GetValue();
	}
	else if (std::dynamic_pointer_cast<gtv::UTF8StringValue>(arg))
	{
		std::shared_ptr<gtv::UTF8StringValue> casted = std::dynamic_pointer_cast<gtv::UTF8StringValue>(arg); //TO-DO switch to dynamic
		gtx_value->choice_ = GTXValueChoice::String;
		gtx_value->string_ = casted->GetValue();
	}
	else if (std::dynamic_pointer_cast<gtv::ArrayValue>(arg))
	{
		std::shared_ptr<gtv::ArrayValue> casted = std::dynamic_pointer_cast<gtv::ArrayValue>(arg);

		gtx_value->choice_ = GTXValueChoice::Array;

		for(int i=0; i<casted->Size(); i++)
		{
			std::shared_ptr<GTXValue> subArgGtxValue = ArgToGTXValue((*casted)[i]);
			gtx_value->array_.push_back(subArgGtxValue);
		}
	}

	return gtx_value;
}


std::shared_ptr<AbstractValue> Gtx::GtxValueToAbstract(std::shared_ptr<GTXValue> arg)
{
	switch (arg->choice_)
	{
		case (GTXValueChoice::Null):
		{
			return AbstractValueFactory::Build(nullptr);
		}
		case (GTXValueChoice::ByteArray):
		{
			return AbstractValueFactory::Build(arg->byte_array_);
		}
		case (GTXValueChoice::String):
		{
			return AbstractValueFactory::Build(arg->string_);
		}
		case (GTXValueChoice::Integer):
		{
			return AbstractValueFactory::Build(arg->integer_);
		}
		case (GTXValueChoice::Array):
		{
			std::shared_ptr<ArrayValue> abs_array = AbstractValueFactory::EmptyArray();
			for (auto &item : arg->array_)
			{
				abs_array->Add(GtxValueToAbstract(item));
			}
			return abs_array;
		}
		case (GTXValueChoice::Dict):
		{
			throw new std::exception("Chromia.PostchainClient.GTX.Messages GTX::GtxValueToAbstract() Dict type not implemented ");
			break;
		}
		default:
		{
			throw new std::exception("Chromia.PostchainClient.GTX.Messages GTX::GtxValueToAbstract() Unknown choice ");
		}
	}
}


void Gtx::AddSignerToGtx(std::vector<BYTE> signer)
{
	if (this->signatures_.size() != 0)
	{
		throw new std::exception("Cannot add signers to an already signed gtx");
	}

	this->signers_.push_back(signer);
}


Gtx* Gtx::AddOperationToGtx(std::string op_name, std::shared_ptr<ArrayValue> args)
{
	if (this->signatures_.size() != 0)
	{
		throw new std::logic_error("Cannot add function calls to an already signed gtx");
	}

	this->operations_.push_back(std::make_shared<PostchainOperation>(op_name, args));
	return this;
}


void Gtx::Sign(std::vector<BYTE> private_key, std::vector<BYTE> public_key)
{
	std::string private_key_str = PostchainUtil::ByteVectorToHexString(private_key);
	std::string public_key_str = PostchainUtil::ByteVectorToHexString(public_key);
	std::vector<BYTE> buffer_to_sign = this->GetBufferToSign();
	std::vector<BYTE> signature = PostchainUtil::Sign(buffer_to_sign, private_key);
	this->AddSignature(public_key, signature);
}


std::vector<BYTE> Gtx::GetBufferToSign()
{
	std::vector<std::vector<BYTE>> old_signatures = this->signatures_;
	std::shared_ptr<AbstractValue> body = GetGtvTxBody();
	std::vector<BYTE> encoded_buffer = AbstractValue::Hash(body);
	this->signatures_ = old_signatures;
	return encoded_buffer;
}


std::shared_ptr<AbstractValue> Gtx::GetGtvTxBody()
{
	std::shared_ptr<ArrayValue> body = AbstractValueFactory::EmptyArray();

	// Add blockchain RID
	std::vector<BYTE> rid_as_byte = PostchainUtil::HexStringToByteVector(this->blockchain_rid_);
	body->Add(AbstractValueFactory::Build(rid_as_byte)); // First Branch

	// Add opearations with arguments
	std::shared_ptr<ArrayValue> body_ops_args = AbstractValueFactory::EmptyArray();
	for (auto &op : this->operations_)
	{
		body_ops_args->Add(op->Raw());
	}
	body->Add(body_ops_args);

	// Add signers
	std::shared_ptr<ArrayValue> body_signers = AbstractValueFactory::EmptyArray();
	for (auto &signer : this->signers_)
	{
		body_signers->Add(AbstractValueFactory::Build(signer));
	}
	body->Add(body_signers);

	return body;
}


int Gtx::GetSignerIndex(std::vector<BYTE> &public_key)
{
	int index = -1;

	for (auto &signer : this->signers_)
	{
		index += 1;

		if (signer.size() != public_key.size()) continue;

		bool equal = true;
		for (int i = 0; i < public_key.size(); i++)
		{
			if (public_key[i] != signer[i])
			{
				equal = false;
				break;
			}
		}
		if (equal) return index;
	}

	return -1;
}

void Gtx::AddSignature(std::vector<BYTE> public_key, std::vector<BYTE> signature)
{
	if (this->signatures_.size() == 0)
	{
		for(auto &signer : this->signers_)
		{
			this->signatures_.push_back(std::vector<BYTE>());
		}
	}

	int signer_index = GetSignerIndex(public_key);
	
	if (signer_index == -1)
	{
		throw new std::logic_error("No such signer, remember to call addSignerToGtx() before adding a signature");
	}

	this->signatures_[signer_index] = signature;
}

std::string Gtx::Serialize()
{
	std::vector<BYTE> byte_encoded = Encode();
	std::string str_encoded = PostchainUtil::ByteVectorToHexString(byte_encoded);
	return str_encoded;
}

std::vector<BYTE> Gtx::Encode()
{
	auto abs_signatures = AbstractValueFactory::EmptyArray();
	for (auto & sig : this->signatures_)
	{
		abs_signatures->Add(AbstractValueFactory::Build(sig));
	}

	std::shared_ptr<ArrayValue> gtv_body = std::dynamic_pointer_cast<ArrayValue>(GetGtvTxBody());

	std::shared_ptr<ArrayValue> gtx_body = AbstractValueFactory::EmptyArray();
	gtx_body->Add(gtv_body);
	gtx_body->Add(abs_signatures);

	std::shared_ptr<GTXValue> gtx_value = Gtx::ArgToGTXValue(gtx_body);
	std::vector<BYTE> encoded = gtx_value->Encode();

	return encoded;
}


std::shared_ptr<Gtx> Gtx::Decode(std::vector<BYTE> encoded_message)
{
	auto gtx = std::make_shared<Gtx>();

	auto reader = new asn1::Reader(encoded_message);
	auto gtx_value = GTXValue::Decode(reader);

	if (gtx_value->array_.size() == 0)
	{
		throw std::exception("Gtx::Decode failed");
	}

	auto gtx_payload = gtx_value->array_[0];

	if (gtx_payload->array_.size() < 3)
	{
		throw std::exception("Gtx::Decode failed");
	}

	gtx->blockchain_rid_ = PostchainUtil::ByteVectorToHexString(gtx_payload->array_[0]->byte_array_);

	for(auto &op_arr : gtx_payload->array_[1]->array_)
	{
		if (op_arr->array_.size() < 2)
		{
			throw std::exception("Gtx::Decode failed");
		}
		std::string op_name = op_arr->array_[0]->string_;
		std::shared_ptr<ArrayValue> op_args = std::dynamic_pointer_cast<ArrayValue>(Gtx::GtxValueToAbstract(op_arr->array_[1]));
		gtx->AddOperationToGtx(op_name, op_args);
	}

	for(auto &signer : gtx_payload->array_[2]->array_)
	{
		gtx->AddSignerToGtx(signer->byte_array_);
	}

	for(auto sig : gtx_value->array_[1]->array_)
	{
		gtx->signatures_.push_back(sig->byte_array_);
	}

	return gtx;
}

}  // namespace client
}  // namespace postchain
}  // namespace chromia
