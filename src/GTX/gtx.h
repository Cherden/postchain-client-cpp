#ifndef POSTCHAIN_CLIENT_GTX_GTX_H_
#define POSTCHAIN_CLIENT_GTX_GTX_H_

#include <memory>
#include <string>
#include <vector>

#include "../common.h"
#include "../GTV/abstract_value.h"
#include "../ASN1/reader.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {

// Forward declaration
class PostchainOperation;

namespace client {

// Forward declaration
class GTXValue;

class Gtx {
public:
	std::string blockchain_rid_;
	std::vector<std::shared_ptr<PostchainOperation>> operations_;
	std::vector<std::vector<byte>> signers_;
	std::vector<std::vector<byte>> signatures_;

	static std::shared_ptr<GTXValue> ArgToGTXValue(std::shared_ptr<AbstractValue> arg);
	
	static std::shared_ptr<AbstractValue> GtxValueToAbstract(std::shared_ptr<GTXValue> arg);

	static std::shared_ptr<Gtx> Decode(std::vector<byte> encoded_message);

	Gtx();

	Gtx(const std::string& blockchain_rid);

	void AddSignerToGtx(std::vector<byte> signer);

	Gtx* AddOperationToGtx(std::string op_name, std::shared_ptr<ArrayValue> args);

	void Sign(std::vector<byte> private_key, std::vector<byte> public_key);

	std::vector<byte> GetBufferToSign();
	
	std::shared_ptr<AbstractValue> GetGtvTxBody();

	int GetSignerIndex(std::vector<byte> &signer);

	void AddSignature(std::vector<byte> public_key, std::vector<byte> signature);

	std::string Serialize();
	
	std::vector<byte> Encode();	
};
}  // namespace client
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTX_DICT_PAIR_H_