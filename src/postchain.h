#ifndef POSTCHAIN_CLIENT_POSTCHAIN_H_
#define POSTCHAIN_CLIENT_POSTCHAIN_H_

#include <memory>
#include <string>
#include <vector>

#include "postchain_transaction.h"

namespace chromia {
namespace postchain {
class Postchain {
  public:
    Postchain(std::string interface);
    PostchainTransaction& Transaction(std::string signers);
    std::string Query(std::string name);
};
}  // namespace postchain
}  // namespace chromia

#endif