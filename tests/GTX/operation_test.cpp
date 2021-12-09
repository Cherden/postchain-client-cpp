#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "../../src/PostchainClient/GTV/abstract_value_factory.h"
#include "../../src/PostchainClient/common.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../../src/PostchainClient/GTX/gtx.h"

#include "../../src/PostchainClient/GTV/abstract_value.h"
#include "../../src/PostchainClient/GTV/abstract_value_factory.h"
#include "../../src/PostchainClient/GTV/array_value.h"
#include "../../src/PostchainClient/GTV/dict_value.h"
#include "../../src/PostchainClient/GTV/integer_value.h"
#include "../../src/PostchainClient/GTV/null_value.h"
#include "../../src/PostchainClient/GTV/octet_string_value.h"
#include "../../src/PostchainClient/GTV/utf8_string_value.h"
#include "../../src/PostchainClient/postchain_util.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

TEST(GTX, EmptyOperationTest) 
{
	std::vector<byte> pub_key;
	std::vector<byte> priv_key;
	PostchainUtil::GenerateKeyPair(priv_key, pub_key);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	gtx->AddOperationToGtx("test", AbstractValueFactory::EmptyArray());
	gtx->AddSignerToGtx(pub_key);
	gtx->Sign(priv_key, pub_key);

	auto encode = gtx->Encode();
	auto decoded = Gtx::Decode(encode);

	ASSERT_EQ(gtx->blockchain_rid_, decoded->blockchain_rid_);
	ASSERT_EQ(gtx->signatures_, decoded->signatures_);
	ASSERT_EQ(gtx->signers_, decoded->signers_);
}


TEST(GTX, SimpleOperationTest)
{
	std::vector<byte> pub_key;
	std::vector<byte> priv_key;
	PostchainUtil::GenerateKeyPair(priv_key, pub_key);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	auto args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build("teststring"));
	gtx->AddOperationToGtx("test", args);
	gtx->AddSignerToGtx(pub_key);
	gtx->Sign(priv_key, pub_key);

	auto encode = gtx->Encode();
	auto decoded = Gtx::Decode(encode);

	ASSERT_EQ(gtx->blockchain_rid_, decoded->blockchain_rid_);
	ASSERT_EQ(gtx->signatures_, decoded->signatures_);
	ASSERT_EQ(gtx->signers_, decoded->signers_);
}


TEST(GTX, MultiSigOperationTest)
{
	std::vector<byte> pub_key_1;
	std::vector<byte> priv_key_1;
	PostchainUtil::GenerateKeyPair(priv_key_1, pub_key_1);

	std::vector<byte> pub_key_2;
	std::vector<byte> priv_key_2;
	PostchainUtil::GenerateKeyPair(priv_key_2, pub_key_2);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	auto args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build("teststring"));
	gtx->AddOperationToGtx("test", args);
	gtx->AddSignerToGtx(pub_key_1);
	gtx->AddSignerToGtx(pub_key_2);
	gtx->Sign(priv_key_1, pub_key_1);
	gtx->Sign(priv_key_2, pub_key_2);

	auto encode = gtx->Encode();
	auto decoded = Gtx::Decode(encode);

	ASSERT_EQ(gtx->blockchain_rid_, decoded->blockchain_rid_);
	ASSERT_EQ(gtx->signatures_, decoded->signatures_);
	ASSERT_EQ(gtx->signers_, decoded->signers_);
}


TEST(GTX, FullOperationTest)
{
	std::vector<byte> priv_key;
	std::vector<byte> pub_key;
	PostchainUtil::GenerateKeyPair(priv_key, pub_key);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	auto args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build("teststring"));
	args->Add(AbstractValueFactory::Build(123));
	args->Add(AbstractValueFactory::Build(std::vector<byte> {0xaf, 0xfe}));
	gtx->AddOperationToGtx("test", args);
	gtx->AddSignerToGtx(pub_key);
	gtx->Sign(priv_key, pub_key);

	auto encode = gtx->Encode();
	auto decoded = Gtx::Decode(encode);

	ASSERT_EQ(gtx->blockchain_rid_, decoded->blockchain_rid_);
	ASSERT_EQ(gtx->signatures_, decoded->signatures_);
	ASSERT_EQ(gtx->signers_, decoded->signers_);
}


TEST(GTX, LengthEdgeCaseOperationTest)
{
	std::vector<byte> priv_key;
	std::vector<byte> pub_key;
	PostchainUtil::GenerateKeyPair(priv_key, pub_key);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");

	auto args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build("teststring"));
	args->Add(AbstractValueFactory::Build(123));
	args->Add(AbstractValueFactory::Build(std::vector<byte> {0xaf, 0xfe}));
	args->Add(AbstractValueFactory::Build(std::vector<std::string> {"hello", "world"}));
	gtx->AddOperationToGtx("test", args);
	gtx->AddSignerToGtx(pub_key);
	gtx->Sign(priv_key, pub_key);

	auto encode = gtx->Encode();
	auto decoded = Gtx::Decode(encode);

	ASSERT_EQ(gtx->blockchain_rid_, decoded->blockchain_rid_);
	ASSERT_EQ(gtx->signatures_, decoded->signatures_);
	ASSERT_EQ(gtx->signers_, decoded->signers_);
}

#endif // COMPILE_TESTS