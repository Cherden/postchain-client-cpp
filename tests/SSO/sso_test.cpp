#include "sso_test.h"

#include <functional>
#include <string>

#include "../../src/common.h"
#include "../../src/postchain_util.h"
#include "../../src/GTX/gtx.h"
#include "../../../ChromaUnreal/Utils.h"

using namespace chromia::postchain::client;
using namespace chromia::postchain;

bool SSOTest::SSORegisterOperationTest()
{
	std::vector<byte> priv_key;
	std::vector<byte> pub_key;
	PostchainUtil::GenerateKeyPair(priv_key, pub_key);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	std::shared_ptr<ArrayValue> args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector("abcdef1234567890abcdef1234567890")));
	args->Add(AbstractValueFactory::Build(pub_key));
	args->Add(AbstractValueFactory::Build(nullptr));
	args->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector("abcdef1234567890abcdef1234567890")));
	
	gtx->AddOperationToGtx("ft3.dev_register_account", args);
	gtx->AddSignerToGtx(pub_key);
	gtx->Sign(priv_key, pub_key);

	auto encoded = gtx->Encode();
	auto decoded = Gtx::Decode(encoded);

	if (!gtx->blockchain_rid_.compare(decoded->blockchain_rid_) == 0) return false;
	if (!PostchainUtil::VectorsAreEqual(gtx->signatures_, decoded->signatures_)) return false;
	if (!PostchainUtil::VectorsAreEqual(gtx->signers_, decoded->signers_)) return false;

	std::string gtx_str = gtx->Serialize();
	std::string dec_str = decoded->Serialize();
	if (gtx_str.compare(dec_str) != 0) return false;

	return true;
}


bool SSOTest::SSOAddAuthOperationTest()
{
	std::vector<byte> priv_key;
	std::vector<byte> pub_key;
	PostchainUtil::GenerateKeyPair(priv_key, pub_key);
	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	std::shared_ptr<ArrayValue> args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector("abcdef1234567890abcdef1234567890")));
	args->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector("abcdef1234567890abcdef1234567890")));
	
	std::shared_ptr<ArrayValue> embeded_list = AbstractValueFactory::EmptyArray();
	embeded_list->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector("abcdef1234567890abcdef1234567890")));
	embeded_list->Add(AbstractValueFactory::Build(pub_key));
	embeded_list->Add(AbstractValueFactory::Build(nullptr));
	embeded_list->Add(AbstractValueFactory::Build(PostchainUtil::HexStringToByteVector("abcdef1234567890abcdef1234567890")));

	args->Add(embeded_list);

	gtx->AddOperationToGtx("ft3.add_auth_descriptor", args);
	gtx->AddSignerToGtx(pub_key);
	gtx->Sign(priv_key, pub_key);

	auto encode = gtx->Encode();
	auto decoded = gtx->Decode(encode);

	if(gtx->blockchain_rid_.compare(decoded->blockchain_rid_) == 0) return false;
	if (!PostchainUtil::VectorsAreEqual(gtx->signatures_, decoded->signatures_)) return false;
	if (!PostchainUtil::VectorsAreEqual(gtx->signers_, decoded->signers_)) return false;

	std::string gtx_str = gtx->Serialize();
	std::string dec_str = decoded->Serialize();
	if (gtx_str.compare(dec_str) != 0) return false;

	return true;
}
