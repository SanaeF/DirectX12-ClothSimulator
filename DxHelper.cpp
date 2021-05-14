#include "DxHelper.h"
#include <iostream>
namespace helper {
	inline void DxHelper::ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}

	size_t DxHelper::AlignmentedSize(size_t size, size_t alignment) {
		return size + alignment - size % alignment;
	}
}