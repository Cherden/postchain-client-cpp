#pragma once

#include <memory>

namespace chromia {
namespace postchain {
namespace ft3 {

class ChromaFileManager
{
public:

	static std::string GetPersistentRoot();

	static void InitPersistentRoot();

	static bool WriteToFile(std::string a_file_name, std::string a_file_contents);

	static bool LoadFromFile(std::string a_file_name, std::string &result);

	// Load file from absolute path
	static bool LoadFromAbsFile(std::string a_file_name, std::string& result);
};

} // namespace ft3
} // namespace postchain
} // namespace chromia