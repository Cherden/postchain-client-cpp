#include "../../src/common.h"
#include "../User/account.h"

#include <vector>

namespace chromia {
namespace postchain {
namespace ft3 {
class Util
{
public:
    static std::string AuthTypeToString(AuthType type)
    {
        switch (type)
        {
		case AuthType::SingleSig:
            return "S";
		case AuthType::MultiSig:
            return "M";
        default:
            return "";
        }
    }

    static std::string FlagTypeToString(FlagsType type)
    {
        switch (type)
        {
		case FlagsType::Account:
            return "A";
		case FlagsType::Transfer:
            return "T";
        default:
            return "";
        }
    }

    static FlagsType StringToFlagType(std::string type)
    {
		if (type.compare("A") == 0)
		{
			return FlagsType::Account;
		}
		if (type.compare("T") == 0)
		{
			return FlagsType::Transfer;
		}
		FlagsType::None;
    }

    static AuthType StringToAuthType(std::string type)
    {
		if (type.compare("S") == 0)
		{
			return AuthType::SingleSig;
		}
		if (type.compare("M") == 0)
		{
			return AuthType::MultiSig;
		}
		return AuthType::None;
    }
};
} // namespace http
} // namespace postchain
} // namespace chromia