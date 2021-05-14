#include "LibHelper.h"
#include <iostream>
namespace helper {
	inline void LibHelper::ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}

	size_t LibHelper::AlignmentedSize(size_t size, size_t alignment) {
		return size + alignment - size % alignment;
	}
}