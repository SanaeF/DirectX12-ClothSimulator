#pragma once
#include <wrl/client.h>
namespace helper {
	class LibHelper {
	public:
		size_t AlignmentedSize(size_t size, size_t alignment);
		void ThrowIfFailed(HRESULT hr);
		bool between(int param, int deltaP1, int deltaP2);
	};
}
