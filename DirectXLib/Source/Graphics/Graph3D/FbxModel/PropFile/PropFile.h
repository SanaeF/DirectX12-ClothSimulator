#pragma once
#include <vector>
namespace model {
	class PropFile {
	public:
		static void outputPreIndex(const char* FileName, std::vector<int>* m_PreIndex);
		static bool inputPreIndex(const char* FileName, std::vector<int>* m_PreIndex);
	};
}