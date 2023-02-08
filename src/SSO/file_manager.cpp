#include "file_manager.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <fstream>

namespace chromia {
namespace postchain {
namespace ft3 {

std::string ChromaFileManager::GetPersistentRoot()
{
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, "APPDATA") == 0 && buf != nullptr)
	{
		std::string path(buf);
		path += "/ChromiaTemp";
		free(buf);
		return path;
	}
	else
	{
		throw std::exception("GetPersistentRoot() failed");
	}
}

void ChromaFileManager::InitPersistentRoot()
{
	std::string path = GetPersistentRoot();
	if (CreateDirectoryA(path.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		// It's ok
	}
	else
	{
		throw std::exception("InitPersistentRoot() failed");
	}
}

bool ChromaFileManager::WriteToFile(std::string a_file_name, std::string a_file_contents)
{
	std::string full_path = GetPersistentRoot() + "/" + a_file_name;

	std::ofstream out(full_path);
	out << a_file_contents;
	out.close();

	return true;
}

bool ChromaFileManager::LoadFromFile(std::string a_file_name, std::string &result)
{
	std::string full_path = GetPersistentRoot() + "/" + a_file_name;

	std::ifstream file(full_path);
	file >> result;
	file.close();

	return true;
}

bool ChromaFileManager::LoadFromAbsFile(std::string a_file_name, std::string& result)
{
	std::ifstream file(a_file_name);
	file >> result;
	file.close();
	return true;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia