#include "test_util.h"
#include "../../src/FT3/User/user.h"
#include "../../src/FT3/User/account.h"
#include "../../src/FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "../../src/FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../src/FT3/Core/key_pair.h"
#include "../../src/PostchainClient/common.h"
#include "../../src/PostchainClient/postchain_util.h"

using namespace chromia;
using namespace chromia::postchain;

int TestUtil::GenerateNumber(int max)
{
	return PostchainUtil::RandomIntInRange(0, max);
}

std::string TestUtil::GenerateAssetName(std::string prefix)
{
	return prefix + "_" + std::to_string(GenerateNumber());
}

std::string TestUtil::GenerateId()
{
	int random_number = GenerateNumber();
	std::vector<BYTE> as_byte = PostchainUtil::IntegerToBinary(random_number);
	std::string sha = PostchainUtil::Sha256(as_byte);
	return sha;
}

std::string TestUtil::BlockchainAccountId(std::string chainId)
{
	std::shared_ptr<ArrayValue> gtv = AbstractValueFactory::EmptyArray();
	gtv->Add(AbstractValueFactory::Build("B"));
	gtv->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector(chainId)));
	std::vector<BYTE> hash = AbstractValue::Hash(gtv);
	return PostchainUtil::ByteVectorToHexString(hash);
}
