#pragma once 

#include <functional>
#include <string>
#include "../../src/common.h"
#include "../../FT3/forward_declarations.h"

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
