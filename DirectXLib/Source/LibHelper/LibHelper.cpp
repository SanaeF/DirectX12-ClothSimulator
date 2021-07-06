#include "LibHelper.h"
#include <iostream>
namespace helper {
	inline void LibHelper::throwIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}

	size_t LibHelper::alignmentedSize(size_t size, size_t alignment) {
		return size + alignment - size % alignment;
	}
	bool LibHelper::between(int param, int deltaP1, int deltaP2) {
		if (deltaP1 <= param && param < deltaP2)return true;
		return false;
	}
}