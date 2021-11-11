#pragma once

#include "../../../src/GTV/abstract_value_factory.h"
#include "../../../src/GTV/array_value.h"
#include "../../../src/postchain_util.h"
#include "../../forward_declarations.h"
#include "../../Core/util.h"

using namespace chromia::postchain::gtv;

namespace chromia {
namespace postchain {
namespace ft3 {

class MultiSignatureAuthDescriptor : public AuthDescriptor
{
public:
	std::vector<std::vector<byte>> pubkeys_;
	std::shared_ptr<Flags> flags_;
	int signature_required_;
	std::shared_ptr<IAuthdescriptorRule> auth_rule_;

	std::vector<std::vector<byte>> Signers()
	{
		return this->pubkeys_;
	}

	std::string ID()
	{
		return PostchainUtil::ByteVectorToHexString(this->Hash());
	}

	std::shared_ptr<IAuthdescriptorRule> Rule()
	{
		return this->auth_rule_;
	}

	MultiSignatureAuthDescriptor(std::vector<std::vector<byte>> pubkeys, int signature_required, std::vector<FlagsType> flags, std::shared_ptr<IAuthdescriptorRule> rule = nullptr);

	std::shared_ptr<gtv::ArrayValue> ToGTV();

	std::vector<byte> Hash();
};
} // namespace ft3
} // namespace postchain
} // namespace chromia



//using Chromia.Postchain.Client;
//using System.Collections.Generic;
//using System.Linq;
//using System;
//
//namespace Chromia.Postchain.Ft3
//{
//    public class MultiSignatureAuthDescriptor : AuthDescriptor
//    {
//        public List<byte[]> PubKeys;
//        public Flags Flags;
//        public int SignatureRequired;
//        public readonly IAuthdescriptorRule AuthRule;
//
//        public MultiSignatureAuthDescriptor(List<byte[]> pubkeys, int signatureRequired, FlagsType[] flags, IAuthdescriptorRule rule = null)
//        {
//            if (signatureRequired > pubkeys.Count)
//            {
//                throw new Exception("Number of required signatures have to be less or equal to number of pubkeys");
//            }
//
//            this.PubKeys = pubkeys;
//            this.SignatureRequired = signatureRequired;
//            this.Flags = new Flags(flags.ToList());
//            this.AuthRule = rule;
//        }
//
//        public List<byte[]> Signers
//        {
//            get => this.PubKeys;
//        }
//
//        public string ID
//        {
//            get => Util.ByteArrayToString(this.Hash());
//        }
//
//        public IAuthdescriptorRule Rule
//        {
//            get => this.AuthRule;
//        }
//
//        public object[] ToGTV()
//        {
//            var hexPubs = new List<string>();
//            foreach (var pubkey in this.PubKeys)
//            {
//                hexPubs.Add(Util.ByteArrayToString(pubkey));
//            }
//
//            var gtv = new object[] {
//                Util.AuthTypeToString(AuthType.MultiSig),
//                hexPubs.ToArray(),
//                new object[]
//                {
//                    this.Flags.ToGTV(),
//                    this.SignatureRequired,
//                    hexPubs.ToArray()
//                },
//                this.AuthRule?.ToGTV()
//            };
//
//            return gtv;
//        }
//
//        public byte[] Hash()
//        {
//            var hexPubs = new List<string>();
//            foreach (var pubkey in this.PubKeys)
//            {
//                hexPubs.Add(Util.ByteArrayToString(pubkey));
//            }
//
//            var gtv = new object[] {
//                Util.AuthTypeToString(AuthType.MultiSig),
//                this.PubKeys.ToArray(),
//                new object[]
//                {
//                    this.Flags.ToGTV(),
//                    this.SignatureRequired,
//                    hexPubs.ToArray()
//                },
//                this.AuthRule?.ToGTV()
//            };
//
//            return PostchainUtil.HashGTV(gtv);
//        }
//    }
//}