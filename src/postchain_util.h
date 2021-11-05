#ifndef POSTCHAIN_CLIENT_POSTCHAINUTIL_H_
#define POSTCHAIN_CLIENT_POSTCHAINUTIL_H_

#include <string>
#include <vector>
#include "SECP256K/include/secp256k1.h"
#include "query.h"
#include "../../nlohmann/json.hpp"

namespace chromia {
namespace postchain {

class PostchainUtil {
  public:

    static std::vector<unsigned char> LongToByteVector(long long number);

    static long long ByteVectorToLong(std::vector<unsigned char> bytes,
                                      bool mind_sign = false);

	/**
	  * Converts hex string to Buffer
	  * @param key: string
	  * @returns {Buffer}
	  */
    static std::vector<unsigned char> HexStringToByteVector(
        const std::string& hex_string);

    static std::string ByteVectorToHexString(
        const std::vector<unsigned char>& bytes);

    static std::string Sha256(std::vector<unsigned char> buffer);

    static bool IsLittleEndian();

	/**
	  * Creates a SECP256K private key
	  * @param private_key - Reference to the key wich will be created
	  * @return true if the execution was successfull, false otherwise
	  */
	static bool GeneratePrivateKey(std::vector<unsigned char> &private_key);

	/**
	  * Creates a SECP256K public key
	  * @param public_key - Reference to the key wich will be created
	  * @return true if the execution was successfull, false otherwise
	  */
	static bool GenerateCompresedPublicKey(std::vector<unsigned char> private_key, std::vector<unsigned char> &public_key);

	/**
	  * Creates a SECP256K public key pair
	  * @param private_key - Reference to the private key wich will be created
	  * @param public_key - Reference to the public key wich will be created
	  * @return true if the execution was successfull, false otherwise
	  */
	static bool GenerateKeyPair(std::vector<unsigned char> &private_key, std::vector<unsigned char> &public_key);

	/** 
	  *Get string representation of some binary private or public key 
	*/
	static std::string KeyToString(std::vector<unsigned char> &key);
	
	/**
	   * @param content to sign. It will be digested before signing.
	   * @param private_key The private key to sign the content with
	   *
	   * @return the signature
	   */
	static std::vector<unsigned char> Sign(std::vector<unsigned char> &content, std::vector<unsigned char> &private_key);

	/**
	* @param digest_buffer to sign. It will not be digested before signing.
	* @param private_key The private key to sign the digest with
	*
	* @return the signature
	*/
	static std::vector<unsigned char> SignDigest(std::vector<unsigned char> &digest_buffer, std::vector<unsigned char> &private_key);


	/**
	* TODO comment this
	*/
	static std::map<std::string, std::shared_ptr<AbstractValue>> QueryToDict(std::string query_name, std::vector<QueryObject> query_objects);

	/**
	* TODO comment this
	*/
	static std::string QueryToJSONString(std::string query_name, std::vector<QueryObject> query_objects);

private:
	static secp256k1_context *secp_context_;

	/**
	* Init secp_context_ static instance, if not already existing.
	*/
	static void InitSECPContext();

};

}  // namespace postchain
}  // namespace chromia

#endif