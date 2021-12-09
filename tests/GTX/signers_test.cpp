#ifdef COMPILE_TESTS

#include <gtest/gtest.h>
#include <vector>

#include "../../src/PostchainClient/GTV/abstract_value_factory.h"
#include "../../src/PostchainClient/common.h"
#include "../../src/PostchainClient/postchain_util.h"
#include "../../src/PostchainClient/GTX/gtx.h"

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

TEST(GTX, GtxTest) {
	std::vector<byte> key_priv_1;
	std::vector<byte> key_pub_1;
	PostchainUtil::GenerateKeyPair(key_priv_1, key_pub_1);

	std::vector<byte> key_priv_2;
	std::vector<byte> key_pub_2;
	PostchainUtil::GenerateKeyPair(key_priv_2, key_pub_2);

	auto gtx = std::make_shared<Gtx>("abcdef1234567890abcdef1234567890");
	
	auto args = AbstractValueFactory::EmptyArray();
	args->Add(AbstractValueFactory::Build("teststring"));
	gtx->AddOperationToGtx("test", args);

	gtx->AddSignerToGtx(key_pub_1);
	gtx->AddSignerToGtx(key_pub_2);

	gtx->Sign(key_priv_1, key_pub_1);
	gtx->Sign(key_priv_2, key_pub_2);

	auto before_sigs = gtx->signatures_;
	gtx->GetBufferToSign();
	auto after_sigs = gtx->signatures_;

	ASSERT_EQ(before_sigs, after_sigs);
}

#endif // COMPILE_TESTS