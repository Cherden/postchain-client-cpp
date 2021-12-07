#include "postchain_util.h"
#include "query.h"
#include "GTX/gtx.h"
#include <openssl/evp.h>
#include <algorithm>
#include <iomanip>
#include <ios>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>


namespace chromia {
namespace postchain {

secp256k1_context* PostchainUtil::secp_context_ = nullptr;

std::vector<unsigned char> PostchainUtil::LongToByteVector(
    long long int number) {
    std::vector<unsigned char> bytes;

    bool is_negative = false;
    if (number < 0) {
        number = number * -1;
        is_negative = true;
    }

    bool add_one = is_negative;
    for (int i = 0; i < sizeof(long long); i++) {
        unsigned char byte = number & (unsigned char)0xff;
        if (is_negative) {
            byte = ~byte;
        }

        bytes.push_back(add_one ? byte + 1 : byte);
        if (byte != (unsigned char)0xff) {
            add_one = false;
        }

        number >>= 8;

        if (number == 0) {
            break;
        }
    }

    if (IsLittleEndian()) {
        std::reverse(bytes.begin(), bytes.end());
    }

    return bytes;
}


long long PostchainUtil::ByteVectorToLong(std::vector<unsigned char> bytes,
                                          bool mind_sign) {
    if (bytes.size() > sizeof(long long)) {
        std::stringstream ss;
        ss << "too many bytes for long long, expected max " << sizeof(long long)
           << ", got " << bytes.size() << std::endl;
        throw std::out_of_range(ss.str());
    }

    // pad out
    unsigned char padding_byte =
        bytes.front() >= (unsigned char)0x80 && !mind_sign ? 0xff : 0x00;
    for (int i = bytes.size(); i < sizeof(long long); i++) {
        bytes.insert(bytes.begin(), padding_byte);
    }

    long long number = 0;
    for (auto byte : bytes) {
        number = (number << 8) | byte;
    }

    return number;
}


std::vector<byte> PostchainUtil::GetByteList(long long integer)
{
	byte* byte_list = static_cast<byte*>(static_cast<void*>(&integer));
	int length = sizeof(long long);

	std::vector<byte> trimmed_bytes;
	if (integer >= 0)
	{
		for (int i = length - 1; i >= 0; i--)
		{
			if (byte_list[i] != 0)
			{
				for (int j = 0; j <= i; j++)
				{
					trimmed_bytes.push_back(byte_list[j]);
				}
				break;
			}
		}
	}
	else 
	{
		// TO-DO check this branch
		for (int i = length - 1; i >= 0; i--)
		{
			if (byte_list[i] != 0xff)
			{
				for (int j = 0; j <= i; j++)
				{
					trimmed_bytes.push_back(byte_list[j]);
				}

				break;
			}
		}

		if (trimmed_bytes.size() == 0 || trimmed_bytes[trimmed_bytes.size() - 1] < 128)
		{
			trimmed_bytes.insert(trimmed_bytes.begin(), 0xff);
			if (integer < 0)
			{
				std::reverse(trimmed_bytes.begin(), trimmed_bytes.end());
			}
		}
	}

	return trimmed_bytes;
}


std::vector<byte> PostchainUtil::IntegerToBytes(long long integer, bool as_length)
{
	std::vector<byte> size_in_bytes = GetByteList(integer);

	if (IsLittleEndian())
	{
		std::reverse(size_in_bytes.begin(), size_in_bytes.end());
	}

	if (size_in_bytes.size() == 0)
	{
		size_in_bytes.push_back(0x00);
	}
	else if (!as_length && integer >= 0 && size_in_bytes[0] >= 128)
	{
		size_in_bytes.insert(size_in_bytes.begin(), 0x00);
	}

	return size_in_bytes;
}


std::vector<unsigned char> PostchainUtil::HexStringToByteVector(
    const std::string& hex_string) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hex_string.length(); i += 2) {
        unsigned char byte = stoi(hex_string.substr(i, 2), nullptr, 16);
        bytes.push_back(byte);
    }

    return bytes;
}


std::string PostchainUtil::ByteVectorToHexString(
    const std::vector<unsigned char>& bytes) {
    std::stringstream ss;

    ss << std::hex << std::setfill('0');
    for (auto byte : bytes) {
        ss << std::setw(2) << (int)byte;
    }

    return ss.str();
}


std::string PostchainUtil::Sha256(std::vector<unsigned char> buffer) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_length = 0;

    if (context == NULL) {
        throw new std::exception(
            "openssl evp context could not be allocated");
    } else if (!EVP_DigestInit_ex(context, EVP_sha256(), NULL)) {
        throw new std::exception(
            "openssl evp digest could not be initialized");
    } else if (!EVP_DigestUpdate(context, &buffer[0], buffer.size())) {
        throw new std::exception("openssl evp digest could not be set");
    } else if (!EVP_DigestFinal_ex(context, hash, &hash_length)) {
        throw new std::exception(
            "openssl evp sha256 hash could not be created");
    }

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    std::string hashed_buffer = ss.str();

    EVP_MD_CTX_free(context);

    return hashed_buffer;
}


std::vector<byte> PostchainUtil::IntegerToBinary(int x)
{
	// Transfrom int to byte array
	std::vector<byte> as_byte;
	byte* raw_cast = static_cast<byte*>(static_cast<void*>(&x));
	for (int i = 0; i < 4; i++)
	{
		as_byte.push_back(raw_cast[i]);
	}
	return as_byte;
}


bool PostchainUtil::IsLittleEndian() {
    int n = 1;
    return *(char*)&n == 1;
}


void PostchainUtil::InitSECPContext()
{
	if (PostchainUtil::secp_context_ == nullptr)
	{
		PostchainUtil::secp_context_ = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
	}
}


bool PostchainUtil::GeneratePrivateKey(std::vector<unsigned char> &private_key)
{
	PostchainUtil::InitSECPContext();
	if (PostchainUtil::secp_context_ == nullptr) return false;

	// Init private key
	private_key.clear();
	for (int i = 0; i < 32; i++)
	{
		private_key.push_back(RandomIntInRange(POSTCHAIN_BYTE_MIN, POSTCHAIN_BYTE_MAX));
	}

	//Check private key
	int result = secp256k1_ec_seckey_verify(PostchainUtil::secp_context_, private_key.data());
	if (result == 0)
	{
		return false;
	}

	return true;
}


bool PostchainUtil::GenerateCompresedPublicKey(std::vector<unsigned char> private_key, std::vector<unsigned char> &public_key)
{
	PostchainUtil::InitSECPContext();
	if (PostchainUtil::secp_context_ == nullptr) return false;

	// Generate secp256k1 raw public key from private key
	secp256k1_pubkey raw_pubkey;
	int result = secp256k1_ec_pubkey_create(PostchainUtil::secp_context_, &raw_pubkey, private_key.data());
	if (result == 0)
	{
		return false;
	}

	// Serialize public key
	size_t pubKeyLen = 33;
	for (int i=0; i<33; i++){
		public_key.push_back(0);
	}
	result = secp256k1_ec_pubkey_serialize(PostchainUtil::secp_context_, public_key.data(), &pubKeyLen, &raw_pubkey, SECP256K1_EC_COMPRESSED);
	if (result == 0)
	{
		return false;
	}

	return true;
}


bool PostchainUtil::GenerateKeyPair(std::vector<unsigned char> &private_key, std::vector<unsigned char> &public_key)
{
	if (!PostchainUtil::GeneratePrivateKey(private_key))
	{
		return false;
	}

	if (!PostchainUtil::GenerateCompresedPublicKey(private_key, public_key))
	{
		return false;
	}

	return true;
}


std::vector<unsigned char> PostchainUtil::Sign(std::vector<unsigned char> &content, std::vector<unsigned char> &private_key)
{
	if (private_key.size() == 0)
	{
		throw new std::exception("PostchainUtil::Sign() missing private_key");
	}

	if (private_key.size() != 32)
	{
		throw new std::exception("PostchainUtil::Sign() Invalid key length.");
	}

	return PostchainUtil::SignDigest(content, private_key);
}


std::vector<unsigned char> PostchainUtil::SignDigest(std::vector<unsigned char> &digest_buffer, std::vector<unsigned char> &private_key)
{
	PostchainUtil::InitSECPContext();
	if (PostchainUtil::secp_context_ == nullptr)
	{
		throw new std::exception("PostchainUtil::SignDigest missing SECPContext");
	}

	if (digest_buffer.size() == 0)
		throw new std::exception("PostchainUtil::SignDigest digest_buffer.size() == 0");

	secp256k1_ecdsa_recoverable_signature recoverable_signature;
	std::vector<unsigned char> sigbytes(64);
	int recid;

	secp256k1_ecdsa_sign_recoverable(PostchainUtil::secp_context_, &recoverable_signature, digest_buffer.data(), private_key.data(), NULL, NULL);
	secp256k1_ecdsa_recoverable_signature_serialize_compact(PostchainUtil::secp_context_, sigbytes.data(), &recid, &recoverable_signature);

	return sigbytes;
}


std::map<std::string, std::shared_ptr<AbstractValue>> PostchainUtil::QueryToDict(std::string query_name, std::vector<QueryObject> query_objects)
{
	std::map<std::string, std::shared_ptr<AbstractValue>> query_dict;

	query_dict.insert(std::pair<std::string, std::shared_ptr<AbstractValue>>("type", AbstractValueFactory::Build(query_name)));

	if (query_objects.size() > 0)
	{
		for(QueryObject &entry : query_objects)
		{
			if (std::dynamic_pointer_cast<gtv::OctetStringValue>(entry.content_))
			{
				std::shared_ptr<gtv::OctetStringValue> casted = std::dynamic_pointer_cast<gtv::OctetStringValue>(entry.content_);
				std::string content_as_string = PostchainUtil::ByteVectorToHexString(casted->GetValue());
				query_dict.insert(std::pair<std::string, std::shared_ptr<AbstractValue>>(entry.name_, AbstractValueFactory::Build(content_as_string)));
			}
			else
			{
				query_dict.insert(std::pair<std::string, std::shared_ptr<AbstractValue>>(entry.name_, entry.content_));
			}
		}
	}

	return query_dict;
}


std::string PostchainUtil::QueryToJSONString(std::string query_name, std::vector<QueryObject> query_objects)
{
	nlohmann::json json_root;

	json_root["type"] = query_name;

	if (query_objects.size() > 0)
	{
		for (QueryObject &entry : query_objects)
		{
			if (std::dynamic_pointer_cast<gtv::OctetStringValue>(entry.content_))
			{
				std::shared_ptr<gtv::OctetStringValue> casted = std::dynamic_pointer_cast<gtv::OctetStringValue>(entry.content_);
				std::string content_as_string = PostchainUtil::ByteVectorToHexString(casted->GetValue());
				json_root[entry.name_] = content_as_string;
			}
			else if (std::dynamic_pointer_cast<gtv::IntegerValue>(entry.content_))
			{
				std::shared_ptr<gtv::IntegerValue> casted = std::dynamic_pointer_cast<gtv::IntegerValue>(entry.content_);
				json_root[entry.name_] = casted->GetValue();
			}
			else if (std::dynamic_pointer_cast<gtv::UTF8StringValue>(entry.content_))
			{
				std::shared_ptr<gtv::UTF8StringValue> casted = std::dynamic_pointer_cast<gtv::UTF8StringValue>(entry.content_);
				json_root[entry.name_] = casted->GetValue();
			}
			else 
			{
				throw new std::exception("PostchainUtil::QueryToJSONString unnaccepted entry.content_ type");
			}
		}
	}

	return json_root.dump();
}


int PostchainUtil::RandomIntInRange(int min, int max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}

	int res = min + rand() % ((max + 1) - min);
	return res;
}


bool PostchainUtil::StringToBool(const std::string & value)
{
	return value.compare("true") == 0;
}


std::string PostchainUtil::GetSafeJSONString(const nlohmann::json & value, std::string key, std::string default_value)
{
	if (value.contains(key))
	{
		return value[key];
	}
	else 
	{
		return default_value;
	}
}


int PostchainUtil::GetSafeJSONInt(const nlohmann::json & value, std::string key, int default_value)
{
	if (value.contains(key))
	{
		return value[key];
	}
	else
	{
		return default_value;
	}
}


long PostchainUtil::GetSafeJSONLong(const nlohmann::json & value, std::string key, long default_value)
{
	if (value.contains(key))
	{
		return value[key];
	}
	else
	{
		return default_value;
	}
}


std::string PostchainUtil::ToUpper(std::string str)
{
	std::string clone = str;
	std::transform(clone.begin(), clone.end(), clone.begin(), ::toupper);
	return clone;
}

long long PostchainUtil::GetCurrentTimeMillis()
{
	// TO-DO replace * 1000 multiplication with precise millis
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	);

	return ms.count();
}

void PostchainUtil::SleepForMillis(long long millis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

std::shared_ptr<client::Gtx> PostchainUtil::DeserializeGTX(std::vector<byte> encoded_message)
{
	return client::Gtx::Decode(encoded_message);
}

bool PostchainUtil::StringsAreEqual(std::string a, std::string b)
{
	return a.compare(b) == 0;
}

template <typename T>
bool PostchainUtil::VectorsAreEqual(std::vector<T> a, std::vector<T> b)
{
	if (a.size() != b.size()) return false;
	for (size_t i = 0; i < a.size(); i++)
	{
		if (a[i] != b[i]) return false;
	}
	return true;
}

template <typename T>
bool PostchainUtil::VectorsAreEqual(std::vector<std::vector<T>> a, std::vector<std::vector<T>> b)
{
	if (a.size() != b.size()) return false;
	for (size_t i = 0; i < a.size(); i++)
	{
		if (!VectorsAreEqual(a[i], b[i])) return false;
	}
	return true;
};

}  // namespace postchain
}  // namespace chromia