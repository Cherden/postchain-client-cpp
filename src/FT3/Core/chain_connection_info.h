#pragma once

#include <string>

namespace chromia {
namespace postchain {
namespace ft3 {

class ChainConnectionInfo
{
public:
	std::string url_;
    std::string chain_id;

    ChainConnectionInfo(std::string chain_id, std::string url)
    {
        this->chain_id = chain_id;
        this->url_ = url;
    }
};

} // namespace ft3
} //namespace postchain
} //namespace chromia