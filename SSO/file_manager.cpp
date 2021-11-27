#include "file_manager.h"

// TO-DO get rid of UE4 dependencies
#include "CoreMinimal.h"
#include "../../ChromaUnreal/Utils.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

namespace chromia {
namespace postchain {
namespace ft3 {

bool FileManager::WriteToFile(std::string a_file_name, std::string a_file_contents)
{
	FString file_name = ChromaUtils::STDStringToFString(a_file_name);
	FString file_contents = ChromaUtils::STDStringToFString(a_file_contents);

	FString persistent_data_path = FPaths::ProjectUserDir();
	FString full_path = FPaths::Combine(persistent_data_path, file_name);

	if (!FFileHelper::SaveStringToFile(
		file_contents,
		*file_name,
		FFileHelper::EEncodingOptions::AutoDetect,
		&IFileManager::Get(),
		0
	))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHROMA::Failed to write to %s"), *full_path);
		return false;
	}

	return false;
}

bool FileManager::LoadFromFile(std::string a_file_name, std::string &result)
{
	FString file_name = ChromaUtils::STDStringToFString(a_file_name);

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

	result = ChromaUtils::FStringToSTDString(f_result);
	return true;
}

} // namespace ft3
} // namespace postchain
} // namespace chromia