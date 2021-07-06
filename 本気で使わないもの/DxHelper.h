#pragma once
#include <wrl/client.h>
namespace helper {
	class DxHelper {
	public:
		size_t AlignmentedSize(size_t size, size_t alignment);
		void ThrowIfFailed(HRESULT hr);
	};
}
 