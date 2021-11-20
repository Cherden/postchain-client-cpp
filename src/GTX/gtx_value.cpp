#include "gtx_value.h"
#include "dict_pair.h"
#include "../ASN1/writer.h"
#include "../ASN1/asn_util.h"
#include "../postchain_util.h"
#include "CoreMinimal.h"
#include "../../../ChromaUnreal/Utils.h"

namespace chromia {
namespace postchain {
namespace client {

GTXValue::GTXValue()
{
	this->choice_ = GTXValueChoice::NotSet;
}


std::vector<unsigned char> GTXValue::Encode()
{
	Writer message_writer;

	std::vector<unsigned char> choice_constants;
	
	switch (this->choice_)
	{
	case (GTXValueChoice::Null):
	{
		choice_constants.push_back(0xa0);
		message_writer.WriteNull();
		break;
	}
	// The CHOICE in Asn1 is not implement in the used (experimental) library, yet.
	// Therefore we have to hack around a bit and create the bytes manually.
	// Since we can't seem to access the standard, we observed that the 2 octets are structured as follow:
	// |--0xa--| |--type--| |----length----|
	case (GTXValueChoice::ByteArray):
	{
		choice_constants.push_back(0xa1);
		message_writer.WriteOctetString(this->byte_array_);
		break;
	}
	case (GTXValueChoice::String):
	{
		choice_constants.push_back(0xa2);
		message_writer.WriteUTF8String(this->string_);
		break;
	}
	case (GTXValueChoice::Integer):
	{
		choice_constants.push_back(0xa3);
		message_writer.WriteInteger(this->integer_);
		break;
	}
	case (GTXValueChoice::Array):
	{
		choice_constants.push_back(0xa5);
		message_writer.PushSequence();
		for(auto &gtxValue : this->array_)
		{
			message_writer.WriteEncodedValue(gtxValue->Encode());
		}
		message_writer.PopSequence();
		break;
	}
	case (GTXValueChoice::Dict):
	{
		choice_constants.push_back(0xa4);
		message_writer.PushSequence();
		for(auto &dictPair : this->dict_)
		{
			message_writer.WriteEncodedValue(dictPair->Encode());
		}
		message_writer.PopSequence();
		break;
	}
	default:
	{
		throw new std::exception("Chromia.PostchainClient.GTX.Messages GTXValue.Encode() GTXValueChoice.Default case. Unknown choice ");
	}
	}

	int choice_size = message_writer.GetEncodedLength();

	if (choice_size < 128)
	{
		choice_constants.push_back((unsigned char)choice_size);
	}
	else
	{
		char* choice_size_as_bytes = static_cast<char*>(static_cast<void*>(&choice_size));

		std::vector<unsigned char> size_in_bytes = TrimByteList(choice_size_as_bytes, 4);
		byte sizeLength = (byte)size_in_bytes.size();
		choice_constants.push_back((byte)(0x80 + sizeLength));
		
		if (PostchainUtil::IsLittleEndian())
		{
			std::reverse(size_in_bytes.begin(), size_in_bytes.end());
		}

		for (size_t i = 0; i < size_in_bytes.size(); i++)
		{
			choice_constants.push_back(size_in_bytes[i]);
		}
	}
	
	std::vector<unsigned char> writer_encoded = message_writer.Encode();
	
	for (int i = 0; i < writer_encoded.size(); i++)
	{
		choice_constants.push_back(writer_encoded[i]);
	}

	return choice_constants;
}


std::shared_ptr<GTXValue> GTXValue::Decode(asn1::Reader* sequence)
{
	std::shared_ptr<GTXValue> val = std::make_shared<GTXValue>();

	byte choice = sequence->ReadChoice();
	sequence->ReadLength();

	std::string choice_hex = PostchainUtil::ByteVectorToHexString({ choice });
	UE_LOG(LogTemp, Warning, TEXT("GTXValue::Decode: %s"), *ChromaUtils::STDStringToFString(choice_hex));

	switch (choice)
	{
	case asn1::AsnUtil::TAG_NULL:
	{
		val->choice_ = GTXValueChoice::Null;
		sequence->ReadChoice();
		sequence->ReadChoice();
		break;
	}
	case asn1::AsnUtil::TAG_BYTE_ARRAY:
	{
		val->choice_ = GTXValueChoice::ByteArray;
		val->byte_array_ = sequence->ReadOctetString();
		break;
	}
	case asn1::AsnUtil::TAG_STRING:
	{
		val->choice_ = GTXValueChoice::String;
		val->string_ = sequence->ReadUTF8String();
		break;
	}
	case asn1::AsnUtil::TAG_INTEGER:
	{
		val->choice_ = GTXValueChoice::Integer;
		val->integer_ = sequence->ReadInteger();
		break;
	}
	case asn1::AsnUtil::TAG_ARRAY:
	{
		val->choice_ = GTXValueChoice::Array;
		Reader inner_sequence =sequence->ReadSequence();
		while (inner_sequence.RemainingBytes() > 0)
		{
			val->array_.push_back(GTXValue::Decode(&inner_sequence));
		}
		break;
	}
	case asn1::AsnUtil::TAG_DICT:
	{
		throw std::exception("GTXValue::Decode() asn1::tag::kDict not implemented");
		
		val->choice_ = GTXValueChoice::Dict;
		Reader inner_sequence = sequence->ReadSequence();
		while (inner_sequence.RemainingBytes() > 0)
		{
			val->dict_.push_back(DictPair::Decode(&inner_sequence));
		}
		break;
	}
	default:
	{
		throw std::exception("GTXValue::Decode() Unknown choice tag");
	}
	}

	return val;
}


std::vector<unsigned char> GTXValue::TrimByteList(char* byteList, int length)
{
	std::vector<unsigned char> trimmedBytes;
	for (int i = length - 1; i >= 0; i--)
	{
		if (byteList[i] != 0)
		{
			for (int j = 0; j <= i; j++)
			{
				trimmedBytes.push_back(byteList[j]);
			}

			break;
		}
	}

	return trimmedBytes;
}


std::string GTXValue::ToString()
{
	switch (this->choice_)
	{
	case (GTXValueChoice::Null):
	{
		return "null";
	}
	case (GTXValueChoice::ByteArray):
	{
		return "0x" + PostchainUtil::ByteVectorToHexString(this->byte_array_);
	}
	case (GTXValueChoice::String):
	{
		return this->string_;
	}
	case (GTXValueChoice::Integer):
	{
		return std::to_string(this->integer_);
	}
	case (GTXValueChoice::Array):
	{
		std::string ret = "[";
		if (this->array_.size() == 0)
		{
			return ret + "]";
		}

		for (auto &elm : this->array_)
		{
			std::string elm_str = elm->ToString();
			ret += std::to_string(elm_str.size()) + "_" + elm_str + ", ";
		}

		ret = ret.substr(0, ret.size() - 2);
		return ret + "]";
	}
	case (GTXValueChoice::Dict):
	{

		/*std::string ret = "[";
		if (Dict.Count == 0)
		{
			return ret + "]";
		}

		foreach(var elm in Dict)
		{
			ret += @"{{""" + elm.Name + @""": " + elm.Value.ToString() + "}, ";
		}

		return ret.Remove(ret.Length - 2) + "]";*/
		throw std::exception("GtxValue::ToString() Dict type not implemented");
	}
	default:
	{
		return "";
	}
	}
}

}  // namespace client
}  // namespace postchain
}  // namespace chromia
