#ifndef POSTCHAIN_CLIENT_TRANSACTION_H_
#define POSTCHAIN_CLIENT_TRANSACTION_H_

#include <memory>
#include <string>
#include <vector>

#include "GTV/abstract_value.h"
#include "GTV/abstract_value_facotry.h"
#include "operation.h"

namespace chromia {
namespace postchain {
class Transaction {
  public:
    Transaction(const std::string& blockchain_rid);
    Operation& AddOperation(const std::string& operation_name);
    Operation& AddOperation(const Operation& operation);
    Transaction& operator<<(const Operation& obj) {
        AddOperation(obj);
        return *this;
    }
    void AddSigner(unsigned char* signer);
    void Sign(unsigned char* private_key, unsigned char* public_key);
    std::vector<unsigned char> GetBufferToSign();
    gtv::AbstractValue GetGtvTxBody();

  private:
    std::string blockchain_rid_;
    std::vector<Operation> operations_;
    std::vector<unsigned char*> signers_;
    std::vector<unsigned char*> signatures_;
};
}  // namespace postchain
}  // namespace chromia

#endif