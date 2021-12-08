#ifdef COMPILE_TESTS

#include <gtest/gtest.h>

#include "../../src/PostchainClient/postchain_util.h"

#include "../../tests/FT3/asset_test.h"
#include "../../tests/FT3/account_test.h"
#include "../../tests/FT3/asset_balance_test.h"
#include "../../tests/FT3/auth_descriptor_rule_test.h"
#include "../../tests/FT3/blockchain_test.h"
#include "../../tests/FT3/rate_limit_test.h"
#include "../../tests/FT3/transfer_test.h"
#include "../../tests/FT3/xc_transfer_test.h"

#include "../../tests/SSO/sso_test.h"
#include "../../src/SSO/sso.h"
#include "../../src/SSO/file_manager.h"
#include "../../src/SSO/sso_store_local_storage.h"

#include "../../src/FT3/Core/postchain.h"

#include "../../src/PostchainClient/common.h"
#include "../../src/PostchainClient/postchain_transaction.h"
#include "../../src/PostchainClient/GTX/gtx.h"

#include <memory>
#include <vector>


using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::client;

// Wrapper for test classes
class FT3TestsMainWrapper
{
public:
	static SSOTest sso_test;
	static RateLimitTest rate_limit_test;
	static 	BlockchainTest blockchain_test;
	static AuthDescriptorRuleTest auth_descriptor_rule_test;
	static AssetTest asset_test;
	static AssetBalanceTest asset_balance_test;
	static AccountTest account_test;
	static TransferTest transfer_test;
	static XCTransferTest xc_transfer_test;

};

SSOTest FT3TestsMainWrapper::sso_test;
RateLimitTest FT3TestsMainWrapper::rate_limit_test;
BlockchainTest FT3TestsMainWrapper::blockchain_test;
AuthDescriptorRuleTest FT3TestsMainWrapper::auth_descriptor_rule_test;
AssetTest FT3TestsMainWrapper::asset_test;
AssetBalanceTest FT3TestsMainWrapper::asset_balance_test;
AccountTest FT3TestsMainWrapper::account_test;
TransferTest FT3TestsMainWrapper::transfer_test;
XCTransferTest FT3TestsMainWrapper::xc_transfer_test;

// -----------------------------------------------------------------------------------------------

TEST(SSO, SSORegisterOperationTest)
{
	ASSERT_TRUE(FT3TestsMainWrapper::sso_test.SSORegisterOperationTest());
}

TEST(SSO, SSOAddAuthOperationTest)
{
	ASSERT_TRUE(FT3TestsMainWrapper::sso_test.SSOAddAuthOperationTest());
}

// -----------------------------------------------------------------------------------------------

TEST(Blockchain, BlockchainTestRun1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun1());
}

TEST(Blockchain, BlockchainTestRun2)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun2());
}

TEST(Blockchain, BlockchainTestRun3)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun3());
}

TEST(Blockchain, BlockchainTestRun4)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun4());
}

TEST(Blockchain, BlockchainTestRun5)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun5());
}

TEST(Blockchain, BlockchainTestRun6)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun6());
}

TEST(Blockchain, BlockchainTestRun7)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun7());
}

TEST(Blockchain, BlockchainTestRun8)
{
	ASSERT_TRUE(FT3TestsMainWrapper::blockchain_test.BlockchainTestRun8());
}

// -----------------------------------------------------------------------------------------------

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun1());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun2)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun2());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun3)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun3());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun4)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun4());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun5)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun5());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun6)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun6());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun7)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun7());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun8)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun8());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun9)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun9());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun10)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun10());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun11)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun11());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun12)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun12());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun13)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun13());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun14)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun14());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun15)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun15());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun16)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun16());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun17)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun17());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun18)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun18());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun19)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun19());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun20)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun20());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun21)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun21());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun22)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun22());
}

TEST(AuthDescriptorRule, AuthDescriptorRuleTestRun23)
{
	ASSERT_TRUE(FT3TestsMainWrapper::auth_descriptor_rule_test.AuthDescriptorRuleTestRun23());
}

// -----------------------------------------------------------------------------------------------

TEST(Asset, AssetTestRun1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::asset_test.AssetTestRun1());
}

TEST(AuthDescAssetriptorRule, AssetTestRun2)
{
	ASSERT_TRUE(FT3TestsMainWrapper::asset_test.AssetTestRun2());
}

TEST(Asset, AssetTestRun3)
{
	ASSERT_TRUE(FT3TestsMainWrapper::asset_test.AssetTestRun3());
}

TEST(Asset, AssetTestRun4)
{
	ASSERT_TRUE(FT3TestsMainWrapper::asset_test.AssetTestRun4());
}

// -----------------------------------------------------------------------------------------------

TEST(TestsMain, AssetBalanceTestRun)
{
	ASSERT_TRUE(FT3TestsMainWrapper::asset_balance_test.AssetBalanceTestRun());
}

// -----------------------------------------------------------------------------------------------

TEST(Account, AccountTest1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest1());
}

TEST(Account, AccountTest2)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest2());
}

TEST(Account, AccountTest3)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest3());
}

TEST(Account, AccountTest4)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest4());
}
TEST(Account, AccountTest5)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest5());
}

TEST(Account, AccountTest6)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest6());
}

TEST(Account, AccountTest7)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest7());
}

TEST(Account, AccountTest8)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest8());
}

TEST(Account, AccountTest9)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest9());
}

TEST(Account, AccountTest10)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest10());
}

TEST(Account, AccountTest11)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest11());
}

TEST(Account, AccountTest12)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest12());
}

TEST(Account, AccountTest13)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest13());
}

TEST(Account, AccountTest14)
{
	ASSERT_TRUE(FT3TestsMainWrapper::account_test.AccountTest14());
}

// -----------------------------------------------------------------------------------------------

TEST(RateLimit, RateLimitTestRun1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::rate_limit_test.RateLimitTestRun1());
}

TEST(RateLimit, RateLimitTestRun2)
{
	ASSERT_TRUE(FT3TestsMainWrapper::rate_limit_test.RateLimitTestRun2());
}

TEST(RateLimit, RateLimitTestRun3)
{
	ASSERT_TRUE(FT3TestsMainWrapper::rate_limit_test.RateLimitTestRun3());
}

TEST(RateLimit, RateLimitTestRun4)
{
	ASSERT_TRUE(FT3TestsMainWrapper::rate_limit_test.RateLimitTestRun4());
}

TEST(RateLimit, RateLimitTestRun5)
{
	ASSERT_TRUE(FT3TestsMainWrapper::rate_limit_test.RateLimitTestRun5());
}

// -----------------------------------------------------------------------------------------------

TEST(TransferTest, TransferTestRun1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::transfer_test.TransferTestRun1());
}

TEST(TransferTest, TransferTestRun2)
{
	ASSERT_TRUE(FT3TestsMainWrapper::transfer_test.TransferTestRun2());
}

TEST(TransferTest, TransferTestRun3)
{
	ASSERT_TRUE(FT3TestsMainWrapper::transfer_test.TransferTestRun3());
}

TEST(TransferTest, TransferTestRun4)
{
	ASSERT_TRUE(FT3TestsMainWrapper::transfer_test.TransferTestRun4());
}

TEST(TransferTest, TransferTestRun5)
{
	ASSERT_TRUE(FT3TestsMainWrapper::transfer_test.TransferTestRun5());
}

// -----------------------------------------------------------------------------------------------

TEST(XCTransferTest, XCTransferTestRun1)
{
	ASSERT_TRUE(FT3TestsMainWrapper::xc_transfer_test.XCTransferTestRun1());
}

#endif // COMPILE_TESTS
