#include "gtx_value.h"
#include "dict_pair.h"
#include "../ASN1/writer.h"
#include "../postchain_util.h"

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


}  // namespace client
}  // namespace postchain
}  // namespace chromia
