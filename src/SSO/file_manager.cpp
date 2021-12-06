#include "file_manager.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <fstream>

//#include "CoreMinimal.h"
//#include "Misc/Paths.h"
//#include "HAL/PlatformFilemanager.h"
//#include "Misc/FileHelper.h"

namespace chromia {
namespace postchain {
namespace ft3 {

std::string FileManager::GetPersistentRoot()
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

	/*const char* env_p = std::getenv("%APPDATA%");
	std::string path(env_p);
	path += "ChromiaTemp";
	return path;*/

	//FString root = FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("CHROMIA_SSO_ROOT"));
	//return ChromaUtils::FStringToSTDString(root);
}

void FileManager::InitPersistentRoot()
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

bool FileManager::WriteToFile(std::string a_file_name, std::string a_file_contents)
{
	std::string full_path = GetPersistentRoot() + "/" + a_file_name;

	std::ofstream out(full_path);
	out << a_file_contents;
	out.close();

	return true;
	/*FString file_name = ChromaUtils::STDStringToFString(a_file_name);
	FString file_contents = ChromaUtils::STDStringToFString(a_file_contents);

	FString persistent_data_path = ChromaUtils::STDStringToFString(FileManager::GetPersistentRoot());
	FString full_path = FPaths::Combine(persistent_data_path, file_name);

	bool write_res = FFileHelper::SaveStringToFile(
		file_contents,
		*full_path,
		FFileHelper::EEncodingOptions::AutoDetect,
		&IFileManager::Get(),
		0
	);

	if (!write_res)
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::Failed to write to %s"), *full_path);
		return false;
	}

	return false;*/
}

bool FileManager::LoadFromFile(std::string a_file_name, std::string &result)
{
	std::string full_path = GetPersistentRoot() + "/" + a_file_name;

	std::ifstream file(full_path);
	file >> result;
	file.close();

	/*FString file_name = ChromaUtils::STDStringToFString(a_file_name);

	FString persistent_data_path = FPaths::ProjectUserDir();
	FString full_path = FPaths::Combine(persistent_data_path, file_name);

	IFileManager* file_manager = &IFileManager::Get();

	if (!file_manager->FileExists(*full_path))
	{
		result = "";
		return false;
	}

	FString f_result;
	if (!FFileHelper::LoadFileToString(
		f_result,
		*full_path,
		FFileHelper::EHashOptions::None,
		0
	))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::Failed to read from %s"), *full_path);
		return false;
	}

	result = ChromaUtils::FStringToSTDString(f_result);*/
	return true;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia