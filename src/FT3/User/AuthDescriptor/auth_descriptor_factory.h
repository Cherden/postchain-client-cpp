#pragma once

#include <vector>
#include <memory>
#include "../../forward_declarations.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class AuthDescriptorFactory
{
public:

	class AuthDescriptorQuery
	{
	public:
		std::string id_;
		std::string type_;
		std::string args_;

		AuthDescriptorQuery(std::string id, std::string type, std::string args)
		{
			this->id_ = id;
			this->type_ = type;
			this->args_ = args;
		}
	};

	std::shared_ptr<AuthDescriptor> Create(AuthType type, std::vector<byte> args);

private:
	std::shared_ptr<SingleSignatureAuthDescriptor> CreateSingleSig(std::vector<byte> args);

	std::shared_ptr<MultiSignatureAuthDescriptor> CreateMultiSig(std::vector<byte> args);
  
};
} // namespace http
} // namespace postchain
} // namespace chromia