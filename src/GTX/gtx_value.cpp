#include "gtx_value.h"
#include "dict_pair.h"
#include "../ASN1/writer.h"

namespace chromia {
namespace postchain {
namespace client {

//TO-DO move this to some class
bool is_big_endian(void)
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };

	return bint.c[0] == 1;
}


GTXValue::GTXValue()
{
	this->choice_ = GTXValueChoice::NotSet;
}


bool GTXValue::Equals(GTXValue *obj)
{
	if (obj == nullptr)
	{
		return false;
	}
	else 
	{
		return false;
		// TO-DO
		/*return this.Choice.Equals(gtxValue.Choice)
			&& ((this.ByteArray == null || gtxValue.ByteArray == null) ? this.ByteArray == gtxValue.ByteArray : Enumerable.SequenceEqual(this.ByteArray, gtxValue.ByteArray))
			&& ((this.String == null || gtxValue.String == null) ? this.String == gtxValue.String : this.String.Equals(gtxValue.String))
			&& this.Integer.Equals(gtxValue.Integer)
			&& ((this.Dict == null || gtxValue.Dict == null) ? this.Dict == gtxValue.Dict : Enumerable.SequenceEqual(this.Dict, gtxValue.Dict))
			&& ((this.Array == null || gtxValue.Array == null) ? this.Array == gtxValue.Array : Enumerable.SequenceEqual(this.Array, gtxValue.Array));*/
	}
}


std::vector<unsigned char> GTXValue::Encode()
{
	// TO-DO update naming convenrions

	Writer messageWriter;

	std::vector<unsigned char> choiceConstants;
	
	switch (this->choice_)
	{
	case (GTXValueChoice::Null):
	{
		choiceConstants.push_back(0xa0);
		messageWriter.WriteNull();
		break;
	}
	// The CHOICE in Asn1 is not implement in the used (experimental) library, yet.
	// Therefore we have to hack around a bit and create the bytes manually.
	// Since we can't seem to access the standard, we observed that the 2 octets are structured as follow:
	// |--0xa--| |--type--| |----length----|
	case (GTXValueChoice::ByteArray):
	{
		choiceConstants.push_back(0xa1);
		messageWriter.WriteOctetString(this->byte_array_);
		break;
	}
	case (GTXValueChoice::String):
	{
		choiceConstants.push_back(0xa2);
		messageWriter.WriteUTF8String(this->string_);
		break;
	}
	case (GTXValueChoice::Integer):
	{
		choiceConstants.push_back(0xa3);
		messageWriter.WriteInteger(this->integer_);
		break;
	}
	case (GTXValueChoice::Array):
	{
		choiceConstants.push_back(0xa5);
		messageWriter.PushSequence();
		for(auto &gtxValue : this->array_)
		{
			messageWriter.WriteEncodedValue(gtxValue->Encode());
		}
		messageWriter.PopSequence();
		break;
	}
	case (GTXValueChoice::Dict):
	{
		choiceConstants.push_back(0xa4);
		messageWriter.PushSequence();
		for(auto &dictPair : this->dict_)
		{
			messageWriter.WriteEncodedValue(dictPair->Encode());
		}
		messageWriter.PopSequence();
		break;
	}
	default:
	{
		// TO-DO handle errors
		//throw new System.Exception("Chromia.PostchainClient.GTX.Messages GTXValue.Encode() GTXValueChoice.Default case. Unknown choice " + this.Choice);
	}
	}

	int choiceSize = messageWriter.GetEncodedLength();

	if (choiceSize < 128)
	{
		choiceConstants.push_back((unsigned char) choiceSize);
	}
	else
	{
		char* choiceSizeAsBytes = static_cast<char*>(static_cast<void*>(&choiceSize));

		std::vector<unsigned char> sizeInBytes = TrimByteList(choiceSizeAsBytes, 4);
		byte sizeLength = (byte) sizeInBytes.size();
		choiceConstants.push_back((byte)(0x80 + sizeLength));
		
		if (!is_big_endian()) // is LittleEndian
		{
			std::reverse(sizeInBytes.begin(), sizeInBytes.end());
		}

		for (size_t i = 0; i < sizeInBytes.size(); i++)
		{
			choiceConstants.push_back(sizeInBytes[i]);
		}
	}
	
	std::vector<unsigned char> writer_encoded = messageWriter.Encode();
	
	for (int i = 0; i < writer_encoded.size(); i++)
	{
		choiceConstants.push_back(writer_encoded[i]);
	}

	return choiceConstants;
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
	//TO-DO
	return std::string();
}

}  // namespace client
}  // namespace postchain
}  // namespace chromia
