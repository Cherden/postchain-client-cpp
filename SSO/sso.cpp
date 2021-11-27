#include "sso.h"

namespace chromia {
namespace postchain {
namespace ft3 {

string SSO::vault_url_ = "https://vault-testnet.chromia.com";

static std::string GetVaultUrl()
{
	return SSO::vault_url_;
}

static void SetVaultUrl(std::string url)
{
	SSO::vault_url_ = url;
}


} // namespace ft3
} // namespace postchain
} // namespace chromia