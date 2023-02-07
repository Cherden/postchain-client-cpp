#pragma once

#include <string>
#include <memory>

namespace chromia {
namespace postchain {
namespace ft3 {

class ProtocolHandler
{
public:
    static void HandleTempTx(std::string name);
};

} // namespace ft3
} // namespace postchain
} // namespace chromia