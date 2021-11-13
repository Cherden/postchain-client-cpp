#pragma once 

//#ifdef FT3_TESTS

#include <string>
#include <vector>
#include "../../src/common.h"

class TestOperators
{
public:

	static bool Equals(int a, int b)
	{
		return a == b;
	}

	static bool Equals(std::string a, std::string b)
	{
		return (a.compare(b) == 0);
	}

	static bool Equals(std::vector<unsigned char> a, std::vector<unsigned char> b)
	{
		if (a.size() != b.size()) return false;
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i] != b[i]) {
				return false;
			}
		}
		return true;
	}

	template <typename T>
	static bool NotNull(std::shared_ptr<T> ptr)
	{
		return ptr != nullptr;
	}
};

//#endif // FT3_TESTS