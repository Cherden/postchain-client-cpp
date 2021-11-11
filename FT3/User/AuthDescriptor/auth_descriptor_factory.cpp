#include "auth_descriptor_factory.h"
#include "../account.h"
#include "single_signature_auth_descriptor.h"
#include "multi_signature_auth_descriptor.h"

namespace chromia {
namespace postchain {
namespace ft3 {

std::shared_ptr<AuthDescriptor> AuthDescriptorFactory::Create(AuthType type, std::vector<byte> args)
{
	switch (type)
	{
	case AuthType::eSingleSig:
		return std::dynamic_pointer_cast<AuthDescriptor>(this->CreateSingleSig(args));
	case AuthType::eMultiSig:
		return std::dynamic_pointer_cast<AuthDescriptor>(this->CreateMultiSig(args));
	}
	return nullptr;
}

std::shared_ptr<SingleSignatureAuthDescriptor> AuthDescriptorFactory::CreateSingleSig(std::vector<byte> args)
{
	/*var gtxTransaction = new AsnReader(args);
	var gtxValue = GTXValue.Decode(gtxTransaction);
	var flags = new List<FlagsType>();

	foreach(var flag in gtxValue.Array[0].Array)
	{
		flags.Add(Util.StringToFlagType((string)flag.String));
	}

	return new SingleSignatureAuthDescriptor(
		Util.HexStringToBuffer((string)gtxValue.Array[1].String),
		flags.ToArray()
	);*/
	return nullptr;
}

std::shared_ptr<MultiSignatureAuthDescriptor> AuthDescriptorFactory::CreateMultiSig(std::vector<byte> args)
{
	/*var gtxTransaction = new AsnReader(args);
	var gtxValue = GTXValue.Decode(gtxTransaction);

	var flags = new List<FlagsType>();
	var pubKeys = new List<byte[]>();
	var signatureRequired = (int)gtxValue.Array[1].Integer;

	foreach(var flag in gtxValue.Array[0].Array)
	{
		flags.Add(Util.StringToFlagType((string)flag.String));
	}

	foreach(var pubKey in gtxValue.Array[2].Array)
	{
		pubKeys.Add(Util.HexStringToBuffer((string)pubKey.String));
	}

	return new MultiSignatureAuthDescriptor(
		pubKeys, signatureRequired, flags.ToArray()
	);*/
	return nullptr;
}

} // namespace http
} // namespace postchain
} // namespace chromia