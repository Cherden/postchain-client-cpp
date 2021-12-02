#include <string>
//#include <Winreg.h>
//#include "libloaderapi.h"

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

		/*HKEY hKey;
		LONG result = 0;
		char filename[] = "C:\test.jpg";
		char *path = "SOFTWARE\\Classes\\";*/

		
		//if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
		//	printf("1. success \n"); //  doesn`t work 
		//else printf("fail\n");

		/*int result = RegCreateKeyEx(HKEY_CURRENT_USER, L"ChromaTestReg1\\shell\\open\\command", 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);*/

		/*result++;
		result++;
		result++;*/
		/*std::string location = "PostchainCPP";

		HKEY hkey;
		char szBuf[MAX_PATH], szExe[MAX_PATH * 2], szShort[MAX_PATH];
		DWORD filename_length = GetModuleFileNameA(hInst, szBuf, sizeof(szBuf));
		GetShortPathNameA(szBuf, szShort, sizeof(szShort));

		LSTATUS RegOpenKeyA(
			[in]           HKEY   hKey,
			[in, optional] LPCSTR lpSubKey,
			[out]          PHKEY  phkResult
		);*/

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