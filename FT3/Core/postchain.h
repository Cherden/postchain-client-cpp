#include <string>
#include <functional>
#include "Blockchain/blockchain.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class Postchain
{
public:
	Postchain(std::string url);

	void Blockchain(std::string id, std::function<void(std::shared_ptr<ft3::Blockchain>)> on_success, std::function<void(std::string)> on_error);

private:
	std::string url_;
};

} // namespace ft3
} // namespace postchain
} // namespace chromia