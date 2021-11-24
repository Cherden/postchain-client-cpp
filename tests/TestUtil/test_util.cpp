//#ifdef FT3_TESTS

#include "test_util.h"
#include "../../FT3/User/user.h"
#include "../../FT3/User/account.h"
#include "../../FT3/User/AuthDescriptor/single_signature_auth_descriptor.h"
#include "../../FT3/User/AuthDescriptor/multi_signature_auth_descriptor.h"
#include "../../FT3/Core/key_pair.h"
#include "../../src/common.h"
#include "../../src/postchain_util.h"

using namespace chromia;
using namespace chromia::postchain;

int TestUtil::GenerateNumber(int max)
{
	return PostchainUtil::RandomIntInRange(0, max);
}

std::string TestUtil::GenerateAssetName(std::string prefix)
{
	return prefix + "_" + std::to_string(GenerateNumber());
	//return prefix + "_" + std::to_string(123456);
}

std::string TestUtil::GenerateId()
{
	int random_number = GenerateNumber();
	//int random_number = 654321;
	std::vector<byte> as_byte = PostchainUtil::IntegerToBinary(random_number);
	std::string sha = PostchainUtil::Sha256(as_byte);
	return sha;
}

std::string TestUtil::BlockchainAccountId(std::string chainId)
{
	std::shared_ptr<ArrayValue> gtv = AbstractValueFactory::EmptyArray();
	gtv->Add(AbstractValueFactory::Build("B"));
	gtv->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector(chainId)));
	std::vector<byte> hash = AbstractValue::Hash(gtv);
	return PostchainUtil::ByteVectorToHexString(hash);
}


//#endif // FT3_TESTS