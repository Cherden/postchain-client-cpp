#pragma once

#include <vector>
#include "../../src/common.h"
#include "../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {
class FT3Util
{
public:
	static std::string AuthTypeToString(AuthType type);

	static std::string FlagTypeToString(FlagsType type);

	static FlagsType StringToFlagType(std::string type);

	static AuthType StringToAuthType(std::string type);
};
} // namespace http
} // namespace postchain
} // namespace chromia