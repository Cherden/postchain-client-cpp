#pragma once 

//#ifdef FT3_TESTS

#include <functional>
#include <string>
#include "../../src/common.h"
#include "../../FT3/forward_declarations.h"
#include "../../FT3/User/AuthDescriptor/auth_descriptor_rule.h"
#include "../../FT3/User/asset.h"
#include "../../FT3/User/user.h"
#include "../../FT3/User/account.h"
#include "../../FT3/User/account_operations.h"
#include "../../FT3/Core/transaction_builder.h"
#include "../../FT3/Core/transaction.h"
#include "../../FT3/Core/Blockchain/blockchain_session.h"
#include "../../FT3/Core/Blockchain/blockchain_info.h"
#include "../../FT3/Core/Blockchain/rate_limit_info.h"
#include "../../FT3/User/rate_limit.h"
#include "../../FT3/Core/key_pair.h"
#include "../../src/postchain_util.h"
#include "../TestUtil/blockchain_util.h"
#include "../TestUtil/test_user.h"
#include "../TestUtil/test_util.h"
#include "../TestUtil/account_builder.h"
#include "../TestUtil/test_operators.h"

//#include "GenericPlatform/GenericPlatformProcess.h"
//#include "CoreMinimal.h" // TO-DO get rid of UE4 dependencies

using namespace chromia;
using namespace chromia::postchain;
using namespace chromia::postchain::ft3;

class RateLimitTest
{
private:
	const int REQUEST_MAX_COUNT = 10;
	const int RECOVERY_TIME = 5000;
	const int POINTS_AT_ACCOUNT_CREATION = 1;
	std::shared_ptr<Blockchain> blockchain_ = nullptr;

	void SetupBlockchain();

	static void EmptyCallback() {}

	static void DefaultErrorHandler(std::string error);

public:

	void MakeRequests(std::shared_ptr<Account> account, int requests, std::function<void(std::string)> on_success);

	// Should have a limit of 10 requests per minute
	bool RateLimitTestRun1();

	// should show 10 at request count
	bool RateLimitTestRun2();

	// waits 20 seconds and gets 4 points
	bool RateLimitTestRun3();

	// can make 4 operations
	bool RateLimitTestRun4();

	// can't make another operation because she has 0 points
	bool RateLimitTestRun5();

};

//#endif // FT3_TESTS