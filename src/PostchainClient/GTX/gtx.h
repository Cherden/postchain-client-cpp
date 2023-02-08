#ifndef POSTCHAIN_CLIENT_GTX_GTX_H_
#define POSTCHAIN_CLIENT_GTX_GTX_H_

#include <memory>
#include <string>
#include <vector>

#include "../common.h"
#include "../forward_declarations.h"
#include "../GTV/abstract_value.h"
#include "../GTV/array_value.h"
#include "../ASN1/reader.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace client {

class Gtx {
public:
	std::string blockchain_rid_;
	std::vector<std::shared_ptr<PostchainOperation>> operations_;
	std::vector<std::vector<BYTE>> signers_;
	std::vector<std::vector<BYTE>> signatures_;

	static std::shared_ptr<GTXValue> ArgToGTXValue(std::shared_ptr<AbstractValue> arg);
	
	static std::shared_ptr<AbstractValue> GtxValueToAbstract(std::shared_ptr<GTXValue> arg);

	static std::shared_ptr<Gtx> Decode(std::vector<BYTE> encoded_message);

	Gtx();

	Gtx(const std::string& blockchain_rid);

	void AddSignerToGtx(std::vector<BYTE> signer);

	Gtx* AddOperationToGtx(std::string op_name, std::shared_ptr<ArrayValue> args);

	void Sign(std::vector<BYTE> private_key, std::vector<BYTE> public_key);

	std::vector<BYTE> GetBufferToSign();
	
	std::shared_ptr<AbstractValue> GetGtvTxBody();

	int GetSignerIndex(std::vector<BYTE> &signer);

	void AddSignature(std::vector<BYTE> public_key, std::vector<BYTE> signature);

	std::string Serialize();
	
	std::vector<BYTE> Encode();	
};
}  // namespace client
}  // namespace postchain
}  // namespace chromia

#endif  // POSTCHAIN_CLIENT_GTX_DICT_PAIR_H_