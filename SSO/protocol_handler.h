#include <string>

namespace chromia {
namespace postchain {
namespace ft3 {

class ProtocolHandler
{
public:
	static void Register(std::string name)
    {
        RegisterCustomProtocolWindows(name);
    }

    static void HandleTempTx(std::string name)
    {
		// TO-DO resolve this
       /* string[] args = System.Environment.GetCommandLineArgs();
        SSOStore store = new SSOStoreLocalStorage();
        store.Load();

        for (int i = 0; i < args.Length; i++)
        {
            if (args[i].StartsWith(name))
            {
                store.TmpTx = args[i];
                store.Save();
                break;
            }
        }
        Application.Quit();
        }*/
    }

private:
	static void RegisterCustomProtocolWindows(std::string name)
	{
		// TO-DO resolve this

		/*auto location = new StringBuilder(Process.GetCurrentProcess().MainModule.FileName);
		location.Replace("/", "\\ ");*/

		// var keyTest = Registry.CurrentUser.OpenSubKey("Software", true).OpenSubKey("Classes", true);
		// RegistryKey key = keyTest.CreateSubKey(name);
		// key.SetValue("URL Protocol", "");
		// key.CreateSubKey(@"shell\open\command").SetValue("", @$"""{location.ToString()}""" + "-batchmode -nographics" + @" ""%1""");
	}

};

} // namespace ft3
} // namespace postchain
} // namespace chromia