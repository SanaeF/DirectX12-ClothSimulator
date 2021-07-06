#pragma once
#include <wrl/client.h>
namespace helper {
	class LibHelper {
	public:
		size_t alignmentedSize(size_t size, size_t alignment);
		void throwIfFailed(HRESULT hr);
		bool between(int param, int deltaP1, int deltaP2);
	};
}
