#include "../Core/key_pair.h"
#include "../User/AuthDescriptor/auth_descriptor_rule.h"

namespace chromia {
namespace postchain {
namespace ft3 {

class User
{
public KeyPair KeyPair;
public AuthDescriptor AuthDescriptor;

//public User(KeyPair keyPair, AuthDescriptor authDescriptor)
//{
//    this.KeyPair = keyPair;
//    this.AuthDescriptor = authDescriptor;
//}
//
//public static User GenerateSingleSigUser(FlagsType[] flags = null)
//{
//    if(flags == null)
//    {
//        flags = new List<FlagsType>(){FlagsType.Account, FlagsType.Transfer}.ToArray();
//    }
//
//    var keyPair = new KeyPair();
//
//    return new User(
//        keyPair,
//        new SingleSignatureAuthDescriptor(
//            keyPair.PubKey,
//            flags
//        )
//    );
//}
};
} // namespace http
} // namespace postchain
} // namespace chromia