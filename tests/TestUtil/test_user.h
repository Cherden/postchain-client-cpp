#pragma once 

#include <string>
#include <vector>
#include "../../src/FT3/forward_declarations.h"

using namespace chromia::postchain::ft3;
using namespace chromia;

class TestUser
{
public:
	static std::shared_ptr<User> SingleSig(std::shared_ptr<IAuthdescriptorRule> rule = nullptr);

	static std::shared_ptr<User> MultiSig(int required_signatures, int number_of_participants, std::shared_ptr<IAuthdescriptorRule> rule = nullptr);
};
