#pragma once

#include "util.h"
#include "../User/account.h"

namespace chromia {
namespace postchain {
namespace ft3 {
std::string FT3Util::AuthTypeToString(AuthType type)
{
	switch (type)
	{
	case AuthType::eSingleSig:
		return "S";
	case AuthType::eMultiSig:
		return "M";
	default:
		return "";
	}
}

std::string FT3Util::FlagTypeToString(FlagsType type)
{
	switch (type)
	{
	case FlagsType::eAccount:
		return "A";
	case FlagsType::eTransfer:
		return "T";
	default:
		return "";
	}
}

FlagsType FT3Util::StringToFlagType(std::string type)
{
	if (type.compare("A") == 0 || type.compare("s_A") == 0)
	{
		return FlagsType::eAccount;
	}
	if (type.compare("T") == 0 || type.compare("s_T") == 0)
	{
		return FlagsType::eTransfer;
	}
	return FlagsType::eFlagNone;
}

AuthType FT3Util::StringToAuthType(std::string type)
{
	if (type.compare("S") == 0)
	{
		return AuthType::eSingleSig;
	}
	if (type.compare("M") == 0)
	{
		return AuthType::eMultiSig;
	}
	return AuthType::eAuthNone;
}
} // namespace http
} // namespace postchain
} // namespace chromia