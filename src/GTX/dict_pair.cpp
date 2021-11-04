#include "dict_pair.h"
#include "gtx_value.h"
#include "../ASN1/writer.h"

namespace chromia {
namespace postchain {
namespace client {
	DictPair::DictPair(std::string name, GTXValue *value)
	{
		this->name_ = name;

		if (value == nullptr)
		{
			this->value_ = new GTXValue();
		}
		else
		{
			this->value_ = value;
		}
	}

	bool DictPair::Equals(DictPair *obj)
	{
		if (obj == nullptr)
		{
			return false;
		}
		else 
		{
			return false;
			//TO-DO
			/*return this.Name.Equals(dictPair.Name)
				&& this.Value.Equals(dictPair.Value);*/
		}
	}

	std::vector<unsigned char> DictPair::Encode()
	{
		Writer* messageWriter = new Writer();

		messageWriter->PushSequence();
		messageWriter->WriteUTF8String(this->name_);
		// TO-DO messageWriter->WriteEncodedValue(this->value_->Encode());
		messageWriter->PopSequence();

		return messageWriter->Encode();
	}

	DictPair DictPair::Decode(Reader *sequence)
	{
		DictPair dict = DictPair();
		Reader dictSequence = sequence->ReadSequence();

		dict.name_ = dictSequence.ReadUTF8String();
		//dict->value_ = GTXValue.Decode(dictSequence);

		return dict;
	}
}  // namespace client
}  // namespace postchain
}  // namespace chromia
