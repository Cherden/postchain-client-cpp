#include <string>

namespace chromia {
namespace postchain {
namespace ft3 {

class Postchain
{
public:
	Postchain(std::string url)
    {
        this->_url = url;
    }

  /*  public IEnumerator Blockchain(string id, Action<Blockchain> onSuccess, Action<string> onError)
    {
        var directoryService = new DirectoryServiceBase(
            new ChainConnectionInfo[] { new ChainConnectionInfo(id, _url) }
        );

        yield return Ft3.Blockchain.Initialize(id, directoryService, onSuccess, onError);
    }*/

private:
	std::string _url;
};

} // namespace ft3
} // namespace postchain
} // namespace chromia