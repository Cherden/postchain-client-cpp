#include "dict_pair.h"
#include "gtx_value.h"
#include "../ASN1/writer.h"

namespace chromia {
namespace postchain {
namespace client {
	DictPair::DictPair(std::string name, std::shared_ptr<GTXValue> value)
	{
		this->name_ = name;

		if (value == nullptr)
		{
			this->value_ = std::make_shared<GTXValue>();
		}
		else
		{
			this->value_ = value;
		}
	}


	std::vector<unsigned char> DictPair::Encode()
	{
		Writer* messageWriter = new Writer();

		messageWriter->PushSequence();
		messageWriter->WriteUTF8String(this->name_);
		messageWriter->WriteEncodedValue(this->value_->Encode());
		messageWriter->PopSequence();

		return messageWriter->Encode();
	}

	std::shared_ptr<DictPair> DictPair::Decode(Reader* sequence)
	{
		std::shared_ptr<DictPair> dict = std::make_shared<DictPair>();
		Reader dict_sequence = sequence->ReadSequence();
		dict->name_ = dict_sequence.ReadUTF8String();
		dict->value_ = GTXValue::Decode(&dict_sequence);
		return dict;
	}
}  // namespace client
}  // namespace postchain
}  // namespace chromia
