#include "gtx.h"
#include "gtx_value.h"
#include "dict_pair.h"
#include "../ASN1/reader.h"
#include "../ASN1/writer.h"
#include "../operation.h"
#include "../postchain_util.h"
#include "../GTV/Merkle/Proof/merkle_proof.h"
#include "CoreMinimal.h"
#include "../postchain_util.h"
#include "../../../ChromaUnreal/Utils.h"

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
		gtx_value->integer_ = (long) (std::dynamic_pointer_cast<gtv::IntegerValue>(arg))->GetValue();
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


void Gtx::AddSignerToGtx(std::vector<byte> signer)
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

	this->operations_.push_back(new PostchainOperation(op_name, args));
	return this;
}


void Gtx::Sign(std::vector<byte> private_key, std::vector<byte> public_key)
{
	std::string private_key_str = PostchainUtil::ByteVectorToHexString(private_key);
	//UE_LOG(LogTemp, Display, TEXT("CHROMA:: Gtx Sign() private key: %s"), *(ChromaUtils::STDStringToFString(private_key_str)));

	std::string public_key_str = PostchainUtil::ByteVectorToHexString(public_key);
	//UE_LOG(LogTemp, Display, TEXT("CHROMA:: Gtx Sign() public key: %s"), *(ChromaUtils::STDStringToFString(public_key_str)));

	std::vector<byte> buffer_to_sign = this->GetBufferToSign();

	std::vector<byte> signature = PostchainUtil::Sign(buffer_to_sign, private_key);

	this->AddSignature(public_key, signature);
}


std::vector<byte> Gtx::GetBufferToSign()
{
	std::vector<std::vector<byte>> old_signatures = this->signatures_;
	std::shared_ptr<AbstractValue> body = GetGtvTxBody();

	std::shared_ptr<GTXValue> gtx_value = Gtx::ArgToGTXValue(body);
	std::string gtx_str = gtx_value->ToString();
	//UE_LOG(LogTemp, Display, TEXT("CHROMA:: GetBufferToSign() GTX body: %d  %s"), gtx_str.size(), *(ChromaUtils::STDStringToFString(gtx_str)));

	std::vector<byte> encoded_buffer = AbstractValue::Hash(body);

	std::string buffer_str = PostchainUtil::ByteVectorToHexString(encoded_buffer);
	//UE_LOG(LogTemp, Display, TEXT("CHROMA:: Gtx Sign() Encoded Buffer: %s"), *(ChromaUtils::STDStringToFString(buffer_str)));

	this->signatures_ = old_signatures;
	return encoded_buffer;
}


std::shared_ptr<AbstractValue> Gtx::GetGtvTxBody()
{
	std::shared_ptr<ArrayValue> body = AbstractValueFactory::EmptyArray();

	// Add blockchain RID
	std::vector<byte> rid_as_byte = PostchainUtil::HexStringToByteVector(this->blockchain_rid_);
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


int Gtx::GetSignerIndex(std::vector<byte> &public_key)
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

void Gtx::AddSignature(std::vector<byte> public_key, std::vector<byte> signature)
{
	if (this->signatures_.size() == 0)
	{
		for(auto &signer : this->signers_)
		{
			this->signatures_.push_back(std::vector<byte>());
		}
	}

	int signer_index = GetSignerIndex(public_key);
	UE_LOG(LogTemp, Warning, TEXT("Gtx::AddSignature: [%d] [%s] [%s]"), signer_index, 
		*(ChromaUtils::STDStringToFString(PostchainUtil::ByteVectorToHexString(public_key))),
		*(ChromaUtils::STDStringToFString(PostchainUtil::ByteVectorToHexString(signature)))
	);

	//int signer_index = this->signers_.FindIndex(signer = > signer.SequenceEqual(pubKeyBuffer));

	if (signer_index == -1)
	{
		throw new std::logic_error("No such signer, remember to call addSignerToGtx() before adding a signature");
	}

	this->signatures_[signer_index] = signature;
}

std::string Gtx::Serialize()
{
	std::vector<byte> byte_encoded = Encode();
	std::string str_encoded = PostchainUtil::ByteVectorToHexString(byte_encoded);
	return str_encoded;
}

std::vector<byte> Gtx::Encode()
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

	std::string gtv_hex = PostchainUtil::ByteVectorToHexString(AbstractValue::Hash(gtv_body));
	std::string gtx_hex = PostchainUtil::ByteVectorToHexString(AbstractValue::Hash(gtx_body));

	std::shared_ptr<GTXValue> gtx_value = Gtx::ArgToGTXValue(gtx_body);

	std::string gtx_str = gtx_value->ToString();
	UE_LOG(LogTemp, Warning, TEXT("CHROMA::gtx string: [%d] [%s]"), gtx_str.size(), *(ChromaUtils::STDStringToFString(gtx_str)));

	std::vector<byte> encoded = gtx_value->Encode();

	std::string encoded_str = PostchainUtil::ByteVectorToHexString(encoded);
	//UE_LOG(LogTemp, Warning, TEXT("CHROMA::encoded_str: [%d] [%s]"), encoded_str.size(), *(ChromaUtils::STDStringToFString(encoded_str)));

	return encoded;
}

}  // namespace client
}  // namespace postchain
}  // namespace chromia
