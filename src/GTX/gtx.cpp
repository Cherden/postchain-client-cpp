#include "gtx.h"
#include "gtx_value.h"
#include "dict_pair.h"
#include "../ASN1/reader.h"
#include "../ASN1/writer.h"
#include "../operation.h"
#include "../postchain_util.h"
#include "../GTV/Merkle/Proof/merkle_proof.h"
#include "CoreMinimal.h"


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

	if (arg == nullptr)
	{
		gtx_value->choice_ = GTXValueChoice::Null;
	}

	if (std::dynamic_pointer_cast<gtv::IntegerValue>(arg))
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::ArgToGTXValue::GTXValueChoice::IntegerValue length"));
		gtx_value->choice_ = GTXValueChoice::Integer;
		gtx_value->integer_ = (long) (std::static_pointer_cast<gtv::IntegerValue>(arg))->GetValue();
	}
	else if (std::dynamic_pointer_cast<gtv::OctetStringValue>(arg))
	{
		UE_LOG(LogTemp, Display, TEXT("CHROMA::ArgToGTXValue::GTXValueChoice::OctetStringValue length [%d]"), (std::static_pointer_cast<gtv::OctetStringValue>(arg))->GetValue().size());
		gtx_value->choice_ = GTXValueChoice::ByteArray;
		gtx_value->byte_array_ = (std::static_pointer_cast<gtv::OctetStringValue>(arg))->GetValue();
	}
	else if (std::dynamic_pointer_cast<gtv::UTF8StringValue>(arg))
	{
		std::shared_ptr<gtv::UTF8StringValue> casted = std::static_pointer_cast<gtv::UTF8StringValue>(arg);
		UE_LOG(LogTemp, Display, TEXT("CHROMA::ArgToGTXValue::GTXValueChoice::UTF8StringValue length [%s]"), *FString(UTF8_TO_TCHAR(casted->GetValue().c_str())));
		gtx_value->choice_ = GTXValueChoice::String;
		gtx_value->string_ = casted->GetValue();
	}
	else if (std::dynamic_pointer_cast<gtv::ArrayValue>(arg))
	{
		std::shared_ptr<gtv::ArrayValue> casted = std::dynamic_pointer_cast<gtv::ArrayValue>(arg);
		UE_LOG(LogTemp, Display, TEXT("CHROMA::ArgToGTXValue::GTXValueChoice::Array length [%d]"), casted->Size());

		gtx_value->choice_ = GTXValueChoice::Array;

		for(int i=0; i<casted->Size(); i++)
		{
			std::shared_ptr<GTXValue> subArgGtxValue = ArgToGTXValue((*casted)[i]);
			gtx_value->array_.push_back(subArgGtxValue);
		}

		//gtxValue->array_ = new List<GTXValue>();
	}
	/*
	else if (arg is object[])
	{
		var array = (object[])arg;
		gtxValue.Choice = GTXValueChoice.Array;

		gtxValue.Array = new List<GTXValue>();
		foreach(var subArg in array)
		{
			gtxValue.Array.Add(ArgToGTXValue((object)subArg));
		}
	}
	else if (arg is Dictionary<string, object>)
	{
		gtxValue.Choice = GTXValueChoice.Dict;

		var dict = (Dictionary<string, object>)arg;

		gtxValue.Dict = new List<DictPair>();
		foreach(var dictPair in dict)
		{
			gtxValue.Dict.Add(new DictPair(dictPair.Key, ArgToGTXValue(dictPair.Value)));
		}
	}
	else if (arg is Operation)
	{
		return ((Operation)arg).ToGtxValue();
	}
	else
	{
		throw new System.Exception("Chromia.PostchainClient.GTX Gtx.ArgToGTXValue() Can't create GTXValue out of type " + arg.GetType());
	}*/


	return gtx_value;
}


void Gtx::AddSignerToGtx(std::vector<byte> signer)
{
	if (this->signatures_.size() != 0)
	{
		return;
		// TO-DO handle errors
		// throw new Exception("Cannot add signers to an already signed gtx");
	}

	this->signers_.push_back(signer);
}


Gtx* Gtx::AddOperationToGtx(std::string op_name, std::shared_ptr<ArrayValue> args)
{
	if (this->signatures_.size() != 0)
	{
		throw new std::logic_error("Cannot add function calls to an already signed gtx");
	}

	this->operations_.push_back(new Operation(op_name, args));
	return this;
}


void Gtx::Sign(std::vector<byte> private_key, std::vector<byte> public_key)
{
	std::vector<byte> buffer_to_sign = this->GetBufferToSign();

	std::vector<byte> signature = PostchainUtil::Sign(buffer_to_sign, private_key);

	this->AddSignature(public_key, signature);
}


std::vector<byte> Gtx::GetBufferToSign()
{
	std::vector<std::vector<byte>> old_signatures = this->signatures_;

	std::shared_ptr<AbstractValue> body = GetGtvTxBody();
	std::vector<byte> encoded_buffer = AbstractValue::Hash(body);

	FString asHex = UTF8_TO_TCHAR(PostchainUtil::ByteVectorToHexString(encoded_buffer).c_str());

	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest() len: %d   %d  [%s]"), encoded_buffer.size(), asHex.Len(), *asHex);

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

		for (int i = 0; i < public_key.size(); i++)
		{
			if (public_key[i] != signer[i]) continue;
		}

		return index;
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

	//int signer_index = this->signers_.FindIndex(signer = > signer.SequenceEqual(pubKeyBuffer));

	if (signer_index == -1)
	{
		throw new std::logic_error("No such signer, remember to call addSignerToGtx() before adding a signature");
	}

	this->signatures_[signer_index] = signature;
}

std::string Gtx::Serialize()
{
	UE_LOG(LogTemp, Display, TEXT("Gtx::Serialize()"))

	std::vector<byte> byte_encoded = Encode();
	std::string str_encoded = PostchainUtil::ByteVectorToHexString(byte_encoded);

	UE_LOG(LogTemp, Display, TEXT("Gtx::Serialize()::ProcessRequest() byte_encoded: %d  str_encoded: %d"), byte_encoded.size(), str_encoded.size());

	return str_encoded;
}

std::vector<byte> Gtx::Encode()
{
	UE_LOG(LogTemp, Display, TEXT("Gtx::Serialize()"));

	auto abs_signatures = AbstractValueFactory::EmptyArray();
	for (auto & sig : this->signatures_)
	{
		UE_LOG(LogTemp, Display, TEXT("add signature"));
		abs_signatures->Add(AbstractValueFactory::Build(sig));
	}

	std::shared_ptr<ArrayValue> gtv_body = std::dynamic_pointer_cast<ArrayValue>(GetGtvTxBody());

	std::shared_ptr<ArrayValue> gtx_body = AbstractValueFactory::EmptyArray();
	gtx_body->Add(gtv_body);
	gtx_body->Add(abs_signatures);

	std::string gtv_hex = PostchainUtil::ByteVectorToHexString(AbstractValue::Hash(gtv_body));
	std::string gtx_hex = PostchainUtil::ByteVectorToHexString(AbstractValue::Hash(gtx_body));

	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest() gtv_hex: %s"), *(FString(UTF8_TO_TCHAR(gtv_hex.c_str()))));
	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest() gtx_hex: %s"), *(FString(UTF8_TO_TCHAR(gtx_hex.c_str()))));

	std::shared_ptr<GTXValue> gtx_value = Gtx::ArgToGTXValue(gtx_body);
	UE_LOG(LogTemp, Display, TEXT("CHROMA::ProcessRequest() gtv_body: %d  gtx_value: %d"), gtv_body->Size(), gtx_body->Size());

	std::vector<byte> encoded = gtx_value->Encode();

	return encoded;
}

/*


	
Gtx Gtx::Decode(std::vector<byte> encodedMessage)
{

}

int Gtx::GetLength(std::vector<byte> encodedMessage)
{

}

byte Gtx::GetOctetLength(std::vector<byte> encodedMessage)
{

}
*/
}  // namespace client
}  // namespace postchain
}  // namespace chromia
